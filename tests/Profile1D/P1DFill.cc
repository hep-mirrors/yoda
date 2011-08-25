#include "YODA/Profile1D.h"

#include <iostream>
using namespace std;
using namespace YODA;

int main() {
  Profile1D p(100, 0, 100);

  cout << "Testing fill operator:                   ";
  p.fill(1,1,2);
  if(p.sumW() != 2 || p.sumW2() != 4) {
    cout << "FAIL" << endl;
    return -1;
  }
  cout << "PASS" << endl;

  cout << "Testing the fill of the underflow:       ";
  p.fill(-10,2,3);
  if(p.underflow().xMean() != -10){
    cout << "FAIL" << endl;
    return -1;
  }
  if(p.underflow().yMean() != 2){
    cout << "FAIL" << endl;
    return -1;
  }
  if(p.underflow().sumW() != 3 || p.underflow().sumW2() != 9){
    cout << "FAIL" << endl;
    return -1;
  }
  cout << "PASS" << endl;
  
  cout << "Testing the fill of the underflow:       ";
  p.fill(110,2,3);
  if(p.overflow().xMean() != 110){
    cout << "FAIL" << endl;
    return -1;
  }
  if(p.overflow().yMean() != 2){
    cout << "FAIL" << endl;
    return -1;
  }
  if(p.overflow().sumW() != 3 || p.overflow().sumW2() != 9){
    cout << "FAIL" << endl;
    return -1;
  }
  cout << "PASS" << endl;

  return EXIT_SUCCESS;
}
