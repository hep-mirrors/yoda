#ifndef YODA_Axis2D_h
#define YODA_Axis2D_h

#include "YODA/AnalysisObject.h"
#include "YODA/Exceptions.h"
#include "YODA/Bin.h"
#include "YODA/Utils/cachedvector.h"
#include "YODA/Utils/MathUtils.h"
#include "YODA/Dbn2D.h"

#include <algorithm>
#include <limits>

namespace YODA {


  /// @brief 2D bin container and provider
  ///
  /// This class handles almost all boiler-plate operations
  /// on 2D bins (like creating axis, adding, searching, testing).
  template <typename BIN2D, typename DBN>
  class Axis2D {

    /// @name Internal typedefs
    /// @todo Note that none of these depend on the class template args
    //@{

    /// When an edge is added to the collection it must obey the following
    /// format: the size_t specifies the bin this edge is a member of, and the
    /// pair of doubles contains the beginning and end of the edge.
    typedef typename std::pair<size_t, std::pair<double,double> > Edge;

    /// A type being a basic substructure of _binHashSparse. It contains a indicator
    /// specifying the major coordinate and a collection of edges sharing the same major
    /// coordinate.
    typedef typename std::pair<double, std::vector<Edge> > EdgeCollection;

    /// A simple point in 2D
    ///
    /// @todo Should Point2D be used?
    typedef typename std::pair<double, double> Point;

    /// Segment, having a beginning and end.
    typedef typename std::pair<Point, Point> Segment;

    //@}


  public:

    // A few helpful typedefs
    typedef BIN2D Bin;

    /// @brief The internal bin container type
    ///
    /// The following vector holds in itself an information about a Bin and a
    /// state in which it is. If the state is set to be false the bin will not
    /// be displayed and in effect will be treated as non-existent. The most
    /// common situation when such a functionality is needed is when merging a
    /// bin. For detailed description about what happens in such case, please
    /// refer to the mergeBins() function.
    ///
    /// @todo Hmm, it is very intrusive to have this on the public interface...
    /// I think we need to review this design. It really complicates the concept
    /// that is just meant to be a collection of bins. This also propagates
    /// right through to the user interface, via the pass-through typedef on Histo2D.
    typedef typename std::vector<Bin> Bins;


  public:

    /// @name Constructors:
    //@{

    /// @brief Empty constructor
    ///
    /// Only added because it is required by SWIG.
    /// It doesn't make much sense to use it.
    /// @todo Really "required"? Eliminate the requirement in the SWIG mapping.
    Axis2D() {
      std::vector<Segment> edges;
      _mkAxis(edges);
    }


    /// Constructor provided with a vector of bin delimiters
    Axis2D(const std::vector<Segment>& binLimits) {
      _mkAxis(binLimits);
      if (isGrid()) _setOutflows();
    }


    /// Most standard constructor, should be self-explanatory
    ///
    /// @todo Never say "should be self-explanatory": explain it, even if it seems obvious
    /// @todo This is too large/complex to be inline: move to the .cc
    Axis2D(size_t nbinsX, double lowerX, double upperX, size_t nbinsY, double lowerY, double upperY) {
      std::vector<Segment> binLimits;
      double coeffX = (upperX - lowerX)/(double)nbinsX;
      double coeffY = (upperY - lowerX)/(double)nbinsY;
      for (double i = lowerX; i < upperX; i += coeffX) {
        for (double j = lowerY; j < upperY; j += coeffY) {
          binLimits.push_back(std::make_pair(std::make_pair(i, j),
                                             std::make_pair((double)(i+coeffX), (double)(j+coeffY))));
        }
      }
      _mkAxis(binLimits);
      _setOutflows();
    }


    /// @brief A constructor accepting a list of bins and all the outflows
    ///
    /// Creates an Axis2D from existing bins
    /// @todo This is too large to be inline: move to the .cc
    Axis2D(const Bins& bins,
           const std::vector<std::vector<DBN> >& outflows,
           const DBN& totalDbn)
    {
      std::vector<Segment> binLimits;
      for (size_t i = 0; i < bins.size(); ++i) {
        binLimits.push_back(std::make_pair(std::make_pair(bins[i].xMin(), bins[i].yMin()),
                                           std::make_pair(bins[i].xMax(), bins[i].yMax())));
      }
      _mkAxis(binLimits);
      for (size_t i = 0; i < _bins.size(); ++i) {
        _bins[i] = bins[i];
      }
      if (isGrid()) _setOutflows();
      _outflows = outflows;

      _dbn = totalDbn;
    }

