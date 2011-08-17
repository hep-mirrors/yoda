#include "YODA/Histo2D.h"
#include "YODA/Scatter3D.h"
#include "YODA/Point3D.h"

#include <iostream>
#include <sys/time.h>

using namespace std;
using namespace YODA;

int main() {
  struct timeval startTime;
  struct timeval endTime;
  cout << "-----------------------------------" << endl;

  cout << "Constructing a sample Scatter3D (empty constructor method):" << endl;
  Scatter3D s1("/path", "title");
  
  cout << "Constructing a sample Scatter3D (from existing points):" << endl;
  
  Point3D p(1, 1, 1, 1, 1, 1);
  vector<Point3D> points;
  points.resize(100000);

  for(size_t i=0; i < points.size(); ++i) points[i] = p;
  gettimeofday(&startTime, NULL);
  Scatter3D s2(points, "/path", "title");
  gettimeofday(&endTime, NULL);
  
  double tS = (startTime.tv_sec*1000000 + startTime.tv_usec)/(double)1000000;
  double tE = (endTime.tv_sec*1000000 + endTime.tv_usec)/(double)1000000;

  cout << "Time to construct a Scatter3D out of 100k Points: " << tE - tS << "s" << endl;


  cout << "Constructing a sample Scatter3D (from point-vectors):" << endl;
  vector<double>                coords;
  vector<pair<double, double> > ecoords;
  coords.resize(100000);
  ecoords.resize(100000);

  for(size_t i=0; i < coords.size(); i++) {
    coords[i] = 1;
    ecoords[i] = make_pair(1, 0.5);
  }
  cout << "Done!" << endl;
  gettimeofday(&startTime, NULL);
  Scatter3D s3(coords, coords, coords, ecoords, ecoords, ecoords, "/path", "title");
  gettimeofday(&endTime, NULL);
  
  tS = (startTime.tv_sec*1000000 + startTime.tv_usec)/(double)1000000;
  tE = (endTime.tv_sec*1000000 + endTime.tv_usec)/(double)1000000;

  cout << "Time to construct a Scatter3D out of 100k Points: " << tE - tS << "s" << endl;

  cout << "Checking if copy constructor works:" << endl;

  gettimeofday(&startTime, NULL);
  Scatter3D s4(s3);
  gettimeofday(&endTime, NULL);
  
  tS = (startTime.tv_sec*1000000 + startTime.tv_usec)/(double)1000000;
  tE = (endTime.tv_sec*1000000 + endTime.tv_usec)/(double)1000000;

  cout << "Time to copy over a 1M Points Scatter3D: " << tE - tS << "s" << endl;

  if(s4 != s3) {
    cout << "Something is wrong in the way the copy constructor works!" << endl;
    return -1;
  }

  cout << "Testing resetting: ";
  s4.reset();
  if(s4.numPoints() != 0) {
    cout << "Resetting didn't work as it should!" << endl;
    return -1;
  }
  cout << "\033[1;31m" << "Fine" << "\033[0m\n";
  cout << "-----------------------------------" << endl;
  return EXIT_SUCCESS;
}
