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


/* Classes for specifing a radioactive mixture and aging it */
#ifndef RADSOURCE_H_INCLUDE
#define RADSOURCE_H_INCLUDE

#include "porting.h"
#include <string>
#include <istream>
#include "decaycomputer.h"
#include "photoncomputer.h"

namespace radsrc {

   /*! \class CRadSource
    *  \brief Encapsulating class for various aspects of the decay radiation problem.
    *
    *  CRadSource simplifies the functions of configuring and executing a decay
    *  computation.  CRadSource provides several input methods for problem specifications:
    *  as a filename, as an open istream, or as a string, which may be delivered as
    *  substrings. The CDecayComputer and CPhotonComputer objects are initializaed
    *  according to the configuration string.  Configuration files can also be specified
    *  by environment variable.
    *
    *  CRadSource also provides a summary of the decay calculation either as a formatted
    *  multi-line string or as a vector of lines.
    */
   class CRadSource {
   public:
      // Constructors, etc.
      CRadSource(void);
      ~CRadSource(void);

      // Public Member Functions
      //! Configure from a file
      bool loadConfig(const char *filename=0);

      //! Configure from an istream
      bool loadConfig(std::istream& isInput);

      //! Load a default configuration
      //! \deprecated Lifetime uncertain. May be useful enough to keep.
      bool setupDefault(void);

      //! Get Report as lines or string
      std::string getReport(const char *pPrefix = 0) const;
      void getReport(std::vector<std::string>& vReportLines) const;

      //! Get the CDecayComputer object
      CDecayComputer& getDecayComputer(void);
      //! Get the CDecayComputer object
      const CDecayComputer& getDecayComputer(void) const;

      //! Get the CPhotonComputer object
      CPhotonComputer& getPhotonComputer(void);
      //! Get the CPhotonComputer object
      const CPhotonComputer& getPhotonComputer(void) const;

      //! Append a string to the accumulating string of configuration instructions
      void addConfigInfo(const char *str);

      //! Append a string to the accumulating string of configuration instructions
      void addConfigInfo(const std::string& str);

      //! Parse configuration string
      int processConfigInfo(void);

      //! Get the current database in use
      const CIsotopeDatabase* getIsotopeDatabase(void) const;
   private:
      CDecayComputer m_decayComputer;
      CPhotonComputer m_photonComputer; // must initialize after m_decayComputer!
      std::string config;

      //! character based input of whitespace terminated words
      void getWord(std::istream&, std::string&);

      // CPhotonComputer is not copyable
      CRadSource(const CRadSource&);
      CRadSource& operator=(const CRadSource&);
   };

   inline CPhotonComputer&
   CRadSource::getPhotonComputer()
   {
      return m_photonComputer;
   }

   inline CDecayComputer&
   CRadSource::getDecayComputer()
   {
      return m_decayComputer;
   }

   inline const CPhotonComputer&
   CRadSource::getPhotonComputer() const
   {
      return m_photonComputer;
   }

   inline const CDecayComputer&
   CRadSource::getDecayComputer() const
   {
      return m_decayComputer;
   }
   inline void
   CRadSource::addConfigInfo(const char *str)
   {
      config += str;
   }
   inline void
   CRadSource::addConfigInfo(const std::string& str)
   {
      config += str;
   }
   inline const CIsotopeDatabase*
   CRadSource::getIsotopeDatabase(void) const
   {
      return m_decayComputer.getIsotopeDatabase();
   }

}
#endif