    /// A constructor with specified x and y axis bin limits.
    Axis2D(const std::vector<double>& xedges, const std::vector<double>& yedges) {
      std::vector<Segment> binLimits;
      for (int i = 0; i < yedges.size() - 1; ++i) {
        for (int j = 0; j < xedges.size() - 1; ++j) {
          binLimits.push_back(std::make_pair(std::make_pair(xedges[j], yedges[i]),
                                             std::make_pair(xedges[j+1], yedges[i+1])));
        }
      }
      _mkAxis(binLimits);
      _setOutflows();
    }


    /// A copy constructor
    Axis2D(const Axis2D& a) {
      _bins = a._bins;
      _dbn = a._dbn;
      _outflows = a._outflows;

      _binHashSparse.first.regenCache();
      _binHashSparse.second.regenCache();
    }

    //@}


    /// @name Bin insertion operators
    //@{

    /// @brief Bin addition operator
    ///
    /// This operator is provided a vector of limiting points in the format
    /// required by _mkAxis().  It should be noted that there is nothing special
    /// about the initiation stage of Axis2D, and the edges can be added online
    /// if they meet all the requirements of non-degeneracy.
    ///
    /// Be aware that adding a Bin to an existing axis created by one of
    /// constructors wipes out all the outflows since a concept of them is no
    /// longer meaningful!
    void addBin(const std::vector<Segment>& binLimits) {
      _mkAxis(binLimits);
      _outflows.resize(0);
    }

    /// @brief Bin addition operator
    ///
    /// This operator is supplied with the extremal coordinates of just a new
    /// bin, which is then constructed and added as usual.
    void addBin(double lowX, double lowY, double highX, double highY) {
      std::vector<Segment> coords;
      coords.push_back(std::make_pair(std::make_pair(lowX, lowY),
                                      std::make_pair(highX, highY)));
      addBin(coords);
    }

    //@}


    /// @name Modifiers
    //@{

    /// @brief Fill operator
    ///
    /// Called when it is wanted to fill a certain position on an Axis
    int fill(double x, double y, double weight) {
      // Filling the total distribution
      _dbn.fill(x, y, weight);

      // Filling a bin if the coordinates point to one.
      int index = getBinIndex(x, y);
      if (index != -1) _bins[index].fill(x, y, weight);

      // If coordinates point outside any of the bins and and the outflows were
      // properly set (i.e. we are dealing with a grid), fill a proper outflow.
      else if (_outflows.size() == 8) _fillOutflows(x, y, weight);

      // Return an information regarding what was filled.
      return index;
    }


    /// Merge a range of bins, given the bin IDs of points at the lower-left and upper-right.
    ///
    /// @todo This is far too big to be inline: move it to the bottom of this file, or perhaps to a .cc
    void mergeBins(size_t from, size_t to) {
      HistoBin2D& start = bin(from);
      HistoBin2D& end = bin(to);
      HistoBin2D temp = start;
      eraseBin(from);

      // Sanity-check input indices
      if (start.midpoint().first > end.midpoint().first) {
        throw RangeError("The start bin has a greater x value than the end bin.");
      }
      if (start.midpoint().second > end.midpoint().second) {
        throw RangeError("The start bin has a greater y value than the end bin.");
      }

      std::vector<size_t> toRemove;
      /// @todo Explain! This is *totally* incomprehensible.
      for (size_t y = (*_binHashSparse.first._cache.lower_bound(start.yMin())).second;
           y <= (*_binHashSparse.first._cache.lower_bound(end.yMin())).second; ++y) {
        for (size_t x = 0; x < _binHashSparse.first[y].second.size(); ++x) {
          if ((_binHashSparse.first[y].second[x].second.first > start.xMin() ||
               fuzzyEquals(_binHashSparse.first[y].second[x].second.first, start.xMin())) &&
              (_binHashSparse.first[y].second[x].second.second < end.xMax() ||
               fuzzyEquals(_binHashSparse.first[y].second[x].second.second, end.xMax())))
            {
              temp += bin(_binHashSparse.first[y].second[x].first);
              toRemove.push_back(_binHashSparse.first[y].second[x].first);
            }
        }
      }
      /// Now, drop the bins to be dropped
      for(size_t i = 0; i < toRemove.size(); ++i) eraseBin(toRemove[i]);

      _addEdge(temp.edges(), _binHashSparse, false);
      _bins.push_back(temp);

      /// @todo Where do the old bins get dropped?

      _binHashSparse.first.regenCache();
      _binHashSparse.second.regenCache();
    }


    /// @todo Add: Merge a given range of bins, given the bin x,y coordinates.


    /// Reset the axis statistics
    void reset() {
      _dbn.reset();
      /// @todo Sort out the Bins definition, then use foreach
      for (size_t i = 0; i < _bins.size(); ++i) {
        _bins[i].reset();
      }
    }

    //@}


    /// @name Accessors
    //@{

    /// Get the total number of bins
    /// @todo Can't this just be numBins?
    const size_t numBinsTotal() const {
      return _bins.size();
    }

