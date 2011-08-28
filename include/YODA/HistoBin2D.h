#ifndef YODA_HistoBin2D_h
#define YODA_HistoBin2D_h

#include "YODA/Bin2D.h"
#include "YODA/Dbn2D.h"
#include "YODA/ProfileBin1D.h"
#include "YODA/Exceptions.h"

#include <cmath>
#include <sys/time.h>

namespace YODA {


  /// @brief A Bin2D specialised for handling histogram-type information
  ///
  /// This is a 2D bin type, which supports all the operations defined for
  /// a generic Bin2D object, but also supplies the specific member functions
  /// for histogram-type data, as opposed to profile-type.
  class HistoBin2D : public Bin2D<Dbn2D> {
  public:

    /// @name Constructors
    //@{

    /// Constructor accepting a set of extremal points of a bin
    HistoBin2D(double lowEdgeX, double highEdgeX,
               double lowEdgeY, double highEdgeY)
      : Bin2D(lowEdgeX, lowEdgeY, highEdgeX, highEdgeY)
    { }

    /// Constructor accepting a set of all edges of a bin
    HistoBin2D(std::vector<std::pair<std::pair<double,double>, std::pair<double,double> > >& edges)
      : Bin2D(edges)
    { }


    /// @todo Add copy constructor

    //@}


    /// @name Modifiers
    //@{

    /// A fill() function accepting coordinates as spearate numbers
    void fill(double x, double y, double weight=1.0) {
        _dbn.fill(x, y, weight);
    }

    /// A fill() function accepting the coordinates as std::pair
    void fill(std::pair<double,double> coords, double weight=1.0) {
      _dbn.fill(coords.first, coords.second, weight);
    }

    /// A function that fills this particular bin.
    void fillBin(double weight=1.0) {
      _dbn.fill(midpoint(), weight);
    }

    /// A reset function
    void reset() {
      Bin2D::reset();
    }

    //@}


    /// @name Accessors
    //@{

    /// The volume of a bin
    double volume() const {
      return sumW();
    }

    /// Error on volume
    double volumeErr() const {
      return sqrt(sumW2());
    }

    /// The height of a bin
    double height() const {
      return volume()/(widthX()*widthY());
    }

    /// Error on height
    double heightErr() const {
      return volumeErr()/(widthX()*widthY());
    }

    //@}


    /// @name Transformers
    //@{

    /// @brief Transformer taking x as the primary axis of ProfileBin1D
    /// @todo Need to think about the name, and clarify what "primary axis" means
    ProfileBin1D transformX() {
      ProfileBin1D ret(xMin(), xMax(), Dbn2D(_dbn));
      return ret;
    }

    /// @brief Transformer taking y as the primary axis of ProfileBin1D
    /// @todo Need to think about the name, and clarify what "primary axis" means
    ProfileBin1D transformY() {
      Dbn2D dbn = _dbn; dbn.flipXY();
      ProfileBin1D ret(yMin(), yMax(), Dbn2D(dbn));
      return ret;
    }

    //@}
  };


  /// Bin addition operator
  inline HistoBin2D operator + (const HistoBin2D& a, const HistoBin2D& b) {
    HistoBin2D rtn(a);
    rtn += a;
    return rtn;
  }

  /// Bin subtraction operator
  inline HistoBin2D operator - (const HistoBin2D& a, const HistoBin2D& b) {
    HistoBin2D rtn(a);
    rtn -= a;
    return rtn;
  }


}

#endif
