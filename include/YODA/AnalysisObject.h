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
      /// @todo Convert to use an annotation instead (with return "" if no "Title" anntn)
      return _title;
    }

    /// Set the title
    void setTitle(const std::string& title) {
      /// @todo Convert to use an annotation instead
      _title = title;
    }


  public:

    /// Reset this analysis object
    virtual void reset() = 0;


    ///@name Annotations
    //@{

    /// Add or set an annotation by name
    void setAnnotation(const std::string& name, const std::string& value) {
      _annotations[name] = value;
    }

    /// Check if an annotation is defined
    bool hasAnnotation(const std::string& name, const std::string& value) const {
      return _annotations.find(name) != _annotations.end();
    }

    /// Get all the annotations (as const ref)
    const std::map<std::string,std::string>& annotations() const {
      return _annotations;
    }

    /// Get an annotation by name
    /// @todo What if it isn't defined? Return by value instead, or throw KeyException?
    const std::string& annotation(const std::string& name) const;

    /// Delete an annotation by name
    void rmAnnotation(const std::string& name) {
      _annotations.erase(name);
    }

    /// Delete an annotation by name
    void clearAnnotations() {
      _annotations.clear();
    }

    //@}


    /// @name Persistency hooks
    //@{

    /// Get name of the analysis object type, for persistency
    virtual std::string _aotype() const = 0;

    //@}


  private:

    /// The title
    /// @todo Merge with annotations
    std::string _title;

    /// The annotations indexed by name
    std::map<std::string,std::string> _annotations;

  };


}

#endif // YODA_AnalysisObject_h