    /// Get the number of bins along X axis
    const size_t numBinsX() const {
      return _binHashSparse.second.size()/2;
    }

    /// Get the number of bins along Y axis
    const size_t numBinsY() const {
      return _binHashSparse.first.size()/2;
    }


    /// Get the value of the lowest x-edge on the axis
    const double lowEdgeX() const {
      return _binHashSparse.second.front().first;
    }

    /// Get the value of the highest x-edge on the axis
    const double highEdgeX() const {
      return _binHashSparse.second.back().first;
    }

    /// Get the value of the lowest y-edge on the axis
    const double lowEdgeY() const {
      return _binHashSparse.first.front().first;
    }

    /// Get the value of the highest y-edge on the axis
    const double highEdgeY() const {
      return _binHashSparse.first.back().first;
    }


    /// Get the bins (non-const version)
    Bins& bins() {
      return _bins;
    }

    /// Get the bins (const version)
    const Bins& bins() const {
      return _bins;
    }


    /// Get the outflows (non-const version)
    /// @todo Explain the index structure
    std::vector<std::vector<DBN> >& outflows() {
      return _outflows;
    }

    /// Get the outflows (const version)
    /// @todo Explain the index structure
    const std::vector<std::vector<DBN> >& outflows() const {
      return _outflows;
    }


    /// Get the bin with a given index (non-const version)
    BIN2D& bin(size_t index) {
      if (index >= _bins.size()) throw RangeError("Bin index out of range.");
      return _bins[index];
    }

    /// Get the bin with a given index (const version)
    const BIN2D& bin(size_t index) const {
      if (index >= _bins.size()) throw RangeError("Bin index out of range.");
      return _bins[index];
    }


    /// Get a bin at given coordinates (non-const version)
    BIN2D& binByCoord(double x, double y) {
      const int ret = getBinIndex(x, y);
      if (ret != -1) return bin(ret);
      else throw RangeError("No bin found!!");
    }

    /// Get a bin at given coordinates (const version)
    const BIN2D& binByCoord(double x, double y) const {
      const int ret = getBinIndex(x, y);
      if (ret != -1) return bin(ret);
      else throw RangeError("No bin found!!");
    }


    /// Get a bin at given coordinates (non-const version)
    BIN2D& binByCoord(std::pair<double, double>& coords) {
      return binByCoord(coords.first, coords.second);
    }

    /// Get a bin at given coordinates (const version)
    BIN2D& binByCoord(std::pair<double, double>& coords) const {
      return binByCoord(coords.first, coords.second);
    }


    /// Get the total distribution (non-const version)
    DBN& totalDbn() {
      return _dbn;
    }

    /// Get the total distribution (const version)
    const DBN& totalDbn() const {
      return _dbn;
    }


    /// @brief Bin index finder
    ///
    /// Looks through all the bins to see which one contains the point of
    /// interest.
    const int getBinIndex(double coordX, double coordY) const {
      // In case we are just operating on a regular grid
      if (isGrid()) {
        /// @todo WTF?! This is bad. No magic numbers! Why would these be unreasonably small offsets?
        /// These are for the case when user requests a coordinate which is exactly (by the meaning of equals)
        /// on the edge. Without those the algorithm will crash.
        /// I am pretty sure they are fine, as they are well below fuzzyEquals() tolerance.
        coordX += 0.00000000001;
        coordY += 0.00000000001;
        size_t indexY = (*_binHashSparse.first._cache.lower_bound(approx(coordY))).second;

        if (indexY < _binHashSparse.first.size()) {
          for (unsigned int i = 0;  i < _binHashSparse.first[indexY].second.size(); i++){
            if (_binHashSparse.first[indexY].second[i].second.first < coordX &&
                _binHashSparse.first[indexY].second[i].second.second > coordX){
              size_t indexX = (*_binHashSparse.second._cache.lower_bound(approx(coordX))).second;
              if (indexX < _binHashSparse.second.size()){
                for (unsigned int j=0; j < _binHashSparse.second[indexX].second.size(); j++) {
                  if (_binHashSparse.second[indexX].second[j].second.first < coordY &&
                      (_binHashSparse.second[indexX].second[j].second.second > coordY) &&
                      (_binHashSparse.second[indexX].second[j].first ==
                       _binHashSparse.first[indexY].second[i].first))
                    return _binHashSparse.second[indexX].second[j].first;
                }
              }
            }
          }
        }
        return -1;
      }

      // In case we have something more complicated
      for (size_t i = 0; i < _bins.size(); ++i) {
        if (_bins[i].xMin() <= coordX && _bins[i].xMax() >= coordX &&
            _bins[i].yMin() <= coordY && _bins[i].yMax() >= coordY)
             return i;
      }
      return -1;
    }


