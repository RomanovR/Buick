
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
    PointXYClass home(0, 0);
    PointXYClass current = home;
    PointXYClass next = home;

    int prevAngle = 0;
    int nextAngle = 0;
    int len = 0;
    auto gap = 5;
    byte incByte = 0;
}

void report(const PointXYClass &current, const PointXYClass &next, int nextAngle, int len);

void setup() {
    Serial.begin(9600);
    Move.init();
    Move.setPwmMin(0);
    Move.setPwmMax(255);
    home.writeEEPROM(0);
    Serial.println("Setup complete.");
    Serial.println("Press 'h' to help;");
}

void loop() {
    if (Serial.available() > 0) {
        incByte = Serial.read();
        Serial.print("key: ");
        Serial.println((char)incByte);
        Serial.flush();
    }

    switch (incByte) {
        case 'h':
        Serial.println("Manual mode:");
        Serial.println("  f - load old current point;");
        Serial.println("  r - save current point;");
        Serial.println("  g - set gap(1-10cm);");
        Serial.println("  w - move forward(cm){x;y+5};");
        Serial.println("  a - turn left(cm){x-5;y};");
        Serial.println("  s - move backward(cm)+{x;y-5};");
        Serial.println("  d - turn right(cm){x+5;y};");
        Serial.println("  q - go home(cm){x0;y0};");
        break;
        case '1':
        current.addXY(5, 5);
        next.addXY(-5, -5);
        nextAngle = getTheta(current, next) - prevAngle;
        len = getGap(current, next);
        report(current, next, nextAngle, len);
        Move.turn(nextAngle);
        prevAngle = nextAngle;
        Move.forward(len);
        current = next;
        break;
        case 'f':
        current.readEEPROM(0);
        Serial.print("Current point updated from EEPROM.");
        current.print();
        break;
        case 'r':
        current.writeEEPROM(0);
        Serial.print("Successfully saved current point.");
        break;
        case 'g':
        Serial.print("Edit gap ");
        Serial.print(gap);
        Serial.println("cm +/- (max 10cm, min 1cm)");
        break;
        case '-':
        if (gap > 0) {
            gap--;
            Serial.print("New gap ");
            Serial.print(gap);
            Serial.println(";");
        }
        else
            Serial.println("Gap 1cm;");
        break;
        case '+':
        if (gap < 11) {
            gap++;
            Serial.print("New gap ");
            Serial.print(gap);
            Serial.println(";");
        }
        else
            Serial.println("Gap 10cm;");
        break;
        case 'w':
        next = current;
        next.addXY(0, gap);
        nextAngle = getTheta(current, next) - prevAngle;
        len = getGap(current, next);
        report(current, next, nextAngle, len);
        Move.turn(nextAngle);
        prevAngle = nextAngle;
        Move.forward(len);
        current = next;
        break;
        case 'a':
        next = current;
        next.addXY(-gap, 0);
        nextAngle = getTheta(current, next) - prevAngle;
        len = getGap(current, next);

        report(current, next, nextAngle, len);

        Move.turn(nextAngle);
        prevAngle = nextAngle;
        Move.forward(len);
        current = next;
        break;
        case 's':
        next = current;
        next.addXY(0, -gap);
        nextAngle = getTheta(current, next) - prevAngle;
        len = getGap(current, next);

        report(current, next, nextAngle, len);

        Move.turn(nextAngle);
        prevAngle = nextAngle;
        Move.forward(len);
        current = next;
        break;
        case 'd':
        next = current;
        next.addXY(gap, 0);
        nextAngle = getTheta(current, next) - prevAngle;
        len = getGap(current, next);

        report(current, next, nextAngle, len);

        Move.turn(nextAngle);
        prevAngle = nextAngle;
        Move.forward(len);
        current = next;
        break;
        case 'q':
        next = home;
        nextAngle = getTheta(current, next) - prevAngle;
        len = getGap(current, next);

        report(current, next, nextAngle, len);

        Move.turn(nextAngle);
        prevAngle = nextAngle;
        Move.forward(len);
        current = home;
        default:
        break;
    }
    incByte = 0;
}

// Вывод отчёта.
void report(const PointXYClass &current, const PointXYClass &next, int nextAngle, int len) {
    Serial.print("Current point ");
    current.print();
    Serial.println(";");
    Serial.print("Next point ");
    next.print();
    Serial.println(";");
    Serial.print("Theta: ");
    Serial.print(nextAngle);
    Serial.println(";");
    Serial.print("Distance: ");
    Serial.print(len);
    Serial.println(";");
}