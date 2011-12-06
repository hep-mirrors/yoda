#ifndef YODA_Bin2D_h
#define YODA_Bin2D_h

#include "YODA/Bin.h"
#include "YODA/Dbn2D.h"
#include <string>
#include <utility>
#include <vector>
#include <cassert>

namespace YODA {


  /// @brief A generic 2D bin type
  ///
  /// This is a generic 2D bin type which supplies the accessors for the two "x"
  /// and "y" axis directions in which it is defined. Bin2D is not intended to be
  /// directly instantiated: it is inherited from to make specific histogram and
  /// profile bin types as HistoBin2D and ProfileBin2D.
  /// The lower bin edges in x and y are inclusive. This base class provides no fill
  /// method, since the signatures for standard and profile histos differ.
  template <class DBN>
  class Bin2D : public Bin {
  public:

    /// Convenience typedefs
    typedef typename std::pair<double, double> Point;
    typedef typename std::pair<Point, Point> Segment;

    /// @name Constructors
    //@{

    /// @brief Constructor that is mostly used in manual bin addition.
    /// Mostly used when creating a bin manually since it requires the smallest
    /// amount of information transferred. All 4 edges are then constructed from
    /// extremal points for which coordinates are provided.
    Bin2D(double lowedgeX, double lowedgeY, double highedgeX, double highedgeY) {
      if (lowedgeX > highedgeX || lowedgeY > highedgeY) {
        throw RangeError("The bins are wrongly defined!");
      }
      _edges.first.first = lowedgeX;
      _edges.first.second = lowedgeY;
      _edges.second.first = highedgeX;
      _edges.second.second = highedgeY;
    }

    /// @brief A constructor usually used by functions creating Bins in bulk.
    /// Since all the edges are provided by an external function it creates a
    /// Bin slightly faster (this claim is very weakly true).  It is not
    /// suggested to use it if it is just needed to add few bins to an already
    /// created Histo2D.
    Bin2D(const std::vector<Segment>& edges) {
      if (edges.size() != 4) {
        throw RangeError("The edge vector does not define a full rectangle!");
      }
      _edges.first.first = edges[0].first.first;
      _edges.first.second = edges[0].first.second;
      _edges.second.first = edges[1].second.first;
      _edges.second.second = edges[1].second.second;
    }

    /// A copy constructor
    Bin2D(const Bin2D& b) {
      _edges  = b._edges;
      _dbn    = b._dbn;
    }


    /// Copy assignment
    Bin2D<DBN>& operator = (const Bin2D<DBN>& b) {
      _edges = b._edges;
      _dbn = b._dbn;
      return *this;
    }

    //@}


    /// @name Modifiers
    //@{

    const std::vector<Segment> edges() const {
      std::vector<Segment> ret;
      ret.push_back(std::make_pair(std::make_pair(xMin(), yMin()), std::make_pair(xMin(), yMax())));
      ret.push_back(std::make_pair(std::make_pair(xMin(), yMax()), std::make_pair(xMax(), yMax())));
      ret.push_back(std::make_pair(std::make_pair(xMax(), yMin()), std::make_pair(xMax(), yMax())));
      ret.push_back(std::make_pair(std::make_pair(xMin(), yMin()), std::make_pair(xMax(), yMin())));
      return ret;
    }

    /// Reset all bin data
    virtual void reset() {
      _dbn.reset();
    }

    /// Scale the x and y coordinates and distributions.
    void scaleXY(double scaleX, double scaleY) {
      _edges.first.first *= scaleX;
      _edges.second.first *= scaleX;

      _edges.first.second *= scaleY;
      _edges.second.second *= scaleY;

      _dbn.scaleXY(scaleX, scaleY);
    }

    void scaleW(double scalefactor) {
      _dbn.scaleW(scalefactor);
    }

    //@}


    /// @name Bin geometry info
    //@{

    /// Get the low x edge of the bin.
    double lowEdgeX() const {
      return _edges.first.first;
    }
    /// Synonym for lowEdgeX()
    double xMin() const { return lowEdgeX(); }

    /// Get the low y edge of the bin.
    double lowEdgeY() const {
      return _edges.first.second;
    }
    /// Synonym for lowEdgeY()
    double yMin() const { return lowEdgeY(); }

    /// Get the high x edge of the bin.
    double highEdgeX() const {
      return _edges.second.first;
    }
    /// Synonym for highEdgeX()
    double xMax() const { return highEdgeX(); }

    /// Get the high y edge of the bin.
    double highEdgeY() const {
      return _edges.second.second;
    }
    /// Synonym for highEdgeY()
    double yMax() const { return highEdgeY(); }

    /// Width of the bin in y
    double widthY() const {
      return yMax() - yMin();
    }

    /// Width of the bin in x
    double widthX() const {
      return xMax() - xMin();
    }
    //@}


