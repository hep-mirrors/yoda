// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2009 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/AnalysisObject.h"
#include "YODA/Exception.h"
using namespace std;

namespace YODA {


  AnalysisObject::AnalysisObject()
    : _annotations () 
  { }

  AnalysisObject::AnalysisObject(const std::string& path, const std::string& title)
    : _path(path), _title(title), _annotations () 
  { }

  AnalysisObject::~AnalysisObject() 
  { }

  string AnalysisObject::path() const {
    return _path;
  }

  void AnalysisObject::setPath(const std::string& path) {
    _path = path;
  }

  string AnalysisObject::title() const {
    return _title;
  }

  void AnalysisObject::setTitle(const std::string& title) {
    _title = title;
  }

  string AnalysisObject::name() const {
    size_t pos = _path.rfind('/');
    if (pos == string::npos)
      throw Exception("No slash found in path.");
    if (pos == _path.size() - 1)
      throw Exception("Trailing slash found in path.");
    return _path.substr(pos + 1);
  }

  void AnalysisObject::setAnnotation(const std::string& name, const std::string& value) {
    _annotations[name] = value;
  }

  map<string,string> AnalysisObject::annotations () const {
    return _annotations;
  }

  string AnalysisObject::annotationValue (const std::string& name) const {
    map<string,string>::const_iterator v = _annotations.find(name);
    if (v == _annotations.end()) {
      string missing = "YODA::AnalysisObject: The AnalysisObject "
        + _path + "/" + _title + " does not contain an annotation named "
        + name;
      throw LogicError(missing);
    }
    return v->second;
  }

}
