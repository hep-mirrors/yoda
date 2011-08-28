#ifndef YODA_ProfileBin2D_h
#define YODA_ProfileBin2D_h

#include "YODA/Bin2D.h"
#include "YODA/Dbn3D.h"
#include "YODA/Exceptions.h"

namespace YODA {
    
  class ProfileBin2D : public Bin2D<Dbn3D> {
  public:
      
    /// @name Constructors
    //@{

    /// Constructor giving lowedgesX/Y
    ProfileBin2D(double lowX, double lowY, double highX, double highY) 
      : Bin2D(lowX, lowY, highX, highY)
    { }

    ProfileBin2D(const std::vector<std::pair<std::pair<double,double>,std::pair<double,double> > >& edges)
      : Bin2D(edges)
    { }

    /// Unpersisting constructor
    ProfileBin2D(double lowX, double lowY, double highX, double highY,
                 const Dbn3D& dbnxyz)
      : Bin2D(lowX, lowY, highX, highY)
    { }

    /// Copy constructor
    ProfileBin2D(const ProfileBin2D& pb)
      : Bin2D(pb)
    { }

    /// Copy assignment
    ProfileBin2D& operator = (const ProfileBin2D& pb) {
      Bin2D::operator=(pb);
      return *this;
    }

    //@}

    /// @name Modifiers
    //@{

    /// Fill by x, y, z values and weight
    void fill(double x, double y, double z, double weight=1.0) {
      assert(x >= xMin() && x <= xMax() && y >= yMin() && y <= yMax());
      _dbn.fill(x, y, z, weight);
    }

    /// Fill the bin at the midpoint with a given z value
    void fillBin(double z, double weight=1.0){
      fill(midpoint().first, midpoint().second, z, weight);
    }
    
    //@}

  public:
    
    /// @name Bin content info
    //@{
      
    double mean() const {
      return _dbn.zMean();
    }

    double stdDev() const {
      return _dbn.zStdDev();
    }

    double variance() const {
      return _dbn.zVariance();
    }

    double stdErr() const {
      return _dbn.zStdErr();
    }

    //@}

    /// @name Raw z distribution statistics
    //@{

    ///@todo: Check if it is correct

    /// The sum of z*weight
    double sumWZ() const {
      return _dbn.sumWX();
    }

    double sumWZ2() const {
      return _dbn.sumWX2();
    }

    //@}

  public:
    
    /// Add two bins (for use by Profile2D)
    ProfileBin2D& operator += (const ProfileBin2D& toAdd) {
      return add(toAdd);
    }

    ProfileBin2D& operator -= (const ProfileBin2D& toSubtract) {
      return subtract(toSubtract);
    }

  protected:
    
    /// Add two bins
    ProfileBin2D& add(const ProfileBin2D& pb) {
      Bin2D<Dbn3D>::add(pb);
      return *this;
    }

    /// Subtract one bin from another
    ProfileBin2D& subtract(const ProfileBin2D& pb) {
      Bin2D<Dbn3D>::subtract(pb);
      return *this;
    }
  };

  inline ProfileBin2D operator + (const ProfileBin2D& a, const ProfileBin2D& b) {
    ProfileBin2D rtn(a);
    rtn += a;
    return rtn;
  }

  inline ProfileBin2D operator - (const ProfileBin2D& a, const ProfileBin2D& b) {
    ProfileBin2D rtn(a);
    rtn -= a;
    return rtn;
  }

}

#endif

