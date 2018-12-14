#include "PointXY.h"
#include "Motor.h"


/*
Cartesian.ino
Программа предназначена для управления роботом
с дифференциальным приводом в декартовой системе
координат, для обратной связи используются инкрементальные
магнитные энкодеры(возможно использование в двухфазном режиме).
В прототипе используется отладочная плата
Arduino Nano V3 c МК ATmega328P
Использованы моторы со встроенными энкодерами:
CHIHAI MOTOR N20 DC6V 90RPM
Romanov R.
*/

/*
В дальнейшем все расчёты опираются на следующие данные и проводятся в миллиметрах:
- диаметр колёс 42мм,
- расстояние от центра до центра колёс 180мм,
- полный оборот колеса даёт приблизительно(люфт редуктора) 700 импульсов.
*/

// Global var-s in motor.ino.
namespace {
    //Head servo PWM pin
    constexpr auto SERVO = 11;
    MotorClass Move;
    PointXYClass hPoint(0, 0);
    PointXYClass cPoint = hPoint;
    PointXYClass tPoint = hPoint;

    int cAngle = 0;
    auto gap = 5;
    byte incByte = 0;
}

void report(const PointXYClass &cPoint, const PointXYClass &tPoint);

void setup() {
    Serial.begin(9600);
    Move.init();
    Move.setPwmMin(55);
    Move.setPwmMax(205);
    hPoint.writeEEPROM(0);
    Serial.println("Setup complete.");
    Serial.println("Press 'h' to help;");
    
}

void loop() {
    if (Serial.available() > 0) {
        incByte = Serial.read();
        Serial.println((char)incByte);
        Serial.flush();
    }

    switch (incByte) {
        case 'h':
        Serial.println("Manual mode:");
        Serial.println("  f - load old cPoint point(default {0;0});");
        Serial.println("  r - save cPoint point;");
        Serial.println("  g - set gap(1-10cm);");
        Serial.println("  w - up(cm){x;y+5};");
        Serial.println("  a - left(cm){x-5;y};");
        Serial.println("  s - down(cm)+{x;y-5};");
        Serial.println("  d - right(cm){x+5;y};");
        Serial.println("  e - set current point as home point;");
        Serial.println("  q - go home {0;0};");
        break;
        case 'f':
        cPoint.readEEPROM(0);
        Serial.print("Current point updated from EEPROM.");
        cPoint.print();
        break;
        case 'r':
        cPoint.writeEEPROM(0);
        Serial.print("Successfully saved cPoint point.");
        break;
        case 'g':
        Serial.print("Edit gap ");
        Serial.print(gap);
        Serial.println("cm +/- (max 10cm, min 1cm)");
        break;
        case '-':
        if (gap > 1) {
            gap--;
            Serial.print("New gap ");
            Serial.print(gap);
            Serial.println(";");
        }
        else
            Serial.println("Gap 1cm;");
        break;
        case '+':
        if (gap < 10) {
            gap++;
            Serial.print("New gap ");
            Serial.print(gap);
            Serial.println(";");
        }
        else
            Serial.println("Gap 10cm;");
        break;
        case 'w':
        tPoint.addXY(0, gap);
        Move.turn(getTheta(cPoint, tPoint) - cAngle);
        cAngle = getTheta(cPoint, tPoint);
        Move.forward(getGap(cPoint, tPoint));
        report(cPoint, tPoint);
        cPoint = tPoint;
        break;
        case 'a':
        tPoint.addXY(-gap, 0);
        Move.turn(getTheta(cPoint, tPoint) - cAngle);
        cAngle = getTheta(cPoint, tPoint);
        Move.forward(getGap(cPoint, tPoint));
        report(cPoint, tPoint);
        cPoint = tPoint;
        break;
        case 's':
        tPoint.addXY(0, -gap);
        Move.turn(getTheta(cPoint, tPoint) - cAngle);
        cAngle = getTheta(cPoint, tPoint);
        Move.forward(getGap(cPoint, tPoint));
        report(cPoint, tPoint);
        cPoint = tPoint;
        break;
        case 'd':
        tPoint.addXY(gap, 0);
        Move.turn(getTheta(cPoint, tPoint) - cAngle);
        cAngle = getTheta(cPoint, tPoint);
        Move.forward(getGap(cPoint, tPoint));
        report(cPoint, tPoint);
        cPoint = tPoint;
        break;
        case 'q':
        tPoint = hPoint;
        Move.turn(getTheta(cPoint, tPoint) - cAngle);
        cAngle = getTheta(cPoint, tPoint);
        Move.forward(getGap(cPoint, tPoint));
        report(cPoint, tPoint);
        cPoint = tPoint;
        break;
        case 'e':
        cPoint.setXY(0, 0);
        tPoint = cPoint;
        cAngle = 0;
        Serial.println("Set current point as home point.");
        break;
        default:
        break;
    }
    incByte = 0;
}

// Вывод отчёта.
void report(const PointXYClass &cPoint, const PointXYClass &tPoint) {
    Serial.print("Current point ");
    cPoint.print();
    Serial.println(";");
    Serial.print("Next point ");
    tPoint.print();
    Serial.println(";");
}