#include "YODA/Histo2D.h"

#include <iostream>
#include <sys/time.h>

using namespace YODA;
using namespace std;

int main() {
  /// This is a bin removal tester
  cout << "-----------------------------" << endl;

  /// Firstly, let's create some bins:
  Histo2D h(100, 0, 100, 100, 0, 100);

  /// Let's try to remove the one that surely exists:
  h.eraseBin(0);

  cout << "Number of bins: " << h.numBins() << endl;
  if (h.numBins() != 9999) {
   cout << "The bin was not correctly removed, or the computation of numBins is broken!" << endl;
   return -1;
  }

  int index = h.fill(0,0,1);
  cout << index << endl;
  if (index != -1) {
    cout << "A bin wasn't correctly removed!" << endl;
    return -1;
  }

  index = h.fill(1,0,1);
  cout << index << endl;
  if (index == -1) {
    cout << "Something went very wrong during removal!" << endl;
    return -1;
  }


  /// Now, some timing
  struct timeval startTime;
  struct timeval endTime;
  gettimeofday(&startTime, NULL);
  for (size_t i = 0; i < 1000; ++i) h.eraseBin(0);
  gettimeofday(&endTime, NULL);
  cout << "After removing 1k bins there is " << h.numBins() << " bins left." << endl;

  double tS = (startTime.tv_sec*1000000 + startTime.tv_usec)/(double)1000000;
  double tE = (endTime.tv_sec*1000000 + endTime.tv_usec)/(double)1000000;

  cout << "It takes " << tE - tS << " s to remove 1k bins." << endl;

  cout << "-------------------------" << endl;
  return EXIT_SUCCESS;
}
