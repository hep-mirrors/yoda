#ifndef YODA_Dbn2D_h
#define YODA_Dbn2D_h

#include "YODA/Exceptions.h"

namespace YODA {


  /// A representation of the vital statistics of a 2D distribution
  class Dbn2D {
  public:

    /// @name Constructors
    //@{

    Dbn2D() {
      reset();
    }

    /// @brief Constructor to set a distribution with a pre-filled state.
    /// Principally designed for internal persistency use.
    Dbn2D(unsigned int numEntries, double sumW, double sumW2,
          double sumWX, double sumWX2, double sumWY, double sumWY2, double sumWXY) {
      _numFills = numEntries;
      _sumW = sumW;
      _sumW2 = sumW2;
      _sumWX = sumWX;
      _sumWX2 = sumWX2;
      _sumWY = sumWY;
      _sumWY2 = sumWY2;
      _sumWXY = sumWXY;
    }

    //@}


    /// @name Modifiers
    //@{

    /// Fill, providing the fill coordinates as two different numbers.
    void fill(double valX, double valY, double weight=1.0);

    /// Fill, providing the fill coordinates as a pair.
    void fill(std::pair<double,double> val, double weight=1.0);

    /// Reset the distribution to an unfilled state.
    void reset();

    /// Rescale as if all fill weights had been different by factor @a scalefactor.
    void scaleW(double scalefactor) {
      const double sf = scalefactor;
      _sumW *= sf;
      _sumW2 *= sf*sf;
      _sumWX *= sf;
      _sumWX2 *= sf*sf;
      _sumWY *= sf;
      _sumWY2 *= sf*sf;
      _sumWXY *= sf;
    }

    /// Rescale x and y: needed if histo bin edges are rescaled.
    void scale(double scaleX, double scaleY) {
      _sumWX *= scaleX;
      _sumWX2 *= scaleX*scaleX;
      _sumWY *= scaleY;
      _sumWY2 *= scaleY*scaleY;
      _sumWXY *= scaleX*scaleY;
    }

    //@}


    /// @name Distribution statistics
    //@{

    /// Weighted mean, \f$ \bar{x} \f$, of distribution.
    double xMean() const;

    /// Weighted mean, \f$ \bar{y} \f$, of distribution.
    double yMean() const;

    /// Weighted \f$ x \f$ variance, \f$ \sigma_x^2 \f$, of distribution.
    double xVariance() const;

    /// Weighted \f$ y \f$ variance, \f$ \sigma_y^2 \f$, of distribution.
    double yVariance() const;

    /// Weighted \f$ x \f$ standard deviation, \f$ \sigma_x \f$, of distribution.
    double xStdDev() const;

    /// Weighted \f$ y \f$ standard deviation, \f$ \sigma_y \f$, of distribution.
    double yStdDev() const;

    /// Weighted standard error on the \f$ x \f$ mean, \f$ \sim \sigma_x/\sqrt{N-1} \f$, of distribution.
    double xStdErr() const;

    /// Weighted standard error on the \f$ y \f$ mean, \f$ \sim \sigma_y/\sqrt{N-1} \f$, of distribution.
    double yStdErr() const;

    //@}


    /// @name Raw distribution running sums
    //@{

    /// Number of entries (number of times @c fill was called, ignoring weights)
    unsigned long numEntries() const;

    /// Effective number of entries \f$ = (\sum w)^2 / \sum w^2 \f$
    double effNumEntries() const;

    /// The sum of weights
    double sumW() const;

    /// The sum of weights squared
    double sumW2() const;

    /// The sum of x*weight
    double sumWX() const;

    /// The sum of y*weight
    double sumWY() const;

    /// The sum of x*x*weight
    double sumWX2() const;

    /// The sum of y*y*weight
    double sumWY2() const;

    /// The sum of x*y*weight
    double sumWXY() const;

    //@}


    /// @name Operators
    //@{

    /// Add two dbns
    Dbn2D& operator += (const Dbn2D&);

    /// Subtract one dbn from another
    Dbn2D& operator -= (const Dbn2D&);

    //@}


  protected:

    /// Add two dbns (internal, explicitly named version)
    Dbn2D& add(const Dbn2D&);

    /// Subtract one dbn from another (internal, explicitly named version)
    Dbn2D& subtract(const Dbn2D&);


  private:

    unsigned long _numFills;
    double _sumW;
    double _sumW2;
    double _sumWX;
    double _sumWY;
    double _sumWX2;
    double _sumWY2;
    double _sumWXY;

  };


  /// Add two dbns
  inline Dbn2D operator + (const Dbn2D& a, const Dbn2D& b);

  /// Subtract one dbn from another
  inline Dbn2D operator - (const Dbn2D& a, const Dbn2D& b);

}

#endif