    /// Fast column number searcher
    const size_t getBinColumn(size_t index) const {
      // Check if assumptions are reasonable
      if (!_isGrid) throw GridError("This operation can only be performed when an array is a grid!");
      if (index >= _bins.size()) throw RangeError("Index is bigger than the size of bins vector!");

      // Find the column number
      size_t ret  = (*_binHashSparse.first._cache.lower_bound(approx(bin(index).yMin()))).second;
      return ret;
    }


    /// Fast row number searcher
    const size_t getBinRow(size_t index) const {
      // Check if assumptions are reasonable
      if (!_isGrid) throw GridError("This operation can only be performed when an array is a grid!");
      if (index >= _bins.size()) throw RangeError("Index is bigger than the size of bins vector!");

      // Find the row number
      size_t ret  = (*_binHashSparse.second._cache.lower_bound(approx(bin(index).xMin()))).second;
      return ret;
    }


    /// Check if the axis has a grid structure or not
    bool isGrid() const {
      return _isGrid;
    }


    /// @brief Bin eraser
    ///
    /// Removes a bin at a position. Additionally, modifies bin cache to
    /// make it represent the new bin set.
    void eraseBin(size_t index) {
      // Check the correctness of assumptions
      if (index >= _bins.size()) throw RangeError("Index is bigger than the size of bins vector!");

      _bins.erase(_bins.begin() + index);
      // Check if delimiter if recomputed after every for run.
      for (size_t i = 0; i < _binHashSparse.first.size(); ++i) {
        for (size_t j = 0; j < _binHashSparse.first[i].second.size(); ++j) {
          if (_binHashSparse.first[i].second[j].first == index) {
            _binHashSparse.first[i].second.erase(_binHashSparse.first[i].second.begin() + j);
            j--;
          }
          else if (_binHashSparse.first[i].second[j].first > index) {
            _binHashSparse.first[i].second[j].first--;
          }
        }
      }

      for (size_t i = 0; i < _binHashSparse.second.size(); ++i) {
        for (size_t j = 0; j < _binHashSparse.second[i].second.size(); ++j) {
          if (_binHashSparse.second[i].second[j].first == index) {
            _binHashSparse.second[i].second.erase(_binHashSparse.second[i].second.begin() + j);
            j--;
          }
          else if (_binHashSparse.second[i].second[j].first > index) {
            _binHashSparse.second[i].second[j].first--;
          }
        }
      }
    }
    //@}


    /// @name Scaling operations
    //@{

    /// @brief Rescale the axes
    ///
    /// Scales the axis with a given scale.
    /// @todo This is far too big to be inline: move it to the bottom of this file, or perhaps to a .cc
    /// @todo Add a specific check for a scaling of 1.0, to avoid doing work when no scaling is wanted.
    void scaleXY(double scaleX, double scaleY) {
      // Two loops are put on purpose, just to protect
      // against improper _binHashSparse
      for (size_t i = 0; i < _binHashSparse.first.size(); ++i) {
        _binHashSparse.first[i].first *= scaleY;
        for (size_t j = 0; j < _binHashSparse.first[i].second.size(); ++j) {
          _binHashSparse.first[i].second[j].second.first *= scaleX;
          _binHashSparse.first[i].second[j].second.second *= scaleX;
        }
      }
      for (size_t i = 0; i < _binHashSparse.second.size(); ++i) {
        _binHashSparse.second[i].first *= scaleX;
        for (size_t j = 0; j < _binHashSparse.second[i].second.size(); ++j){
          _binHashSparse.second[i].second[j].second.first *= scaleY;
          _binHashSparse.second[i].second[j].second.second *= scaleY;
        }
      }

      /// Regenerate the bin edges cache.
      _binHashSparse.first.regenCache();
      _binHashSparse.second.regenCache();

      /// Now, as we have the map rescaled, we need to update the bins
      for (size_t i = 0; i < _bins.size(); ++i) {
        _bins[i].scaleXY(scaleX, scaleY);
      }

      _dbn.scaleXY(scaleX, scaleY);
      for (size_t i = 0; i < _outflows.size(); ++i) {
        for (size_t j =0; j < _outflows[i].size(); ++j) {
          _outflows[i][j].scaleXY(scaleX, scaleY);
        }
      }
    }


    /// Scales the bin weights
    /// @todo This is, I think, a bit too big to be inline: move it to the bottom of this file, or perhaps to a .cc
    void scaleW(double scalefactor) {
      _dbn.scaleW(scalefactor);
      for (size_t i = 0; i < _outflows.size(); ++i) {
        for (size_t j = 0; j < _outflows[i].size(); ++j) {
          _outflows[i][j].scaleW(scalefactor);
        }
      }
      /// @todo Use foreach for situations like this
      for (size_t i = 0; i < _bins.size(); ++i) {
        _bins[i].scaleW(scalefactor);
      }
    }

    //@}


    /// @name Operators
    //@{

