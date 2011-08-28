#ifndef YODA_Dbn3D_h
#define YODA_Dbn3D_h

#include "YODA/Exceptions.h"
#include "YODA/Dbn1D.h"

namespace YODA {
  
  class Dbn3D {
  public:
  
    /// @name COnstructors
    //@{

    /// Default constructor
    Dbn3D() { 
      reset();
    }

    /// Unpersisting constructor
    Dbn3D(unsigned int numEntries, double sumW, double sumW2, double sumWX,
          double sumWX2, double sumWY, double sumWY2, double sumWZ, double sumWZ2,
          double sumWXY, double sumWXZ, double sumWYZ, double sumWXYZ)
      : _dbnX(numEntries, sumW, sumW2, sumWX, sumWX2),
      _dbnY(numEntries, sumW, sumW2, sumWY, sumWY2),
      _dbnZ(numEntries, sumW, sumW2, sumWZ, sumWZ2),
      _sumWXY(sumWXY),
      _sumWXZ(sumWXZ),
      _sumWYZ(sumWYZ),
      _sumWXYZ(sumWYZ)
    { }
   
    /// Copy constructor
    Dbn3D(const Dbn3D& toCopy) {
      _dbnX = toCopy._dbnX;
      _dbnY = toCopy._dbnY;
      _sumWXY = toCopy._sumWXY;
    }

    //@}
   
    /// @name Modifiers
    //@{

    /// Fill, providing coordinates as three distinct numbers.
    void fill(double valX, double valY, double valZ, double weight=1.0) {
      _dbnX.fill(valX, weight);
      _dbnY.fill(valY, weight);
      _dbnZ.fill(valZ, weight);
      _sumWXY += weight*valX*valY;
      _sumWXZ += weight*valX*valZ;
      _sumWYZ += weight*valY*valZ;
      _sumWXYZ += weight*valX*valY*valZ;
    } 

    /// Reset to the unfilled state
    void reset() {
      _dbnX.reset();
      _dbnY.reset();
      _dbnZ.reset();
      _sumWXY =   0.0;
      _sumWXZ =   0.0;
      _sumWYZ =   0.0;
      _sumWXYZ =  0.0;
    }

    /// Rescale the weights by a scalefactor
    void scaleW(double scalefactor) {
      _dbnX.scaleW(scalefactor);
      _dbnY.scaleW(scalefactor);
      _dbnZ.scaleW(scalefactor);
      _sumWXY *= scalefactor;
      _sumWXZ *= scalefactor;
      _sumWYZ *= scalefactor;
      _sumWXYZ *= scalefactor;
    }

    /// Scale x axis: scale X edges
    void scaleX(double xscale) {
      _dbnX.scaleX(xscale);
      _sumWXY *= xscale;
      _sumWXZ *= xscale;
      _sumWXYZ *= xscale;
    }

    /// Scale y axis: scale Y edges
    void scaleY(double yscale) {
      _dbnY.scaleX(yscale);
      _sumWXY *= yscale;
      _sumWYZ *= yscale;
      _sumWXYZ *= yscale;
    }

    /// Scale z axis: scale Z edges
    void scaleZ(double zscale) {
      _dbnZ.scaleX(zscale);
      _sumWXZ *= zscale;
      _sumWYZ *= zscale;
      _sumWXYZ *= zscale;
    }

    void scaleXY(double xscale, double yscale) {
      scaleX(xscale);
      scaleY(yscale);
    }

    void scaleXZ(double xscale, double zscale) {
      scaleX(xscale);
      scaleZ(zscale);
    }

    void scaleYZ(double yscale, double zscale) {
      scaleY(yscale);
      scaleZ(zscale);
    }

    void scaleXYZ(double xscale, double yscale, double zscale) {
      scaleX(xscale);
      scaleY(yscale);
      scaleZ(zscale);
    }

    //@}

    /// @name Distribution statistics
    //@{
    double xMean() const {
      return _dbnX.mean();
    }

    double yMean() const {
      return _dbnY.mean();
    }

    double zMean() const {
      return _dbnZ.mean();
    }

    double xVariance() const {
      return _dbnX.variance();
    }

    double yVariance() const {
      return _dbnY.variance();
    }

    double zVariance() const {
      return _dbnZ.variance();
    }

