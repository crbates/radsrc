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


#include "dbmanager.h"
#include "radsrcerr.h"
#include "isotope.h"
#include "isotopedb.h"
#include "legacyglue.h"

namespace radsrc
{
   CDatabaseManager* CDatabaseManager::m_this = 0;

   /*static*/ CIsotopeDatabase*
   CDatabaseManager::getIsotopeDatabase(int type,std::string info/*=""*/)
   {
      getDatabaseManager();
      // Look up existing identical database
      DatabaseIdentifier id(type,info);
      std::map<DatabaseIdentifier,CIsotopeDatabase*>::const_iterator finddb;
      finddb = m_this->m_databases.find(id);
      if (finddb != m_this->m_databases.end()) {
         return finddb->second;
      }

      // Canonicalize ID (environment lookups, disk checks, etc)
      // CanonicalizeId() should return true if it knows in
      // advance (due to file existence checks or syntax errors in info
      // that the actual call to load the database will fail.
      // Otherwise it should return false and change id such that
      // it will require no further processing.
      bool load_will_fail = false;
      switch (type) {
      case CDatabaseManager::LEGACY:
         load_will_fail = radsrc::CanonicalizeId(id);
         break;
      default:
         return 0;
      }

      if (load_will_fail) {
         return 0;
      }

      // Look it up again
      finddb = m_this->m_databases.find(id);
      if (finddb != m_this->m_databases.end()) {
         return finddb->second;
      }

      CIsotopeDatabase *ptr;

      // Database creation will throgh CRadSourceException on failure
      try {
         switch (type) {
         case CDatabaseManager::LEGACY:
            ptr = ConvertLegacy(id);
            break;
         default:
            throw CRadSourceException(); // not reachable
         }
      }
      catch(...) {
         return 0;
      }

      m_this->m_databases[id] = ptr;
      return ptr;
   }

   CDatabaseManager::~CDatabaseManager()
   {
      std::map<DatabaseIdentifier,CIsotopeDatabase*>::iterator it;
      it = m_databases.begin();
      while (it != m_databases.end()) {
         delete it->second;
      }
      m_databases.clear();
      m_this = 0;
   }

}
