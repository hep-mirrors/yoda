#ifndef YODA_Profile1D_h
#define YODA_Profile1D_h

#include "YODA/AnalysisObject.h"
#include "YODA/ProfileBin2D.h"
#include "YODA/Scatter3D.h"
#include "YODA/Dbn3D.h"
#include "YODA/Axis2D.h"
#include "YODA/Exceptions.h"

#include <vector>
#include <string>
#include <map>

namespace YODA {

  // Forward declarations
  class Histo2D;
  class Scatter3D;

  /// Convenience typedef
  typedef Axis2D<ProfileBin2D, Dbn3D> Profile2DAxis;

  /// A one-dimensional profile histogram.
  class Profile2D : public AnalysisObject {
  public:

    /// Convenience typedefs
    typedef Profile2DAxis Axis;
    typedef Axis::Bins Bins;
    typedef ProfileBin2D Bin;
    typedef std::pair<double, double> Point;
    typedef std::pair<Point, Point> Segment;


    /// @name Constructors
    //@{

    /// Constructor giving range and number of bins
    Profile2D(size_t nbinsX, double lowerX, double upperX,
              size_t nbinsY, double lowerY, double upperY,
              const std::string& path="", const std::string& title="")
      : AnalysisObject("Profile2D", path, title),
        _axis(nbinsX, lowerX, upperX, nbinsY, lowerY, upperY)
    { }

    /// Constructor giving explicit bin edges in the direction of X and Y
    Profile2D(const std::vector<double>& xedges, const std::vector<double>& yedges,
              const std::string& path="", const std::string& title="")
      : AnalysisObject("Profile2D", path, title),
        _axis(xedges, yedges)
    { }

    /// A copy constructor with optional new path
    Profile2D(const Profile2D& p, const std::string& path="");

    /// A constructor from a Scatter3D's binning, with optional new path
    Profile2D(const Scatter3D& s, const std::string& path="");

    /// Constructor from a Histo2D's binning, with optional new path
    Profile2D(const Histo2D& h, const std::string& path="");

    /// A state setting constructor
    Profile2D();

    /// Assignment operator
    Profile2D& operator = (const Profile2D& p1) {
      setPath(p1.path());
      setTitle(p1.title());
      _axis = p1._axis;
      return *this;
    }

    //@}


    /// @name Persistency hooks
    //@{

    /// Get name of the analysis object type for persisting
    std::string type() const { return "Profile2D"; }

    //@}


    /// @name Modifiers
    //@{

    /// Fill histo by value and weight
    void fill(double x, double y, double z, double weight=1.0);

    /// @brief Reset the histogram
    ///
    /// Keep the binning but reset the statistics
    void reset() {
      _axis.reset();
    }

    /// Rescale as if all fill weights had been different by a @a scalefactor
    void scaleW(double scalefactor) {
      _axis.scaleW(scalefactor);
    }

    /// Merge together the bin range with indices from @a from to @a to, inclusive
    void mergeBins(size_t from, size_t to) {
      _axis.mergeBins(from, to);
    }

    /// Merge every group of n bins, starting from the LHS
    void rebin(size_t n) {
      throw "IMPLEMENT!";
      //_axis.rebin(n);
    }


    /// @brief Bin addition operator
    /// Add a bin to an axis described by its lower-left and higher-right point
    void addBin(double lowX, double lowY, double highX, double highY) {
      _axis.addBin(lowX, lowY, highX, highY);
    }

    /// @brief Bin addition operator
    /// Add a set of bins delimiting coordinates of which are contained
    /// in binLimits vector.
    void addBin(const std::vector<Segment>& binLimits) {
      _axis.addBin(binLimits);
    }

    //@}


    /// @name Bin accessors
    //@{

    /// Number of bins of this axis (not counting under/over flow)
    size_t numBins() const {
      return _axis.bins().size();
    }


    /// Access the bin vector (non-const)
    std::vector<YODA::ProfileBin2D>& bins() {
      return _axis.bins();
    }

    /// Access the bin vector (const)
    const std::vector<YODA::ProfileBin2D>& bins() const {
      return _axis.bins();
    }


    /// Access a bin by index (non-const)
    const ProfileBin2D& bin(size_t index) const {
      return _axis.bins()[index];
    }


    /// Access a bin by x-coordinate (non-const)
    ProfileBin2D& binByCoord(double x, double y) {
      return _axis.binByCoord(x, y);
    }

    /// Access a bin by x-coordinate (const)
    const ProfileBin2D& binByCoord(double x, double y) const {
      return _axis.binByCoord(x, y);
    }


    /// Access summary distribution, including gaps and overflows (non-const version)
    Dbn3D& totalDbn() {
      return _axis.totalDbn();
    }

    /// Access summary distribution, including gaps and overflows (const version)
    const Dbn3D& totalDbn() const {
      return _axis.totalDbn();
    }


    /// Access outflows (non-const)
    std::vector<std::vector<Dbn3D> >& outflows() {
      return _axis.outflows();
    }

    /// Access outflows (const)
    const std::vector<std::vector<Dbn3D> >& outflows() const {
      return _axis.outflows();
    }

    //@}


  public:

    /// @name Whole histo data
    //@{

    /// Get sum of weights in histo
    double sumW(bool includeoverflows=true) const;
    double sumW2(bool includeoverflows=true) const;

    //@}

  public:

    /// @name Adding and subtracting histograms
    //@{

    /// Add another profile to this one
    Profile2D& operator += (const Profile2D& toAdd) {
      _axis += toAdd._axis;
      return *this;
    }

    /// Subtract another profile from this one
    Profile2D& operator -= (const Profile2D& toSubtract) {
      _axis -= toSubtract._axis;
      return *this;
    }

    inline bool operator == (const Profile2D& other){
      return _axis == other._axis;
    }

    inline bool operator != (const Profile2D& other){
      return ! operator == (other);
    }
    //@}-

  private:

    /// @name Bin data
    //@{

    /// The bins contained in this profile histogram
    Axis2D<ProfileBin2D, Dbn3D> _axis;

    //@}
  };

  /// @name Combining profile histos: global operators
  //@{

  /// Add two profile histograms
  inline Profile2D add(const Profile2D& first, const Profile2D& second) {
    Profile2D tmp = first;
    tmp += second;
    return tmp;
  }

  /// Add two profile histograms
  inline Profile2D operator + (const Profile2D& first, const Profile2D& second) {
    return add(first,second);
  }

  /// Subtract two profile histograms
  inline Profile2D subtract(const Profile2D& first, const Profile2D& second) {
    Profile2D tmp = first;
    tmp -= second;
    return tmp;
  }

  /// Subtract two profile histograms
  inline Profile2D operator - (const Profile2D& first, const Profile2D& second) {
    return subtract(first,second);
  }

  /// Divide two profile histograms
  Scatter3D divide(const Profile2D& numer, const Profile2D& denom);

  /// Divide two profile histograms
  inline Scatter3D operator / (const Profile2D& numer, const Profile2D& denom) {
    return divide(numer, denom);
  }

  //@}

}

#endif

