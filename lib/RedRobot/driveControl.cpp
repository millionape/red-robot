#include <Arduino.h>
#include <driveControl.h>
#include <motor.h>
#include <constant.h>
#include <sensor.h>
#include <buzzer.h>
#include <button.h>

DriveControl::DriveControl()
{
    motorInstance = Motor();
    defaultSpeed = DEFAULT_SPEED;
}

float DriveControl::getPidSpeed(int sensorError)
{
    positionError = sensorError;
    integral += positionError;
    derivative = positionError - prevError;
    prevError = positionError;
    float pidSpeed = Kp * positionError + Ki * integral + Kd * derivative;
    return pidSpeed;
}

/**
 * @brief Reset PID values to zero
 * @details This function is to be called when we want to reset PID values to zero.
 *          This is useful when we want to start a new PID control loop.
 */
void DriveControl::resetPID(void)
{
    pidSpeed = 0;
    derivative = 0;
    prevError = 0;
    positionError = 0;
}

bool DriveControl::doLaneKeepForwardDrive()
{
    int sensorError = getSensorError();

    if (sensorError == 10 || sensorError == -10)
    {
        return true;
    }

    pidSpeed = getPidSpeed(sensorError);

    if (!pauseLaneKeepDrive)
    {
        // Serial.printf("DriveControl::doLaneKeepForwardDrive[PID:%d] -> left: %d, right: %d\n", pidSpeed, defaultSpeed + pidSpeed, defaultSpeed - pidSpeed);
        motorInstance.leftMotorCommand(defaultSpeed + pidSpeed, driveDirection);
        motorInstance.rightMotorCommand(defaultSpeed - pidSpeed, driveDirection);
        delay(1);
    }
    return false;
}

void DriveControl::updateSensorError(int sensorError)
{
    Serial.printf("DriveControl::updateSensorError -> error: %d\n", sensorError);
    pidSpeed = getPidSpeed(sensorError);
}

void DriveControl::setPauseLaneKeepDrive(bool isPause)
{
    pauseLaneKeepDrive = isPause;
}

void DriveControl::stopDriving(void)
{
    pauseLaneKeepDrive = true;
    motorInstance.stopMotors();
}

void DriveControl::setDefaultSpeed(uint8_t speed)
{
    defaultSpeed = speed;
}

/**
 * @brief เมื่อเรียกฟังก์ชั่นนี้ หุ่นยนต์จะทำการขับไปข้างหน้า จนกว่าจะเจอแยกตัว T หรือ สี่แยก
 *          จึงจะหยุด เพื่อรอทำคำสั่งถัดไป
 */
void DriveControl::drive_to_checkpoint(int additionalDriveTime)
{
    while (positionError != SENSOR_T_INTERSECTION_VALUE && positionError != SENSOR_ALL_WHITE_VALUE)
    {
        pauseLaneKeepDrive = false;
        if (doLaneKeepForwardDrive())
        {
            delay(additionalDriveTime);
            break;
        }
    }

    defaultSpeed = DEFAULT_SPEED;
    pauseLaneKeepDrive = true;
    motorInstance.stopMotorsWithBreak(driveDirection, 50, 50);
    delay(150);
    // while (true)
    // {
    //     /* code */
    // }
}

/**
 * @brief  Drive to checkpoint
 *         คำสั่งเพื่อให้หุ่นเดินไปข้างหน้าจนกว่าจะเจอ checkpoint
 * checkpoint คือ แยกตัว T,สี่แยก,ทางเลี้ยวซ้าย,ทางเลี้ยวขวา
 * หรือแยกอื่นๆที่ไม่ใช่ทางตรง รวมถึงพื้นที่สีขาวล้วนหรือสีดําล้วย
 */
void DriveControl::drive_to_checkpoint(void)
{
    delay(100);
    while (positionError != SENSOR_T_INTERSECTION_VALUE && positionError != SENSOR_ALL_WHITE_VALUE)
    {
        pauseLaneKeepDrive = false;
        if (doLaneKeepForwardDrive())
        {
            break;
        }
    }

    defaultSpeed = DEFAULT_SPEED;
    pauseLaneKeepDrive = true;
    // motorInstance.stopMotorsWithBreak(driveDirection, 40, 37);
    // delay(250);
    // while (true)
    // {
    //     /* code */
    // }
}

