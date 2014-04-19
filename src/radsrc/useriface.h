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


#ifndef USERIFACE_H_INCLUDE
#define USERIFACE_H_INCLUDE

#include <iosfwd>
#include "porting.h"
#include "isotope.h"
#include "useroptions.h"


struct Mixture {
  radsrc::CIsotope isotope;
  double fraction;
};

typedef std::vector<Mixture> Source;

void InputSources(Source& source);
void ParseCommandLine(int argc, const char* argv[], Options& options);
void GetOptions(Options& options, std::ostream& input_log);
void GetString(std::istream* is, std::string& str, bool interactive, const char *msg, const char *def_file, std::ostream& input_log);
void GetLine(std::istream* is, std::string& str);

enum PathFlags { PF_DEFAULT=1, PF_SEARCH=2 };
void OpenDatFile(std::ifstream& is, const char* filename, PathFlags pf);
void ReadBins(const std::string& filename, std::vector<double>& v, PathFlags pf);
void ReadLineFile(std::vector<double>& v, std::string filename, PathFlags pf);


#endif


