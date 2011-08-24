#include "YODA/Profile1D.h"

#include <iostream>
using namespace std;
using namespace YODA;

int main() {
  cout << "Testing range/number constructor         ";
  Profile1D p1(100, 0, 100);
  if(p1.sumW() != 0 || p1.sumW2() != 0 || p1.sumW(false) != 0 || p1.sumW2(false) != 0){
    cout << "FAIL" << endl;
    return -1;
  }
  cout << "PASS" << endl;

  cout << "Testing explicit edges constructor       ";
  vector<double> edges;
  for(size_t i = 0; i < 101; ++i) edges.push_back(i);
  Profile1D p2(edges);
  
  if(p2.sumW() != 0 || p2.sumW2() != 0 || p2.sumW(false) != 0 || p2.sumW2(false) != 0){
    cout << "FAIL" << endl;
    return -1;
  }
  if(p2.numBins() != 100){
    cout << "FAIL" << endl;
    return -1;
  }
  cout << "PASS" << endl;
  
  cout << "Preliminary testing of = operator        ";
  Profile1D p3(edges);
  p3 = p2;
  
  if(p3.sumW() != 0 || p3.sumW2() != 0 || p3.sumW(false) != 0 || p3.sumW2(false) != 0){
    cout << "FAIL" << endl;
    return -1;
  }
  if(p3.numBins() != 100){
    cout << "FAIL" << endl;
    return -1;
  }
  cout << "PASS" << endl;

  return EXIT_SUCCESS;
}
