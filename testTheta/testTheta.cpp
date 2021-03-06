#include "pch.h"
#include <iostream>
#include "PointXY.h"

using std::cout;
using std::cin;

int main()
{
    PointXYClass hPoint(0, 0);
    PointXYClass cPoint = hPoint;
    PointXYClass tPoint = hPoint;
    auto userX = 0;
    auto userY = 0;
    auto dist = 0;
    auto cAngle = 0;
    auto userCh = 'y';
    
    while (userCh != 'n') {
        cout << "\n\nCurrent point is";
        cPoint.print();
        cout << "\nEnter next point\n" << "X:";
        cin >> userX;
        cout << "Y:";
        cin >> userY;
        tPoint.set(userX, userY);
        cout << "Distance from";
        cPoint.print();
        cout << ", to ";
        tPoint.print();
        cout << " = " << getGap(cPoint, tPoint);
        cout << ";\nCurrent angle = " << cAngle;
        cout << "\nTheta = " << getTheta(cPoint, tPoint);
        cout << "\nTheta - cAngle = " << getTheta(cPoint, tPoint) - cAngle;
        cAngle = getTheta(cPoint, tPoint);
        cPoint = tPoint;
        cout << "\nEnter another point?(y/n):";
        cin >> userCh;
    }
    return 0;
}
