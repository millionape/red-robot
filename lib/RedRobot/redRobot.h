#include <button.h>
#include <driveControl.h>
#include <sensor.h>
#include <buzzer.h>
#include <gripperControl.h>

inline volatile int counter = 0;
inline DriveControl driveControl = DriveControl();
inline GripperControl gripperControl = GripperControl();
void init_redRobot();