#include "YODA/Histo1D.h"
#include "YODA/WriterAIDA.h"
#include "YODA/WriterYODA.h"
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

  /// @todo These should be one-liners
  ofstream file("test.aida");
  Writer& w1 = WriterAIDA::create();
  w1.write(file, h);
  file.close();

  ofstream file2("test.yoda");
  WriterYODA::create().write(file2, h);
  file2.close();


  return EXIT_SUCCESS;
}
