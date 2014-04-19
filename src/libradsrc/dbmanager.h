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


#ifndef DBMANAGER_H_INCLUDE
#define DBMANAGER_H_INCLUDE
#include "porting.h"
#include <string>
#include <map>
#include <stdlib.h>
#include <stdio.h>

namespace radsrc
{
   class CIsotopeDatabase;
   /*  -------------------------------------------------------------------- */
   /*! \class CDatabaseManager
    *  \brief Loads databases on demand and provides CIsotopeDatabase objects.
    *
    *  The CDatabase Manager is a singleton that maintains a list of all
    *  loaded databases. A database is identfied by a DatabaseIdentifier,
    *  which is a DatabaseType and a (possibly empty) information string.
    *  This information string will usually be a filename, but is interpreted
    *  by the instantiating routine alone. Instantiated databases are stored
    *  in a mapping of (hopefully) unique databases.
    */
   /*  -------------------------------------------------------------------- */
   class CDatabaseManager
   {
   public:
      // Enums and Typedefs
      //! Types of database formats supported \todo Add ENSDF support with NSDFLIB
      enum DatabaseType {
         LEGACY,        //!< GAMGEN .GAM and .BRM files
         ENSDF,         //!< ENSDF
         ENSDF_ERRATA   //!< ENSDF with a corrections file
      };
      typedef std::pair<int,std::string> DatabaseIdentifier;

      // Public static methods
      static CDatabaseManager* getDatabaseManager(void);
      static CIsotopeDatabase* getIsotopeDatabase(int type, std::string info="");
   private:
      // Properties
      static CDatabaseManager* m_this;
      std::map<DatabaseIdentifier,CIsotopeDatabase*> m_databases;

      // Private constructor / destructor / copy / assign
      CDatabaseManager(void) { }
      ~CDatabaseManager(void);
      CDatabaseManager(const CDatabaseManager&);
      CDatabaseManager& operator=(const CDatabaseManager&);
   };

   inline CDatabaseManager*
   CDatabaseManager::getDatabaseManager()
   {
      if (CDatabaseManager::m_this == 0) {
         CDatabaseManager::m_this = new CDatabaseManager();
      }
      return CDatabaseManager::m_this;
   }
}

#endif
