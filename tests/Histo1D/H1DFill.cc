#include "YODA/Histo1D.h"
#include "YODA/Utils/Formatting.h"

using namespace YODA;
using namespace std;

int main() {
  MSG_BLUE("Testing Histo1D filling: ");

  MSG_(PAD(70) << "Setting up a 100 bin histo from 0-100: ");
  Histo1D h(100, 0, 100);
  MSG_GREEN("PASS");

  MSG_(PAD(70) << "Trying to fill the sample histogram: ");
  h.fill(0,2);
  MSG_GREEN("PASS");

  MSG_(PAD(70) << "Checking sumW: ");
  if (h.sumW() != 2) {
    MSG_RED("FAIL");
    return -1;
  }
  MSG_GREEN("PASS");

  MSG_(PAD(70) << "Checking sumW2: ");
  if (h.sumW2() != 4) {
    MSG_RED("FAIL");
    return -1;
  }
  MSG_GREEN("PASS");

  MSG_(PAD(70) << "Trying to fill again: ");
  h.fill(10, 2);
  MSG_GREEN("PASS");

  MSG_(PAD(70) << "Checking mean: ");
  if (!fuzzyEquals(5, h.mean(false))) {
    MSG_RED("FAIL");
    return -1;
  }
  MSG_GREEN("PASS");

  MSG_(PAD(70) << "Checking variance: ");
  if (!fuzzyEquals(25, h.variance(false))) {
    MSG_RED("FAIL");
    return -1;
  }
  MSG_GREEN("PASS");

  MSG_(PAD(70) << "Checking standard deviation: ");
  if (!fuzzyEquals(5, h.stdDev(false))) {
    MSG_RED("FAIL");
    return -1;
  }
  MSG_GREEN("PASS");

  MSG_(PAD(70) << "Trying to fill the underflow: ");
  h.fill(-10,1);
  MSG_GREEN("PASS");

  MSG_(PAD(70) << "Checking if stats were set correctly: ");
  if (!fuzzyEquals(h.underflow().mean(), -10)) {
    MSG_RED("FAIL");
    return -1;
  }
  MSG_GREEN("PASS");

  MSG_(PAD(70) << "Trying to fill the overflow: ");
  h.fill(110,1);
  MSG_GREEN("PASS");

  MSG_(PAD(70) << "Checking if stats were set correctly: ");
  if (!fuzzyEquals(h.overflow().mean(), 110)) {
    MSG_RED("FAIL");
    return -1;
  }
  MSG_GREEN("PASS");

  return EXIT_SUCCESS;
}
