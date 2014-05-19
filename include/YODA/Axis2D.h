#ifndef YODA_Axis2D_h
#define YODA_Axis2D_h

#include "YODA/AnalysisObject.h"
#include "YODA/Exceptions.h"
#include "YODA/Bin.h"
#include "YODA/Utils/MathUtils.h"
#include "YODA/Utils/Predicates.h"
#include "YODA/Utils/BinSearcher.h"
#include <limits>

#include <string>

namespace YODA {


  /// @brief 2D bin container
  ///
  /// This class handles most of the low-level operations on an axis of bins
  /// arranged in a 2D line (including gaps).
  template <typename BIN2D, typename DBN>
  class Axis2D {
  public:

    /// Typedefs
    //@{

    /// Bin type
    typedef BIN2D Bin;

    /// A vector containing 2D bins. Not used for searching.
    typedef typename std::vector<Bin> Bins;

    // Distinguishing between cuts and pairs is useful
    typedef std::vector<double> EdgeCuts;
    typedef std::pair<double, double> EdgePair1D;
    typedef std::pair<EdgePair1D, EdgePair1D> EdgePair2D;
    typedef std::vector<EdgePair2D> EdgePair2Ds;

    // Ordered in some arbitrary way: see outflow(int, int)
    typedef std::vector<DBN> Outflows;

    //@}

    /// @name Constructors
    //@{

    // Empty constructor
    Axis2D()
      : _locked(false)
    {
      reset();
    }

    /// A constructor with specified x and y axis bin cuts.
    Axis2D(const EdgeCuts &xedges,
           const EdgeCuts &yedges)
      : _locked(false)
    {
      addBins(xedges, yedges);
      reset();
    }

    /// Constructor accepting X/Y ranges and number of bins
    /// on each of the axis. Both axes are divided linearly.
    Axis2D(size_t nbinsX, const std::pair<double,double>& rangeX,
           size_t nbinsY, const std::pair<double,double>& rangeY)
      : _locked(false)
    {
      addBins(linspace(nbinsX, rangeX.first, rangeX.second),
              linspace(nbinsY, rangeY.first, rangeY.second));
      reset();
    }

    /// Constructor accepting a list of bins
    Axis2D(const Bins& bins) {
      addBins(bins);
      reset();
    }

    /// State-setting constructor for persistency
    Axis2D(const Bins& bins,
           const DBN& totalDbn,
           const Outflows& outflows)
      : _dbn(totalDbn), _outflows(outflows),
      _locked(false) // Does this make sense?
    {
      if (_outflows.size() != 8) {
        throw Exception("Axis2D outflow containers must have exactly 8 elements");
      }
      addBins(bins);
    }


    void reset() {
      _dbn.reset();
      _outflows.assign(8, DBN());
    }

    /// Get the number of bins.
    size_t numBins() const {
      return _bins.size();
    }

    /// Get the number of bins on the x-axis. This is only sensible for
    /// perfectly regular gridded bins. For irregular binnings, this is
    /// the number of cuts that were necessary to grid the data.
    size_t numBinsX() const {
      return _nx;
    }

    /// Get the number of bins on the y-axis. This is only sensible for
    /// perfectly regular gridded bins. For irregular binnings, this is
    /// the number of cuts that were necessary to grid the data.
    size_t numBinsY() const {
      return _ny;
    }

    //@}
    //
    /// @name Statistics accessor functions
    //@{

    /// Get the outflow by x-index and y-index -- e.g. (+1, -1) is outside by
    /// being greater than the greatest x-edge and less than the lowest y-edge.
    DBN& outflow(int ix, int iy) {
      // Lookup table for mapping. This is necessary as there is no
      // numerical way to skip the eighth item. This also allows for
      // arbitrary orderings.
      const static unsigned char outflowMapping[9] = {0, 1, 2, 3, 8, 4, 5, 6, 7};
      ++ix;
      ++iy;
      if (ix > 2 || iy > 2)
        throw UserError(
            "Outflow index out of range: valid indices are -1, 0, 1");
      // Find the real index
      size_t realindex = outflowMapping[3 * ix + iy];
      // Check we're not using the invalid index
      if (realindex == 8) {
        throw UserError("(0, 0) is not a valid outflow index");
      }
      return _outflows[realindex];
    }

