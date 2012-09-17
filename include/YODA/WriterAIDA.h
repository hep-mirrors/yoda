// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2012 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_WRITERAIDA_H
#define YODA_WRITERAIDA_H

#include "YODA/AnalysisObject.h"
#include "YODA/Writer.h"

#include <vector>
#include <string>
#include <ostream>

namespace YODA {


  /// @brief Persistency writer for AIDA XML format.
  class WriterAIDA : public Writer {
  public:

    /// Singleton creation function
    static Writer& create() {
      static WriterAIDA _instance;
      // Unused anyway; but if used, does not work in yoda2aida
      // _instance.setPrecision(6);
      return _instance;
    }


    // Include definitions of all write methods (all fulfilled by Writer::write(...))
    #include "YODA/WriterMethods.icc"


  protected:

    void writeHeader(std::ostream& stream);
    void writeFooter(std::ostream& stream);

    void writePlot(std::ostream& os, const Plot& p);
    void writeHisto1D(std::ostream& os, const Histo1D& h);
    void writeHisto2D(std::ostream& os, const Histo2D& h);
    void writeProfile1D(std::ostream& os, const Profile1D& p);
    void writeScatter2D(std::ostream& os, const Scatter2D& s);

  private:

    /// Private since it's a singleton.
    WriterAIDA() { }

  };


}

#endif
