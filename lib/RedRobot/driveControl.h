#include <constant.h>
#include <motor.h>

#ifndef DRIVECONTROL_H
#define DRIVECONTROL_H
class DriveControl
{
private:
    Motor motorInstance;
    uint8_t defaultSpeed;
    bool driveDirection = true; // true: forward, false: backward
    bool pauseLaneKeepDrive = true;
    // pid control
    float Kp = PID_KP;
    float Ki = PID_KI;
    float Kd = PID_KD;
    volatile int positionError = 0;
    volatile int setpoint = 0;
    volatile float integral = 0;
    volatile float derivative = 0;
    volatile int prevError = 0;

    volatile float pidSpeed = 0;

    float getPidSpeed(int sensorError);
    bool doLaneKeepForwardDrive();

public:
    DriveControl();
    void updateSensorError(int sensorError);
    void setPauseLaneKeepDrive(bool isPause);
    void stopDriving(void);
    void setDefaultSpeed(uint8_t speed);
    void resetPID(void);
    void stop_forever(void);
    void custom_brake(int brakeSpeed, int duration);
    void brake(void);
    void stop_and_wait_for_button(void);
    void correct_center(int speed);
    void drive_to_checkpoint(int additionalDriveTime);
    void go_to_the_front(int duration);
    void dash(int dashTime);
    void dash_reverse(int dashTime);
    void drive_to_checkpoint(void);
    void find_left_corner(void);
    void find_right_corner(void);
    void do_turn_right_90(bool isCorrectingLine);
    void do_turn_right_45(bool isCorrectingLine);
    void do_turn_left_90(bool isCorrectingLine);
    void do_turn_left_45(bool isCorrectingLine);
};

#endif // DRIVECONTROL_H