/**
 * @brief คำสั่งเพื่อให้หุ่นเดินไปข้างหน้าเป็นเวลามิลลิวินาที
 * @details 1วินาที เท่ากับ 1000 มิลลิวินาที
 * @param duration เวลาในหน่วยมิลลิวินาที
 */
void DriveControl::go_to_the_front(int duration)
{
    // do 2seconds timeout
    unsigned long startTime = millis();
    while (millis() - startTime < duration)
    {
        int sensorError = getSensorError();

        if (sensorError == 10 || sensorError == -10)
        {
            break;
        }

        pidSpeed = getPidSpeed(sensorError);

        motorInstance.leftMotorCommand(defaultSpeed + pidSpeed, driveDirection);
        motorInstance.rightMotorCommand(defaultSpeed - pidSpeed, driveDirection);
        delay(1);
    }

    motorInstance.stopMotorsWithBreak(driveDirection, 40, 40);
    delay(180);
    // motorInstance.leftMotorCommand(defaultSpeed, driveDirection);
    // motorInstance.rightMotorCommand(defaultSpeed, driveDirection);
    // delay(dashTime);
    // motorInstance.stopMotorsWithBreak(driveDirection, 30, 0);
}

/**
 * @brief คำสั่งเพื่อให้หุ่นเดินไปข้างหน้าเป็นเวลามิลลิวินาที โดยไม่สนใจเส้นดำ
 * @details 1วินาที เท่ากับ 1000 มิลลิวินาที
 * @param dashTime เวลาในหน่วยมิลลิวินาที
 */
void DriveControl::dash(int dashTime)
{
    // do 2seconds timeout
    unsigned long startTime = millis();
    while (millis() - startTime < dashTime)
    {

        motorInstance.leftMotorCommand(defaultSpeed, driveDirection);
        motorInstance.rightMotorCommand(defaultSpeed, driveDirection);
        delay(1);
    }

    motorInstance.stopMotorsWithBreak(driveDirection, 40, 40);
    delay(150);
}

/**
 * @brief คำสั่งเพื่อให้หุ่นถอยหลังเป็นเวลามิลลิวินาที โดยไม่สนใจเส้นดำ
 * @details 1วินาที เท่ากับ 1000 มิลลิวินาที
 * @param dashTime เวลาในหน่วยมิลลิวินาที
 */
void DriveControl::dash_reverse(int dashTime)
{
    // do 2seconds timeout
    unsigned long startTime = millis();
    while (millis() - startTime < dashTime)
    {

        motorInstance.leftMotorCommand(defaultSpeed, false);
        motorInstance.rightMotorCommand(defaultSpeed, false);
        delay(1);
    }

    motorInstance.stopMotorsWithBreak(true, 40, 40);
    delay(150);
}

void DriveControl::correct_center(int speed)
{

    int prev_errror = 0;
    unsigned long startTime = millis();
    while (millis() - startTime < 300)
    {
        int sensorError = getSensorError();
        prev_errror = sensorError;
        if (sensorError >= -7 && sensorError <= 7)
        {
            if (sensorError < 0)
            {
                motorInstance.leftMotorCommand(speed, false);
                motorInstance.rightMotorCommand(speed, true);
            }
            else if (sensorError > 0)
            {
                motorInstance.leftMotorCommand(speed, true);
                motorInstance.rightMotorCommand(speed, false);
            }
        }
        else
        {
            break;
        }
        delay(1);
    }

    if (prev_errror < 0)
    {
        motorInstance.leftMotorCommand(speed - 8, true);
        motorInstance.rightMotorCommand(speed - 8, false);
    }

    if (prev_errror > 0)
    {
        motorInstance.leftMotorCommand(30, false);
        motorInstance.rightMotorCommand(30, true);
    }
    delay(10);
    motorInstance.stopMotors();
}

/**
 * @brief ทำให้หุ่นยนต์หมุนขวา 90 องศา
 * @param isCorrectingLine ถ้าเป็น true จะทำการหันทิศทางให้ตรงเส้น หลังจากหมุนเสร็จแล้ว
 */
