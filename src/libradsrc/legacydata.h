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


/* legacy.h
 * 
 * Structures to hold the information in a legacy .GAM file
 * 
 * Created 7/24/2006 Larry Hiller
 *
 */ 

#ifndef LEGACY_H_INCLUDE
#define LEGACY_H_INCLUDE

#include <vector>
#include <string>

namespace radsrc {
   struct CLegacyGamma {
      double m_energy; // keV
      double m_fraction;
      double m_uncertainty;
      char m_comment[6];
      CLegacyGamma(void) { m_comment[0] = m_comment[1] = m_comment[2] =
         m_comment[3] = m_comment[4] = m_comment[5] = '\0';
      }
   };

   struct CLegacyBrem {
       std::vector<double> m_bins;
       std::vector<double> m_intensity;
       int m_nbins;
   };

   struct CLegacyBranchMember {
       char m_Isotope[8];
       char m_DataFile[13];
       char m_BremFile[13];
       double m_HalfLife; // sec
       double m_HLUncertainty;
       int m_Type;  // alpha or beta or IT or stable;
       double m_Fraction;
       double m_FracUncertainty;
       bool m_fAdd;
   };

   struct CLegacyData {
      char m_Isotope[8];
      double m_AtomicMass;
      double m_systematic;
      std::vector<CLegacyGamma> m_ParentGammas;
      std::vector<std::vector<CLegacyBranchMember> > m_Branches;
   };

   bool ReadLegacyData(CLegacyData& legacydata, const char *dir, const char *isotope, const std::vector<std::string>& parent_list);
   bool ReadLegacyBrem(CLegacyBrem& legacybrem, const char *dir, const char *isotope);
   inline CLegacyData* ReadLegacyData(const char *dir, const char *isotope, const std::vector<std::string>& list) {
      CLegacyData *ptr = new CLegacyData;
      if (ptr && ReadLegacyData(*ptr,dir,isotope,list)) return ptr;
      return 0;
   }
   void ReadLegacyIndex(const char *, std::vector<std::string>&); 
}
#endif