    /// Get the outflow by x-index and y-index -- e.g. (+1, -1) is outside by
    /// being greater than the greatest x-edge and less than the lowest y-edge.
    /// (const version)
    const DBN& outflow(int ix, int iy) const {
      const static unsigned char outflowMapping[9] = {0, 1, 2, 3, 8, 4, 5, 6, 7};
      ++ix;
      ++iy;
      if (ix > 2 || iy > 2)
        throw UserError(
            "Outflow index out of range: valid indices are -1, 0, 1");
      // Find the real index
      size_t realindex = outflowMapping[3 * ix + iy];
      // Check we're not using the invalid index
      if (realindex == 8) {
        throw UserError("(0, 0) is not a valid outflow index");
      }
      return _outflows[realindex];
    }

    /// Scale each bin as if the entire x-axis had been scaled by this
    /// factor.
    void scaleX(double xscale) {
      scaleXY(xscale, 1.0);
    }

    /// Scale each bin as if the entire y-axis had been scaled by this
    /// factor.
    void scaleY(double yscale) {
      scaleXY(1.0, yscale);
    }

    /// Scale each bin as if the entire x and y-axes had been scaled by
    /// their respective factors.
    void scaleXY(double sx, double sy) {
      _dbn.scaleXY(sx, sy);
      foreach (DBN &dbn, _outflows)
        dbn.scaleXY(sx, sy);
      foreach (Bin &bin, _bins)
        bin.scaleXY(sx, sy);
      _updateAxis(_bins);
    }


    /// Rescale as if all fill weights had been different by factor @a
    /// scalefactor.
    void scaleW(double scaleFactor) {
      _dbn.scaleW(scaleFactor);
      foreach (DBN &dbn, _outflows)
        dbn.scaleW(scaleFactor);
      foreach (Bin &bin, _bins)
        bin.scaleW(scaleFactor);
      _updateAxis(_bins);
    }


    /// Remove the bin at the given index. If many bins need to be
    /// removed, prefer eraseBins(vector[size_t] &) over many calls to this,
    /// as recreating the binhash is comparatively expensive.
    void eraseBin(size_t i) {
      if (i >= numBins())
        throw RangeError("Bin index is out of range");

      // Temporarily unlock the axis during the update
      _bins.erase(_bins.begin() + i);
      _updateAxis(_bins);
    }

    /// Erase a rectangle of bins.
    void eraseBins(const size_t from, const size_t to)
    {
      if (from >= numBins())
        throw RangeError("Initial bin index is out of range");
      if (from >= numBins())
        throw RangeError("Final bin index is out of range");

      Bin &fromBin = bin(from);
      Bin &toBin = bin(to);

      eraseBins(
          std::make_pair(fromBin.xMin(), toBin.xMax()),
          std::make_pair(fromBin.yMin(), toBin.yMax()));
    }

    /// Erase bins in an x- and y-range. Any bins which lie entirely within the
    /// region are deleted. If any part of the bin lies outside this
    /// range, the bin remains, so this has similar behaviour to select
    /// tools in vector graphics gui packages.

