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


/* Cubic spline routines
 *
 * Derived from cubspl.f and ppvalu.f by Carl de Boor, U. of Wisc.
 * Published in Practical Guide to Splines, released to Public Domain
 * Available from netlib.org and http://www.cs.wisc.edu/~deboor
 *
 * This is reduced-functionality port from FORTRAN to C of the
 * aformentioned routines. Data must contain at least 3 points
 * and only the not-a-knot boundary condition is supported.
 */

#include "spline.h"
#include <iostream>

namespace radsrc {
   void CSpline::CubicSpline() {
      int n = m_pX->size();
      int i,j,l,m;
      double g,dtau,divdf1,divdf3;
      m_Coeffs.resize(n);
      l = n-1;
      for (m = 1; m < n; m++) {
         C(m,2) = X(m) - X(m-1);
         C(m,3) = (Y(m) - Y(m-1)) / C(m,2);
      }
      C(0,3) = C(2,2);
      C(0,2) = C(1,2) + C(2,2);
      C(0,1) = ((C(1,2) + 2*C(0,2))*C(1,3)*C(2,2)+C(1,2)*C(1,2)*C(2,3))/C(0,2);

      for (m = 1; m < l; m++) {
         g = -C(m+1,2) / C(m-1,3);
         C(m,1) = g*C(m-1,1) + 3*(C(m,2) * C(m+1,3) + C(m+1,2) * C(m,3));
         C(m,3) = g*C(m-1,2) + 2*(C(m,2) +C(m+1,2));
      }

      g = C(l-1,2) + C(l,2);
      C(l,1) = (( C(l,2) + 2*g) * C(l,3) * C(l-1,2) +C(l,2)*C(l,2) * (Y(l-1)-Y(l-2))/C(l-1,2))/g;
      g = -g / C(l-1,3);
      C(l,3) = C(l-1,2);

      C(l,3) = g*C(l-1,2) + C(l,3);
      C(l,1) = (g*C(l-1,1) + C(l,1)) / C(l,3);

      j = l-1;
      while (j >= 0) {
         C(j,1) = (C(j,1) - C(j,2)*C(j+1,1))/C(j,3);
         j--;
      }
      for (i = 1; i < n; i++) {
         dtau = C(i,2);
         divdf1 = (Y(i) - Y(i-1)) / dtau;
         divdf3 = C(i-1,1) + C(i,1) - 2 * divdf1;
         C(i-1,2) = 2*(divdf1-C(i-1,1)-divdf3)/dtau;
         C(i-1,3) = (divdf3/dtau) * (6/dtau);
      }
      m_valid = true;
      return;
   }

   int CSpline::Interval(double x) const {
      static int ilo = 0;
      const ValArray& brk = *m_pX;
      int lxt = brk.size();
      if (ilo < lxt-1 && x >= brk[ilo]) {
         if (x < brk[ilo+1]) return ilo;
      } else {
         ilo = 0;
      }
      if (x < brk[1]) return 0;
      if (x >= brk.rbegin()[1]) return brk.size()-2;
      int ihi = lxt - 1;
      while (ihi != ilo+1) {
         int middle = (ihi + ilo)/2;
         if (x < brk[middle]) ihi = middle;
         else if (x > brk[middle]) ilo = middle;
         else return middle;
      }
      return ilo;
   }
      


   double CSpline::Evaluate(double x) const {
      if (!m_valid) return 0;
      int i = Interval(x);
      double h = x - (*m_pX)[i];
      return ((C(i,3) / 3.0 * h + C(i,2))/2.0 * h + C(i,1)) * h + Y(i);
   }

      
}
