#ifndef YODA_Axis1D_h
#define YODA_Axis1D_h

#include "YODA/AnalysisObject.h"
#include "YODA/Exceptions.h"
#include "YODA/Bin.h"
#include "YODA/Utils/MathUtils.h"

#include <string>

namespace YODA {


  /// @brief 1D bin container
  ///
  /// This class handles most of the low-level operations on an axis of bins
  /// arranged in a 1D line (including gaps).
  template <typename BIN1D, typename DBN>
  class Axis1D {
  public:

    /// Typedefs
    //@{

    /// Bin type
    typedef BIN1D Bin;

    /// A vector containing 1D bins. Not used for searching.
    typedef typename std::vector<Bin> Bins;

    /// @brief Type used to implement a search table of low bin edges mapped to bin indices.
    /// An index of -1 indicates a gap interval, without a corresponding bin.
    typedef std::map<double, long int> BinHash;

    //@}


    /// @name Constructors
    //@{

    /// Empty constructor
    Axis1D()
      : _locked(false)
    { }


    /// Constructor accepting a list of bin edges
    Axis1D(const std::vector<double>& binedges)
      : _locked(false)
    {
      _addBins(binedges);
    }


    /// Constructor with the number of bins and the axis limits
    /// @todo Rewrite interface to use a pair for the low/high
    Axis1D(size_t nbins, double lower, double upper)
      : _locked(false)
    {
      _addBins(linspace(lower, upper, nbins));
    }


    /// @brief Constructor accepting a vector of bins.
    ///
    /// Note that not only dimensions of these bins will be set,
    /// all the contents of the bins will be copied across, including
    /// the statistics
    Axis1D(const std::vector<BIN1D>& bins)
      : _locked(false)
    {
      _addBins(bins);
    }


    /// State-setting constructor for persistency
    Axis1D(const Bins& bins, const DBN& dbn_tot, const DBN& dbn_uflow, const DBN& dbn_oflow)
      : _dbn(dbn_tot), _underflow(dbn_uflow), _overflow(dbn_oflow), _locked(false)
    {
      _addBins(bins);
    }

    //@}


    /// @name Statistics accessor functions
    //@{

    /// Get the number of bins on the axis
    size_t numBins() const {
      return bins().size();
    }

    /// Return a vector of bins (non-const)
    Bins& bins() {
      return _bins;
    }

    /// Return a vector of bins (const)
    const Bins& bins() const {
      return _bins;
    }

    /// Return the lowest-value bin edge on the axis
    double lowEdge() const {
      if (numBins() == 0) throw RangeError("This axis contains no bins and so has no defined range");
      return bins().front().xMin();
    }
    /// A alias for lowEdge()
    double xMin() const { return lowEdge();}

    /// Return the highest-value bin edge on the axis
    double highEdge() const {
      if (numBins() == 0) throw RangeError("This axis contains no bins and so has no defined range");
      return bins().back().xMax();
    }
    /// Alias for highEdge()
    double xMax() const { return highEdge();}

    /// Return a bin at a given index (non-const)
    BIN1D& bin(size_t index) {
      if (index >= numBins()) throw RangeError("YODA::Histo1D: index out of range!");
      return _bins[index];
    }

    /// Return a bin at a given index (const)
    const BIN1D& bin(size_t index) const {
      if (index >= numBins()) throw RangeError("YODA::Histo1D: index out of range!");
      return _bins[index];
    }

    /// Return a bin at a given coordinate (non-const)
    BIN1D& binByCoord(double x) {
      const long int index = getBinIndex(x);
      if (index == -1) throw RangeError("There is no bin at the specified x");
      return bin(index);
    }

    /// Return a bin at a given coordinate (const)
    const BIN1D& binByCoord(double x) const {
      const long int index = getBinIndex(x);
      if (index == -1) throw RangeError("There is no bin at the specified x");
      return bin(index);
    }

    /// Return the total distribution (non-const)
    DBN& totalDbn() {
      return _dbn;
    }

    /// Return the total distribution (const)
    const DBN& totalDbn() const {
      return _dbn;
    }

    /// Return underflow (non-const)
    DBN& underflow() {
      return _underflow;
    }

    /// Return underflow (const)
    const DBN& underflow() const {
      return _underflow;
    }

    /// Return overflow (non-const)
    DBN& overflow() {
      return _overflow;
    }

    /// Return overflow (const)
    const DBN& overflow() const {
      return _overflow;
    }

    //@}


