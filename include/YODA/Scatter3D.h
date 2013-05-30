// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2013 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_SCATTER3D_H
#define YODA_SCATTER3D_H

#include "YODA/AnalysisObject.h"
#include "YODA/Point3D.h"
#include <utility>

namespace YODA {


  // Forward declarations
  class Histo2D;
  class Profile2D;


  /// A very generic data type which is just a collection of 3D data points with errors
  class Scatter3D : public AnalysisObject {
  public:

    /// Types of the native Point3D collection
    typedef Point3D Point;
    typedef std::vector<Point3D> Points;


    /// @name Constructors
    //@{

    /// Empty constructor
    Scatter3D(const std::string& path="", const std::string& title="")
      : AnalysisObject("Scatter3D", path, title)
    {  }


    /// Constructor from a set of points
    Scatter3D(const Points& points,
              const std::string& path="", const std::string& title="")
      : AnalysisObject("Scatter3D", path, title),
        _points(points)
    {
      std::sort(_points.begin(), _points.end());
    }


    /// Constructor from vectors of values with no errors
    Scatter3D(const std::vector<double>& x,
	      const std::vector<double>& y,
	      const std::vector<double>& z,
              const std::string& path="",
	      const std::string& title="")
      : AnalysisObject("Scatter3D", path, title)
    {
      if (x.size() != y.size() || y.size() != z.size()) {
        throw RangeError("There are different numbers of x, y, and z values in the provided vectors.");
      }
      const std::pair<double,double> nullerr = std::make_pair(0.0, 0.0);
      for (size_t i = 0; i < x.size(); ++i) {
        addPoint(Point3D(x[i], y[i], z[i], nullerr, nullerr, nullerr));
      }
      std::sort(_points.begin(), _points.end());
    }


    /// Constructor from vectors of values with asymmetric errors on both x and y
    Scatter3D(const std::vector<double>& x, const std::vector<double>& y, const std::vector<double>& z,
              const std::vector<std::pair<double,double> >& ex, const std::vector<std::pair<double,double> >& ey, const std::vector<std::pair<double,double> >& ez,
              const std::string& path="", const std::string& title="")
      : AnalysisObject("Scatter3D", path, title)
    {
      if (x.size() != y.size() || y.size() != z.size()) {
        throw RangeError("There are different numbers of x, y, and z values in the provided vectors.");
      }
      if (x.size() != ex.size() || y.size() != ey.size() || z.size() != ez.size()) {
        throw RangeError("The sizes of the provided error vectors don't match the corresponding x, y, or z value vectors.");
      }
      for (size_t i = 0; i < x.size(); ++i) {
        addPoint(Point3D(x[i], y[i], z[i], ex[i], ey[i], ez[i]));
      }
      std::sort(_points.begin(), _points.end());
    }


    /// Constructor from vectors of values with completely explicit asymmetric errors
    Scatter3D(const std::vector<double>& x, const std::vector<double>& y, const std::vector<double> z,
              const std::vector<double>& exminus,
              const std::vector<double>& explus,
              const std::vector<double>& eyminus,
              const std::vector<double>& eyplus,
              const std::vector<double>& ezminus,
              const std::vector<double>& ezplus,
              const std::string& path="", const std::string& title="")
      : AnalysisObject("Scatter3D", path, title)
    {
      if(x.size() != y.size() || y.size() != z.size() ||
         x.size() != exminus.size() || x.size() != explus.size() ||
         y.size() != eyminus.size() || y.size() != eyplus.size() ||
         z.size() != ezminus.size() || z.size() != ezplus.size())
        throw RangeError("There are either different amounts of points on x/y/z vectors or not every of these vectors has properly defined error vectors!");

      for (size_t i = 0; i < x.size(); ++i) {
        addPoint(Point3D(x[i], y[i], z[i], exminus[i], explus[i], eyminus[i], eyplus[i], ezminus[i], ezplus[i]));
      }

      std::sort(_points.begin(), _points.end());
    }


    /// Copy constructor
    Scatter3D(const Scatter3D& s, const std::string& path="")
      : AnalysisObject("Scatter3D", (path.size() == 0) ? s.path() : path, s, s.title())
    {
      _points = s._points;
    }

    //@}


    /// @name Modifiers
    //@{

    /// Clear all points
    void reset() {
      _points.clear();
    }

