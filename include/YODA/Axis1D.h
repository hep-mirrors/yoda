#ifndef YODA_Axis1D_h
#define YODA_Axis1D_h

#include "YODA/AnalysisObject.h"
#include "YODA/Exceptions.h"
#include "YODA/Bin.h"
#include "YODA/Utils/MathUtils.h"
#include "YODA/Utils/BinSearcher.h"
#include <limits>

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
      addBins(binedges);
    }


    /// @brief Constructor accepting a vector of bins.
    ///
    /// Note that not only dimensions of these bins will be set,
    /// all the contents of the bins will be copied across, including
    /// the statistics
    Axis1D(const std::vector<BIN1D>& bins)
      : _locked(false)
    {
      addBins(bins);
    }


    /// Constructor with the number of bins and the axis limits
    /// @todo Rewrite interface to use a pair for the low/high
    Axis1D(size_t nbins, double lower, double upper)
      : _locked(false)
    {
      addBins(linspace(lower, upper, nbins));
    }


    /// @brief Constructor accepting a vector of bins.
    ///
    /// Note that not only dimensions of these bins will be set,
    /// all the contents of the bins will be copied across, including
    /// the statistics
    Axis1D(const Bins& bins, const DBN& dbn_tot, const DBN& dbn_uflow, const DBN& dbn_oflow)
      : _dbn(dbn_tot), _underflow(dbn_uflow), _overflow(dbn_oflow), _locked(false)
    {
      addBins(bins);
    }

    /// @name Statistics accessor functions
    //@{

    /// Get the number of bins on the axis
    size_t numBins() const {
      return bins().size();
    }

    /// Return a vector of bins (const)
    const Bins& bins() const {
      return _bins;
    }

    /// Return a vector of bins (non-const)
    Bins& bins() {
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


    /// Return the total distribution (const)
    const DBN& totalDbn() const {
      return _dbn;
    }

    /// Return the total distribution (non-const)
    DBN& totalDbn() {
      return _dbn;
    }

    /// Return underflow (const)
    const DBN& underflow() const {
      return _underflow;
    }

    /// Return underflow (non-const)
    DBN& underflow() {
      return _underflow;
    }

    /// Return overflow (const)
    const DBN& overflow() const {
      return _overflow;
    }

    /// Return overflow (non-const)
    DBN& overflow() {
      return _overflow;
    }


    //@}


    /// @name Modifiers and helpers
    //@{

    /// Returns an index of a bin at a given coord, -1 if no bin matches
    long getBinIndex(double coord) const {
      // Yes, this is robust even with an empty axis.
      return _indexes[_binsearcher.index(coord)];
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
      if (from >= numBins())
        throw RangeError("Initial merge index is out of range");
      if (to >= numBins())
        throw RangeError("Final merge index is out of range");
      if (from > to)
        throw RangeError("Final bin must be greater than initial bin");
      if (_gapInRange(from, to))
        throw RangeError("Bin ranges containing gaps cannot be merged");

      Bin &b = bin(from);

      for (size_t i = from + 1; i <= to; i++)
        b.merge(_bins[i]);

      eraseBins(from+1, to);
    }

    /// Merge every group of @a n bins, starting from the LHS
    void rebin(size_t n) {
      for(size_t m=0; m < numBins(); m++) {
        const size_t end = (m + n - 1 < numBins()) ? m + n -1 : numBins() - 1;
        if (end > m) mergeBins(m, end);
      }
    }

    /// Add a bin, providing its low and high edge
    void addBin(double low, double high) {
      Bins newBins(_bins);
      newBins.push_back(Bin(low, high));
      _updateAxis(newBins);
    }


    /// Add a contiguous set of bins to an axis, via their list of edges
    void addBins(const std::vector<double> &binedges) {
      Bins newBins(_bins);
      if (binedges.size() == 0)
        return;

      double low = binedges.front();

      for (size_t i=1; i < binedges.size(); i++) {
        double high = binedges[i];
        newBins.push_back(Bin(low, high));
        low = high;
      }

      _updateAxis(newBins);
    }

    // (The following proposed method would drastically simplify the
    // Python interfaces handling of bin adding -- it would also
    // simplify some of the hurdles of bins not having default
    // constructors)

    /// Add a list of bins as pairs of lowEdge, highEdge
    void addBins(const std::vector<std::pair<double, double> > &binpairs) {
      // Make a copy of the current binning
      Bins newBins(_bins);

      // Iterate over given bins
      for (size_t i=0; i<binpairs.size(); i++) {
        std::pair<double, double> b = binpairs[i];
        newBins.push_back(Bin(b.first, b.second));
      }
      _updateAxis(newBins);
    }


    void addBins(const Bins &bins) {
      Bins newBins(_bins);
      foreach(const Bin &b, bins) {
        newBins.push_back(b);
      }
      _updateAxis(newBins);
    }

    /// Remove a bin
    void eraseBin(const size_t i) {
      // Might as well erase from the internal bins, as we can guarantee
      // consistency.
      if (i >= numBins())
        throw RangeError("Bin index is out of range");

      const bool wasLocked = _locked;
      _locked=false;
      _bins.erase(_bins.begin() + i);
      _updateAxis(_bins);
      _locked = wasLocked;
    }

    /// Remove a bin range
    void eraseBins(const size_t from, const size_t to) {
      if (from >= numBins())
        throw RangeError("Initial index out of range");
      if (to >= numBins())
        throw RangeError("Final index out of range");
      if (from > to)
        throw RangeError("Final index is less than initial index");

      const bool wasLocked = _locked;
      _locked=false;
      _bins.erase(_bins.begin() + from, _bins.begin() + to + 1);
      _updateAxis(_bins);
      _locked = wasLocked;
    }

    /// Scale the size of an axis by a factor
    // @todo What if somebody passes in negative scalefactor? (test idea)
    void scaleX(double scalefactor) {
      _dbn.scaleX(scalefactor);
      _underflow.scaleX(scalefactor);
      _overflow.scaleX(scalefactor);
      for (size_t i = 0; i < _bins.size(); ++i)
        _bins[i].scaleX(scalefactor);
      _updateAxis(_bins);
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
      for(size_t i=0; i < numBins(); i++)
        if (bin(i).lowEdge() != other.bin(i).lowEdge() ||
            bin(i).highEdge() != other.bin(i).highEdge())
          return false;

      return true;
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

    /// Sort the given bins vector, and regenerate the bin searcher
    //
    /// The bin searcher is purely for searching, and is generated from
    /// the bins list only.
    void _updateAxis(Bins &bins) {
      // Ensure that axis is not locked
      if (_locked) {
        throw LockError("Attempting to update a locked axis");
      }
      // Define the new cuts and indexes
      std::vector<double> edgeCuts;
      std::vector<long> indexes;

      // Sort the bins
      std::sort(bins.begin(), bins.end());

      // Keep a lag of the last high edge
      double last_high = -std::numeric_limits<double>::infinity();

      for (size_t i=0; i < bins.size(); i++) {
        Bin &currentBin = bins[i];

        double new_low  = currentBin.lowEdge();
        double reldiff = (new_low - last_high) / currentBin.width();
        double eps = 1.0e-10;
        if( reldiff < -eps ) {
          throw RangeError("Bin edges overlap");
        } else if ( reldiff > eps ) {
          indexes.push_back(-1);
          edgeCuts.push_back(new_low);
        }

        // Bins check that they are not zero or negative width. It's perfectly
        // okay for them to throw an exception here, as we haven't changed
        // anything yet.
        indexes.push_back(i);
        edgeCuts.push_back(currentBin.highEdge());

        last_high = currentBin.highEdge();
      }
      indexes.push_back(-1);

      // Everything was okay, so let's make our changes
      //std::cout << "In" << std::endl;
      _binsearcher = Utils::BinSearcher(edgeCuts);
      //std::cout << "Out" << std::endl;
      _indexes = indexes;
      _bins = bins;
    }


    /// Check if there are any gaps in the axis' binning between bin indices @a from and @a to, inclusive.
    bool _gapInRange(size_t from, size_t to) const {
      assert(from < numBins() && to < numBins() && from < to);
      if (from == to) return false;

      const size_t from_ix = _binsearcher.index(bin(from).lowEdge());
      const size_t to_ix = _binsearcher.index(bin(to).lowEdge());

      for (size_t i = from_ix; i <= to_ix; i++)
        if (_indexes[i] == -1)
          return true;

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

    // Binsearcher, for searching bins
    Utils::BinSearcher _binsearcher;
    // Mapping from binsearcher indices to bin indices (allowing gaps)
    std::vector<long> _indexes;

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