    // todo: any ideas on how to test this?
    void eraseBins(const std::pair<double, double> xrange,
                   const std::pair<double, double> yrange)
    {
      size_t xiLow = _binSearcherX.index(xrange.first) - 1;
      size_t xiHigh = _binSearcherX.index(xrange.second) - 1;

      size_t yiLow = _binSearcherY.index(yrange.first) - 1;
      size_t yiHigh = _binSearcherY.index(yrange.second) - 1;

      std::vector<bool> deleteMask(numBins(), false);

      for (size_t yi = yiLow; yi < yiHigh; yi++) {
        for (size_t xi = xiLow; xi < xiHigh; xi++) {
          ssize_t i = _indexes[_index(_nx, xi, yi)];
          if (i == -1 || deleteMask[i]) continue;
          if (bin(i).fitsInside(xrange, yrange))
            deleteMask[i] = true;
        }
      }

      // Now we just update
      eraseBins(deleteMask);
    }

    /// Erase using a vector<bool>, where true represents that a bin
    /// will be deleted, and false means it will be kept.
    void eraseBins(const std::vector<bool> deleteMask) {
      Bins newBins;
      for (size_t i = 0; i < numBins(); i++)
        if (!deleteMask[i])
          newBins.push_back(bins(i));

      _update(newBins);
    }

    //@todo
    bool _gapInRange(size_t from, size_t to) {
      Bin &toBin = bin(to);
      Bin &fromBin = bin(from);
      return true;
    }


    //@todo
    void rebin(size_t n) {
    }

    /// Set the axis lock state
    void _setLock(bool locked) {
      _locked = locked;
    }

    /// Return the lowest-valued bin edge along the x-axis
    double lowEdgeX() const {
      return _xRange.first;
    }

    /// Alias for lowEdgeX()
    double xMin() const {
      return lowEdgeX();
    }

    /// Return the highest-valued bin edge along the x-axis
    double highEdgeX() const {
      return _xRange.second;
    }

    /// Alias for highEdgeX()
    double xMax() const {
      return highEdgeX();
    }

    /// Return the lowest-valued bin edge along the y-axis
    double lowEdgeY() const {
      return _yRange.first;
    }

    /// Alias for lowEdgeY()
    double yMin() const {
      return lowEdgeY();
    }

    /// Return the highest-valued bin edge along the y-axis
    double highEdgeY() const {
      return _yRange.second;
    }

    /// Alias for highEdgeY()
    double yMax() const {
      return highEdgeY();
    }


    /// Add a bin, providing its x- and y- edge ranges
    void addBin(EdgePair1D xrange, EdgePair1D yrange) {
      _checkUnlocked();
      Bins newBins(_bins);
      newBins.push_back(Bin(xrange, yrange));
      _updateAxis(newBins);
    }

    /// Add a vector of pre-made bins
    void addBins(const Bins &bins) {
      if (bins.size() == 0) return;

      _checkUnlocked();

      Bins newBins(_bins);

      foreach(const Bin &b, bins) {
        newBins.push_back(b);
      }

      _updateAxis(newBins);
    }


    /// Add a contiguous set of bins to an axis, via their list of edges
    void addBins(const std::vector<double> &xcuts,
                 const std::vector<double> &ycuts) {
      if (xcuts.size() == 0) return;
      if (ycuts.size() == 0) return;

      _checkUnlocked();

      Bins newBins(_bins);

      for (size_t xi=0; xi < xcuts.size() - 1; xi++) {
        for (size_t yi=0; yi < ycuts.size() - 1; yi++) {
          newBins.push_back(Bin(
                std::make_pair(xcuts[xi], xcuts[xi+1]),
                std::make_pair(ycuts[yi], ycuts[yi+1])));
        }
      }

      _updateAxis(newBins);
    }


    /// Access bin by index
    Bin& bin(size_t i) {
      return _bins[i];
    }

    /// Access bin by index (const)
    const Bin& bin(size_t i) const {
      return _bins[i];
    }

    /// Get the bin index of the bin containing point (x, y).
    int binIndexAt(double x, double y) const {
      size_t xi = _binSearcherX.index(x) - 1;
      size_t yi = _binSearcherY.index(y) - 1;
      if (xi > _nx) return -1;
      if (yi > _ny) return -1;

      return _indexes[_index(_nx, xi, yi)];
    }

