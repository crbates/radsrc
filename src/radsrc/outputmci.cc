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


#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <stdio.h>
#include "mcinput.h"

MCNPInputFile::MCNPInputFile() : m_scale(0.001)
{
}

MCNPInputFile::~MCNPInputFile()
{
}

void MCNPInputFile::WriteFile(const std::string& filename, MCInput& InputValues) const {
   std::ofstream fsMCIFile(filename.c_str());
   if (fsMCIFile.is_open() == false) {
      return;
   }

   if (InputValues.header.size()) {
      fsMCIFile << InputValues.header.c_str();
      if (InputValues.vGammaEnergies.size()) {
         fsMCIFile << std::endl << "C " << InputValues.vGammaEnergies.size()
            << " unbinned lines between " << InputValues.vGammaEnergies.front()
            << " keV and " << InputValues.vGammaEnergies.back()
            << " keV" << std::endl;
      }
      fsMCIFile << "C Intensity of Unbinned lines is " << InputValues.totals[MCInput::LINE_TOTAL] << std::endl;
      fsMCIFile << "C Intensity of Binned lines is " << InputValues.totals[MCInput::BINNED_TOTAL] << std::endl;
      fsMCIFile << "C Intensity of Bremmstrahlung " << InputValues.totals[MCInput::BREM_TOTAL] << std::endl;
      fsMCIFile << "C Total intensity of all sources is " << InputValues.totals[MCInput::TOTAL_TOTAL] << std::endl;
      fsMCIFile << "C ========================================================";
      fsMCIFile << std::endl;
   }

   char buffer[256];

   // If we have discrete Gammas
   if (InputValues.vGammaEnergies.size()) {
      sprintf(buffer,"sc  1    DISCRETE GAMMA LINE energies (MeV) -%5d lines:",
         InputValues.vGammaEnergies.size());
      fsMCIFile << buffer << std::endl;
      WriteData(InputValues.vGammaEnergies,fsMCIFile,"SI  1    L", m_scale);
      fsMCIFile << "C " << std::endl;
      fsMCIFile << "SC  1    ASSOCIATED photon intensities (photons/sec/gm):" << std::endl;
      WriteData(InputValues.vGammaIntensities,fsMCIFile,"SP  1    D");
      sprintf(buffer,"%12E",InputValues.totals[MCInput::LINE_TOTAL]);
      fsMCIFile << "C        The integrated intensity for this input type is "
         << buffer << " Photons/s/gm" << std::endl;
      fsMCIFile << "C " << std::endl;
      fsMCIFile << "C " << std::endl;

   }

   // If we have Binned Gammmas
   if (InputValues.vGammaBinBoundaries.size()) {
      sprintf(buffer,"SC  2    Energy boundries (MeV) for BINNED GAMMA LINES -%4d bins:",
       InputValues.vGammaBinBoundaries.size()-1);
      fsMCIFile << buffer << std::endl;
      WriteData(InputValues.vGammaBinBoundaries,fsMCIFile,"SI  2    H", m_scale);
      fsMCIFile << "C " << std::endl;
      fsMCIFile << "SC  2    ASSOCIATED photon intensities (photons/sec/gm):" << std::endl;
      WriteData(InputValues.vGammaBinIntensities,fsMCIFile,"SP  2    D");
      sprintf(buffer,"%12E",InputValues.totals[MCInput::BINNED_TOTAL]);
      fsMCIFile << "C        The integrated intensity for this input type is "
         << buffer << " Photons/s/gm" << std::endl;
      fsMCIFile << "C " << std::endl;
      fsMCIFile << "C " << std::endl;
   }

   // If we have Brem
   if (InputValues.vBremBinBoundaries.size()) {
      sprintf(buffer,"SC  3    BREMSSTRAHLUNG energy bin boundries (MeV) -%4d bins:",
       InputValues.vBremBinBoundaries.size()-1);
      fsMCIFile << buffer << std::endl;
      WriteData(InputValues.vBremBinBoundaries,fsMCIFile,"SI  3    H", m_scale);
      fsMCIFile << "C " << std::endl;
      fsMCIFile << "SC  3    ASSOCIATED photon intensities (photons/sec/gm):" << std::endl;
      WriteData(InputValues.vBremIntensities,fsMCIFile,"SP  3    D");
      sprintf(buffer,"%12E",InputValues.totals[MCInput::BREM_TOTAL]);
      fsMCIFile << "C        The integrated intensity for this input type is "
         << buffer << " Photons/s/gm" << std::endl;
      fsMCIFile << "C" << std::endl;
      fsMCIFile << "C" << std::endl;
   }

   // Tally
   fsMCIFile << "FU104    0 1 nt $ Tally SCATTERED and UNSCATTERED flux separately" << std::endl;;
   fsMCIFile << "C" << std::endl;
   sprintf(buffer,"E104     $ UNSCATTERED FLUX TALLY, energy bin boundries (MeV) -%5d bins:",
      InputValues.vTallyUnscattered.size()-1);
   fsMCIFile << buffer << std::endl;
   WriteData(InputValues.vTallyUnscattered,fsMCIFile,"          ", m_scale);
   fsMCIFile << "C" << std::endl;
   fsMCIFile << "C" << std::endl;
   fsMCIFile << "FU204    0 1 nt $ Tally SCATTERED and UNSCATTERED flux separately" << std::endl;;
   fsMCIFile << "C" << std::endl;
   sprintf(buffer,"E204     $ SCATTERED FLUX TALLY, energy bin boundries (MeV) -%5d bins:",
      InputValues.vTallyScattered.size()-1);
   fsMCIFile << buffer << std::endl;
   WriteData(InputValues.vTallyScattered,fsMCIFile,"          ", m_scale);
   fsMCIFile << "C" << std::endl;
   sprintf(buffer,"%12E",InputValues.totals[MCInput::TOTAL_TOTAL]);
   fsMCIFile << "C       The integrated intensity for ALL INPUT TYPES is " <<
      buffer << " Photons/s/gm" << std::endl;
   fsMCIFile.close();
}

