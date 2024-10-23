#include <redRobot.h>
#include <Arduino.h>
#include <buzzer.h>

void init_timer()
{
    // Disable interrupts during configuration
    cli();

    // Set Timer 1 to CTC mode (WGM12 = 1)
    TCCR1B |= (1 << WGM12);

    // Set prescaler to 64 (CS11:0 = 0b011)
    TCCR1B |= (1 << CS11) | (1 << CS10);

    // Set the compare value for 1ms
    OCR1A = 249;

    // Enable Timer 1 Compare Match A interrupt
    TIMSK |= (1 << OCIE1A);

    // Initialize the Timer/Counter register
    TCNT1 = 0;

    // Enable global interrupts
    sei();
}

void init_redRobot()
{
    init_sensor();
    // timer1_100hz_init();
    init_buttons();
    init_buzzer();
    Serial.begin(9600);
    delay(1000);
    play_buzzer();
    init_timer();
    Serial.println("Ready to start, please press START button......");
    while (!START_BUTTON)
    {
        int sensorValue = getSensorError();
        Serial.printf("SENSOR => %d \n", sensorValue);
        delay(100);
    };
}

ISR(TIMER1_COMPA_vect)
{
    counter += 1;
    if (counter >= 10)
    {
        counter = 0;
        gripperControl.refresh();
    }
}