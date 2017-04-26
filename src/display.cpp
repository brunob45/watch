/*
 * Nom: display.cpp
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 13 fev 2017
 */

#include "display.h"
#include "timer0.h"
#include <util/delay.h>

extern Display display;

Timer0 timer0;

static uint8_t showing = 0;
static Time displayTime;

static void onTimerOut()
{
    if (showing)
		display.clear();
    else
		display.showTime();
}

Display::Display()
{
    outerRing[0] = LED(&PORTC, 3);  // C3
    outerRing[1] = LED(&PORTC, 1);  // C1
    outerRing[2] = LED(&PORTA, 1);  // A1
    outerRing[3] = LED(&PORTA, 0);  // A0
    outerRing[4] = LED(&PORTB, 4);  // B4
    outerRing[5] = LED(&PORTB, 2);  // B2
    outerRing[6] = LED(&PORTB, 1);  // B1
    outerRing[7] = LED(&PORTD, 6);  // D6
    outerRing[8] = LED(&PORTD, 5);  // D5
    outerRing[9] = LED(&PORTB, 6);  // B6
    outerRing[10] = LED(&PORTA, 2); // A2
    outerRing[11] = LED(&PORTD, 2); // D2

    innerRing[0 / 5] = LED(&PORTD, 0);  // D0
    innerRing[5 / 5] = LED(&PORTC, 2);  // C2
    innerRing[10 / 5] = LED(&PORTC, 0); // C0
    innerRing[15 / 5] = LED(&PORTC, 7); // C7
    innerRing[20 / 5] = LED(&PORTB, 5); // B5
    innerRing[25 / 5] = LED(&PORTB, 3); // B3
    innerRing[30 / 5] = LED(&PORTB, 0); // B0
    innerRing[35 / 5] = LED(&PORTD, 7); // D7
    innerRing[40 / 5] = LED(&PORTB, 7); // B7
    innerRing[45 / 5] = LED(&PORTA, 3); // A3
    innerRing[50 / 5] = LED(&PORTD, 4); // D4
    innerRing[55 / 5] = LED(&PORTD, 1); // D1
}

void Display::showTime()
{
    showTime(displayTime);
}

void Display::showTime(Time t)
{
    if (showing)
		clear(); // clear old time

    displayTime = t;
    outerRing[displayTime.h % 12].turnOn();
    innerRing[displayTime.m / 5].turnOn();

    timer0.resetCounter(); // reset blinking timer
    showing = 1;
}

void Display::clear()
{
    outerRing[displayTime.h % 12].turnOff();
    innerRing[displayTime.m / 5].turnOff();
    showing = 0;
}

void Display::startFlash(uint8_t delay)
{
    timer0.enableInterrupt(onTimerOut);
    timer0.start(delay);
}

void Display::stopFlash()
{
    // disable TIM0 interrupts
    timer0.stop();
    showTime();
}

void Display::toggle()
{
    if (showing)
		clear();
    else
		showTime();
}
