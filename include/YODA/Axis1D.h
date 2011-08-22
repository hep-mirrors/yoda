#ifndef YODA_Axis1D_h
#define YODA_Axis1D_h

#include "YODA/AnalysisObject.h"
#include "YODA/Exceptions.h"
#include "YODA/Bin.h"
#include "YODA/Utils/MathUtils.h"

#include <string>
#include <iostream>
using namespace std;

namespace YODA {
  template <typename BIN1D, typename DBN>
  class Axis1D {
    public:
      typedef BIN1D Bin;
      typedef typename std::vector<BIN1D> Bins;
      typedef typename std::pair<double, size_t> Edge;

    /// @name Constructors
    //@{

    /// Empty constructor
    Axis1D() { }

    /// Constructor with a list of bin edges
    Axis1D(const std::vector<double>& binedges) {
      _mkAxis(binedges);
    }

    /// Constructor with histogram limits, number of bins and a bin distribution enum
    Axis1D(size_t nbins, double lower, double upper) {
      _mkAxis(linspace(lower, upper, nbins));
    }

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

    /// Return the array of bins (non-const)
    Bins& bins() {
      return _bins;
    }

    const Bins& bins() const {
      return _bins;
    }

    double lowEdge() const {
      return _binHashSparse.front().first;
    }
    double xMin() const { return lowEdge();}

    double highEdge() const {
      return _binHashSparse.back().first;
    }
    double xMax() const { return highEdge();}

    BIN1D& bin (size_t index) {
      if(index >= numBins()) throw RangeError("YODA::Histo1D: index out of range!");
      return _bins[index];
    }
    
    const BIN1D& bin (size_t index) const {
      if(index >= numBins()) throw RangeError("YODA::Histo1D: index out of range!");
      return _bins[index];
    }
    
    BIN1D& binByCoord(double x) {
      int index = getBinIndex(x);
      if(index == -1) throw RangeError("There is no bin at the specified x");
      return bin(index);
    }

    const BIN1D& binByCoord(double x) const {
      int index = getBinIndex(x);
      if(index == -1) throw RangeError("There is no bin at the specified x");
      return bin(index);
    }

    DBN& totalDbn() {
      return _dbn;
    }

    const DBN& totalDbn() const {
      return _dbn;
    }

    DBN& underflow() {
      return _underflow;
    }

    const DBN& underflow() const {
      return _underflow;
    }

    DBN& overflow() {
      return _overflow;
    }

    const DBN& overflow() const {
      return _overflow;
    }
    //@}

    /// @name Modifiers and helpers
    //@{
    int getBinIndex(double coord) const {
      coord += 0.00000000001; 
      
      size_t index = _binaryS(coord, 0, _binHashSparse.size());

      if(index == _binHashSparse.size() - 1) return -1;

      if(_binHashSparse[index].second == _binHashSparse[index+1].second)
      {
        return _binHashSparse[index].second;
      }
      return -1;
    }

    void reset() {
      _dbn.reset();
      _underflow.reset();
      _overflow.reset();
      for(size_t i = 0; i < _bins.size(); ++i) _bins[i].reset();
    }

    void rebin(int factor) {
      throw std::runtime_error("Implement!");
    }
    
    void mergeBins(size_t from, size_t to) {
      /// Correctness checking
      if(from < 0 || from >= numBins()) throw ("First index is out of range!");
      if(to < 0 || to >= numBins()) throw ("Second index is out of range!");
      if(_bins[from].xMin() > _bins[to].xMin()) throw RangeError("The starting bin is greater than ending bin!");
      throw std::runtime_error("Implement!");  
      //@todo Implement!
    }

    void addBin(double& from, double& to) {
      vector<double> binedges;
      binedges.push_back(from); binedges.push_back(to);

      _mkAxis(binedges);
    }

    void addBin(vector<double>& binedges) {
      _mkAxis(binedges);
    }
    
    void scaleX(double scalefactor) {
      _dbn.scaleX(scalefactor);
      _underflow.scaleX(scalefactor);
      _overflow.scaleW(scalefactor);
      for(size_t i = 0; i < _bins.size(); ++i) _bins[i].scaleX(scalefactor);

      _mkBinHash();
    }
    
