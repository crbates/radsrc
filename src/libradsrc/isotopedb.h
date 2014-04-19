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


/* Classes for maintaining a DB of Isotope Data */

#ifndef ISOTOPEDB_H_INCLUDE
#define ISOTOPEDB_H_INCLUDE

#include "porting.h"
#include <map>
#include "isotopedata.h"
#include "dbmanager.h"
#include "radsrcerr.h"

namespace radsrc {
   // functions and classes that populate CIsotopeDatabases
   class CLegacyCollection;
   CIsotopeDatabase* ConvertLegacy(const CDatabaseManager::DatabaseIdentifier& id);
   void FixupAtomicMasses(CIsotopeDatabase* isodb, CLegacyCollection& legacy);

   /*  -------------------------------------------------------------------- */
   /*! \class CIsotopeDatabase
    *  \brief Encapsulates a collection of CIsotopeData *
    *
    *  Stores CIsotopeData * to allow polymorphism. CIsotopeDatabase is filled
    *  by friend functions that obtain their information from specific
    *  data sources.
    *
    *  \todo Make CIsotopeDatabase a base class for various DB sources.
    */
   /*  -------------------------------------------------------------------- */
   class CIsotopeDatabase {
   // It is highly likely that these friend relationships will become inheritance
   // relationships and public methods will become virtual.
   friend CIsotopeDatabase* ConvertLegacy(const CDatabaseManager::DatabaseIdentifier& id);
   friend void FixupAtomicMasses(CIsotopeDatabase* isodb, CLegacyCollection& legacy);
   public:
      // Typedefs and Enums

      //! Mapping of isotope identifiers to the corresponding isotope data.
      typedef std::map<CIsotope, CIsotopeData*> IsotopeList;
      //! Iterator for the isotopes stored in CIsotopeDatabase.
      typedef std::map<CIsotope, CIsotopeData*>::const_iterator IsotopeListIterator;

      // Constructors, etc

      CIsotopeDatabase(void);
      ~CIsotopeDatabase(void);

      // Public methods

      //! Get data for an isotope
      const CIsotopeData* getIsotopeData(const CIsotope& iso) const;
      //! Indicates isotope exists in database
      bool hasIsotopeData(const CIsotope& iso) const;
      //! Iterator to first isotope in database
      IsotopeListIterator isotopesBegin(void) const;
      //! Iterator after last isotope in database
      IsotopeListIterator isotopesEnd(void) const;
      //! Number of isotopes(isomers) in database
      int getNIsotopes(void) const;

   private:
      // Copy and Assignment

      // Databases are intended to be unique
      //! Copy disallowed
      CIsotopeDatabase(const CIsotopeDatabase&);
      //! Assignment disallowed
      CIsotopeDatabase& operator=(const CIsotopeDatabase&);

      // Implementation
      //! Non-const accessor function for DB creation
      CIsotopeData& accessIsotopeData(const CIsotope& iso);

      // Properties
      //! Mapping of CIsotope containing pointers to CIsotopeData objects.
      IsotopeList m_isotopelist;
   };

   inline
   CIsotopeDatabase::IsotopeListIterator
   CIsotopeDatabase::isotopesBegin(void) const
   {
      return m_isotopelist.begin();
   }
   inline
   CIsotopeDatabase::IsotopeListIterator
   CIsotopeDatabase::isotopesEnd(void) const
   {
      return m_isotopelist.end();
   }
   inline
   int
   CIsotopeDatabase::getNIsotopes(void) const
   {
      return m_isotopelist.size();
   }
   //! \return a pointer to a CIsotopeData object,
   //! or 0 if isotope is not in the database.
   //! \param[in] iso isotope to look up in the database.
   inline
   const CIsotopeData*
   CIsotopeDatabase::getIsotopeData(const CIsotope& iso) const
   {
      IsotopeListIterator findiso = m_isotopelist.find(iso);
      if (findiso != m_isotopelist.end()) {
         return findiso->second;
      }
      return 0;
   }
   //! \param[in] iso isotope to look up in the database.
   inline
   bool
   CIsotopeDatabase::hasIsotopeData(const CIsotope& iso) const
   {
      return m_isotopelist.find(iso) != m_isotopelist.end();
   }
   //! \param[in] iso isotope to look up in the database.
   //! Creates a default entry if isotope is not in DB
   inline
   CIsotopeData&
   CIsotopeDatabase::accessIsotopeData(const CIsotope& iso)
   {
      IsotopeListIterator findiso = m_isotopelist.find(iso);
      if (findiso != m_isotopelist.end()) {
         return *(findiso->second);
      }
      CIsotopeData* ptr = m_isotopelist[iso] = new CIsotopeData(iso);
      return *ptr;
   }
}
#endif
