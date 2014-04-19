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


#ifndef DECAYMODE_H_INCLUDE
#define DECAYMODE_H_INCLUDE

#include "porting.h"
#include <vector>
#include "dbmanager.h"
#include "isotope.h"

namespace radsrc
{
   class COldRadSource;
   class CIsotopeDatabase;
   class CPhotonComputer;

   /*  -------------------------------------------------------------------- */
   /*! \class CPhoton
    *  \brief Basic information about a photon emission line
    */
   /*  -------------------------------------------------------------------- */

   class CPhoton
   {
   friend class COldRadSource; //!< \deprecated for 1.1->1.2 migration only
   public:
      CPhoton(void) : m_energy(0), m_fraction(0), m_ferror(0) { }
      CPhoton(double energy, double fraction, double error=0)
         : m_energy(energy), m_fraction(fraction), m_ferror(error) { }
      double getEnergy(void) const { return m_energy; }
      double getFraction(void) const { return m_fraction; }
      double getError(void) const { return m_ferror; }
   private:
      //! Photon energy in keV
      double m_energy;
      //! Photon probability of emission per decay
      double m_fraction;
      //! Relative uncertainty in m_fraction
      double m_ferror;
   };

   /*  -------------------------------------------------------------------- */
   /*! \class CDecayMode
    *  \brief Class encapsulates information about a decay.
    *
    *  CDecayMode provides information about a decay, including the type of
    *  decay, the resulting daughter nucleus, the branch fraction of this
    *  decay, any photon lines emitted by the decay, and an average
    *  bremmstrahlung spectrum, if available.
    *
    *  \warning Decay type is not yet standardized, and some data sources
    *  may not specify the decay type, or may not be specific enough.
    */
   /*  -------------------------------------------------------------------- */
   class CDecayMode
   {
   //!< \deprecated this global function will be incorporated into a CIsotopeDatabase derived class
   friend CIsotopeDatabase* ConvertLegacy(const CDatabaseManager::DatabaseIdentifier& id);
   friend class CIsotopeDB;
   friend class CPhotonComputer;
   friend class COldRadSource; //!< \deprecated for 1.1->1.2 migration only

   public:
      // typedefs and enums
      //! Type of Decay. For future use.
      enum DecayType {
         UNSPECIFIED,
         ALPHA,
         BETA_GENERIC,
         BETA_MINUS,
         BETA_PLUS,
         ELECTRON_CAPTURE,
         INTERNAL_TRANSITION,
         ALPHANEUTRON,
         BETANEUTRON
      };
      typedef std::vector<CPhoton>::const_iterator PhotonIterator;
      typedef std::vector<double>::const_iterator BremBoundaryIterator;
      typedef std::vector<double>::const_iterator BremIntensityIterator;
   public:
      //! Constructors, etc

      CDecayMode(void);
      ~CDecayMode(void);
      CDecayMode(const CDecayMode& d);
      CDecayMode& operator=(const CDecayMode& d);

      // Public methods

      //! get decay type (possibly unspecified)
      int getDecayType(void) const;
      //! get the daughter isotope of this particular decay
      const CIsotope& getDaughter(void) const;
      //! get the branch fraction of this particular decay
      double getBranchFraction(void) const;
      //! get number of photons produced in decay
      int getNPhotons(void) const;
      //! get first photon entry
      PhotonIterator beginPhotons(void) const;
      //! get last+1 photon entry
      PhotonIterator endPhotons(void) const;
      //! get number of brem bins
      int getNBremBins(void) const;
      //! get first brem bin energy entry
      BremBoundaryIterator beginBremBoundaries(void) const;
      //! get last+1 brem bin energy entry
      BremBoundaryIterator endBremBoundaries(void) const;
      //! get first brem bin intensity entry
      BremIntensityIterator beginBremIntensities(void) const;
      //! get last+1 brem bin intensity entry
      BremIntensityIterator endBremIntensitities(void) const;
   private:

      //! Structure to hold an average bremmstrahlung spectrum
      struct CBrem
      {
         int m_nbins;
         std::vector<double> m_binbounds;
         std::vector<double> m_intensity;
      };
      
      // properties
      int m_type;
      double m_fraction;
      std::vector<CPhoton> m_photons;
      CIsotope m_daughter;
      CBrem *m_pBrem;
      static std::vector<double> m_emptyVector;
   };

   // Inline functions for CDecayMode

   // Constructors etc.
   inline
   CDecayMode::CDecayMode(void) : m_pBrem(0), m_type(UNSPECIFIED)
   {
   }
   inline
   CDecayMode::~CDecayMode(void)
   {
      if (m_pBrem) delete m_pBrem;
   }
   inline
   CDecayMode::CDecayMode(const CDecayMode& d) :
         m_type(d.m_type), m_fraction(d.m_fraction),
         m_daughter(d.m_daughter), m_photons(d.m_photons)
   {
      if (d.m_pBrem) {
         m_pBrem = new CBrem; *m_pBrem = *d.m_pBrem;
      }
      else m_pBrem = 0;
   }
   inline CDecayMode&
   CDecayMode::operator=(const CDecayMode& d)
   {
      m_type = d.m_type; m_fraction = d.m_fraction; m_daughter = d.m_daughter;
      m_photons = d.m_photons;
      if (!m_pBrem) delete m_pBrem;
      if (d.m_pBrem) {
         m_pBrem = new CBrem; *m_pBrem = *d.m_pBrem;\
      }
      else m_pBrem = 0;
      return *this;
   }

   // Member functions
   //! \warning Value is database specific; defaults to CDecayMode::UNSPECIFIED
   inline int
   CDecayMode::getDecayType(void) const
   {
      return m_type;
   }
   inline const CIsotope&
   CDecayMode::getDaughter(void) const
   {
      return m_daughter;
   }
   inline double
   CDecayMode::getBranchFraction(void) const
   {
      return m_fraction;
   }
   inline int
   CDecayMode::getNPhotons(void) const
   {
      return m_photons.size();
   }
   inline CDecayMode::PhotonIterator
   CDecayMode::beginPhotons(void) const
   {
      return m_photons.begin();
   }
   inline CDecayMode::PhotonIterator
   CDecayMode::endPhotons(void) const
   {
      return m_photons.end();
   }
   inline int
   CDecayMode::getNBremBins(void) const
   {
      if (m_pBrem) return m_pBrem->m_nbins; return 0;
   }
   inline CDecayMode::BremBoundaryIterator
   CDecayMode::beginBremBoundaries(void) const
   {
      if (m_pBrem) return m_pBrem->m_binbounds.begin(); return m_emptyVector.begin();
   }
   inline CDecayMode::BremBoundaryIterator
   CDecayMode::endBremBoundaries(void) const
   {
      if (m_pBrem) return m_pBrem->m_binbounds.end(); return m_emptyVector.end();
   }
   inline CDecayMode::BremIntensityIterator
   CDecayMode::beginBremIntensities(void) const
   {
      if (m_pBrem) return m_pBrem->m_intensity.begin(); return m_emptyVector.begin();
   }
   inline CDecayMode::BremIntensityIterator
   CDecayMode::endBremIntensitities(void) const
   {
      if (m_pBrem) return m_pBrem->m_intensity.end(); return m_emptyVector.end();
   }
}
#endif
