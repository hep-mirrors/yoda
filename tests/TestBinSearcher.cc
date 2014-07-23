#include "YODA/Utils/BinSearcher.h"
#include "YODA/Utils/MathUtils.h"
#include "YODA/Utils/Formatting.h"

int main() {

  YODA::Utils::BinSearcher bs1(YODA::linspace(10, 1.0, 105));
  MSG(bs1.index(37.2));
  MSG(bs1.index(-1.2));
  MSG(bs1.index(105));
  MSG(bs1.index(106));

  YODA::Utils::BinSearcher bs2(YODA::logspace(10, 1.0, 105));
  MSG(bs2.index(37.2));
  MSG(bs2.index(-1.2));
  MSG(bs2.index(105));
  MSG(bs2.index(106));

  return EXIT_SUCCESS;
}
