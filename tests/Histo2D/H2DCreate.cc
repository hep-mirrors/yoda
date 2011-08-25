#include "YODA/Histo2D.h"

#include <iostream>
using namespace std;
using namespace YODA;

int main() {
  cout << "Creating histos to be operated on        "; 

  Histo2D first(10, 0, 100, 10, 0, 100);
  first.fill(1,1,1);
  first.fill(23,1,1);
  Histo2D second(10, 0, 100, 10, 0, 100);
  second.fill(1,1,1);
  cout << "PASS" << endl;

  cout << "Testing the copy constructor:            ";
  Histo2D h(20, 0, 100, 20, 0, 100);
  Histo2D copyTest(h);
  cout << "PASS" << endl;

  cout << "Adding/substracting/dividing:            ";
  Histo2D added(first+second);
  Histo2D subtracted(first-second);
  Scatter3D divided(first/second);
  cout << "PASS" << endl;

  return EXIT_SUCCESS;
}
