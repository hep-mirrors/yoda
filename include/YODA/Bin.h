namespace YODA {

  /// @brief A Bin in a 1D histogram
  /// Lower edge is inclusive.
  class Bin {

  public:

    /// @name Constructor giving bin low and high edges.
    //@{
    Bin (double lowedge, double highedge);
    Bin (pair<double,double> edges);
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
    pair<double,double> edges() const;

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

  private:

    double sumWeights () const;

    double sumSquaredWeights () const;

  public:

    // convert bin to datapoint assuming some statistics
    // interpretation
    //template<class Statistics>
    //DataPoint convert ();

  private:

    /// Add two bins (for use by Histo1D).
    Bin operator += (const Bin&);
    Bin operator -= (const Bin&);

  private:

    pair<double,double> _limits;

    size_t _numEntries;

    double _sumWeight;

    double _sumWeight2;

    double _sumXWeight;

    double _sumX2Weight;

  };

}



/////////////////////////////
// FROM AIDA/LWH HISTOGRAM1D


  /**
   * The weighted mean of a bin. 
   * @param index The bin number (0...N-1) or OVERFLOW or UNDERFLOW.
   * @return      The mean of the corresponding bin.
   */
  // double binMean(int index) const {
  //   int i = index + 2;
  //   return sumw[i] != 0.0? sumxw[i]/sumw[i]:
  //     ( vax? vax->binMidPoint(index): fax->binMidPoint(index) );
  // };

  // /**
  //  * The weighted RMS of a bin. 
  //  * @param index The bin number (0...N-1) or OVERFLOW or UNDERFLOW.
  //  * @return      The RMS of the corresponding bin.
  //  */
  // double binRms(int index) const {
  //   int i = index + 2;
  //   return sumw[i] == 0.0 || sum[i] < 2? ax->binWidth(index):
  //     std::sqrt(std::max(sumw[i]*sumx2w[i] - sumxw[i]*sumxw[i], 0.0))/sumw[i];
  // };

  // /**
  //  * Number of entries in the corresponding bin (ie the number of
  //  * times fill was called for this bin).
  //  * @param index The bin number (0...N-1) or OVERFLOW or UNDERFLOW.
  //  * @return      The number of entries in the corresponding bin. 
  //  */
  // int binEntries(int index) const {
  //   return sum[index + 2];
  // }

  // /**
  //  * Total height of the corresponding bin (ie the sum of the weights
  //  * in this bin).
  //  * @param index The bin number (0...N-1) or OVERFLOW or UNDERFLOW.
  //  * @return      The height of the corresponding bin.
  //  */
  // double binHeight(int index) const {
  //   /// @todo While this is compatible with the reference AIDA implementation, it is not the bin height!
  //   return sumw[index + 2];
  // }

  // /**
  //  * The error of a given bin.
  //  * @param index The bin number (0...N-1) or OVERFLOW or UNDERFLOW.
  //  * @return      The error on the corresponding bin.
  //  *
  //  */
  // double binError(int index) const {
  //   return std::sqrt(sumw2[index + 2]);
  // }
