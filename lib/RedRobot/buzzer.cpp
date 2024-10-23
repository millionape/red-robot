#include <Arduino.h>
#include <buzzer.h>

void init_buzzer(void)
{
    // set port B5 as output
    pinMode(PIN_PB5, OUTPUT);
}

void play_buzzer(void)
{
    for (size_t i = 0; i < 5; i++)
    {
        tone(PIN_PB5, 200 * (i + 1));
        // delay 500ms
        delay(100);
    }

    noTone(PIN_PB5);
}

void play_buzzer_2_beep(void)
{
    for (size_t i = 0; i < 2; i++)
    {
        tone(PIN_PB5, 550 * (i + 1));
        // delay 500ms
        delay(40);
    }

    noTone(PIN_PB5);
}