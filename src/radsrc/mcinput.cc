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
#include <algorithm>
#include <numeric>
#include "radsource.h"
#include "dbmanager.h"
#include "bins.h"
#include "useriface.h"
#include "mcinput.h"

using namespace radsrc;
using namespace std;

void SetTallyBins(MCInput& mci, const Options& options, const CRadSource& radsource);

inline void CopyValues(std::vector<double>& dest, const std::vector<double>& source) {
   std::vector<double>::const_iterator start, end;
   end = source.end();
   start = source.begin();
   while (end != start && *(end-1) == 0.0) --end;
   dest.assign(start,end);
}

inline void CopyValues(std::vector<double>& dest, const std::vector<double>& source, int n) {
   dest.assign(source.begin(), source.begin()+n);
}
inline double SumArray(std::vector<double> array) {
   return std::accumulate(array.begin(),array.end(),0.0);
}

void PrepareMCInput(const CRadSource& radsource, const Options& options, MCInput& mci) {
   // Copy Discrete
   
   int gamma_bin = options.lines;
   double e;
   CPhotonComputer::CPhotonIterator it, end;
   it = end;
   if (gamma_bin == BIN_NONE) {
      it = radsource.getPhotonComputer().beginGammas();
      end = radsource.getPhotonComputer().endGammas();
   }
   else if (gamma_bin != BIN_ALL) {
      it = radsource.getPhotonComputer().beginSelectedGammas();
      end = radsource.getPhotonComputer().endSelectedGammas();
   }
   while(it != end) {
      e = it->getEnergy();
      if (e < options.tallyu_min || e > options.tallyu_max) {
         ++it;
         continue;
      }
      mci.vGammaEnergies.push_back(e);
      mci.vGammaIntensities.push_back(it->getIntensity());
      ++it;
   }


   // Copy Binned
   CopyValues(mci.vGammaBinIntensities,radsource.getPhotonComputer().getBinnedGammas().m_intensity);
   CopyValues(mci.vGammaBinBoundaries,radsource.getPhotonComputer().getBinnedGammas().m_energy,
      mci.vGammaBinIntensities.size());
   
   // Copy Brem
   if (options.sample_brem) {
      CopyValues(mci.vBremIntensities,radsource.getPhotonComputer().getBrem().m_intensity);
      CopyValues(mci.vBremBinBoundaries,radsource.getPhotonComputer().getBrem().m_energy,
      mci.vBremIntensities.size());
   }
   mci.totals[0] = SumArray(mci.vGammaIntensities);
   mci.totals[1] = SumArray(mci.vGammaBinIntensities);
   mci.totals[2] = SumArray(mci.vBremIntensities);
   mci.totals[3] = mci.totals[0] + mci.totals[1] + mci.totals[2];

   SetTallyBins(mci,options,radsource);
}

void SetTallyBins(MCInput& mci, const Options& options, const CRadSource& radsource) {
   std::vector<double> vtmp;
   int i;
   // Prepare Unscattered Tally bins
   vtmp = mci.vGammaEnergies;
   
   bool has_511 = false, has_2614 = false;
   for (i = 0; i < vtmp.size(); i++) {
      if (vtmp[i] == 511.008) has_511 = true;
      if (vtmp[i] == 2614.3500) has_2614 = true;
   }
   if (!has_511) vtmp.push_back(511.008);
   if (!has_2614) vtmp.push_back(2614.3500);
   std::sort(vtmp.begin(),vtmp.end());
   mci.vTallyUnscattered.push_back(options.tallyu_min);
   for (i = 0; i < vtmp.size(); i++) {
      if (vtmp[i] > options.tallyu_min && vtmp[i] < options.tallyu_max) {
         mci.vTallyUnscattered.push_back(vtmp[i] * 0.999999);
         mci.vTallyUnscattered.push_back(vtmp[i] * 1.000001);
      }
   }
   mci.vTallyUnscattered.push_back(options.tallyu_max);

   // Prepare Scattered Tally bins
   switch (options.tallybin_options.bin_option) {
   case BIN_DEFAULT:
//      mci.vTallyScattered.assign(DefaultBins, DefaultBins+NDEFAULTBINS+1);
      ReadBins(options.tallybin_options.filename, mci.vTallyScattered,
               PF_DEFAULT);
      break;
   case BIN_FILE:
      ReadBins(options.tallybin_options.filename, mci.vTallyScattered,
               PF_SEARCH);
      break;
   case BIN_TART:
      mci.vTallyScattered.assign(TartBins, TartBins+NTARTBINS+1);
      break;
   case BIN_MORSE:
      mci.vTallyScattered.assign(MorseBins, MorseBins+NMORSEBINS+1);
      break;
   case BIN_GADRAS:
      mci.vTallyScattered = GADRASBins();
      break;
   case BIN_EQUISPACE:
      EquiBins(mci.vTallyScattered, options.tallybin_options.min,
         options.tallybin_options.max, options.tallybin_options.nbins);
      break;
    case BIN_SQRT:
      SqrtBins(mci.vTallyScattered,options.tallybin_options);
      break;
   default:
      break;
   }
}

