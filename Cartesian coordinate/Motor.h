// Motor.h

#ifndef _MOTOR_h
#define _MOTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

// Used pin
namespace pin {
    //Motor control,
    //MD - Motor Driver.
    constexpr uint8_t PWMleftMD = 10;
    constexpr uint8_t PWMrightMD = 9;
    constexpr uint8_t IN1MD = 5;
    constexpr uint8_t IN2MD = 12;
    constexpr uint8_t IN3MD = 7;
    constexpr uint8_t IN4MD = 8;

    //Encoder's interrupts pins.
    constexpr auto ENCleft = 2;
    constexpr auto ENCright = 3;
}

// Constant robot dimensions in mm.
namespace con {
    constexpr auto WHEELdiam = 42;
    constexpr auto WIDTHbase = 108;
    constexpr auto ENC360pulse = 700;
    constexpr auto BASEcircle = WIDTHbase * PI;
    constexpr auto LENof1Deg = BASEcircle / 360;
    constexpr auto LENof1Pulse = (WHEELdiam * PI) / ENC360pulse;


}

class MotorClass {
    // Move forward by len(mm).
    void forwardmm(uint32_t len);

    // Current PWM;
    uint8_t pwmLeft;
    uint8_t pwmRight;
    uint8_t pwmMIN;
    uint8_t pwmMAX;

    void setPwmLeft(uint8_t pwm);
    void setPwmRight(uint8_t pwm);

    // Current direction.
    boolean dirLeft;
    boolean dirRight;

    void setDirLeft(boolean dir);
    void setDirRight(boolean dir);

    //MD state of direction pins.
    boolean in1State;
    boolean in2State;
    boolean in3State;
    boolean in4State;

    uint32_t lcountTemp = 0;
    uint32_t rcountTemp = 0;

    // Set 0 counters of encoders.
    void cleanEnc();

public:
    // pinMode()'s, attachInterrupt()'s and set all to 0 inside.
    void init();

    // Minimum power for movement. Battery dependent.
    void setPwmMin(uint8_t pwm);

    uint8_t getPwmMin();

    // Maximum power for movement. Battery dependent.
    void setPwmMax(uint8_t pwm);

    uint8_t getPwmMax();

    // Don't use in motion!
    void freeInterruptsPins();
    // Use before start movement after freeInterruptsPins().
    void resumeInterrupts();

    // Set PWM(not speed) for both motors(0-255).
    void setPwm(uint8_t pwmL, uint8_t pwmR);

    uint8_t getPwmLeft();
    uint8_t getPwmRight();

    // Set direction for both motor.
    void setDir(boolean dirL, boolean dirR);

    boolean getDirLeft();
    boolean getDirRight();

    // Turn to input degree.
    void turn(int setAngle);

    // Move forward by len(cm).
    void forward(uint32_t len);

};

#endif

