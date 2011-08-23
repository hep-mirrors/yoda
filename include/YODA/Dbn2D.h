#ifndef YODA_Dbn2D_h
#define YODA_Dbn2D_h

#include "YODA/Exceptions.h"
#include "YODA/Dbn1D.h"

namespace YODA {


  /// A representation of the vital statistics of a 2D distribution
  class Dbn2D {
  public:

    /// @name Constructors
    //@{

    /// Default constructor
    Dbn2D() {
      reset();
    }


    /// Constructor to set a distribution with a pre-filled state.
    ///
    /// Principally designed for internal persistency use.
    Dbn2D(unsigned int numEntries, double sumW, double sumW2,
          double sumWX, double sumWX2, double sumWY, double sumWY2, double sumWXY)
      : _dbnX(numEntries, sumW, sumW2, sumWX, sumWX2),
        _dbnY(numEntries, sumW, sumW2, sumWY, sumWY2),
        _sumWXY(sumWXY)
    {  }


    /// Copy constructor
    ///
    /// Sets all the parameters using the ones provided from an existing Dbn2D.
    Dbn2D(const Dbn2D& toCopy) {
      _dbnX = toCopy._dbnX;
      _dbnY = toCopy._dbnY;
      _sumWXY = toCopy._sumWXY;
    }

    //@}


    /// @name Modifiers
    //@{

    /// Fill, providing the fill coordinates as two different numbers.
    void fill(double valX, double valY, double weight=1.0) {
      _dbnX.fill(valX, weight);
      _dbnY.fill(valY, weight);
      _sumWXY += weight*valX*valY;
    }


    /// Fill, providing the fill coordinates as a pair.
    void fill(std::pair<double,double> val, double weight=1.0) {
      fill(val.first, val.second, weight);
    }


    /// Reset the distribution to an unfilled state.
    void reset() {
      _dbnX.reset();
      _dbnY.reset();
      _sumWXY = 0.0;
    }


    /// Rescale as if all fill weights had been different by factor @a scalefactor.
    void scaleW(double scalefactor) {
      _dbnX.scaleW(scalefactor);
      _dbnY.scaleW(scalefactor);
      _sumWXY *= scalefactor;
    }


    /// Rescale x: needed if x histo bin edges are rescaled.
    void scaleX(double xscale) {
      _dbnX.scaleX(xscale);
      _sumWXY *= xscale;
    }


    /// Rescale y: needed if y histo bin edges are rescaled.
    void scaleY(double yscale) {
      _dbnY.scaleX(yscale);
      _sumWXY *= yscale;
    }


    /// Rescale x and y: needed if histo bin edges are rescaled.
    void scaleXY(double xscale, double yscale) {
      scaleX(xscale);
      scaleY(yscale);
    }

    //@}


    /// @name Distribution statistics
    //@{

    /// Weighted mean, \f$ \bar{x} \f$, of distribution.
    double xMean() const {
      return _dbnX.mean();
    }

    /// Weighted mean, \f$ \bar{y} \f$, of distribution.
    double yMean() const {
      return _dbnY.mean();
    }

    /// Weighted \f$ x \f$ variance, \f$ \sigma_x^2 \f$, of distribution.
    double xVariance() const {
      return _dbnX.variance();
    }

    /// Weighted \f$ y \f$ variance, \f$ \sigma_y^2 \f$, of distribution.
    double yVariance() const {
      return _dbnY.variance();
    }

    /// Weighted \f$ x \f$ standard deviation, \f$ \sigma_x \f$, of distribution.
    double xStdDev() const {
      return _dbnX.stdDev();
    }

    /// Weighted \f$ y \f$ standard deviation, \f$ \sigma_y \f$, of distribution.
    double yStdDev() const {
      return _dbnY.stdDev();
    }

    /// Weighted standard error on the \f$ x \f$ mean, \f$ \sim \sigma_x/\sqrt{N-1} \f$, of distribution.
    double xStdErr() const {
      return _dbnX.stdErr();
    }

    /// Weighted standard error on the \f$ y \f$ mean, \f$ \sim \sigma_y/\sqrt{N-1} \f$, of distribution.
    double yStdErr() const {
      return _dbnY.stdDev();
    }

    //@}


    /// @name Raw distribution running sums
    //@{

    /// Number of entries (number of times @c fill was called, ignoring weights)
    unsigned long numEntries() const {
      return _dbnX.numEntries();
    }

    /// Effective number of entries \f$ = (\sum w)^2 / \sum w^2 \f$
    double effNumEntries() const {
      return _dbnX.effNumEntries();
    }

    /// The sum of weights
    double sumW() const {
      return _dbnX.sumW();
    }

    /// The sum of weights squared
    double sumW2() const {
      return _dbnX.sumW2();
    }

    /// The sum of x*weight
    double sumWX() const {
      return _dbnX.sumWX();
    }

    /// The sum of y*weight
    double sumWY() const {
      return _dbnY.sumWX();
    }

    /// The sum of x*x*weight
    double sumWX2() const {
      return _dbnX.sumWX2();
    }

    /// The sum of y*y*weight
    double sumWY2() const {
      return _dbnY.sumWX2();
    }

    /// The sum of x*y*weight
    double sumWXY() const {
      return _sumWXY;
    }

    //@}


    /// @name Operators
    //@{

    /// Add two dbns
    Dbn2D& operator += (const Dbn2D& d) {
      return add(d);
    }

    /// Subtract one dbn from another
    Dbn2D& operator -= (const Dbn2D& d) {
      return subtract(d);
    }

    /// @brief Interchange X and Y subdistributions
    /// Mostly used for operations on total distribution of an Axis
    void flipXY() {
      Dbn1D temp(_dbnX);
      _dbnX = _dbnY;
      _dbnY = temp;
    }

    /// Transform into a Dbn1D parallel to X axis (dropping Y term)
    /// @todo Rename
    Dbn1D transformX() {
      Dbn1D ret(_dbnX);
      return ret;
    }

    /// Transform into a Dbn1D parallel to Y axis (dropping X term)
    /// @todo Rename
    Dbn1D transformY() {
      Dbn1D ret(_dbnY);
      return ret;
    }
    //@}


  protected:

    /// Add two dbns (internal, explicitly named version)
    Dbn2D& add(const Dbn2D& d) {
      _dbnX += d._dbnX;
      _dbnY += d._dbnY;
      _sumWXY += d._sumWXY;
      return *this;
    }

    /// Subtract one dbn from another (internal, explicitly named version)
    Dbn2D& subtract(const Dbn2D& d) {
      _dbnX -= d._dbnX;
      _dbnY -= d._dbnY;
      _sumWXY -= d._sumWXY;
      return *this;
    }


  private:

    /// @name Storage
    //@{

    /// The x moments and the pure-weight quantities are stored in a 1D "x" distribution
    Dbn1D _dbnX;

    /// The y moments are stored in a 1D "y" distribution
    Dbn1D _dbnY;

    /// The second-order "cross-term" that can't be handled using the 1D distributions
    double _sumWXY;

    //@}

  };


  /// Add two dbns
  inline Dbn2D operator + (const Dbn2D& a, const Dbn2D& b) {
    Dbn2D rtn = a;
    rtn += b;
    return rtn;
  }

  /// Subtract one dbn from another
  inline Dbn2D operator - (const Dbn2D& a, const Dbn2D& b) {
    Dbn2D rtn = a;
    rtn -= b;
    return rtn;
  }


}

#endif
