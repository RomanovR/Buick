// PointXY.cpp
#pragma once
#include "pch.h"
#include "PointXY.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
using std::cout;

PointXYClass::PointXYClass(int x, int y) {
    _x = x;
    _y = y;
}

PointXYClass & PointXYClass::operator=(const PointXYClass & orig) {
    if (this == &orig)
        return *this;
        
    setX(orig._x);
    setY(orig._y);
    return *this;
}

void PointXYClass::setX(int x) {
    _x = x;
}

int PointXYClass::getX() {
    return _x;
}

void PointXYClass::setY(int y) {
    _y = y;
}

int PointXYClass::getY() {
    return _y;
}

void PointXYClass::set(int x, int y) {
    _x = x;
    _y = y;
}



void PointXYClass::print() const{
    cout << "{" << _x << "; " << _y << "}";
}

void PointXYClass::addXY(int x, int y) {
    _x += x;
    _y += y;
}

PointXYClass operator+(const PointXYClass & first, const PointXYClass & second) {
    return PointXYClass(first._x + second._x, first._y + second._y);
}

int getTheta(PointXYClass & from, PointXYClass & to) {
    int dX = to._x - from._x;   
    int dY = to._y - from._y;

    if (dX >= 0 && dY >= 0)
        return asin(abs(dX) / sqrt(pow(dX, 2) + pow(dY, 2))) * 180 / M_PI;
    if (dX >= 0 && dY <= 0)
        return asin(abs(dY) / sqrt(pow(dX, 2) + pow(dY, 2))) * 180 / M_PI + 90;
    if (dX <= 0 && dY <= 0)
        return asin(abs(dX) / sqrt(pow(dX, 2) + pow(dY, 2))) * 180 / M_PI + 90 * 2;
    else
        return asin(abs(dY) / sqrt(pow(dX, 2) + pow(dY, 2))) * 180 / M_PI + 90 * 3;
}

/*
int getTheta(PointXYClass & from, PointXYClass & to) {
    int dX = to._x - from._x;
    int dY = to._y - from._y;

    if (dX >= 0 && dY >= 0)
        return atan2((dY), (dX)) * 180 / M_PI;
    if (dX >= 0 && dY <= 0)
        return atan2((dY), (dX)) * 180 / M_PI + 90;
    if (dX <= 0 && dY <= 0)
        return atan2((dY), (dX)) * 180 / M_PI + 90 * 2;
    else
        return atan2((dY), (dX)) * 180 / M_PI + 90 * 3;
}
*/
unsigned int getGap(PointXYClass & from, PointXYClass & to) {
    int dX = to._x - from._x;
    int dY = to._y - from._y;
    return (unsigned int)sqrt(pow(dX, 2) + pow(dY, 2));
}

