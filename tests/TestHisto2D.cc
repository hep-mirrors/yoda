#include "YODA/Histo2D.h"
#include <cmath>
#include <iostream>
#include <unistd.h>
#include <ctime>

using namespace std;
using namespace YODA;

void printStats(Histo2D& h){
    cout << "LowEdgeX = " << h.lowEdgeX() << " HighEdgeX = " << h.highEdgeX() << endl;
    cout << "LowEdgeY = " << h.lowEdgeY() << " HighEdgeY = " << h.highEdgeY() << endl;

    cout << "Sum of weights is " << h.sumW(true) << ", squared: " << h.sumW2(true) << endl;
//    cout << "Means: " << h.xMean(true) << " " << h.yMean(true) << endl;
//    cout << "Variance: " << h.xVariance(true) << " " << h.yVariance(true) << endl;
//    cout << "StdDevs: " << h.xStdDev(true) << " " << h.yStdDev(true) << endl;
}

int main() {

    Histo2D h(100, 0, 100, 110, 0, 100);
    printStats(h);
    
    time_t start = time(NULL);
    for (int i=0; i < 2000000; i++) h.fill(12.0123, 12.213, 1);
    cout << "Time taken: " << time(NULL) - start << endl;

    printStats(h);
    cout << h.numBinsTotal() << endl;   
//    HistoBin2D rightBin = h.bin(10001);
//    HistoBin2D errBin = h.bin(10000000);

    return EXIT_SUCCESS;
}
