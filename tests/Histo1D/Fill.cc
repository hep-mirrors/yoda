#include "YODA/Histo1D.h"

#include <iostream>
using namespace std;
using namespace YODA;

int main() {
  /// Creating a histo:
  Histo1D h(100, 0, 100);
  
  cout << "Trying to fill the sample histogram:     ";
  h.fill(0.5,2);
  cout << "PASS" << endl;
  
  cout << "Checking sumW:                           ";
  if(h.sumW() != 2) {
    cout << "FAIL" << endl;
    return -1;
  }
  cout << "PASS" << endl;
  
  cout << "Checking sumW2:                          ";
  if(h.sumW2() != 4) {
    cout << "FAIL" << endl;
    return -1;
  }
  cout << "PASS" << endl;
  

  h.fill(10, 1);

  cout << "Checking mean:                           ";
  if(!fuzzyEquals(3.66667, h.mean(false))) {
    cout << "FAIL" << endl;
    return -1;
  }
  cout << "PASS" << endl;

  cout << "Checking variance:                       ";
  if(!fuzzyEquals(20.0556, h.variance(false))){
    cout << "FAIL" << endl;
    return -1;
  }
  cout << "PASS" << endl;
  
  cout << "Trying to fill the underflow:            ";
  h.fill(-10,1);
  cout << "PASS" << endl;
  cout << "Checking if stats were set correctly:    ";
  if(!fuzzyEquals(h.underflow().mean(), -10)) {
    cout << "FAIL" << endl;
    return -1;
  }
  cout << "PASS" << endl;

  cout << "Trying to fill the overflow:             ";
  h.fill(110,1);
  cout << "PASS" << endl;
  cout << "Checking if stats were set correctly:    ";
  if(!fuzzyEquals(h.overflow().mean(), 110)) {
    cout << "FAIL" << endl;
    return -1;
  }
  cout << "PASS" << endl;

  return EXIT_SUCCESS;
}