    /// Get the bin containing point (x, y).
    Bin& binAt(double x, double y) {
      const int ret = binIndexAt(x, y);
      if (ret == -1) throw RangeError("No bin found!!");
      return bin(ret);
    }

    /// Get the bin containing point (x, y) (const).
    const Bin& binAt(double x, double y) const {
      const int ret = binIndexAt(x, y);
      if (ret == -1) throw RangeError("No bin found!!");
      return bin(ret);
    }

    /// Return the total distribution (non-const)
    DBN& totalDbn() {
      return _dbn;
    }

    /// Return the total distribution (const)
    const DBN& totalDbn() const {
      return _dbn;
    }


    /// Return the bins vector (non-const)
    Bins& bins() {
      return _bins;
    }

    /// Return the bins vector (const)
    const Bins& bins() const {
      return _bins;
    }

    /// Equality operator (on binning only)

    // (DM: Doesn't this break the semantics of equality?  As it's used only
    // rarely, isn't there a real case for having a "binningsCompatible" or
    // similar method?)

    bool operator == (const Axis2D& other) const {
      if (numBins() != other.numBins())
        return false;
      for (size_t i=0; i < numBins(); i++)
        if (!(fuzzyEquals(bin(i).lowEdgeX(), other.bin(i).lowEdgeX()) &&
              fuzzyEquals(bin(i).highEdgeX(), other.bin(i).highEdgeX()) &&
              fuzzyEquals(bin(i).lowEdgeY(), other.bin(i).lowEdgeY()) &&
              fuzzyEquals(bin(i).highEdgeY(), other.bin(i).highEdgeY())))
          return false;

      return true;
    }

    /// Non-equality operator
    bool operator != (const Axis2D& other) const {
      return ! operator == (other);
    }

    /// Addition operator
    Axis2D<BIN2D, DBN>& operator += (const Axis2D<BIN2D, DBN>& toAdd) {
      if (*this != toAdd) {
        throw LogicError("YODA::Axis2D: Cannot add axes with different binnings.");
      }
      for (size_t i = 0; i < bins().size(); ++i) {
        bin(i) += toAdd.bin(i);
      }
      _dbn += toAdd._dbn;
      return *this;
    }


    /// Subtraction operator
    Axis2D<BIN2D, DBN>& operator -= (const Axis2D<BIN2D, DBN>& toSubtract) {
      if (*this != toSubtract) {
        throw LogicError("YODA::Axis2D: Cannot add axes with different binnings.");
      }
      for (size_t i = 0; i < bins().size(); ++i) {
        bin(i) -= toSubtract.bin(i);
      }
      _dbn -= toSubtract._dbn;
      return *this;
    }

  private:

    void _checkUnlocked(void) {
      // Ensure that axis is not locked
      if (_locked)
        throw LockError("Attempting to update a locked axis");
    }


