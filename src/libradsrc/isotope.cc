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

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "isotope.h"
#include "elements.h"

namespace radsrc {
   CIsotope& CIsotope::fromString(const char *str) {
      int i;
      i = 0;
      while (isalpha(str[i])) i++;
      
      for (Z = 0; Z < NElements; Z++) {
         if (strncmp(Elements[Z],str,i)==0
            || strncmp(ElementsUpper[Z],str,i)==0
            || strncmp(ElementsLower[Z],str,i)==0 ) break;
      }
      if (Z == NElements) {
         Z = A = m = 0;
         return *this;
      }
      Z++;
      if (str[i] == '-') i++;
      if (str[i] == '\0') {
         Z = A = m = 0;
         return *this;
      }
      A = atoi(str+i);
      if (A >= 100) i+=3;
      else if (A >= 10) i+=2;
      else i++;
      m = 0;
      if (str[i] == 'm') {
         i++;
         m = 1;
         if (str[i] != '\0') {
            m = atoi(str+i);
         }
      }
      return *this;
   }

   void CIsotope::toString(char *str) const {
     if (m == 0) {
       sprintf(str,"%s-%d",Elements[Z-1],A);
     } else if (m == 1)  {
       sprintf(str,"%s-%dm",Elements[Z-1],A);
     } else {
       sprintf(str,"%s-%dm%1d",Elements[Z-1],A,m);
     }
   }
}