    double xStdDev() const {
      return _dbnX.stdDev();
    }

    double yStdDev() const {
      return _dbnY.stdDev();
    }

    double zStdDev() const {
      return _dbnZ.stdDev();
    }

    double xStdErr() const {
      return _dbnX.stdErr();
    }

    double yStdErr() const {
      return _dbnY.stdErr();
    }

    double zStdErr() const {
      return _dbnZ.stdErr();
    }

    //@}

    /// @name Raw distribution running sums
    //@{

    /// Number of entries
    size_t numEntries() const {
      return _dbnX.numEntries();
    }

    double effNumEntries() const {
      return _dbnX.effNumEntries();
    }

    double sumW() const {
      return _dbnX.sumW();
    }

    double sumW2() const {
      return _dbnX.sumW2();
    }

    double sumWX() const {
      return _dbnX.sumWX();
    }

    double sumWX2() const {
      return _dbnX.sumWX2();
    }

    double sumWY() const {
      return _dbnY.sumWX();
    }

    double sumWY2() const {
      return _dbnY.sumWX2();
    }

    double sumWZ() const {
      return _dbnZ.sumWX();
    }

    double sumWZ2() const {
      return _dbnZ.sumWX2();
    }

    double sumWXY() const {
      return _sumWXY;
    }

    double sumWXZ() const {
      return _sumWXZ;
    }

    double sumWYZ() const {
      return _sumWXZ;
    }

    double sumWXYZ() const {
      return _sumWXYZ;
    }

    //@}

    // @name Operators
    //@{

    /// Add two dbns
    Dbn3D& operator += (const Dbn3D& d) {
      return add(d);
    }

    /// Substract two dbns
    Dbn3D& operator -= (const Dbn3D& d) {
      return subtract(d);
    }

    /// Interchange X and Y subdistribution
    void flipXY() {
      Dbn1D temp(_dbnX);
      _dbnX = _dbnY;
      _dbnY = temp;
    }

    void flipXZ() {
      Dbn1D temp(_dbnX);
      _dbnX = _dbnZ;
      _dbnZ = temp;
    }

    void flipYZ() {
      Dbn1D temp(_dbnY);
      _dbnY = _dbnZ;
      _dbnZ = temp;
    }

    /// Transform into a Dbn1D parallel to X axis
    Dbn1D transformX() {
      return _dbnX;
    }

    Dbn1D transformY() {
      return _dbnY;
    }

    Dbn1D transformZ() {
      return _dbnZ;
    }
    //@}

  protected:
  
    /// Add two dbns
    Dbn3D& add(const Dbn3D& d) {
      _dbnX += d._dbnX;
      _dbnY += d._dbnY;
      _dbnZ += d._dbnZ;
      _sumWXY += d._sumWXY;
      _sumWXZ += d._sumWXZ;
      _sumWYZ += d._sumWYZ;
      _sumWXYZ += d._sumWXYZ;
      return *this;
    }

    Dbn3D& subtract(const Dbn3D& d) {
      _dbnX -= d._dbnX;
      _dbnY -= d._dbnY;
      _dbnZ -= d._dbnZ;
      _sumWXY -= d._sumWXY;
      _sumWXZ -= d._sumWXZ;
      _sumWYZ -= d._sumWYZ;
      _sumWXYZ -= d._sumWXYZ;
      return *this;
    }

  private:
    /// @name Storage
    // @{
  
    /// The x moments and the pure-weight quantities are stored in a 1D 'x' distribution
    Dbn1D _dbnX;

    /// The y moments are stored in a 1D 'y' distribution
    Dbn1D _dbnY;

    /// The z moments are stored in a 1D 'z' distribution
    Dbn1D _dbnZ;

    /// Cross-terms
    double _sumWXY;
    double _sumWXZ;
    double _sumWYZ;
    double _sumWXYZ;
    //@}
  };
  
  /// Add two dbns
  inline Dbn3D operator + (const Dbn3D& a, const Dbn3D& b) {
    Dbn3D rtn = a;
    rtn += b;
    return rtn;
  }

  inline Dbn3D operator - (const Dbn3D& a, const Dbn3D& b) {
    Dbn3D rtn = a;
    rtn -= b;
    return rtn;
  }
}

#endif


