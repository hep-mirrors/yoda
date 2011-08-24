#include "YODA/Profile1D.h"

#include <iostream>
using namespace std;
using namespace YODA;

int main() {
  cout << "Testing range/number constructor         ";
  Profile1D p1(100, 0, 100);
  
  cout << "PASS" << endl;
  return EXIT_SUCCESS;
}
