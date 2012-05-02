#include "YODA/Profile1D.h"

#include <iostream>
using namespace std;
using namespace YODA;

int main() {
  Profile1D p(100,0,100);
  p.fill(1,1,2);

  cout << "Scaling the height:                      ";
  p.scaleW(3);
  if(p.sumW() != 6 || p.sumW2() != 36) {
    cout << "FAIL" << endl;
    return -1;
  }
  cout << "PASS" << endl;

  cout << "Resetting the profile:                   ";
  p.reset();
  if(p.sumW() != 0 || p.sumW2() != 0){
    cout << "FAIL" << endl;
    return -1;
  }
  cout << "PASS" << endl;

  cout << "Merging the bins:                        ";
  p.mergeBins(0,10);
  if(p.bin(0).xMin() != 0 || p.bin(0).xMax() != 11){
    cout << "FAIL" << endl;
    return -1;
  }
  if(p.numBins() != 90){
    cout << "FAIL" << endl;
    return -1;
  }
  cout << "PASS" << endl;

  cout << "Testing rebinning:                       ";
  p.rebin(2);
  for(size_t i = 1; i < p.bins().size() - 1; ++i){
    if(2 != p.bin(i).width()){
      cout << "FAIL" << endl;
      return -1;
    }
  }
  if(p.numBins() != 45){
    cout << "FAIL" << endl;
    return -1;
  }
  cout << "PASS" << endl;

  cout << "Trying to add a bin (first method):      ";
  p.addBin(110, 120);
  if(p.numBins() != 46){
    cout << "FAIL" << endl;
    return -1;
  }
  cout << "PASS" << endl;

  cout << "Trying to add a bin (second method):     ";
  vector<double> test;
  test.push_back(120); test.push_back( 140); test.push_back(145);
  p.addBins(test);
  if(p.numBins() != 48){
    cout << "FAIL" << endl;
    return -1;
  }
  cout << "PASS" << endl;

  // cout << "Trying to add a bin (third method):      ";
  // vector<pair<double,double> > test2;
  // test2.push_back(make_pair(180,190));
  // p.addBins(test2);
  // if(p.numBins() != 49){
  //   cout << "FAIL" << endl;
  //   return -1;
  // }
  // cout << "PASS" << endl;

  return EXIT_SUCCESS;
}
