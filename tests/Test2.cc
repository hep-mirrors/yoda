#include "YODA/Histo1D.h"
#include "YODA/WriterAIDA.h"
#include <cmath>
#include <fstream>
#include <unistd.h>

using namespace std;
using namespace YODA;


int main() {

  Histo1D h(20, 0.0, 1.0);
  for (size_t n = 0; n < 1000; ++n) {
    const double num = rand()/static_cast<double>(RAND_MAX);
    //cout << "Filling with " << num << endl;
    h.fill(num);
  }

  ofstream file("test2.aida");

  Writer& w = WriterAIDA::create();
  w.write(file, h);

  file.close();

  return EXIT_SUCCESS;
}
