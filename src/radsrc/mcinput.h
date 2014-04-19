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


#ifndef MCINPUT_H_INCLUDE
#define MCINPUT_H_INCLUDE

#include <string>
#include <vector>

struct MCInput {
  std::vector<double> vGammaEnergies;
  std::vector<double> vGammaIntensities;
  std::vector<double> vGammaBinBoundaries;
  std::vector<double> vGammaBinIntensities;
  std::vector<double> vBremBinBoundaries;
  std::vector<double> vBremIntensities;
  std::vector<double> vTallyUnscattered;
  std::vector<double> vTallyScattered;
  std::string header;
  double totals[4];
  enum {LINE_TOTAL, BINNED_TOTAL, BREM_TOTAL, TOTAL_TOTAL};
};


class MCNPInputFile {
public:
   MCNPInputFile(void);
   ~MCNPInputFile(void);
   void WriteFile(const std::string& filename, MCInput& InputValues) const;
private:
   double m_scale;
   void WriteData(const std::vector<double>& array, std::ostream& os, const char *identifier, double scale=1.0) const;
   MCNPInputFile(const MCNPInputFile&);
   MCNPInputFile& operator=(const MCNPInputFile&);
};

class COGInputFile {
public:
   COGInputFile(void);
   ~COGInputFile(void);
   void WriteFile(const std::string& filename, MCInput& InputValues) const;
private:
   double m_scale;
   void WriteData(const std::vector<double>& array, std::ostream& os, double scale=1.0) const;
   void WriteData(const std::vector<double>& array1, const std::vector<double>& array2, bool binned, std::ostream& os, double scale=1.0) const;
   COGInputFile(const COGInputFile&);
   COGInputFile& operator=(const COGInputFile&);
};
#endif