    /// Equality operator
    /// @todo TEST!!11!
    bool operator == (const Axis2D& other) const {
      if (isGrid()) {
        for (size_t i = 0; i < _bins.size(); ++i) {
          int index = other.getBinIndex(_bins[i].midpoint().first, _bins[i].midpoint().second);
          if (index != -1){
            if (other.bin(index) != _bins[i]) return false;
          }
          else return false;
        }
        return true;
      }
      else return _binHashSparse == other._binHashSparse;
    }


    /// Non-equality operator
    bool operator != (const Axis2D& other) const {
      return ! operator == (other);
    }


    /// @brief Addition operator
    /// At this stage it is only possible to add histograms with the same binnings.
    /// @todo Compatible but not equal binning to come soon.
    Axis2D<BIN2D, DBN>& operator += (const Axis2D<BIN2D, DBN>& toAdd) {
      if (*this != toAdd) {
        throw LogicError("YODA::Histo1D: Cannot add axes with different binnings.");
      }
      for (size_t i = 0; i < bins().size(); ++i) {
        bin(i) += toAdd.bin(i);
      }
      _dbn += toAdd._dbn;
      return *this;
    }


    /// Subtraction operator
    ///
    /// At this stage it is only possible to subtract histograms with the same binnings.
    /// @todo Compatible but not equal binning to come soon.
    Axis2D<BIN2D, DBN>& operator -= (const Axis2D<BIN2D, DBN>& toSubtract) {
      if (*this != toSubtract) {
        throw LogicError("YODA::Histo1D: Cannot add axes with different binnings.");
      }
      for (size_t i = 0; i < bins().size(); ++i) {
        bin(i) -= toSubtract.bin(i);
      }
      _dbn -= toSubtract._dbn;
      return *this;
    }

    //@}


  private:

    /// @brief Outflows pre-setter
    /// Sets the correct number of bins in each of the outflows.
    void _setOutflows() {
      _outflows.resize(8);

      _outflows[0].resize(1);
      _outflows[1].resize(_binHashSparse.second.size());
      _outflows[2].resize(1);
      _outflows[3].resize(_binHashSparse.first.size());
      _outflows[4].resize(1);
      _outflows[5].resize(_binHashSparse.second.size());
      _outflows[6].resize(1);
      _outflows[7].resize(_binHashSparse.first.size());

    }


    /// Outflow filler
    /// @todo This is far too big to be inline: move it to the bottom of this file, or perhaps to a .cc
    void _fillOutflows(double x, double y, double weight) {
      if (x < _lowEdgeX && y > _highEdgeY) _outflows[0][0].fill(x, y, weight);
      else if (x > _lowEdgeX && x < _highEdgeX && y > _highEdgeY)
      {
        size_t element = _binaryS(_binHashSparse.second, x, 0, _binHashSparse.second.size());
        _outflows[1][element].fill(x, y, weight);
      }
      else if (x > _highEdgeX && y > _highEdgeY) _outflows[2][0].fill(x, y, weight);
      else if (x > _highEdgeX && y > _lowEdgeY && y < _highEdgeY)
      {
        size_t element = _binaryS(_binHashSparse.first, y, 0, _binHashSparse.first.size());
        _outflows[3][element].fill(x, y, weight);
      }
      else if (x > _highEdgeX && y < _lowEdgeY) _outflows[4][0].fill(x, y, weight);
      else if (x > _lowEdgeX && x < _highEdgeX && y < _lowEdgeY)
      {
        size_t element = _binaryS(_binHashSparse.second, x, 0, _binHashSparse.second.size());
        _outflows[5][element].fill(x, y, weight);
      }
      else if (x < _lowEdgeX && y < _lowEdgeY) _outflows[6][0].fill(x, y, weight);
      else if (x < _lowEdgeX && y > _lowEdgeY && y < _highEdgeY)
      {
        size_t element = _binaryS(_binHashSparse.first, y, 0, _binHashSparse.first.size());
        _outflows[7][element].fill(x, y, weight);
      }

    }

    /// @brief Checks if our bins form a grid.
    ///
    /// This function uses a neat property of _binHashSparse.  If it is
    /// containing a set of edges forming a grid without gaps in the middle it
    /// will have the same number of edges in the inner subcaches and half of
    /// this amount in the outer (grid boundary) subcaches. This makes isGrid()
    /// a very, very fast function.
    ///
    /// @todo This is far too big to be inline: move it to the bottom of this file, or perhaps to a .cc
    void _genGridCache() {
      // Check if the number of edges parallel to the x axis is proper in every
      // subcache.
      size_t sizeX = _binHashSparse.first[0].second.size();
      for (size_t i = 1; i < _binHashSparse.first.size(); ++i) {
        if (i == _binHashSparse.first.size() - 1) {
          if (_binHashSparse.first[i].second.size() != sizeX) {
            _isGrid = false;
          }
        }
        else if (_binHashSparse.first[i].second.size() != 2*sizeX) {
          _isGrid = false;
        }
      }

      // Do the same for edges parallel to the y axis.
      size_t sizeY = _binHashSparse.second[0].second.size();
      for (size_t i=1; i < _binHashSparse.second.size(); ++i) {
        if (i != _binHashSparse.second.size() - 1) {
          if (2*sizeY != _binHashSparse.second[i].second.size()) {
            _isGrid = false;
          }
        }
        else if (_binHashSparse.second[i].second.size() != sizeY) {
          _isGrid = false;
        }
      }

      // If everything is proper, announce it.
      _isGrid = true;
    }


