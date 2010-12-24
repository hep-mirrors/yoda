// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2010 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_AnalysisObject_h
#define YODA_AnalysisObject_h

#include <string>
#include <map>

namespace YODA {

  /// AnalysisObject is the base class for histograms and scatters
  class AnalysisObject {

  public:

    /// Default constructor
    AnalysisObject() { }

    /// Constructor giving a title
    AnalysisObject(const std::string& title)
      : _title(title)
    { }

    /// Default destructor
    virtual ~AnalysisObject() { }

    /// Get the title
    const std::string& title() const {
      return _title;
    }

    /// Set the title
    void setTitle(const std::string& title) {
      _title = title;
    }


  public:

    ///@name Annotations
    //@{

    /// Add or set an annotation by name
    void setAnnotation(const std::string& name, const std::string& value) {
      _annotations[name] = value;
    }

    /// Get the annotations
    std::map<std::string,std::string> annotations() const {
      return _annotations;
    }

    /// Get an annotation by name
    const std::string& annotation(const std::string& name) const;

    //@}


  public:

    /// Reset this analysis object
    virtual void reset () = 0;


  private:

    /// The title
    std::string _title;

    /// The annotations indexed by name
    std::map<std::string,std::string> _annotations;

  };


}

#endif // YODA_AnalysisObject_h