    /// @name Modifiers and helpers
    //@{

    /// Returns an index of a bin at a given coord, -1 if no bin matches
    long int getBinIndex(double coord) const {
      // First check that we are within the axis bounds at all
      if (_binhash.empty() || coord < lowEdge() || coord >= highEdge()) return -1;
      // Then return the lower-edge lookup from the hash map.
      // NB. both upper_bound and lower_bound return values *greater* than (or equal) to coord,
      // so we have to step back one iteration to get to the lower-or-equal containing bin edge.
      BinHash::const_iterator itabove = _binhash.upper_bound(coord);
      long int index = (--itabove)->second;
      return index;
    }


    /// Reset all the bin statistics on the axis
    void reset() {
      _dbn.reset();
      _underflow.reset();
      _overflow.reset();
      foreach(Bin& bin, _bins) bin.reset();
      _locked = false;
    }


    /// Set the axis lock state
    void _setLock(bool locked) {
      _locked = locked;
    }


    /// Merge together the bin range with indices from @a from to @a to, inclusive.
    /// Merge a series of bins, between the bins identified by indices @a from and @a to
    void mergeBins(size_t from, size_t to) {
      // Correctness checking
      if (from < 0 || from >= numBins()) throw RangeError("First index is out of range!");
      if (to < 0 || to >= numBins()) throw RangeError("Second index is out of range!");
      if (_bins[from].xMin() > _bins[to].xMin()) throw RangeError("The starting bin is greater than ending bin!");
      if (_gapInRange(from, to)) throw RangeError("Bin ranges containing binning gaps cannot be merged!");

      BIN1D& b = _bins[from];
      for (size_t i = from+1; i <= to; ++i) {
        b.merge(_bins[i]);
      }
      eraseBins(from+1, to);
    }


    /// Merge every group of @a n bins, starting from the LHS
    void rebin(int n) {
      size_t m = 0;
      while (m < numBins()) {
        const size_t end = (m + n - 1 < numBins()) ? m + n -1 : numBins() - 1;
        if (end > m) mergeBins(m, end);
        m += 1;
      }
    }


    /// Add a bin, providing its low and high edge
    void addBin(double low, double high) {
      std::vector<double> edges;
      edges.push_back(low);
      edges.push_back(high);
      _addBins(edges);
    }


    /// Add a contiguous set of bins to an axis, via their list of edges
    void addBins(const std::vector<double>& binedges) {
      _addBins(binedges);
    }


    /// Remove a bin
    void eraseBin(size_t index) {
      if (index >= _bins.size()) throw RangeError("Index out of range!");
      _bins.erase(_bins.begin() + index);
      _updateAxis();
    }


    /// Remove a bin range
    void eraseBins(size_t from, size_t to) {
      if (from < 0 || from >= numBins()) throw ("First index is out of range!");
      if (to < 0 || to >= numBins()) throw ("Second index is out of range!");
      if (_bins[from].xMin() > _bins[to].xMin()) throw RangeError("The starting bin is greater than ending bin!");
      _bins.erase(_bins.begin() + from, _bins.begin() + to + 1);
      _updateAxis();
    }


    /// Scale the size of an axis by a factor
    void scaleX(double scalefactor) {
      _dbn.scaleX(scalefactor);
      _underflow.scaleX(scalefactor);
      _overflow.scaleX(scalefactor);
      for (size_t i = 0; i < _bins.size(); ++i) _bins[i].scaleX(scalefactor);
      _updateAxis();
    }


    /// Scale the amount of fills by a factor
    void scaleW(double scalefactor) {
      _dbn.scaleW(scalefactor);
      _underflow.scaleW(scalefactor);
      _overflow.scaleW(scalefactor);
      for (size_t i = 0; i < _bins.size(); ++i) _bins[i].scaleW(scalefactor);
    }

    //@}


    /// @name Operators
    //@{

    /// Check if two of the Axis have the same binning
    bool operator == (const Axis1D& other) const {
      return _binhash == other._binhash;
    }


    /// Check if the binning of two of the Axis is different
    bool operator != (const Axis1D& other) const {
      return ! operator == (other);
    }


    /// Add two axes together
    Axis1D<BIN1D,DBN>& operator += (const Axis1D<BIN1D,DBN>& toAdd) {
      if (*this != toAdd) throw LogicError("YODA::Histo1D: Cannot add axes with different binnings.");

      for (size_t i = 0; i < _bins.size(); ++i) {
        _bins[i] += toAdd.bins().at(i);
      }

      _dbn += toAdd._dbn;
      _underflow += toAdd._underflow;
      _overflow += toAdd._overflow;
      return *this;
    }


