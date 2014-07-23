#include "YODA/Utils/BinSearcher.h"
#include "YODA/Utils/MathUtils.h"
#include "YODA/Utils/Formatting.h"
using namespace YODA;
using namespace std;

int main() {

  int rtn = EXIT_SUCCESS;

  #define TESTBS(bs, x, iref) \
    MSG(x << " => " << bs.index(x) << ": " << boolalpha << (bs.index(x) == iref)); \
    if (bs.index(x) != iref) rtn = 1

  const vector<double> linedges = linspace(10, 1, 101);
  cout << "Lin edges: "; BOOST_FOREACH(double x, linedges) cout << x << " "; cout << endl;
  YODA::Utils::BinSearcher bs1(linedges);
  TESTBS(bs1, 0, 0);
  TESTBS(bs1, 1-1e-5, 0);
  TESTBS(bs1, 1, 1);
  TESTBS(bs1, 2, 1);
  TESTBS(bs1, 51-1e-5, 5);
  TESTBS(bs1, 51, 6);
  TESTBS(bs1, 100, 10);
  TESTBS(bs1, 101-1e-5, 10);
  TESTBS(bs1, 101, 11);
  TESTBS(bs1, 102, 11);

  const vector<double> logedges = logspace(10, 1, 101);
  cout << "Log edges: "; BOOST_FOREACH(double x, logedges) cout << x << " "; cout << endl;
  YODA::Utils::BinSearcher bs2(logedges);
  TESTBS(bs2, 0, 0);
  TESTBS(bs2, 1, 1);
  TESTBS(bs2, 2, 2);
  TESTBS(bs2, 100, 10);
  TESTBS(bs2, 101, 11);
  TESTBS(bs2, 102, 11);

  return rtn;
}
