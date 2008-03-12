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
