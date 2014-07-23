#include "YODA/Utils/BinSearcher.h"
#include "YODA/Utils/MathUtils.h"
#include "YODA/Utils/Formatting.h"
using namespace YODA;
using namespace std;

int main() {

  const vector<double> linedges = linspace(10, 1, 101);
  cout << "Lin edges: "; BOOST_FOREACH(double x, linedges) cout << x << " "; cout << endl;
  YODA::Utils::BinSearcher bs1(linedges);
  #define TEST1(x) MSG(x << " => " << bs1.index(x))
  TEST1(0);
  TEST1(1);
  TEST1(2);
  TEST1(51);
  TEST1(100);
  TEST1(101);
  TEST1(102);

  const vector<double> logedges = logspace(10, 1, 101);
  cout << "Log edges: "; BOOST_FOREACH(double x, logedges) cout << x << " "; cout << endl;
  YODA::Utils::BinSearcher bs2(logedges);
  #define TEST2(x) MSG(x << " => " << bs2.index(x))
  TEST2(0);
  TEST2(1);
  TEST2(2);
  TEST2(51);
  TEST2(100);
  TEST2(101);
  TEST2(102);

  return EXIT_SUCCESS;
}