    /// @brief Segment validator function
    ///
    /// This a 'dispatcher' function. It checks if the segment in question
    /// is vertical or horizontal and launches the appropriate function
    /// searching for cuts in the prope direction. Since it operates on
    /// a vector of segments it is prepared to act on arbitrarly large sets of edges,
    /// in practice usually being four sides of a rectangular bin.
    ///
    /// Notice that it will never be checked, in the current state, if there is a cut
    /// in edges in the edgeset. This imposes the requirement of provide the program
    /// with non-degenerate bins, until checking in implemented. However, it doesn't seem
    /// to be needed, as edges are not generated by a user.
    ///
    /// This is also a perfect place to parallelize, if required.
    ///
    /// @todo This is far too big to be inline: move it to the bottom of this file, or perhaps to a .cc
    ///
    /// @todo Is this really a method? Can't it just be a function?
    bool _validateEdge(std::vector<Segment>& edges) {
      // Setting the return variable. True means that no cuts were detected.
      bool ret = true;

      // Looping over all the edges provided
      for (size_t i = 0; i < edges.size(); ++i) {
        // If the X coordinate of the starting point is the same
        // as X coordinate of the ending one, checks if there are cuts
        // on this vertical segment.
        if (fuzzyEquals(edges[i].first.first, edges[i].second.first)) {
          ret = _findCutsY(edges[i]);
        }

        /// Check if the segment is horizontal and is it cutting any bin that already exists
        else if (fuzzyEquals(edges[i].first.second, edges[i].second.second)) {
          ret =  _findCutsX(edges[i]);
        }

        // This is a check that discards the bin if it is not a rectangle
        // composed of vertical and horizontal segments.
        else {
          ret = false;
        }

        // If a cut was detected, return false immediately.
        if (!ret) return false;
      }
      // If no cuts were detected in any of the edges, tell the launching function about this
      return true;
    }


    /// @brief A binary search function
    ///
    /// This is conceptually the same implementation as in STL
    /// but because it returns the index of an element in a vector,
    /// it is easier to use in our case than the STL implementation
    /// that returns a pointer at the element.
    ///
    /// @todo This is far too big to be inline: move it to the bottom of this file, or perhaps to a .cc
    /// @todo Does this actually have to be a member function? Isn't it just a function?
    size_t _binaryS(Utils::cachedvector<EdgeCollection>& toSearch,
                    double value, size_t lower, size_t higher) {
      // Just a check if such degenerate situation happens
      if (lower == higher) return lower;

      // Choose a midpoint that will be our pivot
      size_t where = (higher+lower)/2;

      // Launch the same procedure on half of the range above the pivot
      if (value >= toSearch[where].first) {
        if (where == toSearch.size() - 1) return where;
        if (value <= toSearch[where+1].first) return where;
        return _binaryS(toSearch, value, where, higher);
      }

      // This is not a redundant check, because of the nature of int division.
      if (where == 0) return where;

      // Check if the value is somewhere in-between an element at the position
      // in question and an element at a lower position. If so, return an index
      // to the current positon.
      if (value >= toSearch[where-1].first) return where;

      // If none of the above occurs, the value must be smaller that the element
      // at the current position. In such case, launch the search on half of the
      // interval below the current position.
      return _binaryS(toSearch, value, lower, where);
    }


    /// @brief Function that finds cuts of horizontal edges.
    ///
    /// A specialised function that tries to exploit the fact
    /// that edges can cut one another only on right angles
    /// to the highest extent. The inner workings are explained
    /// in the comments placed in the function body.
    ///
    /// @todo Explain what "cuts" are -- this is not obvious
    /// @todo This is far too big to be inline: move it to the bottom of this file, or perhaps to a .cc
    bool _findCutsX(Segment& edge) {
      // Look up the limits of search in the _binHashSparse
      // structure. We are not interested in the vertical edges
      // that are before the beginning of our segment or after its end.
      size_t i = _binaryS(_binHashSparse.second, edge.first.first, 0, _binHashSparse.second.size());
      size_t end = _binaryS(_binHashSparse.second, edge.second.first, 0, _binHashSparse.second.size());

      for (; i < end; ++i) {
        // Scroll through all the vertical segments with a given X coordinate
        // and see if any of those fulfills the cutting requirement. If it does,
        // announce it.
        for (size_t j = 0; j < _binHashSparse.second[i].second.size(); ++j) {
          // Note that we are not taking into account the edges touching the
          // segment in question. That's because sides of a bin touch
          if (_binHashSparse.second[i].second[j].second.first < edge.first.second &&
              _binHashSparse.second[i].second[j].second.second > edge.first.second &&
              !fuzzyEquals(_binHashSparse.second[i].second[j].second.first, edge.first.second) &&
              !fuzzyEquals(_binHashSparse.second[i].second[j].second.second, edge.first.second)) {
            return false;
          }
        }
      }
      // If none of the existing edges is cutting this edge, announce it
      return true;
    }


