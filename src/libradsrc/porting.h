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


#ifndef PORTING_H_INCLUDE
#define PORTING_H_INCLUDE

/*
#define USE_HIGH_PRECISION
*/

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

/*! \define USE_HIGH_PRECISION
 *  Define this to use your system's largest floating type
 */

/*! \typedef HighPrecisionType
 *  Floating-point type used for calculating concentrations of aged isotopes.
 *
 *  This type may be double, doubledouble, long double, or another
 *  type according to platform, the USE_HIGH_PRECISION macro, and the
 *  implementation of porting.h.
 */
#ifdef USE_HIGH_PRECISION
# include <math.h>
  typedef long double HighPrecisionType;
# ifdef _MSC_VER
   inline long double Exp(long double x) { return expl(x); }
# else
   inline long double Exp(long double x) { return exp(x); }
# endif
#else
# include <math.h>
  typedef double HighPrecisionType;
  inline double Exp(double x) { return exp(x); }
#endif

#endif

