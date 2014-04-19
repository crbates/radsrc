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


#ifndef CONTRIBUTIONS_H_INCLUDE
#define CONTRIBUTIONS_H_INCLUDE
#include "porting.h"

#include <set>
#include <map>
#include "isotope.h"

namespace radsrc
{
   struct SContribution {
      CIsotope m_isotope;
      double m_decayrate;
      HighPrecisionType m_coefficient;
      SContribution(void) : m_coefficient(0) { }
   };
   
   
   class CBatemanSolution
   {
   public:
      CBatemanSolution(void) { }
      ~CBatemanSolution(void) { }
      const CIsotope& forIsotope(void) const;
      void addContribution(const SContribution& contribution);
      const std::set<CIsotope>& getChainParents(void) const;
      void setIsotope(const CIsotope& isotope);
      void addChainParent(const CIsotope& isotope);
      HighPrecisionType eval(double age) const;
   private:
      std::set<CIsotope> m_chainParents;
      std::map<CIsotope,SContribution> m_terms;
      CIsotope m_whoami;
   };

   inline const CIsotope&
   CBatemanSolution::forIsotope() const {
      return m_whoami;
   }
   inline const std::set<CIsotope>&
   CBatemanSolution::getChainParents() const
   {
      return m_chainParents;
   }
   inline void
   CBatemanSolution::setIsotope(const CIsotope&isotope)
   {
      m_whoami = isotope;
   }
   inline void
   CBatemanSolution::addChainParent(const CIsotope& isotope)
   {
      m_chainParents.insert(isotope);
   }
   inline void
   CBatemanSolution::addContribution(const SContribution& c)
   {
      SContribution& contribution = m_terms[c.m_isotope];
      contribution.m_isotope = c.m_isotope;
      contribution.m_decayrate = c.m_decayrate;
      contribution.m_coefficient += c.m_coefficient;
   }
}

#endif
