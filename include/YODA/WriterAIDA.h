// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2009 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_WRITERAIDA_H
#define YODA_WRITERAIDA_H

#include "YODA/AnalysisObject.h"
#include "YODA/Writer.h"

#include <vector>
#include <string>
#include <ostream>

namespace YODA {


  /// Are we going to use inheritance or polymorphism anywhere between output format objects?
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
    void writeHisto(std::ostream& stream, const Histo1D& h, const std::string& path);
    void writeProfile(std::ostream& stream, const Profile1D& p, const std::string& path);
    /// @todo Do Scatter2D I/O
    //void writeScatter(std::ostream& stream, const Scatter& p, const std::string& path);
  };

}

#endif
