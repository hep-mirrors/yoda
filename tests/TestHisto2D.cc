#include "YODA/Histo2D.h"
#include <cmath>
#include <iostream>
#include <unistd.h>
#include <sys/time.h>

using namespace std;
using namespace YODA;


/// A stats printing function.
/**
 * This is a very, very unpolished version of a stats
 * printing function. It prints some stats sometimest 
 * looking at the full distribution and sometimes not.
 * A better verion is not to high in priority list now.
 */
void printStats(Histo2D& h, bool full=false){
    cout << "-----------------------------" << endl;
    cout << "LowEdgeX = " << h.lowEdgeX() << " HighEdgeX = " << h.highEdgeX() << endl;
    cout << "LowEdgeY = " << h.lowEdgeY() << " HighEdgeY = " << h.highEdgeY() << endl;

    cout << "Sum of weights is " << h.sumW(false) << ", squared: " << h.sumW2(false) << endl;

    if(full) {
        cout << "Means: " << h.xMean(true) << " " << h.yMean(true) << endl;
        cout << "Variance: " << h.xVariance(true) << " " << h.yVariance(true) << endl;
        cout << "StdDevs: " << h.xStdDev(true) << " " << h.yStdDev(true) << endl;
    }
    cout << "-----------------------------" << endl;
}

int main() {
    
    /// Creating a histogram and measuring the time it takes to do it.
    struct timeval startTime;
    struct timeval endTime;
    gettimeofday(&startTime, NULL);
    Histo2D h(200, 0, 100, 200, 0, 100);
    gettimeofday(&endTime, NULL);

    double tS = (startTime.tv_sec*1000000 + startTime.tv_usec)/(double)1000000;
    double tE = (endTime.tv_sec*1000000 + endTime.tv_usec)/(double)1000000;
    cout << "Time to create 40K bins: " << tE - tS << "s" << endl;
    printStats(h);
    
    
    /// Trying to fill a bin.
    gettimeofday(&startTime, NULL);
    for (int i=0; i < 2000000; i++) {
        int out = h.fill(16.0123, 12.213, 10);
        if(out == -1) {
            cout << "I wasn't able to find the bin, something must be incorecct in search algorithm:" << endl;
            return -1;
        }
    }
    gettimeofday(&endTime, NULL);

    tS = (startTime.tv_sec*1000000 + startTime.tv_usec)/(double)1000000;
    tE = (endTime.tv_sec*1000000 + endTime.tv_usec)/(double)1000000;
    cout << "Time taken to fill 2M bins: " << tE - tS << "s" << endl; 
    if((tE - tS) > 50.0) {
        cout << "Performance is not sufficient. Probably broken caches?" << endl;
        return -1;
    }

    printStats(h, true);
    cout << h.numBinsTotal() << endl;   
    
    /// Testing if fill() function does what it should
    unsigned int beforeAdd = h.numBinsTotal();
    cout << "Does a too high bin exist? " << h.fill(10000, 34234, 1) << endl;
    if(h.numBinsTotal() != beforeAdd) {
        cout << "An incorrect bin seems to have been added. The other solution is an error in numBinsTotal()" << endl;
        return -1;
    }

    /// Checking if everything is still working as desired.
    /** 
     * It is actually a good thing to do, as at some earlier stages 
     * in developement adding a broken bin destroyed the cache of edges.
     */
    int originIndex = h.fill(0.0, 0.0, 1);
    cout << "And is the origin working in the right way? " << originIndex << endl;
    if(originIndex == -1) {
        cout << "The origin was not found!" << endl;
        return -1;
    }

    printStats(h, true);
    
    /// Now, adding a square that is in a non-overlapping location:
    beforeAdd = h.numBinsTotal();
    h.addBin(150, 150, 200, 200);
    cout << "Added proprely? " << h.fill(150.1, 150.1, 1) << " Size: " << h.numBinsTotal() << endl;
    if(beforeAdd == h.numBinsTotal()) {
        cout << "A bin that should be added wasn't added." << endl;
        return -1;
    }

    /// Checking if a broken bin triggers _dropEdge().
    beforeAdd = h.numBinsTotal();
    h.addBin(0.756, 0.213, 12.1234, 23);
    cout << "Size: " << h.numBinsTotal() << endl;
    if(beforeAdd != h.numBinsTotal()) {
        cout << "Detection of overlapping edges doesn't work!" << endl;
        return -1;
    }

    int fillTest = h.fill(0.0, 1.0, 1);
    cout << "And a fill sanity check: " << fillTest << endl;
    if(fillTest == -1) {
        cout << "An undefined error with fill had occured." << endl;
        return -1;
    }

    /// A check testing mostly _fixOrientation()
    cout << "Now, how about another quadrant? " << endl;
    beforeAdd = h.numBinsTotal();
    h.addBin(-12, -12, -1, -1);
    if(beforeAdd == h.numBinsTotal()) {
        cout << "A bin that should be added could not be added." << endl;
        return -1;
    }

    fillTest = h.fill(-3, -3, 1);
    cout << "Trying to fill the newly created bin: " << fillTest << endl;
    if(fillTest == -1 || fillTest != (int)h.numBinsTotal() - 1) {
        cout << "Could not fill the bin that should be filled" << endl;
        return -1;
    }
    

    /// And a scaling test.
    printStats(h, true);
    cout << "Scaling: " << endl;

    gettimeofday(&startTime, NULL);
    h.scale(2.0, 3.0);
    gettimeofday(&endTime, NULL);
    tS = (startTime.tv_sec*1000000 + startTime.tv_usec)/(double)1000000;
    tE = (endTime.tv_sec*1000000 + endTime.tv_usec)/(double)1000000;

    cout << "Time to scale: " << tE - tS << "s" << endl;
    printStats(h, true);

    fillTest = h.fill(180, 180, 1);
    cout << "Was everything scaled as it should be? " << fillTest << endl;
    if(fillTest == -1) {
        cout << "Something went wrong while scaling." << endl;
        return -1;
    }


    return EXIT_SUCCESS;
}
