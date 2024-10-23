#include "Motor.h"
#include <constant.h>
#include <Arduino.h> // If you're using Arduino functions like analogWrite, digitalWrite

// Constructor: Initialize direction and speed pins
Motor::Motor()
{
    pinMode(PIN_PD6, OUTPUT);
    pinMode(PIN_PD7, OUTPUT);
    pinMode(PIN_PB6, OUTPUT);
    pinMode(PIN_PB7, OUTPUT);
    pinMode(PIN_PG1, OUTPUT);
    pinMode(PIN_PG0, OUTPUT);
}

uint8_t Motor::getAbsSpeed(int speed)
{
    if (speed < 0)
    {
        speed = 0;
    }

    if (speed > 200)
    {
        speed = 200;
    }

    if (speed >= MAX_SPEED)
    {
        speed = MAX_SPEED;
    }

    return map(speed, 0, 200, 0, 255);
    ;
}

// Set motor speed (0-255)
void Motor::leftMotorCommand(int speed, boolean isForwardDirection)
{

    if (isForwardDirection)
    {
        digitalWrite(PIN_PD6, HIGH);
        digitalWrite(PIN_PD7, LOW);
    }
    else
    {
        digitalWrite(PIN_PD6, LOW);
        digitalWrite(PIN_PD7, HIGH);
    }
    // Serial.printf("true speed left: %d\n", getAbsSpeed(speed));
    analogWrite(PIN_PB6, getAbsSpeed(speed));
}

// Set motor direction (1 = forward, 0 = backward)
void Motor::rightMotorCommand(int speed, boolean isForwardDirection)
{
    if (isForwardDirection)
    {
        digitalWrite(PIN_PG1, HIGH);
        digitalWrite(PIN_PG0, LOW);
    }
    else
    {
        digitalWrite(PIN_PG1, LOW);
        digitalWrite(PIN_PG0, HIGH);
    }
    // Serial.printf("true speed right: %d\n", getAbsSpeed(speed));
    analogWrite(PIN_PB7, getAbsSpeed(speed));
}

// Stop the motor
void Motor::stopMotors(void)
{
    leftMotorCommand(0, false);
    rightMotorCommand(0, false);
}

void Motor::stopMotorsWithBreak(bool directionForward, int breakSpeed, int breakTime)
{
    if (breakTime == 0)
    {
        breakTime = 50;
    }
    leftMotorCommand(getAbsSpeed(breakSpeed), !directionForward);
    rightMotorCommand(getAbsSpeed(breakSpeed), !directionForward);
    delay(breakTime);
    stopMotors();
}