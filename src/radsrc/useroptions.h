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


#ifndef USEROPTIONS_H_INCLUDE
#define USEROPTIONS_H_INCLUDE

#include <string>

enum Binning {BIN_NONE=0, BIN_DEFAULT=1, BIN_FILE, BIN_EQUISPACE, BIN_SQRT,
   BIN_TART, BIN_MORSE, BIN_GADRAS, BIN_SAMEGAMMA, BIN_SAMEBREM, BIN_ALL};

struct Options {
   struct SubOptions {
      Binning bin_option;
      std::string filename;
      int nbins;
      double max,min;
      double slope, pedestal;
   };
   SubOptions gammabin_options;
   SubOptions brembin_options;
   SubOptions tallybin_options;
   int lines;
   bool sample_brem;
   bool do_mci;
   double source_min, source_max;
   double tallyu_min, tallyu_max;
   std::string linesfile;
   bool interactive;
   bool quiet;
   std::string configfile;
   mutable std::istream* inputstream;
};

#endif
