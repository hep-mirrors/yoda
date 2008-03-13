namespace YODA {

// bin in a 1d histogram
class Bin {

public:

  // constructor giving  limits
  Bin (pair<double,double> limits);

  // default destructor
  virtual ~Bin ();

public:

  void fill (double weight = 1.0);

public:

  pair<double,double> limits () const;

  size_t numEntries () const;

  double sumWeights () const;

  double sumSquaredWeights () const;

  size_t nanEntries () const;

public:

  // convert bin to datapoint assuming some statistics
  // interpretation
  template<class Statistics>
  DataPoint convert ();

public:

  // add two bins
  Bin operator += (const Bin&);

private:

  // default constructor is private
  // there's no bin without limits specified
  Bin ();

  pair<double,double> _limits;

  size_t _numEntries;

  double _sumWeights;

  double _sumSquaredWeights;

  size_t _nanEntries;

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
