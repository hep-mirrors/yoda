#ifndef YODA_Bin2D_h
#define YODA_Bin2D_h

#include "YODA/Bin.h"
#include "YODA/Dbn2D.h"
#include <string>
#include <utility>
#include <vector>
#include <cassert>
using namespace std;

namespace YODA {


  class Bin2D : public Bin {
  public:

    /// @name Constructors
    //@{

    /// @brief Constructor that is mostly used in manual bin addition.
    /// Mostly used when creating a bin manually since it requires the smallest
    /// amount of information transferred. All 4 edges are then constructed from
    /// extremal points for which coordinates are provided.
    Bin2D(double lowedgeX, double lowedgeY, double highedgeX, double highedgeY);

    /// @brief A constructor usually used by functions creating Bins in bulk.
    /// Since all the edges are provided by an external function it creates a
    /// Bin slightly faster (this claim is very weakly true).  It is not
    /// suggested to use it if it is just needed to add few bins to an already
    /// created Histo2D.
    Bin2D(std::vector<std::pair<std::pair<double, double>, std::pair<double, double> > > edges);

    //@}


    /// @name Modifiers
    //@{

    /// Reset all bin data
    virtual void reset() {
      _dbn.reset();
    }

    /// Scale the x and y coordinates and distributions.
    void scaleXY(double scaleX, double scaleY);

    //@}


    /// @name Bin geometry info
    //@{

    /// Get the low x edge of the bin.
    double lowEdgeX() const {
      return _edges[0].first.first;
    }
    /// Synonym for lowEdgeX()
    double xMin() const { return lowEdgeX(); }

    /// Get the low y edge of the bin.
    double lowEdgeY() const {
      return _edges[0].first.second;
    }
    /// Synonym for lowEdgeY()
    double yMin() const { return lowEdgeY(); }

    /// Get the high x edge of the bin.
    double highEdgeX() const {
      return _edges[1].second.first;
    }
    /// Synonym for highEdgeX()
    double xMax() const { return highEdgeX(); }

    /// Get the high y edge of the bin.
    double highEdgeY() const {
      return _edges[1].second.second;
    }
    /// Synonym for highEdgeY()
    double yMax() const { return highEdgeY(); }

    /// Width of the bin in x
    double widthX() const {
      return _edges[1].second.first - _edges[0].first.first;
    }

    /// Width of the bin in y
    double widthY() const {
      return _edges[0].second.second - _edges[0].first.second;
    }

    /// Find the geometric midpoint of the bin
    std::pair<double, double> midpoint() const {
      return make_pair(_edges[1].second.first-_edges[0].first.first,
                       _edges[0].second.second-_edges[0].first.second);
    }

    /// Find the weighted mean point of the bin, or the midpoint if unfilled
    std::pair<double, double> focus() const {
      if (_dbn.sumW() != 0) return make_pair(xMean(), yMean());
      return midpoint();
    }

    //@}


    /// @name Distribution statistics
    //@{

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

    Bin2D& operator += (const Bin2D& b) {
      return add(b);
    }

    Bin2D& operator -= (const Bin2D& b) {
      return subtract(b);
    }

    //@}


  protected:

    Bin2D& add(const Bin2D& b) {
      assert(_edges == b._edges);
      _dbn += b._dbn;
      return *this;
    }

    Bin2D& subtract(const Bin2D& b) {
      assert(_edges == b._edges);
      _dbn -= b._dbn;
      return *this;
    }


  protected:

    std::vector<std::pair<std::pair<double,double>,std::pair<double,double> > > _edges;
    Dbn2D _dbn;

  };


  inline Bin2D operator + (const Bin2D& a, const Bin2D& b) {
    Bin2D rtn = a;
    rtn += b;
    return rtn;
  }

  inline Bin2D operator - (const Bin2D& a, const Bin2D& b) {
    Bin2D rtn = a;
    rtn -= a;
    return rtn;
  }


}


#endif
