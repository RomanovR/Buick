// Motor.cpp
#include "Motor.h"

// Impulse counters.
namespace {
    volatile unsigned int lcount = 0;
    volatile unsigned int rcount = 0;
}

void leftcounter() {
    lcount++;
}
void rightcounter() {
    rcount++;
}

void MotorClass::init() {
    pinMode(pin::PWMleftMD, OUTPUT);
    pinMode(pin::PWMrightMD, OUTPUT);
    pinMode(pin::IN1MD, OUTPUT);
    pinMode(pin::IN2MD, OUTPUT);
    pinMode(pin::IN3MD, OUTPUT);
    pinMode(pin::IN4MD, OUTPUT);

    pinMode(pin::ENCleft, INPUT);
    pinMode(pin::ENCright, INPUT);

    setDir(1, 1);
    setPwm(0, 0);

    attachInterrupt(0, leftcounter, RISING);
    attachInterrupt(1, rightcounter, RISING);
    cleanEnc();
}

void MotorClass::setPwmMin(uint8_t pwm) {
    pwmMIN = pwm;
}

uint8_t MotorClass::getPwmMin() {
    return pwmMIN;
}

void MotorClass::setPwmMax(uint8_t pwm) {
    pwmMAX = pwm;
}

uint8_t MotorClass::getPwmMax() {
    return pwmMAX;
}

void MotorClass::freeInterruptsPins() {
    detachInterrupt(0);
    detachInterrupt(1);
}

void MotorClass::resumeInterrupts() {
    attachInterrupt(0, leftcounter, RISING);
    attachInterrupt(1, rightcounter, RISING);
}

void MotorClass::cleanEnc() {
    lcount = 0;
    rcount = 0;
}

void MotorClass::setPwm(uint8_t pwmL, uint8_t pwmR) {
    setPwmLeft(pwmL);
    setPwmRight(pwmR);
}

uint8_t MotorClass::getPwmLeft() {
    return pwmLeft;
}

uint8_t MotorClass::getPwmRight() {
    return pwmRight;
}

void MotorClass::setPwmLeft(uint8_t pwm) {
    pwmLeft = pwm;
    analogWrite(pin::PWMleftMD, pwmLeft);
}

void MotorClass::setPwmRight(uint8_t pwm) {
    pwmRight = pwm;
    analogWrite(pin::PWMrightMD, pwmRight);
}

void MotorClass::setDir(boolean dirL, boolean dirR) {
    dirLeft = dirL;
    dirRight = dirR;
    setDirLeft(dirLeft);
    setDirRight(dirRight);
}

boolean MotorClass::getDirLeft() {
    return dirLeft;
}

boolean MotorClass::getDirRight() {
    return dirRight;
}

void MotorClass::turn(int setAngle) {
    setPwm(0, 0);
    cleanEnc();
    while (setAngle > 180)
        setAngle -= 2 * 180;
    while (setAngle < -180)
        setAngle += 2 * 180;
    
    // Number of impulses to rotation.
    uint32_t numOfImpulses = (con::LENof1Deg * abs(setAngle)) / con::LENof1Pulse;
    // Turn right.
    if (setAngle >= 0)
        setDir(1, 0);
    //Turn left.
    else
        setDir(0, 1);

    uint8_t tempPWM = pwmMIN;

    while (lcount < numOfImpulses && rcount < numOfImpulses) {
        // Speed-up.
        setPwm(tempPWM, tempPWM);
        if (lcount < (numOfImpulses / 2) && rcount < (numOfImpulses / 2)) {
            if (tempPWM < pwmMAX)
                tempPWM++;
        }
        // Speed-down.
        else {
            if (tempPWM > pwmMIN);
                tempPWM--;
        }
    }
    setPwm(0, 0);
    setDir(1, 1);
    delay(5);
    cleanEnc();
}

void MotorClass::forwardmm(uint32_t len) {
    uint32_t length = len / con::LENof1Pulse;
    setDir(1, 1);
    setPwm(0, 0);
    cleanEnc();

    uint8_t ltempPWM = pwmMIN;
    uint8_t rtempPWM = pwmMIN;
    while (lcount < length && rcount < length) {
        
        // ���� ����� ������ ������.
        if (lcount < rcount) {
            if (rtempPWM > pwmMIN)
                rtempPWM -= 60;
            if (ltempPWM < pwmMAX)
                ltempPWM += 10;
        }
        // ���� ������ ������ ������.
        else if (lcount > rcount) {
            if (rtempPWM < pwmMAX)
                rtempPWM += 10;
            if (ltempPWM > pwmMIN)
                ltempPWM -= 10;
        }
        // ���� ���� �����.
        else {
            //if (rtempPWM < pwmMAX)
               // rtempPWM += 10;
            //if (ltempPWM < pwmMAX)
              //  ltempPWM += 10;
        }
        setPwm(ltempPWM, rtempPWM);
    }
    setDir(0, 0);
    delay(5);
    setPwm(0, 0);
}

void MotorClass::forward(uint32_t len) {
    forwardmm(len * 10);
}


void MotorClass::setDirLeft(boolean dir) {
    if (dir) {
        digitalWrite(pin::IN1MD, 0);
        digitalWrite(pin::IN2MD, 1);
    }
    else {
        digitalWrite(pin::IN1MD, 1);
        digitalWrite(pin::IN2MD, 0);
    }
    delay(5);
}

void MotorClass::setDirRight(boolean dir) {
    if (dir) {
        digitalWrite(pin::IN3MD, 1);
        digitalWrite(pin::IN4MD, 0);
    }
    else {
        digitalWrite(pin::IN3MD, 0);
        digitalWrite(pin::IN4MD, 1);
    }
    delay(5);
}
