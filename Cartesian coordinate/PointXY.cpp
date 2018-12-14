// PointXY.cpp
#include "PointXY.h"
#include <math.h>
#include <EEPROM.h>

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

void PointXYClass::print() const{
    Serial.print("{");
    Serial.print(_x);
    Serial.print("; ");
    Serial.print(_y);
    Serial.println("}");
}

void PointXYClass::writeEEPROM(unsigned int firstAddr) {
    byte raw[2];
    (int&)raw = _x;
    for (byte i = 0; i < 2; i++) EEPROM.write(firstAddr + i, raw[i]);

    (int&)raw = _y;
    for (byte i = 0; i < 2; i++) EEPROM.write(firstAddr + 2 + i, raw[i]);
}

void PointXYClass::readEEPROM(unsigned int firstAddr) {
    byte raw[2];
    for (byte i = 0; i < 2; i++) raw[i] = EEPROM.read(firstAddr + i);
    _x = (int&)raw;
    
    for (byte i = 0; i < 2; i++) raw[i] = EEPROM.read(firstAddr + 2 + i);
    _y = (int&)raw;
}

void PointXYClass::addXY(int x, int y) {
    _x += x;
    _y += y;
}

void PointXYClass::setXY(int x, int y) {
    _x = x;
    _y = y;
}

PointXYClass operator+(const PointXYClass & first, const PointXYClass & second) {
    return PointXYClass(first._x + second._x, first._y + second._y);
}

int getTheta(PointXYClass & from, PointXYClass & to) {
    int dX = to._x - from._x;
    int dY = to._y - from._y;

    if (dX >= 0 && dY >= 0)
        return asin(abs(dX) / sqrt(pow(dX, 2) + pow(dY, 2))) * 180 / PI;
    if (dX >= 0 && dY <= 0)
        return asin(abs(dY) / sqrt(pow(dX, 2) + pow(dY, 2))) * 180 / PI + 90;
    if (dX <= 0 && dY <= 0)
        return asin(abs(dX) / sqrt(pow(dX, 2) + pow(dY, 2))) * 180 / PI + 90 * 2;
    else
        return asin(abs(dY) / sqrt(pow(dX, 2) + pow(dY, 2))) * 180 / PI + 90 * 3;
}

/*
int getTheta(PointXYClass & from, PointXYClass & to) {
    int dX = to._x - from._x;
    int dY = to._y - from._y;

    if (dX >= 0 && dY >= 0)
        return atan2((dY), (dX)) * 180 / PI;
    if (dX >= 0 && dY <= 0)
        return atan2((dY), (dX)) * 180 / PI + 90;
    if (dX <= 0 && dY <= 0)
        return atan2((dY), (dX)) * 180 / PI + 90 * 2;
    else
        return atan2((dY), (dX)) * 180 / PI + 90 * 3;
}
*/
unsigned int getGap(PointXYClass & from, PointXYClass & to) {
    int dX = to._x - from._x;
    int dY = to._y - from._y;
    return (unsigned int)sqrt(pow(dX, 2) + pow(dY, 2));
}

