#include <avr/io.h>
#include <sensor.h>
#include <constant.h>
#include <Arduino.h>

void init_sensor(void)
{
    // set port A as input
    DDRA = 0x00;
}

/*
00000001 = 7
00000011 = 6
00000010 = 5
00000110 = 4
00000100 = 3
00001100 = 2
00001000 = 1
00011000 = 0
00010000 = -1
00110000 = -2
00100000 = -3
01100000 = -4
01000000 = -5
11000000 = -6
10000000 = -7
*/
char readFrontSensors()
{
    return PINA;
}

uint8_t reverseBits(uint8_t val)
{
    uint8_t reversed = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        reversed <<= 1;        // Shift reversed to the left to make room for the next bit
        reversed |= (val & 1); // Get the least significant bit of input and add it to reversed
        val >>= 1;             // Shift input to the right to process the next bit
    }

    return reversed;
}

uint8_t readSensor()
{
    return reverseBits(PINA);
}

int transalteUndefinedSensorValue(uint8_t sensorValue)
{

    if (sensorValue <= 15)
    {
        /// sensor is too the left
        return 10;
    }
    if (sensorValue <= 241)
    {
        /// sensor is too the right
        return -10;
    }

    // return int(sensorValue);
    return -10;
}

int getSensorError()
{
    int sensorValue = int(readSensor());
    switch (sensorValue)
    {
    case 127:
        return -7;
    case 63:
        return -6;
    case 191:
        return -5;
    case 159:
        return -4;
    case 223:
        return -3;
    case 207:
        return -2;
    case 239:
        return -1;
    case 231:
        return 0;
    case 247:
        return 1;
    case 243:
        return 2;
    case 251:
        return 3;
    case 249:
        return 4;
    case 253:
        return 5;
    case 252:
        return 6;
    case 254:
        return 7;
    case 0: // all black
        return SENSOR_T_INTERSECTION_VALUE;
    case 255: // all white
        return SENSOR_ALL_WHITE_VALUE;
    default:
        return transalteUndefinedSensorValue(sensorValue);
    }
}
