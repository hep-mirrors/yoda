// -*- C++ -*-
//
// This file is part of YODA -- Yet more Objects for Data Analysis
// Copyright (C) 2008-2014 The YODA collaboration (see AUTHORS for details)
//
#ifndef YODA_SCATTER2D_H
#define YODA_SCATTER2D_H

#include "YODA/AnalysisObject.h"
#include "YODA/Point2D.h"
#include "YODA/Utils/sortedvector.h"
#include <vector>
#include <set>
#include <string>
#include <utility>

namespace YODA {


  // Forward declarations
  class Histo1D;
  class Profile1D;


  /// A very generic data type which is just a collection of 2D data points with errors
  class Scatter2D : public AnalysisObject {
  public:

    /// Type of the native Point2D collection
    typedef Point2D Point;
    typedef Utils::sortedvector<Point2D> Points;


    /// @name Constructors
    //@{

    /// Empty constructor
    Scatter2D(const std::string& path="", const std::string& title="")
      : AnalysisObject("Scatter2D", path, title)
    {  }


    /// Constructor from a set of points
    Scatter2D(const Points& points,
              const std::string& path="", const std::string& title="")
      : AnalysisObject("Scatter2D", path, title),
        _points(points)
    {  }


    /// Constructor from a vector of values with no errors
    Scatter2D(const std::vector<double>& x, const std::vector<double>& y,
              const std::string& path="", const std::string& title="")
      : AnalysisObject("Scatter2D", path, title)
    {
      assert(x.size() == y.size());
      for (size_t i = 0; i < x.size(); ++i) {
        addPoint(x[i], y[i]);
      }
    }


    /// Constructor from vectors of values with symmetric errors on x and y
    Scatter2D(const std::vector<double>& x, const std::vector<double>& y,
              const std::vector<double>& ex, const std::vector<double>& ey,
              const std::string& path="", const std::string& title="")
      : AnalysisObject("Scatter2D", path, title)
    {
      assert(x.size() == y.size() && x.size() == ex.size() && x.size() == ey.size());
      for (size_t i = 0; i < x.size(); ++i) {
        addPoint(x[i], y[i], ex[i], ey[i]);
      }
    }


    // /// Constructor from values with symmetric errors on x and asymmetric errors on y
    // Scatter2D(const std::vector<double>& x, const std::vector<double>& y,
    //           const std::vector<double>& ex, const std::vector<std::pair<double,double> >& ey,
    //           const std::string& path="", const std::string& title="")
    //   : AnalysisObject("Scatter2D", path, title)
    // {
    //   assert(x.size() == y.size() && x.size() == ex.size() && x.size() == ey.size());
    //   for (size_t i = 0; i < x.size(); ++i) {
    //     addPoint(Point2D(x[i], y[i], ex[i], ey[i]));
    //   }
    // }


    // /// Constructor from values with asymmetric errors on x and symmetric errors on y
    // Scatter2D(const std::vector<double>& x, const std::vector<double>& y,
    //           const std::vector<std::pair<double,double> >& ex, const std::vector<double>& ey,
    //           const std::string& path="", const std::string& title="")
    //   : AnalysisObject("Scatter2D", path, title)
    // {
    //   assert(x.size() == y.size() && x.size() == ex.size() && x.size() == ey.size());
    //   for (size_t i = 0; i < x.size(); ++i) {
    //     addPoint(Point2D(x[i], y[i], ex[i], ey[i]));
    //   }
    // }


    /// Constructor from values with asymmetric errors on both x and y
    Scatter2D(const std::vector<double>& x, const std::vector<double>& y,
              const std::vector<std::pair<double,double> >& ex, const std::vector<std::pair<double,double> >& ey,
              const std::string& path="", const std::string& title="")
      : AnalysisObject("Scatter2D", path, title)
    {
      assert(x.size() == y.size() && x.size() == ex.size() && x.size() == ey.size());
      for (size_t i = 0; i < x.size(); ++i) {
        addPoint(Point2D(x[i], y[i], ex[i], ey[i]));
      }
    }


