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
  /// arranged in a 1D line.
  template <typename BIN1D, typename DBN>
  class Axis1D {
  public:

    /// Convenience typedefs
    typedef BIN1D Bin;

    /// A vector containing 1D bins. It is not used for searching,
    /// only for bins storage.
    typedef typename std::vector<BIN1D> Bins;

    /// A standard edge container. First number defines location
    /// of the edge, the second one a bin nuber the edge is a member of.
    typedef typename std::pair<double, size_t> Edge;


    /// @name Constructors
    //@{


    /// Empty constructor
    Axis1D() { }


    /// Constructor accepting a list of bin edges
    Axis1D(const std::vector<double>& binedges) {
      _mkAxis(binedges);
    }


    /// Constructor with histogram limits, number of bins and a bin distribution enum
    Axis1D(size_t nbins, double lower, double upper) {
      _mkAxis(linspace(lower, upper, nbins));
    }


    /// @brief Constructor accepting a vector of bins.
    ///
    /// Note that not only dimensions of these bins will be set,
    /// all the contents of the bins will be copied across, including
    /// the statistics
    Axis1D(const std::vector<BIN1D>& bins) {
      _mkAxis(bins);
    }


    /// State-setting constructor
    Axis1D(const Bins& bins, const DBN& dbn_tot, const DBN& dbn_uflow, const DBN& dbn_oflow)
      : _dbn(dbn_tot), _underflow(dbn_uflow), _overflow(dbn_oflow)
    {
      _mkAxis(bins);
    }

    //@}


    /// @name Statistics accessor functions
    //@{

    /// Get the number of bins on the axis
    size_t numBins() const {
      return _bins.size();
    }

    /// Return a vector of bins (non-const)
    Bins& bins() {
      return _bins;
    }

    /// Return a vector of bins (const)
    const Bins& bins() const {
      return _bins;
    }

    /// Return inf of the Axis
    double lowEdge() const {
      return _binHashSparse.front().first;
    }
    /// A alias for lowEdge()
    double xMin() const { return lowEdge();}

    /// Return sup of the Axis
    double highEdge() const {
      return _binHashSparse.back().first;
    }
    /// Alias for highEdge()
    double xMax() const { return highEdge();}

    /// Return a bin at a given index (non-const)
    BIN1D& bin (size_t index) {
      if(index >= numBins()) throw RangeError("YODA::Histo1D: index out of range!");
      return _bins[index];
    }

    /// Return a bin at a given index (const)
    const BIN1D& bin (size_t index) const {
      if(index >= numBins()) throw RangeError("YODA::Histo1D: index out of range!");
      return _bins[index];
    }

    /// Return a bin at a given coordinate (non-const)
    BIN1D& binByCoord(double x) {
      int index = getBinIndex(x);
      if(index == -1) throw RangeError("There is no bin at the specified x");
      return bin(index);
    }

    /// Return a bin at a given coordinate (const)
    const BIN1D& binByCoord(double x) const {
      int index = getBinIndex(x);
      if(index == -1) throw RangeError("There is no bin at the specified x");
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

    /// Returns an index of a bin at a given coord, -1 if no bin.
    int getBinIndex(double coord) const {
      // This is NOT a magic number, it is merely a 'trick' to yield a
      // correct answer in the case of coord pointing exactly on an edge.
      // This will never change the answer, since it is *much* smaller than
      // the fuzzyEquals() tolerance!
      coord += 0.00000000001;

      // Search for an edge
      size_t index = _binaryS(coord, 0, _binHashSparse.size());

      // If lower bound is a last edge, it means that we are 'above' the
      // axis and no bin canbe found in such case. Therefore, announce it.
      if (index == _binHashSparse.size() - 1) return -1;

      // If on the left to the point in consideration there is an edge that is
      // a member of the same bin as the one on the right, it means that our point
      // is inside a bin. In such case, announce it providing the index of the
      // bin in question.
      if (_binHashSparse[index].second == _binHashSparse[index+1].second) {
        return _binHashSparse[index].second;
      }

      // If we are inside an axis, but not inside a bin, it means that we must
      // be trying to fill an empty space.
      return -1;
    }


    /// Reset all the statistics on the Axis
    void reset() {
      _dbn.reset();
      _underflow.reset();
      _overflow.reset();
      for (size_t i = 0; i < _bins.size(); ++i) _bins[i].reset();
    }


    /// Merge together the bin range with indices from @a from to @a to, inclusive
    void mergeBins(size_t from, size_t to) {
      // Correctness checking
      if (from < 0 || from >= numBins()) throw ("First index is out of range!");
      if (to < 0 || to >= numBins()) throw ("Second index is out of range!");
      if (_bins[from].xMin() > _bins[to].xMin()) throw RangeError("The starting bin is greater than ending bin!");
      BIN1D& b = _bins[from];
      // std::cout << "a " << b.focus() << std::endl;
      for (size_t i = from+1; i <= to; ++i) {
        b.merge(_bins[i]);
      }
      // std::cout << "b " << b.focus() << std::endl;
      eraseBins(from+1, to);
    }


    /// Merge every group of n bins, starting from the LHS
    void rebin(int n) {
      size_t m = 0;
      while (m < _bins.size()) {
        const size_t end = (m + n - 1 < _bins.size()) ? m + n -1 : _bins.size() - 1;
        //std::cout << m << ", " << end << ", " << _bins.size() << std::endl;
        if (end > m) mergeBins(m, end);
        m += 1;
      }
    }


    /// Add a bin, providing its low and high edge
    void addBin(double from, double to) {
      std::vector<double> binedges;
      binedges.push_back(from); binedges.push_back(to);
      _mkAxis(binedges);
    }


    /// Add a set of bins to an axis, providing their boundaries.
    ///
    /// Notice that no empty space will be created.
    void addBin(const std::vector<double>& binedges) {
      _mkAxis(binedges);
    }


    /// Add a set of bins specifying start/end of each
    void addBin(const std::vector<std::pair<double,double> > edges) {
      for (size_t i = 0; i < edges.size(); ++i) {
        std::vector<double> temp;
        temp.push_back(edges[i].first);
        temp.push_back(edges[i].second);
        _mkAxis(temp);
      }
    }


    /// Remove a bin
    void eraseBin(size_t index) {
      if (index >= _bins.size()) throw RangeError("Index out of range!");
      _bins.erase(_bins.begin() + index);
      _mkBinHash();
    }


    /// Remove a bin range
    void eraseBins(size_t from, size_t to) {
      if (from < 0 || from >= numBins()) throw ("First index is out of range!");
      if (to < 0 || to >= numBins()) throw ("Second index is out of range!");
      if (_bins[from].xMin() > _bins[to].xMin()) throw RangeError("The starting bin is greater than ending bin!");
      _bins.erase(_bins.begin() + from, _bins.begin() + to + 1);
      _mkBinHash();
    }


    /// Scale the size of an axis by a factor
    void scaleX(double scalefactor) {
      _dbn.scaleX(scalefactor);
      _underflow.scaleX(scalefactor);
      _overflow.scaleW(scalefactor);
      for (size_t i = 0; i < _bins.size(); ++i) _bins[i].scaleX(scalefactor);
      _mkBinHash();
    }


    /// Scale the amount of fills by a factor
    void scaleW(double scalefactor) {
      _dbn.scaleW(scalefactor);
      _underflow.scaleW(scalefactor);
      _overflow.scaleW(scalefactor);
      for(size_t i = 0; i < _bins.size(); ++i) _bins[i].scaleW(scalefactor);
    }

    //@}


    /// @name Operators
    //@{

    /// Check if two of the Axis have the same binning
    bool operator == (const Axis1D& other) const {
      return _binHashSparse == other._binHashSparse;
    }


    /// Check if the binning of two of the Axis is different
    bool operator != (const Axis1D& other) const {
      return ! operator == (other);
    }


    /// Add two Axis together
    Axis1D<BIN1D,DBN>& operator += (const Axis1D<BIN1D,DBN>& toAdd) {
      if (*this != toAdd) throw LogicError("YODA::Histo1D: Cannot add axes with differen binnings.");

      for (size_t i = 0; i < _bins.size(); ++i) {
        _bins[i] += toAdd.bins().at(i);
      }

      _dbn += toAdd._dbn;
      _underflow += toAdd._underflow;
      _overflow += toAdd._overflow;
      return *this;
    }


    /// Subtract two Axis
    Axis1D<BIN1D,DBN>& operator -= (const Axis1D<BIN1D,DBN>& toSubtract) {
      if (*this != toSubtract) throw LogicError("YODA::Histo1D: Cannot add axes with differen binnings.");

      for (size_t i = 0; i < _bins.size(); ++i) {
        _bins[i] -= toSubtract.bins().at(i);
      }

      _dbn += toSubtract._dbn;
      _underflow += toSubtract._underflow;
      _overflow += toSubtract._overflow;
      return *this;
    }

    //@}


  private:

    /// Makes an axis out of a vector of edges
    void _mkAxis(const std::vector<double>& binedges) {
      for (size_t i = 0; i < binedges.size() - 1; ++i) {
        // If the bin that is to be added doesn't conflict with an
        // existing axis
        if (!_findCuts(binedges[i], binedges[i+1])) {
          // Add this bin to the bin vector
          _bins.push_back(BIN1D(binedges[i], binedges[i+1]));
        }
      }

      // Regenerate the Bin hash
      _mkBinHash();
    }


    /// Makes an axis from a vector of bins
    void _mkAxis(const Bins& bins) {
      _bins = bins;
      _mkBinHash();
    }


    /// Regenerate the bin hash
    void _mkBinHash() {
      // Remove the old cache, if it exists
      _binHashSparse.clear();

      // For each of the bins, add the low and high edge to the cache
      for (size_t i = 0; i < _bins.size(); ++i) {
        _binHashSparse.push_back(std::make_pair(_bins[i].xMin(), i));
        _binHashSparse.push_back(std::make_pair(_bins[i].xMax(), i));
      }

      // Sort the cache to be nondecreasing to enable
      // fast searches
      std::sort(_binHashSparse.begin(), _binHashSparse.end());
    }


    // Binary search algorithm. For an in-depth explanation
    // please see the documentation of an analogous one in Axis2D
    size_t _binaryS(double value, size_t lower, size_t higher) const {
      if (lower == higher) return lower;
      size_t where = (higher+lower)/2;

      if (value >= _binHashSparse[where].first) {
        if (where == _binHashSparse.size() - 1) return where;
        if(value <= _binHashSparse[where+1].first) return where;
        return _binaryS(value, where, higher);
      }

      if (where == 0) return where;
      return _binaryS(value, lower, where);
    }


    /// Check if a hypothetical bin to be added, starting at from and
    /// ending at to will partially overlap any of the existing bins.
    bool _findCuts(const double& from, const double& to) const {
      size_t index1 = _binaryS(from, 0, _binHashSparse.size());
      size_t index2 = _binaryS(to, 0, _binHashSparse.size());
      return !(index1 == index2);
    }


    /// Check if there are any gaps in the Axis
    bool _isGapless(size_t from, size_t to) {
      size_t start =  _binaryS(_bins[from].xMin(), 0, _binHashSparse.size());
      size_t end =    _binaryS(_bins[to].xMin(), 0, _binHashSparse.size());
      for(size_t i = start; i < end; i++) {
        if(!fuzzyEquals(_binHashSparse[i].first, _binHashSparse[i+1].first) &&
           _binHashSparse[i].second != _binHashSparse[i+1].second) return false;
      }
      return true;
    }


  private:

    /// @name Data structures
    //@{

    /// Bins vector
    Bins _bins;

    /// Total distribution
    DBN _dbn;

    /// Under- and over- flows
    DBN _underflow;
    DBN _overflow;

    /// Cached bin edges
    std::vector<Edge> _binHashSparse;

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
