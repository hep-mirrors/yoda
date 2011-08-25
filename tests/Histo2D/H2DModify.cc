#include "YODA/Histo2D.h"
#include "YODA/Profile1D.h"
#include "YODA/Scatter3D.h"

#include <cmath>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <sys/time.h>

using namespace std;
using namespace YODA;

int main() {

    // Creating a histogram and measuring the time it takes to do it.
    struct timeval startTime;
    struct timeval endTime;
    Histo2D h(200, 0, 100, 200, 0, 100);

    double tS; 
    double tE;
    cout << "Adding bins                              ";
    size_t before = h.numBins();
    h.addBin(0.1, 0.1, 0.2, 0.2);
    h.addBin(110, 0, 200, 12.100);
    h.addBin(16.0, 200, 17.0, 300);
    
    /// @todo Add position checking
    if(before != h.numBins() - 3) {
      cout << "FAIL" << endl;
      return -1;
    }
    cout << "PASS" << endl;

    cout << "Checking if filling does modify the grid:";
    size_t beforeAdd = h.numBins();
    h.fill(10000, 34234, 1);
    if (h.numBins() != beforeAdd) {
        cout << "FAIL" << endl;
        return -1;
    }
    cout << "PASS" << endl;

    // Checking if everything is still working as desired.
    // It is a good thing to do, as at some earlier stages
    // in development adding a broken bin destroyed the cache of edges.
    cout << "Is the origin working correctly?         ";
    int originIndex = h.fill(0.0, 0.0, 1);
    if (originIndex == -1) {
        cout << "FAIL" << endl;
        return -1;
    }
    cout << "PASS" << endl;

    // Now, adding a square that is in a non-overlapping location:
    cout << "Does bin addition work?                  ";
    beforeAdd = h.numBins();
    h.addBin(150, 150, 200, 200);
    if (beforeAdd == h.numBinsTotal()) {
        cout << "FAIL" << endl;
        return -1;
    }
    cout << "PASS" << endl;

    // Checking if a broken bin triggers _dropEdge().
    cout << "Does a broken bin trigger _droEdge()?    ";
    beforeAdd = h.numBinsTotal();
    h.addBin(0.756, 0.213, 12.1234, 23);
    if (beforeAdd != h.numBinsTotal()) {
        cout << "FAIL" << endl;
        return -1;
    }
    cout << "PASS" << endl;

    // A check testing mostly _fixOrientation()
    cout << "How about adding a bin in 3rd quadrant?  ";
    beforeAdd = h.numBinsTotal();
    h.addBin(-12, -12, -1, -1);
    if (beforeAdd == h.numBinsTotal()) {
        cout << "FAIL" << endl;
        return -1;
    }
    cout << "PASS" << endl;
    
    cout << "Trying to fill the newly created bin:    ";
    int fillTest = h.fill(-3, -3, 1);
    if (fillTest == -1 || fillTest != (int)h.numBins() - 1) {
        cout << "FAIL" << endl;
        return -1;
    }
    cout << "PASS" << endl;

    cout << "Scaling test:                            ";
    gettimeofday(&startTime, NULL);
    int xMin, xMax, yMin, yMax;
    xMin = h.xMin(); xMax = h.xMax(); yMin = h.yMin(); yMax = h.yMax();
    h.scaleXY(2.0, 3.0);
    if(h.xMin() != 2*xMin || h.xMax() != 2*xMax || 
       h.yMin() != 3*yMin || h.yMax() != 3*yMax) {
      cout << "FAIL" << endl;
      return -1;
    }
    gettimeofday(&endTime, NULL);

    fillTest = h.fill(180, 180, 1);
    if (fillTest == -1) {
        cout << "FAIL" << endl;
        return -1;
    }
    tS = (startTime.tv_sec*1000000 + startTime.tv_usec)/(double)1000000;
    tE = (endTime.tv_sec*1000000 + endTime.tv_usec)/(double)1000000;
    cout << "PASS (" << tE - tS << "s)" << endl;
    
    cout << "Testing cuts:                            ";
    Histo2D sampleHisto(50, 0, 100, 39, 0, 10);
    sampleHisto.fill(0,0,123121);
    if (!fuzzyEquals(sampleHisto.sumW(false), (double)123121)) {
        cout << "FAIL" << endl;
        return -1;
    }
    if (!fuzzyEquals(sampleHisto.sumW2(false), 1.51588e+10)) {
        cout << "FAIL" << endl; 
        return -1;
    }
    cout << "PASS" << endl;
    
    cout << "Testing cutterX:                         ";
    Histo1D atY(sampleHisto.cutterX(0));
    if (!fuzzyEquals(atY.sumW(false), sampleHisto.sumW(false))){
        cout << "FAIL" << endl;
        return -1;
    }
    cout << "PASS" << endl;
    
    cout << "Another cutterX test:                    ";
    Histo1D atX2(sampleHisto.cutterX(2));
    if (!fuzzyEquals(atX2.sumW(false), 0)){
        cout << "FAIL" << endl;
        return -1;
    }
    cout << "PASS" << endl;
    
    cout << "Testing cutterY:                         ";
    Histo1D atX(sampleHisto.cutterY(0));
    if (!fuzzyEquals(atX.sumW(false), sampleHisto.sumW(false))){
        cout << "FAIL" << endl;
        return -1;
    }
    cout << "PASS" << endl;

    cout << "Testing bin merger:                      "; 
    gettimeofday(&startTime, NULL);
    h.mergeBins(0, 20000);
    gettimeofday(&endTime, NULL);
    tS = (startTime.tv_sec*1000000 + startTime.tv_usec)/(double)1000000;
    tE = (endTime.tv_sec*1000000 + endTime.tv_usec)/(double)1000000;
    cout << "PASS (" << tE - tS << ")" << endl;
    
    cout << "Testing if histo2D transforms to Profile:";
    Histo2D test1(100, 0, 100, 100, 0, 100);
    Profile1D test2(100, 0, 100);

    test1.fill(1,1,1);
    test2.fill(1,1,1);
    if(test2 != test1.mkProfileX()){
      cout << "FAIL" << endl;
      return -1;
    }
    cout << "PASS" << endl;
    cout << "A more elaborate test:                   ";
    for(size_t i = 0; i < 100000; ++i){
      size_t x = rand()%100;
      size_t y = rand()%100;
      test1.fill(x,y,1);
      test2.fill(x,y,2);
    }
    if(test2 != test1.mkProfileX()){
      cout << "FAIL" << endl;
      return -1;
    }
    cout << "PASS" << endl;

    return EXIT_SUCCESS;
}
