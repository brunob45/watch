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
#include "state.hpp"

#include <util/delay.h>
#include <avr/sleep.h>

volatile STATE::state_t state = STATE::SLEEP;
volatile uint8_t ledState = 0;
Time time;

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
    time = rtc.getTime();
    display.showTime(time);
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
        if (state == STATE::PROG)
        {
            rtc.setTime(time);
        }

        display.stopFlash();
        timer1.stop();
        display.clear();
        state = STATE::SLEEP;
        sleepNow();
    }
    sei();
}

void onButtonPressed(void)
{
    switch (state)
    {
    case STATE::SLEEP:
        wakeUpSequence();
        STATE::nextState(state);
        break;

    case STATE::DISP:
        if (button.getDebState())
        {
            STATE::nextState(state);
        }
        break;

    case STATE::PROG:
        if (button.getDebState())
        {
            display.startFlash();
            while (button.getState())
            {
                time.increment(5);
                display.showTime(time);
                _delay_ms(250);
            }
            timer1.start(2000);
        }
        break;
    }
}

void setup()
{
    // disable all peripherals.
    PRR = 0xFF;

    //setLowClockSpeed();

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
