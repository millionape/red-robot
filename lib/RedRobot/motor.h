#include <Arduino.h>

#ifndef MOTOR_H
#define MOTOR_H

class Motor
{
private:
    //     int speed;      // Speed of the motor (0 to 255)
    //     bool direction; // Direction of the motor (0: forward, 1: reverse)
    //     bool isRunning; // Motor state (true: running, false: stopped)

public:
    // Constructor: Initializes motor with direction and speed control pins
    Motor();

    void leftMotorCommand(int speed, boolean isForwardDirection);

    void rightMotorCommand(int speed, boolean isForwardDirection);

    void stopMotors(void);
    void stopMotorsWithBreak(bool directionForward, int breakSpeed, int breakTime);

    uint8_t getAbsSpeed(int speed);
};

#endif // MOTOR_H