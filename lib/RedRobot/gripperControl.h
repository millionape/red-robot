#include <Arduino.h>
#include <SoftwareServo.h>
#include <constant.h>

#ifndef GRIPPER_H
#define GRIPPER_H
class GripperControl
{
private:
    SoftwareServo servoLeftInstance;
    SoftwareServo servoRightInstance;
    SoftwareServo servoVerticalInstance;

public:
    GripperControl();
    void up();
    void down();
    void grab();
    void release();
    void refresh();
};

#endif