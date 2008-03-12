// -*- C++ -*-
#include <vector>
#include <string>
#include "Point.h"
#include "Histo.h"

class Scatter {
public:
  Scatter(std::string name);
  Scatter(const Histo &); // explicit?

  Scatter(std::string name,
	  const std::vector<double> & data, 
	  const std::vector<double> & yerrors,
	  const std::vector<double> & xerrors = std::vector<double>());

  void addErrors(const std::vector<double> & yerrors, 
		 const std::vector<double> & xerrors = std::vector<double>());
  
  Scatter & operator+=(const Scatter &);
  Scatter & operator-=(const Scatter &);
  Scatter & operator*=(const Scatter &);
  Scatter & operator/=(const Scatter &);

  // don't use implicit Scatter -> Histo conversion if runtime problem
  Scatter & operator+=(const Histo &);
  Scatter & operator-=(const Histo &);
  Scatter & operator*=(const Histo &);
  Scatter & operator/=(const Histo &);
  
  string name() const;
private:
  // for debugging
  bool isConsistent() const;

private:
  std::vector<Point> _points;
  // treat as cache
  std::vector<double> _binlimits;
  double _sumweights;
  std::string _name;
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
