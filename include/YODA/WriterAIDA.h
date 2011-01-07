// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2010 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_WRITERAIDA_H
#define YODA_WRITERAIDA_H

#include "YODA/AnalysisObject.h"
#include "YODA/Writer.h"

#include <vector>
#include <string>
#include <ostream>

namespace YODA {


  /// @todo Are we going to use inheritance or polymorphism anywhere between output format objects?
  class WriterAIDA : public Writer {
  private:

    WriterAIDA() { }

    static Writer* _instance;


  public:

    static Writer& create() {
      if (!_instance) {
        _instance = new WriterAIDA();
      }
      return *_instance;
    }

  protected:
    void writeHeader(std::ostream& stream);
    void writeFooter(std::ostream& stream);

    void writeHisto1D(std::ostream& os, const Histo1D& h, const std::string& path);
    void writeProfile1D(std::ostream& os, const Profile1D& h, const std::string& path);
    // void writeScatter2D(std::ostream& os, const Scatter2D& h, const std::string& path);

  };


}

#endif