void MCNPInputFile::WriteData(const std::vector<double>& array, std::ostream& os, const char *identifier, double scale) const {
   static const char blank[11] = "          ";
   char buffer[15];
   int i,j;
   os << identifier;
   for (j = 0; j < 4 && j < array.size(); j++) {
      sprintf(buffer," %13.6E", array[j]*scale);
      os << buffer;
   }
   os << std::endl;
   for (i = 4; i < array.size(); i+=4) {
      os << blank;
      for (j = 0; j < 4 && i+j < array.size(); j++) {
        sprintf(buffer," %13.6E", array[i+j]*scale);
        os << buffer;
      }
      os << std::endl;
   }
}

/* -------------------------------------------- */


COGInputFile::COGInputFile() : m_scale(0.001)
{
}

COGInputFile::~COGInputFile()
{
}

void COGInputFile::WriteFile(const std::string& filename, MCInput& InputValues) const {
   std::ofstream fsMCIFile(filename.c_str());
   if (fsMCIFile.is_open() == false) {
      return;
   }

   if (InputValues.header.size()) {
      fsMCIFile << InputValues.header.c_str();
      if (InputValues.vGammaEnergies.size()) {
         fsMCIFile << std::endl << "$ " << InputValues.vGammaEnergies.size()
            << " unbinned lines between " << InputValues.vGammaEnergies.front()
            << " keV and " << InputValues.vGammaEnergies.back()
            << " keV" << std::endl;
      }
      fsMCIFile << "$ Total Intensity of Unbinned lines is " << InputValues.totals[MCInput::LINE_TOTAL] << std::endl;
      fsMCIFile << "$ Total Intensity of Binned lines is " << InputValues.totals[MCInput::BINNED_TOTAL] << std::endl;
      fsMCIFile << "$ Intensity of Bremmstrahlung " << InputValues.totals[MCInput::BREM_TOTAL] << std::endl;
      fsMCIFile << "$ Total intensity of all sources is " << InputValues.totals[MCInput::TOTAL_TOTAL] << std::endl;
      fsMCIFile << "$ ========================================================";
      fsMCIFile << std::endl;
   }

   char buffer[256];
   int deckcounter = 1;

   // If we have discrete Gammas
   if (InputValues.vGammaEnergies.size()) {
      sprintf(buffer,"     DEFINE ENERGY =   %d  PHOTON\nLINE       $%7d Lines", deckcounter,
         InputValues.vGammaEnergies.size());
      fsMCIFile << buffer << std::endl;
      WriteData(InputValues.vGammaEnergies,InputValues.vGammaIntensities,false,fsMCIFile, m_scale);
      fsMCIFile << "$" << std::endl;
      fsMCIFile << "$  " << deckcounter << "    ASSOCIATED photon intensities (photons/sec/gm):" << std::endl;
      sprintf(buffer,"%12E",InputValues.totals[MCInput::LINE_TOTAL]);
      fsMCIFile << "$        The integrated intensity for this input type is "
         << buffer << " Photons/s/gm" << std::endl;
      fsMCIFile << "$ " << std::endl;
      fsMCIFile << "$ " << std::endl;
      deckcounter++;

   }

   // If we have Binned Gammmas
   if (InputValues.vGammaBinBoundaries.size()) {
      sprintf(buffer,"     DEFINE ENERGY =   %d  PHOTON\nBIN       $%7d Bins", deckcounter,
         InputValues.vGammaBinBoundaries.size()-1);
      fsMCIFile << buffer << std::endl;
      WriteData(InputValues.vGammaBinBoundaries,InputValues.vGammaBinIntensities,true,fsMCIFile, m_scale);
      fsMCIFile << "$ " << std::endl;
      fsMCIFile << "$  " << deckcounter << "    ASSOCIATED photon intensities (photons/sec/gm):" << std::endl;
      sprintf(buffer,"%12E",InputValues.totals[MCInput::BREM_TOTAL]);
      fsMCIFile << "$        The integrated intensity for this input type is "
         << buffer << " Photons/s/gm" << std::endl;
      fsMCIFile << "$" << std::endl;
      fsMCIFile << "$" << std::endl;
      deckcounter++;
   }

   // If we have Brem
   if (InputValues.vBremBinBoundaries.size()) {
      sprintf(buffer,"SC  %d    BREMSSTRAHLUNG energy bin boundries (MeV) -%4d bins:", deckcounter,
       InputValues.vBremBinBoundaries.size()-1);
      fsMCIFile << buffer << std::endl;
      WriteData(InputValues.vBremBinBoundaries,InputValues.vBremIntensities,true,fsMCIFile, m_scale);
      fsMCIFile << "$ " << std::endl;
      fsMCIFile << "$  " << deckcounter << "    ASSOCIATED photon intensities (photons/sec/gm):" << std::endl;
      sprintf(buffer,"%12E",InputValues.totals[MCInput::BREM_TOTAL]);
      fsMCIFile << "$        The integrated intensity for this input type is "
         << buffer << " Photons/s/gm" << std::endl;
      fsMCIFile << "$" << std::endl;
      fsMCIFile << "$" << std::endl;
      deckcounter++;
   }

   // Tally
   fsMCIFile
      << "DETECTOR NUMBER = 104    TITLE = UNSCAT BINS" << std::endl
      << "     REACTION" << std::endl
      << "     DRF" << std::endl
      << "     MASKS" << std::endl
      << "     BIN ENERGY = PHOTON" << std::endl
      << std::endl
      << "DETECTOR NUMBER = ***    TITLE = UNSCAT BINS" << std::endl
      << "     REACTION" << std::endl
      << "     DRF" << std::endl
      << "     MASKS" << std::endl
      << "     BIN ENERGY = PHOTON" << std::endl
      << std::endl;

   WriteData(InputValues.vTallyUnscattered,fsMCIFile, m_scale);

   fsMCIFile
      << "DETECTOR NUMBER = 204    TITLE = SCATRD BINS" << std::endl
      << "     REACTION" << std::endl
      << "     DRF" << std::endl
      << "     MASKS" << std::endl
      << "     BIN ENERGY = PHOTON" << std::endl
      << std::endl
      << "DETECTOR NUMBER = 100    TITLE = SCATRD BINS" << std::endl
      << "     REACTION" << std::endl
      << "     DRF" << std::endl
      << "     MASKS" << std::endl
      << "     BIN ENERGY = PHOTON" << std::endl
      << std::endl;

   WriteData(InputValues.vTallyScattered,fsMCIFile, m_scale);

   fsMCIFile << "$" << std::endl;
   sprintf(buffer,"%12E",InputValues.totals[MCInput::TOTAL_TOTAL]);
   fsMCIFile << "$       The integrated intensity for ALL INPUT TYPES is " <<
      buffer << " Photons/s/gm" << std::endl;
   fsMCIFile.close();
}

void COGInputFile::WriteData(const std::vector<double>& array, std::ostream& os, double scale) const {
   char buffer[15];
   int i,j;
   for (i = 0; i < array.size(); i+=4) {
      os << "            ";
      for (j = 0; j < 4 && i+j < array.size(); j++) {
        sprintf(buffer," %13.6E", array[i+j]*scale);
        os << buffer;
      }
      os << std::endl;
   }
}
void COGInputFile::WriteData(const std::vector<double>& array1, const std::vector<double>& array2, bool binned, std::ostream& os, double scale) const {
   char buffer[40];
   int i,j;

   int n = array1.size();
   if (array2.size() != n) {
      std::cerr << "Array length mismatch...not written" << std::endl;
      return;
   }
   for (i = 0, j = binned?1:0; i < n; i++,j++) {
      if (j < n) sprintf(buffer, " %13.6E %13.6E", array1[i] * scale, array2[j]);
      else sprintf(buffer, " %13.6E", array1[i] * scale);
      os << buffer << std::endl;
   }
}