    /// Constructor from values with completely explicit asymmetric errors
    Scatter2D(const std::vector<double>& x, const std::vector<double>& y,
              const std::vector<double>& exminus,
              const std::vector<double>& explus,
              const std::vector<double>& eyminus,
              const std::vector<double>& eyplus,
              const std::string& path="", const std::string& title="")
      : AnalysisObject("Scatter2D", path, title)
    {
      assert(x.size() == y.size() &&
             x.size() == exminus.size() && x.size() == explus.size() &&
             x.size() == eyminus.size() && x.size() == eyplus.size());
      for (size_t i = 0; i < x.size(); ++i) {
        addPoint(Point2D(x[i], y[i], exminus[i], explus[i], eyminus[i], eyplus[i]));
      }
    }


    /// Copy constructor with optional new path
    Scatter2D(const Scatter2D& s2, const std::string& path="")
      : AnalysisObject("Scatter2D", (path.size() == 0) ? s2.path() : path, s2, s2.title()),
        _points(s2._points)
    {  }


    /// Assignment operator
    Scatter2D& operator = (const Scatter2D& s2) {
      AnalysisObject::operator = (s2); //< AO treatment of paths etc.
      _points = s2._points;
      return *this;
    }

    /// Make a copy on the stack
    Scatter2D clone() const {
      return Scatter2D(*this);
    }

    /// Make a copy on the heap, via 'new'
    Scatter2D* newclone() const {
      return new Scatter2D(*this);
    }

    //@}


    /// @name Modifiers
    //@{

    /// Clear all points
    void reset() {
      _points.clear();
    }

    /// Scaling
    void scale(double scaleX, double scaleY) {
      for (size_t i = 0; i < _points.size(); ++i) {
        _points[i].scale(scaleX, scaleY);
      }
    }

    /// @todo Add scaleX, scaleY

    //@}


    ///////////////////////////////////////////////////


    /// @name Point accessors
    //@{

    /// Number of points in the scatter
    size_t numPoints() const {
      return _points.size();
    }


    /// Get the collection of points (non-const)
    Points& points() {
      return _points;
    }


    /// Get the collection of points (const)
    const Points& points() const {
      return _points;
    }


    /// Get a reference to the point with index @a index
    Point2D& point(size_t index) {
      assert(index < numPoints());
      return _points.at(index);
    }


    /// Get the point with index @a index (const version)
    const Point2D& point(size_t index) const {
      assert(index < numPoints());
      return _points.at(index);
    }

    //@}


    /// @name Point inserters
    //@{

    /// Insert a new point
    Scatter2D& addPoint(const Point2D& pt) {
      _points.insert(pt);
      return *this;
    }

    /// Insert a new point, defined as the x/y value pair
    Scatter2D& addPoint(double x, double y) {
      _points.insert(Point2D(x, y));
      return *this;
    }

    /// Insert a new point, defined as the x/y value pair and symmetric errors
    Scatter2D& addPoint(double x, double y, double ex, double ey) {
      _points.insert(Point2D(x, y, ex, ey));
      return *this;
    }

    // /// Insert a new point, defined as the x/y value pair and mixed errors
    // Scatter2D& addPoint(double x, double y, std::pair<double,double> ex, double ey) {
    //   _points.insert(Point2D(x, y, ex, ey));
    //   return *this;
    // }

    // /// Insert a new point, defined as the x/y value pair and mixed errors
    // Scatter2D& addPoint(double x, double y, double ex, std::pair<double,double> ey) {
    //   _points.insert(Point2D(x, y, ex, ey));
    //   return *this;
    // }

    /// Insert a new point, defined as the x/y value pair and asymmetric errors
    Scatter2D& addPoint(double x, double y, std::pair<double,double> ex, std::pair<double,double> ey) {
      _points.insert(Point2D(x, y, ex, ey));
      return *this;
    }

    /// Insert a new point, defined as the x/y value pair and asymmetric errors
    Scatter2D& addPoint(double x, double y,
                        double exminus, double explus,
                        double eyminus, double eyplus) {
      _points.insert(Point2D(x, y, exminus, explus, eyminus, eyplus));
      return *this;
    }

    /// Insert a collection of new points
    Scatter2D& addPoints(Points pts) {
      BOOST_FOREACH (const Point2D& pt, pts) {
        addPoint(pt);
      }
      return *this;
    }

    //@}


    /// @name Combining sets of scatter points
    //@{

    /// @todo Better name?
    Scatter2D& combineWith(const Scatter2D& other) {
      addPoints(other.points());
      return *this;
    }