    /// @brief Function that finds cuts of vertical edges
    ///
    /// For a detailed description, see the documentation for _findCutsX().
    ///
    /// @todo Explain what "cuts" are -- this is not obvious
    ///
    /// @todo This is far too big to be inline: move it to the bottom of this file, or perhaps to a .cc
    ///
    /// @todo Can the code for "cut" finding be abstracted to work in both
    /// directions, to avoid writing (and maintaining) it twice?
    bool _findCutsY(Segment& edge) {
      size_t i = _binaryS(_binHashSparse.first, edge.first.second, 0, _binHashSparse.first.size());
      size_t end = _binaryS(_binHashSparse.first, edge.second.second, 0, _binHashSparse.first.size());
      for (; i < end; ++i) {
        for (size_t j = 0; j < _binHashSparse.first[i].second.size(); ++j) {
          if (_binHashSparse.first[i].second[j].second.first < edge.first.first &&
              _binHashSparse.first[i].second[j].second.second > edge.first.first &&
              !fuzzyEquals(_binHashSparse.first[i].second[j].second.first, edge.first.first) &&
              !fuzzyEquals(_binHashSparse.first[i].second[j].second.second, edge.first.first)) {
            return false;
          }
        }
      }
      return true;
    }


    /// @brief Bin inserter
    ///
    /// It contains all the commands that need to executed to properly add a
    /// bin. Specifically edges are added to the edge cache (_binHashSparse) and
    /// a bin is created from those edges.
    ///
    /// @todo This is far too large to be inline: move to the .cc
    void _addEdge(std::vector<Segment> edges, std::pair<Utils::cachedvector<EdgeCollection>,
                  Utils::cachedvector<EdgeCollection> >& binHash, bool addBin = true) {
      // Check if there was no mistake made when adding segments to a vector.
      if (edges.size() != 4) throw Exception("The segments supplied don't describe a full bin!");

      // This is the part in charge of adding each of the segments
      // to the edge cache. Segments are assumed to be validated
      // beforehand.
      for (size_t j = 0; j < edges.size(); ++j) {
        // Association made for convinience.
        Segment edge = edges[j];

        // Do the following if the edge is vertical.
        // Those two cases need to be distinguished
        // because of the way in which edge cache is structured.
        if (edge.first.first == edge.second.first) {
          // See if our edge has the same X coordinate as any other
          // edge that is currently in the cache.

          // Keeps the status of the search
          bool found = false;

          // There is only a certain set of X coordinates that we need to sweep
          // to check if our segment has the same X coordinate. Find them.
          size_t i = _binaryS(binHash.second, edge.first.first, 0, binHash.second.size()) - 1;
          if (i < 0) i = 0;
          size_t end = i + 3;

          // For the coordinates in range, check if one of them is an X
          // coordinate of the segment.
          for (; i < binHash.second.size() && i < end ; ++i) {
            /// If this is the case, do what is needed to be done.
            if (fuzzyEquals(binHash.second[i].first, edge.first.first)) {
              binHash.second[i].second.push_back(std::make_pair(_bins.size(),
                                                                std::make_pair(edge.first.second, edge.second.second)));
              found = true;
              break;
            }
          }

          // If no edge with the same X coordinate exist, create
          // a new subhash at the X coordinate of a segment.
          if (!found) {
            std::vector<Edge> temp;
            temp.push_back(std::make_pair(_bins.size(), std::make_pair(edge.first.second, edge.second.second)));
            binHash.second.push_back(std::make_pair(edge.first.first, temp));
            sort(binHash.second.begin(), binHash.second.end());
          }
        }

        // See the vertical case for description of a horizontal one
        else if (edge.first.second == edge.second.second) {
          bool found = false;
          size_t i = _binaryS(binHash.first, edge.first.second, 0, binHash.first.size())-1;
          if (i < 0) i = 0;
          size_t end = i+3;
          for (; i < binHash.first.size() && i < end; ++i) {
            if (fuzzyEquals(binHash.first[i].first, edge.first.second)) {
              binHash.first[i].second.push_back(std::make_pair(_bins.size(),
                                                               std::make_pair(edge.first.first, edge.second.first)));
              found = true;
            }
          }
          if (!found) {
            std::vector<Edge> temp;
            temp.push_back(std::make_pair(_bins.size(), std::make_pair(edge.first.first, edge.second.first)));
            binHash.first.push_back(std::make_pair(edge.second.second, temp));
            sort(binHash.first.begin(), binHash.first.end());
          }
        }
      }

      // Now, create a bin with the edges provided
      if (addBin) _bins.push_back(BIN2D(edges));
    }


