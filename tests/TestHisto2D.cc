#include "YODA/Histo2D.h"
#include "YODA/Scatter3D.h"

#include <cmath>
#include <iostream>
#include <unistd.h>
#include <sys/time.h>

using namespace std;
using namespace YODA;


// A stats printing function.
//
// A very unpolished stats printing function. It prints some stats, sometimes
// looking at the full distribution and sometimes not. A better version is not
// too high on the priority list right now!
void printStats(Histo2D& h, bool full=false){
    cout << "-----------------------------" << endl;
    cout << "LowEdgeX = " << h.lowEdgeX() << " HighEdgeX = " << h.highEdgeX() << endl;
    cout << "LowEdgeY = " << h.lowEdgeY() << " HighEdgeY = " << h.highEdgeY() << endl;

    cout << "Sum of weights is " << h.sumW(true) << ", squared: " << h.sumW2(true) << endl;

    if (full) {
        cout << "Means: " << h.xMean(true) << " " << h.yMean(true) << endl;
        cout << "Variance: " << h.xVariance(true) << " " << h.yVariance(true) << endl;
        cout << "StdDevs: " << h.xStdDev(true) << " " << h.yStdDev(true) << endl;
    }
    cout << "-----------------------------" << endl;
}



