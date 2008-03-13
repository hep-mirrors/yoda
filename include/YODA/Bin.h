// -*- C++ -*-

#ifndef YODA_Bin_h
#define YODA_Bin_h

#include "YODA/Histo.fh"

namespace YODA {

  /// @brief A Bin in a 1D histogram
  /// Lower edge is inclusive.
  class Bin {

    /// Histo is a friend to add/subtract bins
    friend class Histo;

  public:

    /// @name Constructor giving bin low and high edges.
    //@{
    Bin (double lowedge, double highedge);
    Bin (std::pair<double,double> edges);
    //@}

  public:

    /// @brief Fill this bin with weight @a weight.
    /// @todo Be careful about negative weights.
    void fill(double coord, double weight=1.0);

  public:

    /// @name X-axis info
    //@{
    /// Lower limit of the bin (inclusive).
    double lowEdge() const;

    /// Upper limit of the bin (exclusive).
    double highEdge() const;

    /// Get the {low,high} edges as an STL @c pair.
    std::pair<double,double> edges() const;

    /// Separation of low and high edges, i.e. high-low.
    double width() const;

    /// The mean position in the bin.
    double focus() const;

    /// Geometric centre of the bin, i.e. high+low/2.0
    double midpoint() const;
    //@}

    /// @name Bin content info
    //@{
    /// The area is the sum of weights in the bin, i.e. the
    /// width of the bin has no influence on this figure.
    double area() const;

    /// The height is defined as area/width.
    double height() const;
    //@}

    /// @name Error info
    //@{
    /// Error computed using binomial statistics on the sum of bin weights,
    /// i.e. err_area = sqrt{sum{weights}}
    double areaError() const;

    /// As for the height vs. area, the height error includes a scaling factor
    /// of the bin width, i.e. err_height = sqrt{sum{weights}} / width.
    double heightError() const;

    /// The x error is the standard error on the bin focus. 
    double xError() const;

    /// @todo RMS?
    //@}

  public:

    // @todo conversion to data point

  private:

    /// Add two bins (for use by Histo1D).
    Bin operator += (const Bin&);

    /// Subtract two bins
    Bin operator -= (const Bin&);

  private:

    /// The bin limits
    std::pair<double,double> _edges;

    /// The number of entries
    unsigned long _numEntries;

    /// The sum of weights
    double _sumWeight;

    /// The sum of weights squared
    double _sumWeight2;

    /// The sum of x*weight
    double _sumXWeight;

    /// The sum of x^2 * weight
    double _sumX2Weight;

  };

}

#endif // YODA_Bin_h
