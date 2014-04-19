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
#include <map>
#include "isotopemix.h"
#include "isotopedb.h"
#include "radsrcerr.h"

namespace radsrc
{
   CIsotopeMixture::CIsotopeMixture(void) : BaseType(), m_pDatabase(0)
   {
   }
   CIsotopeMixture::~CIsotopeMixture(void)
   {
   }
   CIsotopeMixture::CIsotopeMixture(const CIsotopeMixture& im) : BaseType(im), m_pDatabase(im.m_pDatabase)
   {
   }
   CIsotopeMixture CIsotopeMixture::operator=(const CIsotopeMixture& im)
   {
      *static_cast<BaseType*>(this) = *static_cast<const BaseType*>(&im);
      m_pDatabase = im.m_pDatabase;
      return *this;
   }

   double
   CIsotopeMixture::computeAverageMass() const
   {
      double average = 0;
      const_iterator it, e;
      it = begin();
      e = end();
      if (m_pDatabase == 0) {
         throw CRadSourceException();
      }
      while(it != e) {
         const CIsotopeData* pIsotope = m_pDatabase->getIsotopeData(it->first);
         if (pIsotope == 0) {
            throw CRadSourceException();
         }
         average += it->second * pIsotope->getAtomicMass();
         ++it;
      }
      return average;
   }
}
