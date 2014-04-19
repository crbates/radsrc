/* 
Copyright (c) 2006 The Regents of the University of California. 
All rights reserved.
UCRL-CODE-224806

This software was developed by Lawrence Livermore National Laboratory.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer. 
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution. 
3. The name of the author may not be used to endorse or promote products
   derived from this software without specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include "porting.h"

#include <iostream>

#include "isotopedb.h"
#include "legacyfiles.h"
#include "elements.h"
#include "legacyglue.h"
#include "dbmanager.h"

// Functions in the file understand both the CLegacyCollection and the CIsotopeDatabase classes

namespace radsrc {
   extern std::vector<std::string> BuggyParentList;

   void FixupAtomicMasses(CIsotopeDatabase* isodb, CLegacyCollection& legacy);
   void FixupBranchRatios(CIsotopeDatabase* isodb);

   bool CanonicalizeId(CDatabaseManager::DatabaseIdentifier& id) {
      return CLegacyCollection::CanonicalizeId(id);
   }
   CIsotopeDatabase* ConvertLegacy(const CDatabaseManager::DatabaseIdentifier& id) {
      CLegacyCollection legacy;
      legacy.Read(id);
      if (legacy.m_GamFiles.size() == 0) {
         std::cerr << "Unable to read legacy DB files; Check RADSRC_DATA or RADSRC_LEGACYDATA environment variable." << std::endl;
         return 0;
      }

      CIsotopeDatabase* isodb = new CIsotopeDatabase();
      CLegacyCollection::GamDataList::const_iterator it;
      for (it = legacy.m_GamFiles.begin(); it != legacy.m_GamFiles.end(); ++it) {
         int n_branches = it->second.m_Branches.size();
         if (n_branches == 0) continue;
         if (it->second.m_Branches[0].size() == 0) continue;
         CIsotope ParentID;
         ParentID.fromString(it->second.m_Isotope);
         if (!ParentID.isValid()) continue;
         isodb->accessIsotopeData(ParentID).m_atomicmass = it->second.m_AtomicMass;
         isodb->accessIsotopeData(ParentID).m_halflife = 0.0;
         double systematic_error = it->second.m_systematic;
         for (int branch = 0; branch < n_branches; branch++) {
            CIsotope did, pid = ParentID;
            const std::vector<CLegacyBranchMember>& CurrentBranch =
               it->second.m_Branches[branch];
            int n_members = CurrentBranch.size();
            for (int i = 0; i < n_members-1; i++,pid=did) { 
               CIsotopeData& ParentData = isodb->accessIsotopeData(pid);
               ParentData.m_isotope = pid;
               pid.toString(ParentData.m_name);
               const CLegacyBranchMember& legacydecay = CurrentBranch[i];
               ParentData.m_halflife = legacydecay.m_HalfLife;
               did.fromString(CurrentBranch[i+1].m_Isotope);
               if (!did.isValid()) break;
               CIsotopeData& DaughterData = isodb->accessIsotopeData(did); // creates
               DaughterData.m_isotope = did;
               did.toString(DaughterData.m_name);
               if (legacydecay.m_fAdd == false)  {
                  continue;
               }
               int j;
               for (j = 0; j < ParentData.m_decays.size(); j++) {
                  if (ParentData.m_decays[j].getDaughter() == did) break;
               }
               if (j != ParentData.m_decays.size()) continue;
               std::string gammadata = LegacyFileToID(legacydecay.m_DataFile);
               if (gammadata.length() == 0) continue;
               std::vector<CLegacyGamma>& legacygamma =
                  legacy.m_GamFiles[gammadata].m_ParentGammas;
               ParentData.m_decays.resize(ParentData.m_decays.size()+1);
               // rewrite this later
               CDecayMode& decay = *ParentData.m_decays.rbegin();
               decay.m_fraction = legacydecay.m_Fraction;
               decay.m_daughter = did;
               decay.m_type = -1;
               decay.m_photons.resize(0);
               for (j = 0; j < legacygamma.size(); j++) {
                  // Photons with relative uncertainty >= 100% are deleted
                  if (legacygamma[j].m_fraction == 0 || legacygamma[j].m_uncertainty >= legacygamma[j].m_fraction) continue;
                  double error = legacygamma[j].m_uncertainty/legacygamma[j].m_fraction;
                  error = error * error + systematic_error * systematic_error;
                  CPhoton photon(legacygamma[j].m_energy, legacygamma[j].m_fraction, sqrt(error));
                  decay.m_photons.push_back(photon);
               } // for gammas
               if (CurrentBranch[i].m_BremFile != "NO_FILE" && decay.m_pBrem == 0) {
                  std::string BremID = LegacyFileToID(CurrentBranch[i].m_BremFile);
                  CLegacyCollection::BremDataList::const_iterator itBrem =
                    legacy.m_BremFiles.find(BremID);
                  if (itBrem != legacy.m_BremFiles.end()) {
                     decay.m_pBrem = new CDecayMode::CBrem;
                     decay.m_pBrem->m_nbins = itBrem->second.m_nbins;
                     decay.m_pBrem->m_binbounds = itBrem->second.m_bins;
                     decay.m_pBrem->m_intensity = itBrem->second.m_intensity;
                  }
               }
            } // for decays in branch
         } // for branches
      } // for legacy data
      FixupBranchRatios(isodb);
      FixupAtomicMasses(isodb,legacy);
      return isodb;
   }


   void FixupAtomicMasses(CIsotopeDatabase* isodb, CLegacyCollection& legacy) {
      CIsotopeDatabase::IsotopeList::iterator it;
      it = isodb->m_isotopelist.begin();
      CLegacyCollection::GamDataList::iterator notfound = legacy.m_GamFiles.end();
      while (it != isodb->m_isotopelist.end()) {
         if (it->second->m_atomicmass == 0.0) {
             char legacyid[8];
             sprintf(legacyid,"%s%d",Elements[it->second->m_isotope.getAtomicNumber()-1],it->second->m_isotope.getMassNumber());
             legacyid[1] = toupper(legacyid[1]);
             std::string legacyidstr = legacyid;
             if (legacy.m_GamFiles.find(legacyidstr) != notfound) {
                it->second->m_atomicmass = legacy.m_GamFiles[legacyidstr].m_AtomicMass;
                ++it;
                continue;
             }
             legacyidstr+='A';
             if (legacy.m_GamFiles.find(legacyidstr) != notfound) {
                it->second->m_atomicmass = legacy.m_GamFiles[legacyidstr].m_AtomicMass;
                ++it;
                continue;
             }
             int lastchar = legacyidstr.length()-1;
             legacyidstr[lastchar] = 'B';
             if (legacy.m_GamFiles.find(legacyidstr) != notfound) {
                it->second->m_atomicmass = legacy.m_GamFiles[legacyidstr].m_AtomicMass;
                ++it;
                continue;
             }
             legacyidstr[lastchar] = 'M';
             if (legacy.m_GamFiles.find(legacyidstr) != notfound) {
                it->second->m_atomicmass = legacy.m_GamFiles[legacyidstr].m_AtomicMass;
                ++it;
                continue;
             }
             legacyidstr+='A';
             if (legacy.m_GamFiles.find(legacyidstr) != notfound) {
                it->second->m_atomicmass = legacy.m_GamFiles[legacyidstr].m_AtomicMass;
                ++it;
                continue;
             }
             lastchar = legacyidstr.length()-1;
             legacyidstr[lastchar] = 'B';
             if (legacy.m_GamFiles.find(legacyidstr) != notfound) {
                it->second->m_atomicmass = legacy.m_GamFiles[legacyidstr].m_AtomicMass;
                ++it;
                continue;
             }
         }
         ++it;
      }

   }
   void FixupBranchRatios(CIsotopeDatabase* isodb) { }

}