    void _updateAxis(Bins& bins) {

      // Deal with the case that there are no bins supplied (who called that?!)
      if (bins.size() == 0) {
        _binSearcherX = Utils::BinSearcher();
        _binSearcherY = Utils::BinSearcher();
        _nx = 0;
        _ny = 0;
        _xRange = std::make_pair(0, 0);
        _yRange = std::make_pair(0, 0);
      }

      // Sort the bins
      std::sort(bins.begin(), bins.end());

      // Create the cuts
      std::vector<double> xcuts, ycuts, xwidths, ywidths;
      foreach (Bin &bin, bins) {
        xcuts.push_back(bin.xMin());
        xcuts.push_back(bin.xMax());
        xwidths.push_back(bin.xWidth());
        ycuts.push_back(bin.yMin());
        ycuts.push_back(bin.yMax());
        ywidths.push_back(bin.yWidth());
      }

      // Sort the cuts and widths
      std::sort(xcuts.begin(), xcuts.end());
      std::sort(ycuts.begin(), ycuts.end());
      std::sort(xwidths.begin(), xwidths.end());
      std::sort(ywidths.begin(), ywidths.end());

      // Obtain the median widths as a typical scale for uniqueness comparisons
      const double medianxwidth = xwidths[ (xwidths.size()-1)/2 ];
      const double medianywidth = ywidths[ (ywidths.size()-1)/2 ];

      // Uniqueify the bin edges in the x- and y-cut vectors, with some numerical fuzziness
      xcuts.resize(std::unique(xcuts.begin(), xcuts.end(), CmpFloats(1e-3, medianxwidth)) - xcuts.begin());
      ycuts.resize(std::unique(ycuts.begin(), ycuts.end(), CmpFloats(1e-3, medianywidth)) - ycuts.begin());

      size_t nx = xcuts.size();
      size_t ny = ycuts.size();
      size_t N = nx * ny;
      std::cout << "Unique Axis2D edge list sizes: nx = " << nx << ", ny = " << ny << ", ntot = " << N << std::endl;

      // Create a sea of indices, starting with an all-gaps configuration
      std::vector<ssize_t> indexes(N, -1);

      // Iterate through bins and find out which
      Utils::BinSearcher xSearcher(xcuts);
      Utils::BinSearcher ySearcher(ycuts);
      for (size_t i = 0; i < bins.size(); ++i) {
        Bin& bin = bins[i];

        std::cout << "Bin #" << i << " edges: "
                  << "[(" << bin.xMin() << "," << bin.xMax() << "), "
                  << "(" << bin.yMin() << "," << bin.yMax() << ")] ";

        const size_t xiMin= xSearcher.index(bin.xMin()) - 1;
        const size_t xiMax= xSearcher.index(bin.xMax()) - 1;
        const size_t yiMin = ySearcher.index(bin.yMin()) - 1;
        const size_t yiMax = ySearcher.index(bin.yMax()) - 1;

        // Loop over sub-bins in the edge list and assign indices / detect overlaps
        for (size_t xi = xiMin; xi < xiMax; xi++) {
          for (size_t yi = yiMin; yi < yiMax; yi++) {
            const size_t ii = _index(nx, xi, yi);
            if (indexes[ii] != -1) {
              std::stringstream ss;
              ss << "Bin edges overlap! Bin #" << i << " with edges "
                 << "[(" << bin.xMin() << "," << bin.xMax() << "), "
                 << "(" << bin.yMin() << "," << bin.yMax() << ")] "
                 << "overlaps bin #" << indexes[ii] << " in sub-bin #" << ii;
              throw RangeError(ss.str());
            }
            indexes[ii] = i;
          }
        }
      }

      // Job's a good'n - let's change our class.
      _nx = nx;
      _ny = ny;

      _xRange = std::make_pair(xcuts.front(), xcuts.back());
      _yRange = std::make_pair(ycuts.front(), ycuts.back());

      _indexes = indexes;
      _bins = bins;

      _binSearcherX = xSearcher;
      _binSearcherY = ySearcher;
    }


  private:

    /// @todo WTF?
    static size_t _index(size_t nx, size_t x, size_t y) {
      return y * nx + x;
    }

    /// @name Data structures
    //@{

    /// Bins vector
    Bins _bins;

    /// Total distribution
    DBN _dbn;

    // Outflows
    std::vector<DBN> _outflows;

    // Binsearcher, for searching bins
    Utils::BinSearcher _binSearcherX;
    Utils::BinSearcher _binSearcherY;

    std::pair<double, double> _xRange;
    std::pair<double, double> _yRange;

    // Mapping from binsearcher indices to bin indices (allowing gaps)
    std::vector<ssize_t> _indexes;

    // Necessary for bounds checking and indexing
    size_t _nx;
    size_t _ny;

    /// Whether modifying bin edges is permitted
    bool _locked;

    //@}

  };

}

#endif
