#include "YODA/Histo1D.h"

#include <iostream>
using namespace std;
using namespace YODA;

int main() {
  
  Histo1D h(100, 0, 100);
  cout << "Trying to merge bins:                    ";
  h.mergeBins(0, 10);
  cout << "PASS" << endl;

  cout << "Testing if bin nuber was updated:        ";
  if(h.numBins() != 90) {
    cout << "FAIL" << endl;
    return -1;
  }
  cout << "PASS" << endl;
  
  cout << "Checking if bin size was updated:        ";
  if(!fuzzyEquals(h.bin(0).xMin(), 0) || !fuzzyEquals(h.bin(0).xMax(),11)){
    cout << "FAIL" << endl;
    return -1;
  }
  cout << "PASS" << endl;

  
  
  cout << "Checking if bin removal works:           ";
  h.eraseBin(0);
  cout << "PASS" << endl;
  
  cout << "Was the bin number updated properly?     ";
  if(h.numBins() != 89) {
    cout << "FAIL" << endl;
    return -1;
  }
  cout << "PASS" << endl;

  cout << "Was the right bin removed?               ";
  if(fuzzyEquals(h.bin(0).xMin(), 0) && fuzzyEquals(h.bin(0).xMax(), 11)){
    cout << "FAIL" << endl;
    return -1;
  }
  cout << "PASS" << endl;


  return EXIT_SUCCESS;
}
