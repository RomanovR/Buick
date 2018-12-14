// PointXY.h
#ifndef _POINTXY_h
#define _POINTXY_h



class PointXYClass
{
    // cm.
    int _x;
    int _y;

 public:
    PointXYClass(int x, int y);
    PointXYClass& operator=(const PointXYClass &orig);
    friend PointXYClass operator+(const PointXYClass &first, const PointXYClass &second);
    void setX(int x);
    int getX();
    void setY(int y);
    int getY();
    void set(int x, int y);
    void print()const;
    void addXY(int x, int y);
    friend int getTheta(PointXYClass & from, PointXYClass & to);
    friend unsigned int getGap(PointXYClass & from, PointXYClass & to);
};


#endif

