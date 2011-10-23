#include "YODA/Histo2D.h"
#include "YODA/Scatter3D.h"
#include "YODA/Utils/MathUtils.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/time.h>
using namespace std;
using namespace YODA;

int main() {
  Histo2D h(200, 0, 100, 200, 0, 100);
  
  struct timeval startTime;
  struct timeval endTime;
  gettimeofday(&startTime, NULL);
  cout << "Testing fill operation:                  ";
  for (int i=0; i < 2000; i++) {
      int out = h.fill(16.0123, 12.213, 2);
      if (out == -1) {
          cout << "FAIL" << endl;
          return -1;
      }
  }
  gettimeofday(&endTime, NULL);

  double tS = (startTime.tv_sec*1000000 + startTime.tv_usec)/(double)1000000;
  double tE = (endTime.tv_sec*1000000 + endTime.tv_usec)/(double)1000000;
  if ((tE - tS) > 50.0) {
      cout << "Performance is not sufficient. Probably broken caches?" << endl;
      return -1;
  }
  cout << "PASS (" << tE - tS << "s)" << endl;

  cout << "Now, generating benchmark...             ";
  ofstream file("bench2.dat");

  for(int i = 10; i < 310; i+=10){
    Histo2D temp(i, 0, 100, i, 0, 100);

    gettimeofday(&startTime, NULL);
    for(int j = 0; j < 100000; j++) temp.fill(99, 99, 2);
    gettimeofday(&endTime, NULL);

    tE = (endTime.tv_sec*1000000 + endTime.tv_usec)/(double)1000000; 
    tS = (startTime.tv_sec*1000000 + startTime.tv_usec)/(double)1000000;
    file << i*i << " " << tE-tS << " ";
    cout << tE-tS << endl;

    Histo2D temp2(logspace(1, 100, i), logspace(1, 100, i));
    gettimeofday(&startTime, NULL);
    for(int j = 0; j < 100000; j++) temp2.fill(99, 99, 2);
    gettimeofday(&endTime, NULL);

    tE = (endTime.tv_sec*1000000 + endTime.tv_usec)/(double)1000000; 
    tS = (startTime.tv_sec*1000000 + startTime.tv_usec)/(double)1000000;
    file << tE-tS << " ";
    cout << tE-tS << endl;

    temp.addBin(200, 0, 300, 300);
    gettimeofday(&startTime, NULL);
    for(int j = 0; j < 1000; j++) temp.fill(99, 99, 2);
    gettimeofday(&endTime, NULL);

    tE = (endTime.tv_sec*1000000 + endTime.tv_usec)/(double)1000000; 
    tS = (startTime.tv_sec*1000000 + startTime.tv_usec)/(double)1000000;
    file << tE-tS << endl;
    cout << tE-tS << endl;
    
    cout << i << endl;
  }
  cout << endl;



  // Testing if fill() function does what it should
  cout << "Does fill() do what it should?           ";
  if(-1 != h.fill(1123123,312312,1)){
    cout << "FAIL" << endl;
    return -1;
  }
  cout << "PASS" << endl;
  
  return EXIT_SUCCESS;
}