    /// @brief Orientation fixer
    ///
    /// Check if the orientation of an edge is proper
    /// for the rest of the algorithm to work on, and if it is not
    /// fix it.
    ///
    /// @todo This is arguably too large to be inline: move to the .cc?
    void _fixOrientation(Segment& edge) {
      if (fuzzyEquals(edge.first.first, edge.second.first)) {
        if (edge.first.second > edge.second.second) {
          double temp = edge.second.second;
          edge.second.second = edge.first.second;
          edge.first.second = temp;
        }
      }
      else if (edge.first.first > edge.second.first) {
        double temp = edge.first.first;
        edge.first.first = edge.second.first;
        edge.second.first = temp;
      }
    }


    /// @brief Axis creator
    ///
    /// The top-level function taking part in the process of adding
    /// edges. Creating an axis is the same operation for it as adding new bins
    /// so it can be as well used to add some custom bins.
    ///
    /// It accepts two extremal points of a rectangle (top-right and
    /// bottom-left) as input.
    ///
    /// @todo This is far too large to be inline: move to the .cc
    void _mkAxis(const std::vector<Segment>& binLimits) {

      // For each of the rectangles
      for (size_t i = 0; i < binLimits.size(); ++i) {
        // Produce the segments that a rectangle is composed of
        Segment edge1 =
          std::make_pair(binLimits[i].first,
                         std::make_pair(binLimits[i].first.first, binLimits[i].second.second));
        Segment edge2 =
          std::make_pair(std::make_pair(binLimits[i].first.first, binLimits[i].second.second),
                         binLimits[i].second);
        Segment edge3 =
          std::make_pair(std::make_pair(binLimits[i].second.first, binLimits[i].first.second),
                         binLimits[i].second);
        Segment edge4 =
          std::make_pair(binLimits[i].first,
                         std::make_pair(binLimits[i].second.first, binLimits[i].first.second));

        // Check if they are made properly
        _fixOrientation(edge1); _fixOrientation(edge2);
        _fixOrientation(edge3); _fixOrientation(edge4);

        // Add all the segments to a vector
        std::vector<Segment> edges;
        edges.push_back(edge1); edges.push_back(edge2);
        edges.push_back(edge3); edges.push_back(edge4);

        // And check if a bin is a proper one, if it is, add it.
        if (_validateEdge(edges))  _addEdge(edges, _binHashSparse);
      }

      // Setting all the caches
      _binHashSparse.first.regenCache();
      _binHashSparse.second.regenCache();
      _genGridCache();
    }


  private:

    /// Bins contained in this histogram
    Bins _bins;

    /// Overflow distributions
    ///
    /// @todo Explain how the vector structure works!
    std::vector<std::vector<DBN> > _outflows;

    /// Total distribution
    /// @todo This needs to be a templated DBN type
    DBN _dbn;

    /// @brief Bin hash structure
    ///
    /// First in pair is holding the horizontal edges indexed by first.first
    /// which is an y coordinate. The last pair specifies x coordinates (begin, end) of
    /// the horizontal edge.
    /// Analogous for the second member of the pair.
    ///
    /// For the fullest description, see typedefs at the beginning
    /// of this file.
    std::pair<Utils::cachedvector<EdgeCollection>, Utils::cachedvector<EdgeCollection> > _binHashSparse;

    /// Low/high edges
    double _highEdgeX, _highEdgeY, _lowEdgeX, _lowEdgeY;

    /// Flag to mark if this histogram has a grid structure or not
    bool _isGrid;

  };


  /// @name Operators
  //@{

  /// Addition operator
  template <typename BIN2D, typename DBN>
  inline Axis2D<BIN2D, DBN> operator + (const Axis2D<BIN2D, DBN>& first, const Axis2D<BIN2D, DBN>& second) {
    Axis2D<BIN2D, DBN> tmp = first;
    tmp += second;
    return tmp;
  }


  /// Subtraction operator
  template <typename BIN2D, typename DBN>
  inline Axis2D<BIN2D, DBN> operator - (const Axis2D<BIN2D, DBN>& first, const Axis2D<BIN2D, DBN>& second) {
    Axis2D<BIN2D, DBN> tmp = first;
    tmp -= second;
    return tmp;
  }

  //@}


}

#endif
