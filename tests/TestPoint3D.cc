#include "YODA/Point3D.h"
#include "YODA/Utils/MathUtils.h"

#include <iostream>
#include <sys/time.h>

using namespace std;
using namespace YODA;

int main() {
  cout << "-----------------------------------------------" << endl;
  cout << "Creating a Point3D (empty constructor):" << endl;
  Point3D p1();

  cout << "Creating a Point3D (constructor with (double)values)" << endl;
  Point3D p2(1, 2, 3);
  cout << "Checking if everything was set correctly:" << endl;
  cout << "x = " << p2.x() << ", y = " << p2.y() << ", z = " << p2.z() << endl;

  if (!fuzzyEquals(p2.x(), 1) || !fuzzyEquals(p2.y(), 2) || !fuzzyEquals(p2.z(), 3)){
    cout << "One of the coordinates  was not set correctly!" << endl;
    return -1;
  }
  if (!fuzzyEquals(p2.xErrMinus(), p2.xErrPlus()) ||
      !fuzzyEquals(p2.xErrPlus(), p2.yErrMinus()) ||
      !fuzzyEquals(p2.yErrMinus(), p2.yErrPlus()) ||
      !fuzzyEquals(p2.yErrPlus(), p2.zErrMinus()) ||
      !fuzzyEquals(p2.zErrMinus(), p2.zErrPlus()) ||
      !fuzzyEquals(p2.zErrPlus(), 0)) {
    cout << "Errors were not set correctly!" << endl;
    return -1;
  }

  cout << "Creating a Point3D (constructor with explicit assymetric errors)" << endl;
  Point3D p3(1, 2, 3, 1, 2, 1, 2, 1, 2);

  cout << "Checking if everything was set correctly:" << endl;
  cout << "x = " << p3.x() << ", y = " << p3.y() << ", z = " << p3.z() << endl;

  if (!fuzzyEquals(p3.x(), 1) || !fuzzyEquals(p3.y(), 2) || !fuzzyEquals(p3.z(), 3)){
    cout << "One of the coordinates  was not set correctly!" << endl;
    return -1;
  }
  if (!fuzzyEquals(p3.xErrMinus(), 1) ||
      !fuzzyEquals(p3.xErrPlus(), 2) ||
      !fuzzyEquals(p3.yErrMinus(), 1) ||
      !fuzzyEquals(p3.yErrPlus(), 2) ||
      !fuzzyEquals(p3.zErrMinus(), 1) ||
      !fuzzyEquals(p3.zErrPlus(), 2)) {
    cout << "Errors were not set correctly!" << endl;
    return -1;
  }

  cout << "Creating a Point3D (constructor with assymetric, std::pair errrors)" << endl;
  pair<double, double> err = make_pair(1, 2);
  Point3D p4(1, 2, 3, err, err, err);

  cout << "Checking if everything was set correctly:" << endl;
  cout << "x = " << p4.x() << ", y = " << p4.y() << ", z = " << p4.z() << endl;

  if (!fuzzyEquals(p4.x(), 1) || !fuzzyEquals(p4.y(), 2) || !fuzzyEquals(p4.z(), 3)){
    cout << "One of the coordinates  was not set correctly!" << endl;
    return -1;
  }
  if (!fuzzyEquals(p4.xErrMinus(), 1) ||
      !fuzzyEquals(p4.xErrPlus(), 2) ||
      !fuzzyEquals(p4.yErrMinus(), 1) ||
      !fuzzyEquals(p4.yErrPlus(), 2) ||
      !fuzzyEquals(p4.zErrMinus(), 1) ||
      !fuzzyEquals(p4.zErrPlus(), 2)) {
    cout << "Errors were not set correctly!" << endl;
    return -1;
  }

  cout << "And a quick performance check: " << endl;
  struct timeval startTime;
  struct timeval endTime;

  const size_t num = 100000;
  gettimeofday(&startTime, NULL);
  vector<Point3D> points(num, Point3D(1, 1, 1, 1, 2, 1, 2, 1, 2));
  gettimeofday(&endTime, NULL);
  double tS = (startTime.tv_sec*1000000 + startTime.tv_usec)/(double)1000000;
  double tE = (endTime.tv_sec*1000000 + endTime.tv_usec)/(double)1000000;
  cout << "Time to make 100k Points3D is: " << tE - tS << "s" << endl;


  cout << "-----------------------------------------------" << endl;
  return EXIT_SUCCESS;
}