    /// Subtract two axes
    Axis1D<BIN1D,DBN>& operator -= (const Axis1D<BIN1D,DBN>& toSubtract) {
      if (*this != toSubtract) throw LogicError("YODA::Histo1D: Cannot add axes with different binnings.");

      for (size_t i = 0; i < _bins.size(); ++i) {
        _bins[i] -= toSubtract.bins().at(i);
      }

      _dbn -= toSubtract._dbn;
      _underflow -= toSubtract._underflow;
      _overflow -= toSubtract._overflow;
      return *this;
    }

    //@}


  private:


    /// Add new bins, constructed from a sorted vector of edges, to the axis
    void _addBins(const std::vector<double>& binedges) {
      //std::sort(binedges.begin(), binedges.end());
      if (_locked) {
        throw LockError("Attempting to add bins to a locked axis");
      }
      if (_edgeInRange(binedges.front(), binedges.back())) {
        throw RangeError("New bin range overlaps with existing bin edges");
      }
      for (size_t i = 0; i < binedges.size() - 1; ++i) {
        _bins.push_back(BIN1D(binedges[i], binedges[i+1]));
      }
      _updateAxis();
    }


    /// Add new bins to the axis
    void _addBins(const Bins& bins) {
      if (_locked) {
        throw LockError("Attempting to add bins to a locked axis");
      }
      for (size_t i = 0; i < bins.size(); ++i) {
        if (_edgeInRange(bins[i].xMin(), bins[i].xMax())) {
          throw RangeError("New bin range overlaps with existing bin edges");
        }
        _bins.push_back(bins[i]);
      }
      _updateAxis();
    }


    /// Sort the bins vector, and regenerate the bin hash
    ///
    /// The bin hash is purely for searching, and is generated from the bins list only.
    void _updateAxis() {
      std::sort(_bins.begin(), _bins.end());
      _binhash.clear();
      for (size_t i = 0; i < numBins(); ++i) {
        // Add low edge hash for each bin
        _binhash[bin(i).xMin()] = i;
        // If the next bin is not contiguous, add a gap index for the high edge of this bin
        if (i+1 < numBins() && !fuzzyEquals(bin(i).xMax(), bin(i+1).xMin())) {
          _binhash[bin(i).xMax()] = -1;
        }
      }
    }


    /// Check if there are any bin edges between values @a from and @a to.
    bool _edgeInRange(double from, double to) const {
      if ( _binhash.empty() ) return false;
      return (--_binhash.upper_bound(from)) != (--_binhash.lower_bound(to));
    }


    /// Check if there are any gaps in the axis' binning between bin indices @a from and @a to, inclusive.
    bool _gapInRange(size_t ifrom, size_t ito) const {
      assert(ifrom < numBins() && ito < numBins() && ifrom <= ito);
      if (ifrom == ito) return false;
      BinHash::const_iterator start = (--_binhash.upper_bound(bin(ifrom).midpoint()));
      BinHash::const_iterator end = _binhash.upper_bound(bin(ifrom).midpoint());
      for (BinHash::const_iterator it = start; it != end; ++it) {
        if (it->second == -1) return true;
      }
      return false;
    }


  private:

    /// @name Data structures
    //@{

    /// Bins vector
    Bins _bins;

    /// Total distribution
    DBN _dbn;

    /// Under- and overflows
    DBN _underflow;
    DBN _overflow;

    /// Cached bin edges for searching
    BinHash _binhash;

    /// Whether modifying bin edges is permitted
    bool _locked;

    //@}

  };


  /// Add the statistics on two axes.
  template <typename BIN1D, typename DBN>
  inline Axis1D<BIN1D,DBN> operator + (const Axis1D<BIN1D,DBN>& first, const Axis1D<BIN1D,DBN>& second) {
    Axis1D<BIN1D,DBN> tmp = first;
    tmp += second;
    return tmp;
  }

  /// Subtract the statistics on two axis.
  template <typename BIN1D, typename DBN>
  inline Axis1D<BIN1D,DBN> operator - (const Axis1D<BIN1D,DBN>& first, const Axis1D<BIN1D,DBN>& second) {
    Axis1D<BIN1D,DBN> tmp = first;
    tmp -= second;
    return tmp;
  }


}

#endif
