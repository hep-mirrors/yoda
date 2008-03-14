// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008 The YODA collaboration (see AUTHORS for details)
//
#include "YODA/AnalysisObject.h"
#include "YODA/Exception.h"

using namespace YODA;

using std::string;
using std::map;

AnalysisObject::AnalysisObject (const string& path, const string& title)
  : _path(path), _title(title), _annotations () {}

AnalysisObject::~AnalysisObject () {}

string AnalysisObject::getPath() {
  return _path;
}

void AnalysisObject::setPath(const string& path) {
  _path = path;
}

string AnalysisObject::getTitle() {
  return _title;
}

void AnalysisObject::setTitle(const string& title) {
  _title = title;
}

void AnalysisObject::annotation (const string& name, const string& value) {
  _annotations[name] = value;
}
    
map<string,string> AnalysisObject::annotations () const {
  return _annotations;
}

string AnalysisObject::annotationValue (const string& name) const {
  map<string,string>::const_iterator v = _annotations.find(name);
  if (v == _annotations.end()) {
    string missing = "YODA::AnalysisObject: The AnalysisObject "
      + _path + "/" + _title + " does not contain an annotation named "
      + name;
    throw LogicError(missing);
  }
  return v->second;
}
