#include "YODA/Profile1D.h"

#include <iostream>
using namespace std;
using namespace YODA;

int main() {
  Profile1D p(100, 0, 100);

  cout << "Testing fill operator                    ";
  p.fill(1,1,2);
  if(p.sumW() != 2 || p.sumW2() != 4) {
    cout << "FAIL" << endl;
    return -1;
  }
  cout << "PASS" << endl;

  return EXIT_SUCCESS;
}