    /// Scale
    void scale(double scaleX, double scaleY, double scaleZ) {
      for (size_t i = 0; i < _points.size(); ++i) {
        _points[i].scale(scaleX, scaleY, scaleZ);
      }
    }

    //@}


    /// @name Point accessors
    //@{

    size_t numPoints() const {
      return _points.size();
    }


    const Points& points() const {
      return _points;
    }


    Point3D& point(size_t index) {
      if(index >= numPoints()) throw RangeError("There is no point with such index!");
      return _points.at(index);
    }


    const Point3D& point(size_t index) const {
      if(index >= numPoints()) throw RangeError("There is no point with such index!");
      return _points.at(index);
    }

    //@}


    /// @name Point inserters
    //@{

    Scatter3D& addPoint(const Point3D& pt) {
      _points.push_back(pt);
      return *this;
    }

    Scatter3D& addPoint(double x, double y, double z) {
      _points.push_back(Point3D(x, y, z));
      return *this;
    }

    Scatter3D& addPoint(double x, double y, double z,
                        std::pair<double,double> ex, std::pair<double,double> ey, std::pair<double,double> ez) {
      _points.push_back(Point3D(x, y, z, ex, ey, ez));

      return *this;
    }

    Scatter3D& addPoint(double x, double exminus, double explus,
                        double y, double eyminus, double eyplus,
                        double z, double ezminus, double ezplus) {
      _points.push_back(Point3D(x, y, z, exminus, explus, eyminus, eyplus, ezminus, ezplus));

      return *this;
    }

    Scatter3D& addPoints(Points pts) {
      foreach (const Point3D& pt, pts) {
        addPoint(pt);
      }
      std::sort(_points.begin(), _points.end());

      return *this;
    }

    //@}


    /// @todo Better name?
    Scatter3D& combineWith(const Scatter3D& other) {
      addPoints(other.points());
      return *this;
    }


    /// @todo Better name?
    /// @todo Convert to accept a Range or generic
    Scatter3D& combineWith(const std::vector<Scatter3D>& others) {
      foreach (const Scatter3D& s, others) {
        combineWith(s);
      }
      return *this;
    }


    /// Equality operator
    bool operator == (const Scatter3D& other) {
      return _points == other._points;
    }

    /// Non-equality operator
    bool operator != (const Scatter3D& other) {
      return ! operator == (other);
    }


  private:

    Points _points;

    /// @todo Needed? Isn't this now stored on AnalysisObject?
    std::string _myaotype;

  };



  /// @name Combining scatters by merging sets of points
  //@{

  inline Scatter3D combine(const Scatter3D& a, const Scatter3D& b) {
    Scatter3D rtn = a;
    rtn.combineWith(b);
    return rtn;
  }


  inline Scatter3D combine(const std::vector<Scatter3D>& scatters) {
    Scatter3D rtn;
    for (std::vector<Scatter3D>::const_iterator s = scatters.begin(); s != scatters.end(); ++s) {
      rtn.combineWith(*s);
    }
    return rtn;
  }

  //@}


  //////////////////////////////////


  /// @name Conversion functions from other data types
  //@{

  /// Make a Scatter3D representation of a Histo2D
  Scatter3D mkScatter(const Histo2D& h);

  /// Make a Scatter3D representation of a Profile2D
  Scatter3D mkScatter(const Profile2D& h);

  //@}


  /////////////////////////////////


  /// @name Combining scatters: global operators, assuming aligned points
  //@{

  /// Add two scatters
  Scatter3D add(const Scatter3D& first, const Scatter3D& second);


  /// Add two scatters
  inline Scatter3D operator + (const Scatter3D& first, const Scatter3D& second) {
    return add(first, second);
  }


  /// Subtract two scatters
  Scatter3D subtract(const Scatter3D& first, const Scatter3D& second);


  /// Subtract two scatters
  inline Scatter3D operator - (const Scatter3D& first, const Scatter3D& second) {
    return subtract(first, second);
  }


  /// Divide two scatters
  Scatter3D divide(const Scatter3D& numer, const Scatter3D& denom);


  /// Divide two scatters
  inline Scatter3D operator / (const Scatter3D& numer, const Scatter3D& denom) {
    return divide(numer, denom);
  }

  //@}


}

#endif
