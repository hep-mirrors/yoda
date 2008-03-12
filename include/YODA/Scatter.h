// -*- C++ -*-
#include <vector>
#include "Point.h"
#include "Histo.h"

class Scatter {
public:
  Scatter();
  Scatter(const Histo &); // explicit?

  Scatter(const std::vector<double> & data, 
	  const std::vector<double> & yerrors,
	  const std::vector<double> & xerrors = std::vector<double>());

  void addErrors(const std::vector<double> & yerrors, 
		 const std::vector<double> & xerrors = std::vector<double>());
  
  Scatter & operator+=(const Scatter &);
  Scatter & operator-=(const Scatter &);
  Scatter & operator*=(const Scatter &);
  Scatter & operator/=(const Scatter &);
  
  Scatter & operator+=(const Histo &);
  Scatter & operator-=(const Histo &);
  Scatter & operator*=(const Histo &);
  Scatter & operator/=(const Histo &);
  
private:
  // for debugging
  bool isConsistent() const;

private:
  std::vector<Point> _points;
  // treat as cache
  std::vector<double> _binlimits;
  double _sumweights;
};

Scatter operator+(const Scatter &, const Scatter &);
Scatter operator-(const Scatter &, const Scatter &);
Scatter operator*(const Scatter &, const Scatter &);
Scatter operator/(const Scatter &, const Scatter &);

Scatter operator+(const Scatter &, const Histo &);
Scatter operator-(const Scatter &, const Histo &);
Scatter operator*(const Scatter &, const Histo &);
Scatter operator/(const Scatter &, const Histo &);

Scatter operator+(const Histo &, const Scatter &);
Scatter operator-(const Histo &, const Scatter &);
Scatter operator*(const Histo &, const Scatter &);
Scatter operator/(const Histo &, const Scatter &);
