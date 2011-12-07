// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2011 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_READERYODA_H
#define YODA_READERYODA_H

#include "YODA/AnalysisObject.h"
#include "YODA/Reader.h"

namespace YODA {


  /// @brief Persistency reader from YODA flat text data format.
  class ReaderYODA : public Reader {
  public:

    static Reader& create() {
      static ReaderYODA _instance;
      return _instance;
    }

    void read(std::istream& stream, std::vector<AnalysisObject*>& aos) {
      _readDoc(stream, aos);
    }

  protected:

    void _readDoc(std::istream& stream, std::vector<AnalysisObject*>& aos);

  private:
    ReaderYODA() { }

  };


}

#endif
