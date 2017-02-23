/*
 * Nom: main.cpp
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 22 fev 2017
 */

#include "main.h"
#include "display.h"
#include "timer1.h"
#include "time.hpp"
#include "button.h"
#include "rtc.h"
#include <util/delay.h>
#include <avr/sleep.h>

enum state_t
{
    SLEEP,
    DISP,
    PROG
};
volatile state_t state = SLEEP;
void nextState()
{
    switch (state)
    {
    case SLEEP:
        state = DISP;
        break;
    case DISP:
        state = PROG;
        break;
    case PROG:
        state = DISP;
        break;
    }
}

volatile uint8_t ledState = 0;
static Time t;

Timer1 timer1;
Display display;
Button button;
RTC rtc;

/*
void setLowClockSpeed()
{
	CLKPR = _BV(CLKPCE);
	CLKPR = _BV(CLKPS2) | _BV(CLKPS0); // set clock prescaler to 32 (8MHz /32 = 250kHz)
}*/

void wakeUpSequence()
{
    t = rtc.getTime();
    display.showTime(t);
    timer1.start(5000);
    _delay_ms(100); // make sure button isn't bouncing
}

void sleepNow()
{
    //http://www.atmel.com/webdoc/AVRLibcReferenceManual/group__avr__sleep.html
    set_sleep_mode(SM1); // full sleep mode
    cli();
    {
        sleep_enable();
        sleep_bod_disable();
        sei();
        sleep_cpu();
        sleep_disable();
    }
    sei();
}

void sleepSequence(void)
{
    cli();
    {
        display.stopFlash();
        timer1.stop();
        display.clear();
        state = SLEEP;
        sleepNow();
    }
    sei();
}

void onButtonPressed(void)
{
    switch (state)
    {
    case SLEEP:
    {
        wakeUpSequence();
        nextState();
        break;
    }
    case DISP:
    {
        if (!button.getDebState())
            break;

        nextState();
        break;
    }
    case PROG:
    {
        if (!button.getDebState())
            break;

        display.startFlash();
        while (button.getState())
        {
            t.increment(5);
            display.showTime(t);
            _delay_ms(250);
        }
        timer1.start(2000);
        break;
    }
    }
}

void setup()
{
    // disable all peripherals.
    PRR = 0xFF;

    //setLowClockSpeed();

    /*
	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xFF;*/

    button.enableInterrupt(onButtonPressed);
    timer1.enableInterrupt(sleepSequence);

    wakeUpSequence();
}

int main()
{
    cli();
    {
        setup();
    }
    sei();

    for (;;)
        ;

    return 0;
}
