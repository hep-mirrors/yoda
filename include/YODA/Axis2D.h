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
  template <typename BIN>
  class Axis2D {
  public:

    // A few helpful typedefs
    typedef BIN Bin;
    typedef typename std::vector<BIN> Bins;

    /// When edge is added to the collection it must
    /// obey the following format. size_t specifies the bin this
    /// edge is a member of, a pair contains a beginning and end of the edge.
    typedef typename std::pair<size_t, std::pair<double,double> > Edge;

    /// A type being a basic substructure of _binHashSparse. It contains a indicator
    /// specifying the major coordinate and a collection of edges sharing the same major
    /// coordinate.
    typedef typename std::pair<double, std::vector<Edge> > EdgeCollection;

    /// A simple point in 2D @todo Should Point2D be used?
    typedef typename std::pair<double, double> Point;

    /// Segment, having a beginning and end.
    typedef typename std::pair<Point, Point> Segment;


  public:

    /// @name Constructors:
    //@{

    /// @brief Empty constructor
    /// Only added because it is required by SWIG.
    /// It doesn't make much sense to use it.
    /// @todo Really "required"? Eliminate the requirement in the SWIG mapping.
    Axis2D() {
      vector<Segment> edges;
      _mkAxis(edges);
    }


    /// Constructor provided with a vector of bin delimiters
    Axis2D(const vector<Segment>& binLimits) {
      _mkAxis(binLimits);
      if(isGriddy()) _setOutflows();
    }


    /// Most standard constructor, should be self-explanatory
    Axis2D(size_t nbinsX, double lowerX, double upperX, size_t nbinsY, double lowerY, double upperY) {
      vector<Segment> binLimits;
      double coeffX = (upperX - lowerX)/(double)nbinsX;
      double coeffY = (upperY - lowerX)/(double)nbinsY;
      for (double i = lowerX; i < upperX; i += coeffX) {
        for(double j = lowerY; j < upperY; j += coeffY) {
          binLimits.push_back(make_pair(make_pair(i, j), make_pair((double)(i+coeffX), (double)(j+coeffY))));
        }
      }
      _mkAxis(binLimits);
      _setOutflows();
    }


    /// @brief A bin constructor
    /// Creates an Axis2D from existing bins
    Axis2D(Bins bins) {
      vector<Segment> binLimits;
      for (size_t i = 0; i < bins.size(); ++i) {
        binLimits.push_back(make_pair(make_pair(bins[i].xMin(), bins[i].yMin()),
                                      make_pair(bins[i].xMax(), bins[i].yMax())));
      }
      _mkAxis(binLimits);
      for (size_t i = 0; i < _bins.size(); ++i) {
        _bins[i] = bins[i];
      }
      _regenDelimiters();
      if(isGriddy()) _setOutflows();
    }

    //@}

    /// @name Addition operators:
    //@{

    /// @brief Bin addition operator
    /// This operator is provided a vector of limiting
    /// points in the format required by _mkAxis().
    /// It should be noted that there is nothing special about
    /// the initiation stage of Axis2D, and the edges can be added
    /// online if they meet all the requirements of non-degeneracy.
    /// No merging is supported, and I don't think it should before the support
    /// for merging for '+' operator (codewise it should be the same thing).
    void addBin(const vector<Segment>& binLimits) {
      _mkAxis(binLimits);
      _outflows.resize(0);
    }
        
    /// @brief Bin addition operator
    /// This operator is supplied with whe extreamal coordinates of just
    /// one bin. It then launches the standard bin addition procedure.
    void addBin(double lowX, double lowY, double highX, double highY) {
      vector<Segment> coords;
      coords.push_back(make_pair(make_pair(lowX, lowY), make_pair(highX, highY)));
      addBin(coords);
    }

    //@}

    /// @name Helper functions
    //@{

    /// Outflow filler
    void fillOutflows(double x, double y, double weight) {
      if(x < _lowEdgeX && y > _highEdgeY) _outflows[0][0].fill(x, y, weight);
      else if(x > _lowEdgeX && x < _highEdgeX && y > _highEdgeY) 
      {
        size_t element = _binaryS(_binHashSparse.second, x, 0, _binHashSparse.second.size());
        _outflows[1][element].fill(x, y, weight);
      }
      else if(x > _highEdgeX && y > _highEdgeY) _outflows[2][0].fill(x, y, weight);
      else if(x > _highEdgeX && y > _lowEdgeY && y < _highEdgeY) 
      {
        size_t element = _binaryS(_binHashSparse.first, y, 0, _binHashSparse.first.size());
        _outflows[3][element].fill(x, y, weight);
      }
      else if(x > _highEdgeX && y < _lowEdgeY) _outflows[4][0].fill(x, y, weight);
      else if(x > _lowEdgeX && x < _highEdgeX && y < _lowEdgeY)
      {
        size_t element = _binaryS(_binHashSparse.second, x, 0, _binHashSparse.second.size());
        _outflows[5][element].fill(x, y, weight);
      }
      else if(x < _lowEdgeX && y < _lowEdgeY) _outflows[6][0].fill(x, y, weight);
      else if(x < _lowEdgeX && y > _lowEdgeY && y < _highEdgeY) 
      {
        size_t element = _binaryS(_binHashSparse.first, y, 0, _binHashSparse.first.size());
        _outflows[7][element].fill(x, y, weight);
      }
        
    }


    /// @brief Bin merging
    /// Try to merge a certain amount of bins
    void mergeBins(size_t from, size_t to) {
      HistoBin2D& start = bin(from);
      HistoBin2D& end = bin(to);
      HistoBin2D temp = start;
      start.isReal = false;

      if (start.midpoint().first > end.midpoint().first ||
          start.midpoint().second > end.midpoint().second)
        throw GridError("The start/end bins are wrongly defined.");

      /// @todo Tidy!
      for (size_t y = (*_binHashSparse.first._cache.lower_bound(start.yMin())).second;
           y <= (*_binHashSparse.first._cache.lower_bound(end.yMin())).second; ++y) {
        for (size_t x = 0; x < _binHashSparse.first[y].second.size(); ++x) {
          /// @todo Tidy!
          if ((_binHashSparse.first[y].second[x].second.first > start.xMin() ||
               fuzzyEquals(_binHashSparse.first[y].second[x].second.first, start.xMin())) &&
              (_binHashSparse.first[y].second[x].second.second < end.xMax() ||
               fuzzyEquals(_binHashSparse.first[y].second[x].second.second, end.xMax())) &&
              bin(_binHashSparse.first[y].second[x].first).isReal)
            {
              temp += bin(_binHashSparse.first[y].second[x].first);
              bin(_binHashSparse.first[y].second[x].first).isReal = false;
            }
        }
      }
      _addEdge(temp.edges(), _binHashSparse, false);
      _bins.push_back(temp);

      _binHashSparse.first.regenCache();
      _binHashSparse.second.regenCache();
      _regenDelimiters();
    }


    /// @brief Checks if our bins form a grid.
    /// This function uses a neat property of _binHashSparse.
    /// If it is containing a set of edges forming a grid without
    /// gaps in the middle it will have the same number of edges in the
    /// inner subcaches and half of this amount in the outer (grid boundary)
    /// subcaches. This makes isGriddy() a very, very fast function.
    /// @todo Is the name appropriate? Should it be private?
    bool isGriddy() const {

      /// Check if the number of edges parallel to X axis
      /// is proper in every subcache.
      size_t sizeX = _binHashSparse.first[0].second.size();
      for (size_t i = 1; i < _binHashSparse.first.size(); ++i) {
        if (i == _binHashSparse.first.size() - 1) {
          if (_binHashSparse.first[i].second.size() != sizeX) {
            return false;
          }
        }
        else if (_binHashSparse.first[i].second.size() != 2*sizeX) {
          return false;
        }
      }

      /// Do the same for edges parallel to Y axis.
      size_t sizeY = _binHashSparse.second[0].second.size();
      for (size_t i=1; i < _binHashSparse.second.size(); ++i) {
        if (i != _binHashSparse.second.size() - 1) {
          if (2*sizeY != _binHashSparse.second[i].second.size()) {
            return false;
          }
        }
        else if (_binHashSparse.second[i].second.size() != sizeY) {
          return -1;
        }
      }

      /// If everything is proper, announce it.
      return true;
    }


    /// Get the total number of bins
    unsigned int numBinsTotal() const {
      return _bins.size();
    }


    /// Get the value of the lowest x-edge on the axis
    double lowEdgeX() const {
      return _lowEdgeX;
    }


    /// Get the value of the highest x-edge on the axis
    double highEdgeX() const {
      return _highEdgeX;
    }


    /// Get the value of the lowest y-edge on the axis
    double lowEdgeY() const {
      return _lowEdgeY;
    }


    /// Get the value of the highest y-edge on the axis
    double highEdgeY() const {
      return _highEdgeY;
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
    vector<vector<Dbn2D> >& outflows() {
      return _outflows;
    }
    /// Get the outflows (const version)
    const vector<vector<Dbn2D> >& outflows() const {
      return _outflows;
    }

    /// Get the bin with a given index (non-const version)
    BIN& bin(size_t index) {
      if (index >= _bins.size()) throw RangeError("Bin index out of range.");
      return _bins[index];
    }

    /// Get the bin with a given index (const version)
    const BIN& bin(size_t index) const {
      if (index >= _bins.size()) throw RangeError("Bin index out of range.");
      return _bins[index];
    }


    /// Get a bin at given coordinates (non-const version)
    BIN& binByCoord(double x, double y) {
      const int ret = _findBinIndex(x, y);
      if (ret != -1) return bin(ret);
      else throw RangeError("No bin found!!");
    }

    /// Get a bin at given coordinates (const version)
    const BIN& binByCoord(double x, double y) const {
      const int ret = _findBinIndex(x, y);
      if (ret != -1) return bin(ret);
      else throw RangeError("No bin found!!");
    }

    /// Get a bin at given coordinates (non-const version)
    BIN& binByCoord(pair<double, double>& coords) {
      return binByCoord(coords.first, coords.second);
    }

    /// Get a bin at given coordinates (const version)
    const BIN& binByCoord(pair<double, double>& coords) const {
      return binByCoord(coords.first, coords.second);
    }


    /// Get a total distribution (non-const version)
    Dbn2D& totalDbn() {
      return _dbn;
    }

    /// Get a total distribution (const version)
    const Dbn2D& totalDbn() const {
      return _dbn;
    }


    // /// Get the overflow distribution (non-const version)
    // Dbn2D& overflow()
    // {
    //   return _overflow;
    // }

    // /// Get the overflow distribution (const version)
    // const Dbn2D& overflow() const
    // {
    //   return _overflow;
    // }

    // /// Get the underflow distribution (non-const version)
    // Dbn2D& underflow()
    // {
    //   return _underflow;
    // }

    // /// Get the underflow distribution (const version)
    // const Dbn2D& underflow() const
    // {
    //   return _underflow;
    // }


    /// Get bin index from external classes
    int getBinIndex(double coordX, double coordY) const
    {
      return _findBinIndex(coordX, coordY);
    }

    /// Reset the axis statistics
    void reset()
    {
      _dbn.reset();
      // _underflow.reset();
      // _overflow.reset();
      for (size_t i = 0; i < _bins.size(); ++i) {
        _bins[i].reset();
      }
    }


    /// @brief Axis scaler
    /// Scales the axis with a given scale. If no scale is given, assumes
    /// identity transform.
    void scaleXY(double scaleX = 1.0, double scaleY = 1.0) {
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
      // _underflow.scaleXY(scaleX, scaleY);
      // _overflow.scaleXY(scaleX, scaleY);

      // Making sure that we have correct boundaries set after rescaling
      _regenDelimiters();
    }


    /// Scales the bin weights
    void scaleW(double scalefactor) {
      _dbn.scaleW(scalefactor);
      // _underflow.scaleW(scalefactor);
      // _overflow.scaleW(scalefactor);
      for (size_t i = 0; i < _bins.size(); ++i) {
        _bins[i].scaleW(scalefactor);
      }
    }
    //@}


    /// @name Operators
    //@{

    /// Equality operator
    bool operator == (const Axis2D& other) const {
      return _binHashSparse == other._binHashSparse;
    }

    /// Non-equality operator
    bool operator != (const Axis2D& other) const {
      return ! operator == (other);
    }

    /// @brief Addition operator
    /// At this stage it is only possible to add histograms with the same binnings.
    /// @todo Compatible but not equal binning to come soon.
    Axis2D<BIN>& operator += (const Axis2D<BIN>& toAdd) {
      if (*this != toAdd) {
        throw LogicError("YODA::Histo1D: Cannot add axes with different binnings.");
      }
      for (size_t i = 0; i < bins().size(); ++i) {
        bins().at(i) += toAdd.bins().at(i);
      }
      _dbn += toAdd._dbn;
      // _underflow += toAdd._underflow;
      // _overflow += toAdd._overflow;
      return *this;
    }


    /// Subtraction operator
    ///
    /// At this stage it is only possible to subtract histograms with the same binnings.
    /// @todo Compatible but not equal binning to come soon.
    Axis2D<BIN>& operator -= (const Axis2D<BIN>& toSubtract) {
      if (*this != toSubtract) {
        throw LogicError("YODA::Histo1D: Cannot add axes with different binnings.");
      }
      for (size_t i = 0; i < bins().size(); ++i) {
        bins().at(i) -= toSubtract.bins().at(i);
      }
      _dbn -= toSubtract._dbn;
      // _underflow -= toSubtract._underflow;
      // _overflow -= toSubtract._overflow;
      return *this;
    }

    //@}


  private:

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
    bool _validateEdge(vector<Segment>& edges) {
      /// Setting the return variable. True means that no cuts were detected.
      bool ret = true;

      /// Looping over all the edges provided
      for (size_t i = 0; i < edges.size(); ++i) {
        /// If the X coordinate of the starting point is the same
        /// as X coordinate of the ending one, checks if there are cuts
        /// on this vertical segment.
        if (fuzzyEquals(edges[i].first.first, edges[i].second.first)) {
          ret = _findCutsY(edges[i]);
        }

        /// Check if the segment is horizontal and is it cutting any bin that already exists
        else if(fuzzyEquals(edges[i].first.second, edges[i].second.second)) {
          ret =  _findCutsX(edges[i]);
        }

        /// This is a check that discards the bin if it is not a rectangle
        /// composed of vertical and horizontal segments.
        else {
          ret = false;
        }

        /// If a cut was detected, return false immediately.
        if (!ret) return false;
      }
      /// If no cuts were detected in any of the edges, tell the launching function about this
      return true;
    }


    /// @brief A binary search function
    ///
    /// This is conceptually the same implementation as in STL
    /// but because it returns the index of an element in a vector,
    /// it is easier to use in our case than the STL implementation
    /// that returns a pointer at the element.
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
      if(value >= toSearch[where-1].first) return where;

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
      /// If none of the existing edges is cutting this edge, announce it
      return true;
    }


    /// @brief Function that finds cuts of vertical edges
    ///
    /// For a detailed description, see the documentation for _findCutsX().
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


    /// @brief Function executed when a set of edges is dropped.
    ///
    /// It does not have any information about which edge in the set
    /// had failed the check. If this is needed such additional information
    /// can be readily implemented.
    void _dropEdge(vector<Segment>& edges) {
      std::cerr << "A set of edges was dropped." << endl;
    }


    /// @brief Bin adder
    ///
    /// It contains all the commands that need to executed
    /// to properly add a bin. Specifially edges are added to
    /// the edge cache (_binHashSparse) and a bin is created from
    /// those edges.
    void _addEdge(vector<Segment> edges, pair<Utils::cachedvector<EdgeCollection>,
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
              binHash.second[i].second.push_back(make_pair(_bins.size(),make_pair(edge.first.second, edge.second.second)));
              found = true;
              break;
            }
          }

          // If no edge with the same X coordinate exist, create
          // a new subhash at the X coordinate of a segment.
          if (!found) {
            vector<Edge> temp;
            temp.push_back(make_pair(_bins.size(), make_pair(edge.first.second, edge.second.second)));
            binHash.second.push_back(make_pair(edge.first.first,temp));
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
              binHash.first[i].second.push_back(make_pair(_bins.size(),make_pair(edge.first.first, edge.second.first)));
              found = true;
            }
          }
          if (!found) {
            vector<Edge> temp;
            temp.push_back(make_pair(_bins.size(), make_pair(edge.first.first, edge.second.first)));
            binHash.first.push_back(make_pair(edge.second.second, temp));
            sort(binHash.first.begin(), binHash.first.end());
          }
        }
      }

      // Now, create a bin with the edges provided
      if (addBin) _bins.push_back(BIN(edges));
    }


    /// @brief Orientation fixer
    ///
    /// Check if the orientation of an edge is proper
    /// for the rest of the algorithm to work on, and if it is not
    /// fix it.
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
    void _mkAxis(const vector<Segment>& binLimits) {

      // For each of the rectangles
      for (size_t i = 0; i < binLimits.size(); ++i) {
        // Produce the segments that a rectangle is composed of
        Segment edge1 =
          make_pair(binLimits[i].first,
                    make_pair(binLimits[i].first.first, binLimits[i].second.second));
        Segment edge2 =
          make_pair(make_pair(binLimits[i].first.first, binLimits[i].second.second),
                    binLimits[i].second);
        Segment edge3 =
          make_pair(make_pair(binLimits[i].second.first, binLimits[i].first.second),
                    binLimits[i].second);
        Segment edge4 =
          make_pair(binLimits[i].first,
                    make_pair(binLimits[i].second.first, binLimits[i].first.second));

        // Check if they are made properly
        _fixOrientation(edge1); _fixOrientation(edge2);
        _fixOrientation(edge3); _fixOrientation(edge4);

        // Add all the segments to a vector
        vector<Segment> edges;
        edges.push_back(edge1); edges.push_back(edge2);
        edges.push_back(edge3); edges.push_back(edge4);

        // And check if a bin is a proper one, if it is, add it.
        if (_validateEdge(edges))  _addEdge(edges, _binHashSparse);
        else _dropEdge(edges);
      }

      // Setting all the caches
      _binHashSparse.first.regenCache();
      _binHashSparse.second.regenCache();
      _regenDelimiters();
    }


    /// @brief Plot extrema (re)generator.
    ///
    /// Since scrolling through every bin is an expensive operation to do every
    /// time we need the limits of the plot, there are caches set up. This
    /// function regenerates them. It should be run after any change is made to
    /// bin layout.
    void _regenDelimiters() {
      double highEdgeX = numeric_limits<double>::min();
      double highEdgeY = numeric_limits<double>::min();
      double lowEdgeX = numeric_limits<double>::max();
      double lowEdgeY = numeric_limits<double>::max();

      // Scroll through the bins and set the delimiters.
      for (size_t i = 0; i < _bins.size(); ++i) {
        if (_bins[i].xMin() < lowEdgeX) lowEdgeX = _bins[i].xMin();
        if (_bins[i].xMax() > highEdgeX) highEdgeX = _bins[i].xMax();
        if (_bins[i].yMin() < lowEdgeY) lowEdgeY = _bins[i].yMin();
        if (_bins[i].yMax() > highEdgeY) highEdgeY = _bins[i].yMax();
      }

      _lowEdgeX = lowEdgeX;
      _highEdgeX = highEdgeX;
      _lowEdgeY = lowEdgeY;
      _highEdgeY = highEdgeY;
    }

    /// @brief Bin index finder
    ///
    /// Looks through all the bins to see which one contains the point of
    /// interest.
    int _findBinIndex(double coordX, double coordY) const
    {
      for (size_t i=0; i < _bins.size(); ++i) {
        if(_bins[i].xMin() <= coordX && _bins[i].xMax()  >= coordX &&
           _bins[i].yMin() <= coordY && _bins[i].yMax() >= coordY &&
           _bins[i].isReal) return i;
      }
      return -1;
    }


  private:

    /// Bins contained in this histogram
    Bins _bins;

    /// Underflow distribution
    // /// @todo Need many (binned?) outflows instead. This can't work at the moment.
    // Dbn2D _underflow;
    // Dbn2D _overflow;
    vector<vector<Dbn2D> > _outflows;

    /// The total distribution
    Dbn2D _dbn;

    /// @brief Bin hash structure
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

  };


  /// @name Operators
  //@{

  /// Addition operator
  template <typename BIN>
  inline Axis2D<BIN> operator + (const Axis2D<BIN>& first, const Axis2D<BIN>& second) {
    Axis2D<BIN> tmp = first;
    tmp += second;
    return tmp;
  }


  /// Subtraction operator
  template <typename BIN>
  inline Axis2D<BIN> operator - (const Axis2D<BIN>& first, const Axis2D<BIN>& second)
  {
    Axis2D<BIN> tmp = first;
    tmp -= second;
    return tmp;
  }

  //@}


}

#endif
