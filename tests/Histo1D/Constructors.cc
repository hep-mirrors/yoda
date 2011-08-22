#include "YODA/Histo1D.h"
#include "YODA/Utils/MathUtils.h"

#include <iostream>
#include <cmath>

using namespace YODA;
using namespace std;

int main() {
  cout << "--------------------" << endl;
  cout << "Testing constructors: " << endl;

  cout << "The most basic, linear constructor.      ";
  Histo1D h(100, 0, 100);
  if(h.numBins() != 100) {
    cout << "FAIL" << endl << "Wrong number of bins was created!" << endl;
    return -1;
  }
  if(h.lowEdge() != 0) {
    cout << "FAIL" << endl << "Low edge wasn't properly set!" << endl;
    return -1;
  }
  if(h.highEdge() != 100) {
    cout << "FAIL" << endl << "High edge wasn't properly set!" << endl;
    return -1;
  }
  if(!fuzzyEquals(h.integral(), 0)) {
    cout << "FAIL" << endl << "The constructor is setting some statistics!" << endl;
    return -1;
  }
  cout << "PASS" << endl;

  cout << "Explitit bin edges constructor:          ";
  vector<double> edges;
  for(int i = 0; i < 101; ++i) edges.push_back(i);
  Histo1D h1(edges);
  if(h1.numBins() != 100) {
    cout << "FAIL" << endl << "Wrong number of bins was created!" << endl;
    return -1;
  }
  if(h1.lowEdge() != 0) {
    cout << "FAIL" << endl << "Low edge wasn't properly set!" << endl;
    return -1;
  }
  if(h1.highEdge() != 100) {
    cout << "FAIL" << endl << "High edge wasn't properly set!" << endl;
    return -1;
  }
  if(!fuzzyEquals(h1.integral(), 0)) {
    cout << "FAIL" << endl << "The constructor is setting some statistics!" << endl;
    return -1;
  }
  cout << "PASS" << endl;

  cout << "Copy constructor:                        ";
  Histo1D h2(h);
  if(h2.numBins() != 100) {
    cout << "FAIL" << endl << "Wrong number of bins was created!" << endl;
    return -1;
  }
  if(h2.lowEdge() != 0) {
    cout << "FAIL" << endl << "Low edge wasn't properly set!" << endl;
    return -1;
  }
  if(h2.highEdge() != 100) {
    cout << "FAIL" << endl << "High edge wasn't properly set!" << endl;
    return -1;
  }
  if(!fuzzyEquals(h2.integral(), 0)) {
    cout << "FAIL" << endl << "The constructor is setting some statistics!" << endl;
    return -1;
  }
  cout << "PASS" << endl;
  
  return EXIT_SUCCESS;
}
