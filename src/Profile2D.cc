#include "YODA/Profile2D.h"
#include "YODA/Scatter3D.h"
#include "YODA/Histo2D.h"

namespace YODA { 
  
  void Profile2D::fill(double x, double y, double z, double weight) {
    _axis.fill(x, y, z, weight);
  }

  double Profile2D::sumW(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().sumW2();
    double sumw = 0;
    foreach (const ProfileBin2D& b, bins()) {
      sumw += b.sumW();
    }
    return sumw;
  }

  double Profile2D::sumW2(bool includeoverflows) const {
    if (includeoverflows) return _axis.totalDbn().sumW2();
    double sumw = 0;
    foreach (const ProfileBin2D& b, bins()) {
      sumw += b.sumW();
    }
    return sumw;
  }

  /// Constructor from a Scatter3D's binning, with optional new path
  Profile2D::Profile2D(const Scatter3D& s, const std::string& path)
    : AnalysisObject("Profile2D", (path.size() == 0) ? s.path() : path, s, s.title())
  {
    Bins bins;
    foreach (const Scatter3D::Point& p, s.points()) {
      bins.push_back(ProfileBin2D(p.xMin(), p.yMin(), p.xMax(), p.yMax()));
    }
    _axis = Profile2DAxis(bins);
  }

  /// Constructor from a Histo2D's binning, with optional new path
  Profile2D::Profile2D(const Histo2D& h, const std::string& path)
    : AnalysisObject("Profile2D", (path.size() == 0) ? h.path() : path, h, h.title())
  {
    Bins bins;
    foreach (const HistoBin2D& b, h.bins()) {
      bins.push_back(ProfileBin2D(b.xMin(), b.yMin(), b.xMax(), b.yMax()));
    }
    _axis = Profile2DAxis(bins);
  }

  /// Divide two profile histograms
  Scatter3D divide(Profile2D& numer, Profile2D& denom) {
    /// @todo Make this check work
    if(numer != denom) throw "It is impossible to add two incompatibly binned profile histograms!";
    Scatter3D tmp;
    for (size_t i = 0; i < numer.numBins(); ++i) {
      const ProfileBin2D& b1 = numer.bin(i);
      const ProfileBin2D& b2 = denom.bin(i);
      const ProfileBin2D& bA = b1 + b2;

      assert(fuzzyEquals(b1.focus().first, b2.focus().first));
      assert(fuzzyEquals(b1.focus().second, b2.focus().second));

      const double x = bA.focus().first/2;
      const double y = bA.focus().second/2;
      const double z = b1.mean()/b2.mean();
    }
  }  
    //return Scatter3D();
  }
