#include "YODA/Histo2D.h"
#include <cmath>
#include <iostream>
#include <unistd.h>

using namespace std;
using namespace YODA;

void printStats(Histo2D& h){
    cout << "LowEdgeX = " << h.lowEdgeX() << " HighEdgeX = " << h.highEdgeX() << endl;
    cout << "LowEdgeY = " << h.lowEdgeY() << " HighEdgeY = " << h.highEdgeY() << endl;

    cout << "Sum of weights is " << h.sumW(true) << ", squared: " << h.sumW2(true) << endl;
    cout << "Means: " << h.xMean(true) << " " << h.yMean(true) << endl;
//    cout << "Variance: " << h.xVariance(true) << " " << h.yVariance(true) << endl;
//    cout << "StdDevs: " << h.xStdDev(true) << " " << h.yStdDev(true) << endl;
}

int main() {

    Histo2D h(1000, 0, 100, 1000, 0, 100);
    printStats(h);

    h.fill(1, 1, 1);
    printStats(h);
    
    HistoBin2D rightBin = h.bin(10001);
    HistoBin2D errBin = h.bin(10000000);

    return EXIT_SUCCESS;
}
