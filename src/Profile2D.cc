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

  /// A copy constructor with optional new path
  Profile2D::Profile2D(const Profile2D& p, const std::string& path)
    : AnalysisObject("Profile2D", p.path(), p, p.title())
  {
    _axis = p._axis;
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
  /// Note: I have remove the const requirement as it makes the program compile,
  /// was it right to do?
  Scatter3D divide(Profile2D& numer, Profile2D& denom) {
    /// @todo Make this check work
    if(numer != denom) throw "It is impossible to add two incompatibly binned profile histograms!";
    Scatter3D tmp;
    for (size_t i = 0; i < numer.numBins(); ++i) {
      const ProfileBin2D& b1 = numer.bin(i);
      const ProfileBin2D& b2 = denom.bin(i);
      const ProfileBin2D& bL = b1 + b2;

      assert(fuzzyEquals(b1.focus().first, b2.focus().first));
      assert(fuzzyEquals(b1.focus().second, b2.focus().second));

      const double x = bL.focus().first/2;
      const double y = bL.focus().second/2;
      const double z = b1.mean()/b2.mean();

      const double exminus = x - bL.xMin()/2;
      const double explus = bL.xMax()/2 - x;

      const double eyminus = y - bL.yMin()/2;
      const double eyplus = bL.yMax()/2 - y;

      const double ez = z * sqrt(sqr(b1.stdErr()/b1.mean()) + sqr(b2.stdErr()/b2.mean()));

      tmp.addPoint(x, exminus, explus, y, eyminus, eyplus, z, ez, ez);
    }
    assert(tmp.numPoints() == numer.numBins());
    return tmp;
  }
  
}
