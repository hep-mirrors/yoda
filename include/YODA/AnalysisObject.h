// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2011 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_AnalysisObject_h
#define YODA_AnalysisObject_h

#include "YODA/Exceptions.h"
#include <string>
#include <map>

namespace YODA {

  /// AnalysisObject is the base class for histograms and scatters
  class AnalysisObject {

  public:

    /// Default constructor
    AnalysisObject() { }

    /// Constructor giving a path and optional title
    AnalysisObject(const std::string& path, const std::string& title="") {
      /// @todo Don't set annotations if value is an empty string?
      setPath(title);
      setTitle(title);
    }

    /// Default destructor
    virtual ~AnalysisObject() { }

    /// Reset this analysis object
    virtual void reset() = 0;


  public:

    ///@name Annotations
    //@{

    typedef std::map<std::string,std::string> Annotations;

    /// Add or set an annotation by name
    /// @todo Template on arg type with lexical_cast, cf. AGILe setParam
    void setAnnotation(const std::string& name, const std::string& value) {
      _annotations[name] = value;
    }

    /// Check if an annotation is defined
    bool hasAnnotation(const std::string& name) const {
      return _annotations.find(name) != _annotations.end();
    }

    /// Get all the annotations (as const ref)
    const Annotations& annotations() const {
      return _annotations;
    }

    /// Set all annotations at once
    void setAnnotations(const Annotations& anns) {
      _annotations = anns;
    }

    /// Get an annotation by name
    /// @todo Template on return type with lexical_cast, cf. AGILe setParam. Default is string
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


    /// @name Standard annotations
    //@{

    /// Get the AO title.
    /// Returns a null string if undefined, rather than throwing an exception cf. the Title annotation.
    const std::string title() const {
      try {
        return annotation("Title");
      } catch (AnnotationError& ae) {
        return "";
      }
    }

    /// Set the AO title
    void setTitle(const std::string& title) {
      setAnnotation("Title", title);
    }

    /// Get the AO path.
    /// Returns a null string if undefined, rather than throwing an exception cf. the Title annotation.
    const std::string path() const {
      try {
        return annotation("Path");
      } catch (AnnotationError& ae) {
        return "";
      }
    }

    /// Set the AO path
    void setPath(const std::string& path) {
      setAnnotation("Path", path);
    }

    //@}


  public:

    /// @name Persistency hooks
    //@{

    /// @todo Maybe make these private, and make Writer a friend of AO

    /// Get name of the analysis object type, for persistency
    virtual std::string _aotype() const = 0;

    //@}


  private:

    /// The annotations indexed by name
    std::map<std::string,std::string> _annotations;

  };


}

#endif // YODA_AnalysisObject_h