    /// @todo Better name?
    /// @todo Convert to accept a Range or generic
    Scatter2D& combineWith(const std::vector<Scatter2D>& others) {
      BOOST_FOREACH (const Scatter2D& s, others) {
        combineWith(s);
      }
      return *this;
    }

    //@}


  private:

    Points _points;

    /// @todo Needed? Isn't this now stored on AnalysisObject?
    std::string _myaotype;

  };


  /// @name Combining scatters by merging sets of points
  //@{

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

  //@}


  //////////////////////////////////


  /// @name Conversion functions from other data types
  //@{

  /// Make a Scatter2D representation of a Histo1D
  Scatter2D mkScatter(const Histo1D& h);

  /// Make a Scatter2D representation of a Profile1D
  Scatter2D mkScatter(const Profile1D& p);

  //@}


  /////////////////////////////////


  /// @name Combining scatters: global operators, assuming aligned points
  //@{

  /// Add two scatters
  Scatter2D add(const Scatter2D& first, const Scatter2D& second);


  /// Add two scatters
  inline Scatter2D operator + (const Scatter2D& first, const Scatter2D& second) {
    return add(first, second);
  }


  /// Subtract two scatters
  Scatter2D subtract(const Scatter2D& first, const Scatter2D& second);


  /// Subtract two scatters
  inline Scatter2D operator - (const Scatter2D& first, const Scatter2D& second) {
    return subtract(first, second);
  }


  /// Divide two scatters
  Scatter2D divide(const Scatter2D& numer, const Scatter2D& denom);


  /// Divide two scatters
  inline Scatter2D operator / (const Scatter2D& numer, const Scatter2D& denom) {
    return divide(numer, denom);
  }


  //@}


  /// @name Transforming operations on Scatter2D
  //@{

  /// @brief Apply transformation fx(x) to all values and error positions (operates in-place on @a s)
  ///
  /// fx should be a function which takes double x -> double newx
  template<typename FNX>
  inline void transformX(Scatter2D& s, FNX fx) {
    for (size_t i = 0; i < s.numPoints(); ++i) {
      Point2D& p = s.point(i);
      const double newx = fx(p.x());
      const double fx_xmin = fx(p.xMin());
      const double fx_xmax = fx(p.xMax());
      // Deal with possible inversions of min/max ordering under the transformation
      const double newxmin = std::min(fx_xmin, fx_xmax);
      const double newxmax = std::max(fx_xmin, fx_xmax);
      // Set new point x values
      p.setX(newx);
      /// @todo Be careful about transforms which could switch around min and max errors, or send both in the same direction!
      p.setXErrMinus(newx - newxmin);
      p.setXErrPlus(newxmax - newx);
    }
  }


  /// @brief Apply transformation fy(y) to all values and error positions (operates in-place on @a s)
  ///
  /// fy should be a function which takes double y -> double newy
  template<typename FNY>
  inline void transformY(Scatter2D& s, FNY fy) {
    for (size_t i = 0; i < s.numPoints(); ++i) {
      Point2D& p = s.point(i);
      const double newy = fy(p.y());
      const double fy_ymin = fy(p.yMin());
      const double fy_ymax = fy(p.yMax());
      // Deal with possible inversions of min/max ordering under the transformation
      const double newymin = std::min(fy_ymin, fy_ymax);
      const double newymax = std::max(fy_ymin, fy_ymax);
      // Set new point y values
      p.setY(newy);
      /// @todo Be careful about transforms which could switch around min and max errors, or send both in the same direction!
      p.setYErrMinus(newy - newymin);
      p.setYErrPlus(newymax - newy);
    }
  }


  /// @todo Add external scale, scaleX, scaleY functions


  /// Exchange the x and y axes (operates in-place on @a s)
  inline void flip(Scatter2D& s) {
    for (size_t i = 0; i < s.numPoints(); ++i) {
      Point2D& p = s.point(i);
      const double newx = p.y();
      const double newy = p.x();
      const double newxmin = p.yMin();
      const double newxmax = p.yMax();
      const double newymin = p.xMin();
      const double newymax = p.xMax();
      p.setX(newx);
      p.setY(newy);
      /// @todo Be careful about transforms which could switch around min and max errors, or send both in the same direction!
      p.setXErrMinus(newx - newxmin);
      p.setXErrPlus(newxmax - newx);
      p.setYErrMinus(newy - newymin);
      p.setYErrPlus(newymax - newy);
    }
  }

  //@}


}

#endif
