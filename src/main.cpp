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
#include <avr/power.h>

volatile STATE::state_t state = STATE::SLEEP;
volatile uint8_t ledState = 0;
Time time;

Timer1 sleepTimer;
Display display;
Button button;
RTC rtc;

void onButtonPressed()
{
    switch (state)
    {
    case STATE::SLEEP:
        wakeUpSequence();
        STATE::nextState(state);
        break;

    case STATE::DISP:
        STATE::nextState(state);
        break;

    case STATE::PROG:
        sleepTimer.stop();
        setTime();
        sleepTimer.start(2000);
        break;
    }
}

void setup()
{
    // disable all peripherals.
    power_all_disable();

    //setLowClockSpeed();

    button.enableInterrupt(onButtonPressed);
    sleepTimer.enableInterrupt(sleepSequence);

    wakeUpSequence();
}

int main()
{
    cli();
    {
        setup();
    }
    sei();

    for (;;) ;

    return 0;
}


void setTime()
{
    uint8_t cnt = 0;
    display.startFlash();
    while (button.isPressed())
    {
        time.next(5);
        display.showTime(time);
        if (cnt < 5)
        {
            _delay_ms(250);
            cnt++;
        }
        else
        {
            _delay_ms(100);
        }
    }
}

void wakeUpSequence()
{
    time = rtc.getTime();
    display.showTime(time);
    sleepTimer.start(5000);
}

void sleepSequence()
{
    cli();
    {
        if (time.isDirty())
        {
            rtc.setTime(time);
        }

        sleepTimer.stop();

        display.stopFlash();
        display.clear();

        state = STATE::SLEEP;

        sleepNow();
    }
    sei();
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
