#include "YODA/Scatter2D.h"
#include "YODA/Point2D.h"

#include <iostream>
using namespace std;
using namespace YODA;

int main() {
  cout << "Constructing a scatter (empty const):    ";
  Scatter2D s1();
  cout << "PASS" << endl;
  
  
  cout << "Constructing a scatter (vector of points)";
  vector<Point2D> points;
  Point2D apoint(0,0,0);
  points.push_back(apoint);

  Scatter2D s2(points);
  cout << "PASS" << endl;

  cout << "Constructing a scatter (values, no errs) ";
  vector<double> values;
  values.push_back(0);
  Scatter2D s3(values, values);
  cout << "PASS" << endl;

  cout << "Constructing a scatter (values, sym errs)";
  Scatter2D s4(values, values, values, values);
  cout << "PASS" << endl;

  cout << "Constructing a scatter (sym err x,asym y)";
  vector<pair<double, double> > valuesS;
  valuesS.push_back(make_pair(0,0));
  Scatter2D s5(values, values, values, valuesS);
  cout << "PASS" << endl;

  cout << "Constructing a scatter (asym x, asym y)  ";
  Scatter2D s6(values, values, valuesS, valuesS);
  cout << "PASS" << endl;

  cout << "Constructing a scatter (axym x, sym y)   ";
  Scatter2D s7(values, values, valuesS, values);
  cout << "PASS" << endl;

  cout << "Constructing a scatter (explicit asym)   ";
  Scatter2D s8(values, values, values, values, values, values);
  cout << "PASS" << endl;

  cout << "Testing a copy operator:                 ";
  Scatter2D s9(s8);
  cout << "PASS" << endl;

  cout << "Testing an assignment operator:          ";
  Scatter2D s10(s9);
  s10 = s7;
  cout << "PASS" << endl;

  
  return EXIT_SUCCESS;
}
