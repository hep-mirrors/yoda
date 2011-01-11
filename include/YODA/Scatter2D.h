// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2011 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_SCATTER2D_H
#define YODA_SCATTER2D_H

#include "YODA/AnalysisObject.h"
#include "YODA/Point2D.h"
#include "YODA/Utils/sortedvector.h"
#include "YODA/Histo1D.h"
#include "YODA/Profile1D.h"
#include <vector>
#include <set>
#include <string>

namespace YODA {


  /// A very generic data type which is just a collection of 2D data points with errors
  class Scatter2D : public AnalysisObject {
  public:

    typedef Utils::sortedvector<Point2D> Points;


    /// @name Constructors
    //@{

    Scatter2D(const std::string& path="", const std::string& title="")
      : AnalysisObject(path, title)
    {  }


    Scatter2D(const Points& points,
              const std::string& path="", const std::string& title="")
      : AnalysisObject(path, title),
        _points(points)
    {  }

    /// @todo Add constructor from Range

    /// @todo Add convenience 2, 4 and 6 vector<double> constructors

    //@}


    /// Clear all points
    void reset() {
      _points.clear();
    }


    /// @name Persistency hooks
    //@{

    /// Get name of the analysis object type, for persisting
    std::string _aotype() const { return "Scatter2D"; }

    /// Set the state of the profile object, for unpersisting
    /// @todo Need to set annotations (do that on AO), all-histo Dbns, and dbns for every bin. Delegate!
    // void _setstate() = 0;

    //@}


    ///////////////////////////////////////////////////


    size_t numPoints() const {
      return _points.size();
    }


    const Points& points() const {
      return _points;
    }


    Point2D& point(size_t index) {
      assert(index < numPoints());
      return _points.at(index);
    }


    const Point2D& point(size_t index) const {
      assert(index < numPoints());
      return _points.at(index);
    }


    Scatter2D& addPoint(const Point2D& pt) {
      _points.insert(pt);
      return *this;
    }


    /// @todo Add convenience 2, 4 and 6 double addPoint variants, plus pairs


    Scatter2D& combineWith(const Scatter2D& other) {
      foreach (const Point2D& pt, other.points()) {
        addPoint(pt);
      }
      return *this;
    }


    /// @todo Convert to accept a Range
    Scatter2D& combineWith(const std::vector<Scatter2D>& others) {
      foreach (const Scatter2D& s, others) {
        combineWith(s);
      }
      return *this;
    }


  private:

    Points _points;
  };



  inline Scatter2D combine(const Scatter2D& a, const Scatter2D& b) {
    Scatter2D rtn = a;
    rtn.combineWith(b);
    return rtn;
  }


  inline Scatter2D combine(const std::vector< Scatter2D >& scatters) {
    Scatter2D rtn;
    for (std::vector<Scatter2D>::const_iterator s = scatters.begin();
         s != scatters.end(); ++s) {
      rtn.combineWith(*s);
    }
    return rtn;
  }


  //////////////////////////////////


  /// @name Conversion functions from other data types
  //@{

  /// Make a Scatter2D representation of a Histo1D
  inline Scatter2D mkScatter(const Histo1D& h) {
    Scatter2D rtn;
    rtn.setAnnotations(h.annotations());
    foreach (const HistoBin& b, h.bins()) {
      const double x = b.focus();
      const double ex_m = b.focus() - b.lowEdge();
      const double ex_p = b.highEdge() - b.focus();
      const double y = b.height();
      const double ey = b.heightError();
      const Point2D pt(x, ex_m, ex_p, y, ey, ey);
      rtn.addPoint(pt);
    }
    assert(h.numBins() == rtn.numPoints());
    return rtn;
  }


  /// Make a Scatter2D representation of a Profile1D
  inline Scatter2D mkScatter(const Profile1D& p) {
    Scatter2D rtn;
    rtn.setAnnotations(p.annotations());
    foreach (const ProfileBin& b, p.bins()) {
      const double x = b.focus();
      const double ex_m = b.focus() - b.lowEdge();
      const double ex_p = b.highEdge() - b.focus();
      const double y = b.mean();
      const double ey = b.stdErr();
      const Point2D pt(x, ex_m, ex_p, y, ey, ey);
      rtn.addPoint(pt);
    }
    assert(p.numBins() == rtn.numPoints());
    return rtn;
  }

  //@}


}

#endif
