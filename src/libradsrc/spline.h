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


/* C++ wrapper for cubic spline routines
 * Limitations:
 *   minimum 3 points
 *   "not-a-knot" boundary conditions only
 */

#ifndef SPLINE_H_INCLUDE
#define SPLINE_H_INCLUDE

#include <vector>

namespace radsrc {
   class CSpline {
   public:
      typedef std::vector<double> ValArray;
      CSpline(void) : m_valid(false) { }
      ~CSpline(void) { }
   //
      void Clear(void) { m_Coeffs.clear(); m_valid = false; }
      bool AttachValues(const ValArray& x, const ValArray& y) {
         if (x.size() < 3 || x.size() != y.size()) Clear();
         else { m_pX = &x; m_pY = &y; CubicSpline(); }
         return m_valid;
      }
      double operator()(double x) const { return Evaluate(x); }
      double Evaluate(double x) const;
      bool IsValid(void) const { return m_valid; }
   //
   private:
      struct _coeffs {
         double c[3];
      };
      const ValArray *m_pX, *m_pY;
      std::vector<_coeffs> m_Coeffs;
      bool m_valid;
      const double& X(int i) const { return (*m_pX)[i]; }
      const double& Y(int i) const { return (*m_pY)[i]; }
      const double& C(int p, int i) const {
         if (i == 0) throw; return m_Coeffs[p].c[i-1];
      } 
      double& C(int p, int i) {
         if (i == 0) throw; return m_Coeffs[p].c[i-1];
      } 
      void CubicSpline(void);
      int Interval(double x) const;
   };
}
#endif

