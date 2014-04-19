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


#ifndef LEGACYFILES_H
#define LEGACYFILES_H
#include <map>
#include <string>
#include <vector>
#include "legacydata.h"
#include "dbmanager.h"

namespace radsrc {
   bool LegacyFileHasChain(const char *, const std::vector<std::string>&);
   void ReadIndex(const char *, std::vector<std::string>&);
   bool IsLegacyFile(const char *,int);
   class CLegacyCollection {
   public:
      typedef std::map<std::string, CLegacyData> GamDataList;
      typedef std::map<std::string, CLegacyBrem> BremDataList;
      GamDataList m_GamFiles;
      BremDataList m_BremFiles;
      static const char *GamFileList[];
      static const char *BremFileList[];
      static bool CanonicalizeId(CDatabaseManager::DatabaseIdentifier& id);
      void Read(const CDatabaseManager::DatabaseIdentifier& id);
   };

   std::string LegacyFileToID(const char *);
}
#endif