    void scaleW(double scalefactor) {
      _dbn.scaleW(scalefactor);
      _underflow.scaleW(scalefactor);
      _overflow.scaleW(scalefactor);
      for(size_t i = 0; i < _bins.size(); ++i) _bins[i].scaleW(scalefactor);
    }
    //@}

    /// @name Operators
    //@{

    bool operator == (const Axis1D& other) const {
      return _binHashSparse == other._binHashSparse;
    }

    bool operator != (const Axis1D& other) const {
      return ! operator == (other);
    }

    Axis1D<BIN1D,DBN>& operator += (const Axis1D<BIN1D,DBN>& toAdd) {
      if(*this != toAdd) throw LogicError("YODA::Histo1D: Cannot add axes with differen binnings.");
      
      for(size_t i = 0; i < _bins.size(); ++i) {
        _bins[i] += toAdd.bins().at(i);
      }

      _dbn += toAdd._dbn;
      _underflow += toAdd._underflow;
      _overflow += toAdd._overflow;
      return *this;
    }

    Axis1D<BIN1D,DBN>& operator -= (const Axis1D<BIN1D,DBN>& toSubtract) {
      if(*this != toSubtract) throw LogicError("YODA::Histo1D: Cannot add axes with differen binnings.");
      
      for(size_t i = 0; i < _bins.size(); ++i) {
        _bins[i] -= toSubtract.bins().at(i);
      }

      _dbn += toSubtract._dbn;
      _underflow += toSubtract._underflow;
      _overflow += toSubtract._overflow;
      return *this;
    }
    
    //@}

  private:
    void _mkAxis(const std::vector<double>& binedges) {
      for(size_t i = 0; i < binedges.size() - 1; ++i) {
        if(!_findCuts(binedges[i], binedges[i+1])) {
          _bins.push_back(BIN1D(binedges[i], binedges[i+1]));
        }
      }

      _mkBinHash();
    }

    void _mkAxis(const Bins& bins) {
      _bins = bins;
      _mkBinHash();
    }

    void _mkBinHash() {
      _binHashSparse.clear();

      for(size_t i = 0; i < _bins.size(); ++i) {
        _binHashSparse.push_back(std::make_pair(_bins[i].xMin(), i));
        _binHashSparse.push_back(std::make_pair(_bins[i].xMax(), i));
      }

      std::sort(_binHashSparse.begin(), _binHashSparse.end());
    }

    size_t _binaryS(double value, size_t lower, size_t higher) const {
      if(lower == higher) return lower;
      size_t where = (higher+lower)/2;

      if(value >= _binHashSparse[where].first) {
        if(where == _binHashSparse.size() - 1) return where;
        if(value <= _binHashSparse[where+1].first) return where;
        return _binaryS(value, where, higher);
      }

      if (where == 0) return where;
      return _binaryS(value, lower, where);
    }

    bool _findCuts(const double& from, const double& to) const {
      size_t index1 = _binaryS(from, 0, _binHashSparse.size());
      size_t index2 = _binaryS(to, 0, _binHashSparse.size());
      return !(index1 == index2);
    }

  private:
    /// @name Data structures
    //@{
    
    /// Bins vector
    Bins _bins;

    /// Total distribution
    DBN _dbn;

    /// Under- and over- flows
    DBN _underflow; DBN _overflow;

    /// Cached bin edges
    std::vector<Edge> _binHashSparse;

    //@}
  };

  template <typename BIN1D, typename DBN>
  Axis1D<BIN1D,DBN> operator + (const Axis1D<BIN1D,DBN>& first, const Axis1D<BIN1D,DBN>& second) {
    Axis1D<BIN1D,DBN> tmp = first;
    tmp += second;
    return tmp;
  }
  
  template <typename BIN1D, typename DBN>
  Axis1D<BIN1D,DBN> operator - (const Axis1D<BIN1D,DBN>& first, const Axis1D<BIN1D,DBN>& second) {
    Axis1D<BIN1D,DBN> tmp = first;
    tmp -= second;
    return tmp;
  }

}

#endif