void DriveControl::do_turn_right_90(bool isCorrectingLine)
{
    motorInstance.leftMotorCommand(TRUN_SPEED, true);
    motorInstance.rightMotorCommand(TRUN_SPEED, false);
    delay(TRUN_90_DEGREE_DURATION);
    motorInstance.leftMotorCommand(TRUN_BRAKE_SPEED, false);
    motorInstance.rightMotorCommand(TRUN_BRAKE_SPEED, true);
    delay(50);
    motorInstance.stopMotors();
    if (isCorrectingLine)
    {
        correct_center(TRUN_SPEED);
    }
    play_buzzer_2_beep();
}

/**
 * @brief ทำให้หุ่นยนต์หมุนขวา 45 องศา
 * @param isCorrectingLine ถ้าเป็น true จะทำการหันทิศทางให้ตรงเส้น หลังจากหมุนเสร็จแล้ว
 */
void DriveControl::do_turn_right_45(bool isCorrectingLine)
{
    motorInstance.leftMotorCommand(TRUN_SPEED, true);
    motorInstance.rightMotorCommand(TRUN_SPEED, false);
    delay(TRUN_45_DEGREE_DURATION);
    motorInstance.leftMotorCommand(TRUN_BRAKE_SPEED, false);
    motorInstance.rightMotorCommand(TRUN_BRAKE_SPEED, true);
    delay(50);
    motorInstance.stopMotors();
    if (isCorrectingLine)
    {
        correct_center(TRUN_SPEED);
    }
    play_buzzer_2_beep();
}

/**
 * @brief ทำให้หุ่นยนต์หมุนซ้าย 90 องศา
 * @param isCorrectingLine ถ้าเป็น true จะทำการหันทิศทางให้ตรงเส้น หลังจากหมุนเสร็จแล้ว
 */
void DriveControl::do_turn_left_90(bool isCorrectingLine)
{
    motorInstance.leftMotorCommand(TRUN_SPEED, false);
    motorInstance.rightMotorCommand(TRUN_SPEED, true);
    delay(TRUN_90_DEGREE_DURATION);
    motorInstance.leftMotorCommand(TRUN_BRAKE_SPEED, true);
    motorInstance.rightMotorCommand(TRUN_BRAKE_SPEED, false);
    delay(50);
    motorInstance.stopMotors();
    if (isCorrectingLine)
    {
        correct_center(TRUN_SPEED);
    }
    play_buzzer_2_beep();
}

/**
 * @brief ทำให้หุ่นยนต์หมุนซ้าย 45 องศา
 * @param isCorrectingLine ถ้าเป็น true จะทำการหันทิศทางให้ตรงเส้น หลังจากหมุนเสร็จแล้ว
 */
void DriveControl::do_turn_left_45(bool isCorrectingLine)
{
    motorInstance.leftMotorCommand(TRUN_SPEED, false);
    motorInstance.rightMotorCommand(TRUN_SPEED, true);
    delay(TRUN_45_DEGREE_DURATION);
    motorInstance.leftMotorCommand(TRUN_BRAKE_SPEED, true);
    motorInstance.rightMotorCommand(TRUN_BRAKE_SPEED, false);
    delay(50);
    motorInstance.stopMotors();
    if (isCorrectingLine)
    {
        correct_center(TRUN_SPEED);
    }
    play_buzzer_2_beep();
}

void DriveControl::stop_forever()
{
    motorInstance.stopMotors();
    while (true)
    {
        /* code */
    }
}

void DriveControl::stop_and_wait_for_button()
{
    while (!START_BUTTON)
    {
        Serial.println("waiting for press START button......");
        delay(50);
    }
}

void DriveControl::custom_brake(int brakeSpeed, int duration)
{
    motorInstance.stopMotorsWithBreak(driveDirection, brakeSpeed, duration);
}

/**
 * @brief  Brake the robot
 *         คำสั่งเพื่อให้หุ่นยนต์เบรก
 */
void DriveControl::brake()
{
    motorInstance.stopMotorsWithBreak(driveDirection, BRAKE_SPEED, BRAKE_DURATION);
    motorInstance.stopMotors();
}