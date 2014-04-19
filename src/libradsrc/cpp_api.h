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


#ifndef CPP_API_H_INCLUDE
#define CPP_API_H_INCLUDE


#include "porting.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>


namespace radsrc {

   class CRadSource;

/*! \class CApi
 *  This class provides a simplified interface for using
 *  the radsource library to compute radioactive decay
 *  and radioactive emissions. This is identical to the
 *  libradsrc FORTRAN interface.
 *
 *  Initialization will generally follow one of the following forms:
 *  \code
 *  CRadSource* handle = newSource();
 *  addIsotope(HANDLE,92,238,0,100.0)
 *  bool success = mix(HANDLE,25.0)
 *  \endcode
 *  \code
 *  Handle handle = newSource();
 *  bool success = loadConfig(handle,"config.txt")
 *  \endcode
 *  \code
 *  Handle handle = newSource();
 *  while(configstring) {
 *    addConfig(handle,configstring);
 *  }
 *  bool success = sourceConfig(handle)
 *  \endcode
 */
   class CApi
   {
   public:
      //! Create a new radiation decay problem
      static CRadSource* newSource(void);

      //! Load the configuration from a file, or from a default location and perform the calculations.
      //! \param[in] filename File to load, or empty string for default.
      static int loadConfig(CRadSource* pRadSource, const std::string& filename);

      //! Add an isotope to the input mixture
      static void addIsotope(CRadSource* pRadSource, int z, int a, int m, double perc);
   
      //! Initiate the decay computation of previously specified input mixture.
      //! \param[in] age age in years
      static int mix(CRadSource* pRadSource, double age);

      //! Sort the photon list by energy or intensity.
      //! \param[in] field CPhotonComputer::Energy or CPhotonComputer::Intensity
      static void sort(CRadSource* pRadSource, int field);

      //! Get the number of discrete photons.
      static int nLines(const CRadSource* pRadSource);

      //! Get the first nmax (energy,intensity) discrete photon entries.
      static void getLines(CRadSource* pRadSource, double lines[][2], int nmax);

      //! Get a random photon energy in keV.
      static double getPhoton(const CRadSource* pRadSource, double (*prng)(void));

      //! Enable/Disable sampling of the bremmstrahlung distribution (if present)
      static void sampleBrem(CRadSource* pRadSource, bool f);

      //! Get a random 4-vector E,px,py,pz in natural units (keV).
      static void get4V(const CRadSource* pRadSource, double e[4], double (*prng)(void));

      //! Get a random photon energy in keV.
      static double getPhoton(const CRadSource* pRadSource, float (*prng)(void));

      //! Get a random 4-vector E,px,py,pz in natural units (keV).
      static void get4V(const CRadSource* pRadSource, double e[4], float (*prng)(void));

      //! Store a summary into a character variable.
      static std::string getReport(const CRadSource* pRadSource);

      //! Add to the growing string of configuration information.
      static void addConfig(CRadSource* pRadSource, const std::string& input);

      //! Parse the configuration information and perform the calculations.
      static int sourceConfig(CRadSource* pRadSource);
   };
}
#endif