    /// @name Distribution statistics
    //@{

    /// Find the geometric midpoint of the bin
    Point midpoint() const {
    return std::make_pair((double)(xMax() - xMin())/2 + xMin(), (double)(yMax() - yMin())/2 + yMin());
    }


    /// Find the weighted mean point of the bin, or the midpoint if unfilled
    Point focus() const {
      if (_dbn.sumW() != 0) return std::make_pair(xMean(), yMean());
      return midpoint();
    }

    /// Mean x value
    double xMean() const {
      return _dbn.xMean();
    }

    /// Mean y value
    double yMean() const {
      return _dbn.yMean();
    }

    /// Variance on x values
    double xVariance() const {
      return _dbn.xVariance();
    }

    /// Variance on y values
    double yVariance() const {
      return _dbn.yVariance();
    }

    /// Standard deviation of x values
    double xStdDev() const {
      return _dbn.xStdDev();
    }

    /// Standard deviation of y values
    double yStdDev() const {
      return _dbn.yStdDev();
    }

    /// Standard error on x mean
    double xStdErr() const {
      return _dbn.xStdErr();
    }

    /// Standard error on y mean
    double yStdErr() const {
      return _dbn.yStdErr();
    }

    /// RMS of x values
    double xRMS() const {
      return _dbn.xRMS();
    }

    /// RMS of y values
    double yRMS() const {
      return _dbn.yRMS();
    }

    //@}


    /// @name Raw distribution statistics
    //@{

    /// Number of times the bin has been filled (weight-independent).
    unsigned long numEntries() const {
      return _dbn.numEntries();
    }

    /// Sum of weights
    double sumW() const {
      return _dbn.sumW();
    }

    double sumW2() const {
      return _dbn.sumW2();
    }
    double sumWX() const {
      return _dbn.sumWX();
    }
    double sumWY() const {
      return _dbn.sumWY();
    }
    double sumWXY() const {
      return _dbn.sumWXY();
    }
    double sumWX2() const {
      return _dbn.sumWX2();
    }
    double sumWY2() const {
      return _dbn.sumWY2();
    }

    //@}


    /// @name Operators
    //@{

    Bin2D<DBN>& operator += (const Bin2D<DBN>& b) {
      return add(b);
    }

    Bin2D<DBN>& operator -= (const Bin2D<DBN>& b) {
      return subtract(b);
    }

    /// Equality operator that checks if the location
    /// of the two bins is the same
    bool operator == (const Bin2D<DBN>& other) const {
      return _edges == other._edges;
    }

    bool operator != (const Bin2D<DBN>& other) const {
      return ! operator == (other);
    }
    //@}

  protected:

    /// Boundaries setter
    void _setBounds(double xMin, double yMin, double xMax, double yMax) {
      _edges.first.first = xMin;
      _edges.first.second = yMin;
      _edges.second.first = xMax;
      _edges.second.second = yMax;
    }


  protected:

    Bin2D<DBN>& add(const Bin2D<DBN>& b) {
      if (_edges != b._edges) {
        if (b.highEdgeX() > highEdgeX()) _setBounds(xMin(), yMin(), b.xMax(), yMax());
        if (b.yMax() > yMax()) _setBounds(xMin(), yMin(), xMax(), b.yMax());
        if (b.xMin() < xMin()) _setBounds(b.xMin(), yMin(), xMax(), yMax());
        if (b.yMin() < yMin()) _setBounds(xMin(), b.yMin(), xMax(), yMax());
      }
      _dbn += b._dbn;
      return *this;
    }

    Bin2D<DBN>& subtract(const Bin2D<DBN>& b) {
    // Automatically resize if adding a bin that does not have the same location
    // this way merging the bins works perfectly
    if (_edges != b._edges) {
      if (b.xMax() > xMax()) _setBounds(xMin(), yMin(), b.xMax(), yMax());
      if (b.yMax() > yMax()) _setBounds(xMin(), yMin(), xMax(), b.yMax());
      if (b.xMin() < xMin()) _setBounds(b.xMin(), yMin(), xMax(), yMax());
      if (b.yMin() < yMin()) _setBounds(xMin(), b.yMin(), xMax(), yMax());
    }
    _dbn -= b._dbn;
    return *this;
  }

    Segment _edges;
    DBN _dbn;

  };


  /// @name Operators
  //@{
  template <class DBN>
  inline Bin2D<DBN> operator + (const Bin2D<DBN>& a, const Bin2D<DBN>& b) {
    Bin2D<DBN> rtn = a;
    rtn += b;
    return rtn;
  }

  template<class DBN>
  inline Bin2D<DBN> operator - (const Bin2D<DBN>& a, const Bin2D<DBN>& b) {
    Bin2D<DBN> rtn = a;
    rtn -= a;
    return rtn;
  }

  //@}


}


#endif
