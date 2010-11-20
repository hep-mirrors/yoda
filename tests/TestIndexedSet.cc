#include "YODA/indexedset.h"
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace YODA;

int main() {
  utils::indexedset<int> iset;
  iset.insert(1);
  iset.insert(5);
  iset.insert(3);
  iset.insert(2);
  iset.insert(4);

  cout << iset[3] << " == 4: "
       << boolalpha << (iset[3] == 4)
       << endl;

  return EXIT_SUCCESS;
}
