// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2010 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_WRITERYODA_H
#define YODA_WRITERYODA_H

#include "YODA/AnalysisObject.h"
#include "YODA/Writer.h"

#include <vector>
#include <string>
#include <ostream>


namespace YODA {

  /// Are we going to use inheritance or polymorphism anywhere between output format objects?
  class WriterYODA : public Writer {
  private:
    WriterYODA() { }

    static Writer* _instance;

  public:

    static Writer& create() {
      if (!_instance) {
        _instance = new WriterYODA();
      }
      return *_instance;
    }

  protected:
    void writeHeader(std::ostream& stream);
    void writeFooter(std::ostream& stream);
    void writeHisto1D(std::ostream& stream, const Histo1D& h, const std::string& path);
    void writeProfile1D(std::ostream& stream, const Profile1D& p, const std::string& path);
    //void writeScatter2D(std::ostream& stream, const Scatter2D& p, const std::string& path);
  };

}

#endif
