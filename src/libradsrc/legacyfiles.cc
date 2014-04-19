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
#include <string.h>

#include "legacyfiles.h"
#include "radsrcerr.h"

/*
 * This file contains functions and data relating to
 * the legacy database as a whole
 */

namespace radsrc {
   const char *DefaultLegacyDataDir = "./data";

   void CLegacyCollection::Read(const CDatabaseManager::DatabaseIdentifier& id) {
      if (id.first != CDatabaseManager::LEGACY) throw CRadSourceException();
      std::vector<std::string> parent_list;
      ReadLegacyIndex(id.second.c_str(),parent_list);
      CLegacyCollection *pCollection = new CLegacyCollection;
      for (int filenum = 0; filenum < parent_list.size(); filenum++) {
         CLegacyData& current_file= m_GamFiles[parent_list[filenum]];
         if (ReadLegacyData(current_file,id.second.c_str(),parent_list[filenum].c_str(),parent_list) == false) {
            m_GamFiles.erase(parent_list[filenum]);
            continue;
         }
         int n_branches = current_file.m_Branches.size();
         for (int i = 0; i < n_branches; i++) {
            int n_members = current_file.m_Branches[i].size();
            for (int j = 0; j < n_members; j++) {
               CLegacyBranchMember& decay = current_file.m_Branches[i][j];
               std::string referenced_file;
               referenced_file = LegacyFileToID(decay.m_DataFile);
               if (referenced_file.length() &&
                  m_GamFiles.find(referenced_file)==m_GamFiles.end()) {
                  ReadLegacyData(m_GamFiles[referenced_file],id.second.c_str(),referenced_file.c_str(),parent_list);
               }
               referenced_file = LegacyFileToID(decay.m_BremFile);
               if (referenced_file.length() &&
                  m_BremFiles.find(referenced_file)==m_BremFiles.end()) {
                  ReadLegacyBrem(m_BremFiles[referenced_file],id.second.c_str(),referenced_file.c_str());
               }
            } // all decays in chain
         } // all branches
      } // all chains
   }

   std::string LegacyFileToID(const char *filename) {
      std::string id = filename;
      if (id == "NO_FILE" || id.length() <= 4) return "";
      id.resize(id.length()-4);
      return id;
   }

   /* Yes, okay, should have been a set, not a vector. This preserves previous behavior, though */
   bool LegacyFileHasChains(const char *name, const std::vector<std::string>& list) {
      int i;
      for (i = 0; i < list.size(); i++) {
         if (list[i] == name) return true;
      }
      return false;
   }

   /*! Parse the id string field, applying defaults, etc.
    *
    *  Leaves ID in a state with all ambiguity removed. Syntax checking,
    *  environment variable substitution, and file existence tests should
    *  be done here. The ID should be left in a state so that this
    *  processing need not be repeated.  If this routine discovers that
    *  the database will not load sucessfully, it should return true.
    *
    *  \return - true if id does not represent a loadable database.
    */
   bool /*static*/
   CLegacyCollection::CanonicalizeId(CDatabaseManager::DatabaseIdentifier& id)
   {
      if (id.first != CDatabaseManager::LEGACY) return true;
      const char *legacydatadir;
      if (id.second == "") {
         legacydatadir = getenv("RADSRC_LEGACYDATA");
         if (legacydatadir == 0) {
            legacydatadir = getenv("RADSRC_DATA");
         }
         if (legacydatadir == 0) {
            legacydatadir = DefaultLegacyDataDir;
         }
         id.second = legacydatadir;
      }
      return false;
   }

}

