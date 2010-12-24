// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2010 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/AnalysisObject.h"
#include "YODA/Exception.h"
using namespace std;

namespace YODA {

  const string& AnalysisObject::annotation(const string& name) const {
    map<string,string>::const_iterator v = _annotations.find(name);
    if (v == _annotations.end()) {
      string missing = "YODA::AnalysisObject: No annotation named " + name;
      throw LogicError(missing);
    }
    return v->second;
  }

}
