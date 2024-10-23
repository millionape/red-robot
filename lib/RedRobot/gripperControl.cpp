#include <Arduino.h>
#include <gripperControl.h>

GripperControl::GripperControl()
{
    pinMode(PIN_SERVO_VERTICAL, OUTPUT);
    pinMode(PIN_SERVO_LEFT, OUTPUT);
    pinMode(PIN_SERVO_RIGHT, OUTPUT);

    servoLeftInstance.attach(PIN_SERVO_LEFT);
    // servoLeftInstance.setMaximumPulse(SEFVO_MAX_PULSE);
    servoRightInstance.attach(PIN_SERVO_RIGHT);
    // servoRightInstance.setMaximumPulse(SEFVO_MAX_PULSE);
    servoVerticalInstance.attach(PIN_SERVO_VERTICAL);
    // servoVerticalInstance.setMaximumPulse(SEFVO_MAX_PULSE);
}

void GripperControl::up()
{
    servoVerticalInstance.write(SERVO_VERTICAL_UP_VALUE);
}

void GripperControl::down()
{
    for (int i = SERVO_VERTICAL_DOWN_VALUE; i > 0; i--)
    {
        servoVerticalInstance.write(i);
        delay(8);
        servoVerticalInstance.refresh();
    }
}

void GripperControl::grab()
{
    servoLeftInstance.write(SERVO_LEFT_GRAB_VALUE);
    servoRightInstance.write(SERVO_RIGHT_GRAB_VALUE);
    for (int i = 0; i < 10; i++)
    {
        delay(20);
        refresh();
    }
}

void GripperControl::release()
{
    servoLeftInstance.write(SERVO_LEFT_RELEASE_VALUE);
    servoRightInstance.write(SERVO_RIGHT_RELEASE_VALUE);
    for (int i = 0; i < 10; i++)
    {
        delay(20);
        refresh();
    }
}

void GripperControl::refresh()
{
    servoVerticalInstance.refresh();
    servoLeftInstance.refresh();
    servoRightInstance.refresh();
}

/*

Pin mapping

PORT_E_2 => OUT_1 | arduino pin 2
PORT_E_3 => OUT_2 | arduino pin 3 *PWM
PORT_B_0 => OUT_3 | arduino pin 8
PORT_B_2 => OUT_4 | arduino pin 10
PORT_B_3 => OUT_5 | arduino pin 11
PORT_B_4 => OUT_6 | arduino pin 12 *PWM
*/