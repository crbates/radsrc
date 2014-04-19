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


#ifndef ISOTOPE_DATA_H_INCLUDE
#define ISOTOPE_DATA_H_INCLUDE

#include <math.h>
#include "isotope.h"
#include "decaymode.h"

namespace radsrc
{
   class CLegacyCollection;
   class COldRadSource;
   /*  -------------------------------------------------------------------- */
   /*! \class CIsotopeData
    *  \brief Provides mass, decays, photons, and perhaps other information on an isotope.
    *
    *
    *  \todo Refactor to base and derived class as this is perhaps only one way to store information.
    */
   /*  -------------------------------------------------------------------- */

   class CIsotopeData
   {
      //! \todo Remove Legacy and COldRadSource friendship from CIsotopeData
      // Friendship
      friend class CIsotopeDatabase; // granted for DB creation
      // Legacy routines to be moved to derived class of CIsotopeDatabase
      friend CIsotopeDatabase* ConvertLegacy(const CDatabaseManager::DatabaseIdentifier& id);
      friend void FixupAtomicMasses(CIsotopeDatabase* isodb, CLegacyCollection& legacy);
      // COldRadSource is deprecated and will disapppear in v1.2
      friend class COldRadSource;

   public:
      // Typedefs and enums

      //! Iterator for enumerating all decay modes
      typedef std::vector<CDecayMode>::const_iterator DecayIterator;

      // Constructors, etc.

      //! Blank data
      CIsotopeData(void) : m_halflife(0.0), m_atomicmass(0.0) { }
      //! Blank data for a specified isotope
      CIsotopeData(const CIsotope& iso) : m_halflife(0.0), m_atomicmass(0.0), m_isotope(iso) { iso.toString(m_name); }
      //! Fully specified with isotope properties
      CIsotopeData(const CIsotope& iso, double hl, double m) : m_halflife(hl), m_atomicmass(m), m_isotope(iso) { iso.toString(m_name); }
      // Default Destructor, copy, assignment

      // Public Methods

      //! \brief Get the decay rate of a particular branch
      double getDecayRate(int branch) const;
      //! get the decay rate of the isotope
      double getDecayRate(void) const;
      //! Get the decay rate of a particular branch
      double getDecayRate(const DecayIterator& it) const;
      //! begin iterator for the isotope decays
      DecayIterator decaysBegin(void) const;
      //! end iterator for the isotope decays
      DecayIterator decaysEnd(void) const;
      //! the number of decay modes for this isotope
      int getNDecayModes(void) const;
      //! the isotope identifier
      const CIsotope& getIsotope(void) const;
      //! the isotope name
      const char* getName(void) const;
      //! the average atomic mass
      double getAtomicMass(void) const;
      //! the halflife
      double getHalflife(void) const;

   private:
      // Properties

      //! the isotope identifier.
      CIsotope m_isotope;
      //! the standard isotope name.
      char m_name[10]; // e.g. Uxx-YYYmN, N=nul,1,2
      //! the halflife in seconds.
      double m_halflife;
      //! the atomic weight in grams.
      double m_atomicmass;
      //! vector of all of the decays of the isotope.
      std::vector<CDecayMode> m_decays;

   };

   // Inline functions
   //!
   //! Get the decay rate of a particular branch.
   //! \param[in] branch select branch (no range checking).
   //! \todo remove getDecayRate(int) along with COldRadSource.
   inline
   double
   CIsotopeData::getDecayRate(int branch) const
   {
      return log(2.0) / m_halflife * m_decays[branch].getBranchFraction();
   }
   inline
   double
   CIsotopeData::getDecayRate(void) const
   {
      return log(2.0) / m_halflife;
   }
   //! \param[in] it select branch pointed to by iterator.
   inline
   double
   CIsotopeData::getDecayRate(const DecayIterator& it) const
   {
      return getDecayRate() * it->getBranchFraction();
   }
   //! \return an iterator pointing to the first decay mode.
   inline
   CIsotopeData::DecayIterator
   CIsotopeData::decaysBegin(void) const
   {
      return m_decays.begin();
   }
   //! \return an iterator pointing beyond the last decay mode.
   inline
   CIsotopeData::DecayIterator
   CIsotopeData::decaysEnd(void) const
   {
      return m_decays.end();
   }
   //! \return the number of decay recorded for this isotope.
   inline
   int
   CIsotopeData::getNDecayModes(void) const
   {
      return m_decays.size();
   }
   //! \return the isotope identifier object for this CIsotopeData object.
   inline
   const CIsotope&
   CIsotopeData::getIsotope(void) const
   {
      return m_isotope;
   }
   //! \return the standard name of this isotope a nul-terminated C string.
   inline
   const char*
   CIsotopeData::getName(void) const
   {
      return m_name;
   }
   //! \return the average atomic mass in grams.
   inline
   double
   CIsotopeData::getAtomicMass(void) const
   {
      return m_atomicmass;
   }
   //! \return the halflife in seconds.
   inline
   double
   CIsotopeData::getHalflife(void) const
   {
      return m_halflife;
   }

}
#endif
