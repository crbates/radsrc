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


/* Classes for specifing a radioactive mixture and aging it */
#ifndef DECAYCOMPUTER_H_INCLUDE
#define DECAYCOMPUTER_H_INCLUDE

#include <map>
#include <string>
#include <istream>
#include "porting.h"
#include "isotopedata.h"
#include "isotopemix.h"
#include "radsrcerr.h"
#include "batemansolution.h"

namespace radsrc
{
   /*! \struct CDecay
    * Link in the decay graphs generated by CDecayCompute::computeDecayChains
    */
   struct CDecay {
      CIsotope m_Parent;
      const CDecayMode *m_pDecay;
   };
    
   typedef std::vector<CDecay> DecayChain;
   typedef std::vector<DecayChain> DecayChainSet;
   
   class CIsotopeDatabase;
   class CContributions;

   /*  -------------------------------------------------------------------- */
   /*! \class CDecayComputer
    *  \brief Compute the radioisotope concentration function wrt time
    */
   /*  -------------------------------------------------------------------- */
   class CDecayComputer
   {
   public:
      // Typedefs and Enums
      //! Isotope & concentration
      typedef std::map<CIsotope,CBatemanSolution> Solution;
      
      enum {ATOM_FRACTION, MASS_FRACTION, GRAMS}; //! Possible input units
      
      // Constructors etc.
      CDecayComputer(void);
      ~CDecayComputer(void);
      CDecayComputer(const CDecayComputer&);
      CDecayComputer& operator=(const CDecayComputer&);
      
      // Public Methods
      
      //! \brief Convert input to canonical units
      void normalizeInputUnits(void);
      
      //! Append a radioisotope to the input mixture list.
      void addInputItem(const CIsotope& iso, double amount);
      
      //! Reset the object
      void clear();
      
      //! Compute the isotope concetrations at a particular age.
      CIsotopeMixture& ageMixture(double age);

      //! Get the detailed solution
      const std::map<CIsotope,CBatemanSolution>& getFullSolution(void) const;

      //! Get the detailed solution for an Isotope
      const CBatemanSolution* getSolution(const CIsotope& isotope) const;
      
      //! Get mixture at a particular time
      const CIsotopeMixture& getAgedMixture(void) const;

      //! Get mixture at a particular time
      const CIsotopeMixture& getInputMixture(void) const;

      //! Get average atomic mass
      double getAverageAtomicMass(void) const;

      //! Look up some data in the current database
      const CIsotopeData* getIsotopeData(const CIsotope& isotope) const;

      //! Get the current database
      const CIsotopeDatabase* getIsotopeDatabase(void) const;

      //! Set the database to be use; Resets the object
      void initialize(const CIsotopeDatabase* pisotopedb);

      //! Express decay graph as vector of linear chains
      DecayChainSet& computeDecayChains(const CIsotope&) const;

      //! Get the age of the mixture (seconds)
      double getAge(void) const;

      static const double m_secs_per_year;
   private:
      
      // Properties
      //! Concentrations of isotopes at time = 0
      CIsotopeMixture m_inputMixture;
      CIsotopeMixture m_agedMixture;

      //! Full Bateman solution for all isotopes, with parent info
      Solution m_solution;

      //! Pointer to the current isotope database in use
      const CIsotopeDatabase* m_database;
      int m_inputMixtureUnits;
      double m_age;
      
      // Typedefs and Enums
      class CPathInfo;
      typedef std::vector<CPathInfo> ChainPath;
      
      // Implementation Methods
      void buildAgingChain(double age, ChainPath& helper, CIsotope iso, double);
      void computeAddAgedIsotope(double age, const CIsotope&, double, ChainPath&);
      //! Express decay graph as vector of linear chains
      void buildChain(DecayChainSet&, CIsotope) const;


   };
   
   inline double CDecayComputer::getAge() const
   {
      return m_age;
   }
   inline const CDecayComputer::Solution&
   CDecayComputer::getFullSolution() const
   {
      return m_solution;
   }
   inline const CIsotopeMixture&
   CDecayComputer::getAgedMixture() const
   {
      return m_agedMixture;
   }
   inline const CIsotopeMixture&
   CDecayComputer::getInputMixture() const
   {
      return m_inputMixture;
   }
   inline const CBatemanSolution*
   CDecayComputer::getSolution(const CIsotope& isotope) const
   {
      Solution::const_iterator findsolution = m_solution.find(isotope);
      if (findsolution != m_solution.end()) {
         return &(findsolution->second);
      }
      return 0;
   }
   inline void CDecayComputer::initialize(const CIsotopeDatabase* pisotopedb)
   {
      m_database = pisotopedb;
      m_agedMixture.clear();
      m_solution.clear();
      m_age = 0;
      m_inputMixtureUnits = ATOM_FRACTION;
      m_inputMixture.setDatabase(pisotopedb);
      m_agedMixture.setDatabase(pisotopedb);
   }
   inline const CIsotopeDatabase*
   CDecayComputer::getIsotopeDatabase() const
   {
      return m_database;
   }
}

#endif

