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

static uint8_t showing = 0;
Time displayTime;
Timer0 timer0;

static void onTimerOut()
{
	if( showing )
		display.clear();
	else
		display.showTime();
}

Display::Display()
{
	H[0]  = LED(&PORTC, 3); // C3
	H[1]  = LED(&PORTC, 1); // C1
	H[2]  = LED(&PORTA, 1); // A1
	H[3]  = LED(&PORTA, 0); // A0
	H[4]  = LED(&PORTB, 4); // B4
	H[5]  = LED(&PORTB, 2); // B2
	H[6]  = LED(&PORTB, 1); // B1
	H[7]  = LED(&PORTD, 6); // D6
	H[8]  = LED(&PORTD, 5); // D5
	H[9]  = LED(&PORTB, 6); // B6
	H[10] = LED(&PORTA, 2); // A2
	H[11] = LED(&PORTD, 2); // D2
	                 
	M[ 0/5]  = LED(&PORTD, 0); // D0
	M[ 5/5]  = LED(&PORTC, 2); // C2
	M[10/5] = LED(&PORTC, 0); // C0
	M[15/5] = LED(&PORTC, 7); // C7
	M[20/5] = LED(&PORTB, 5); // B5
	M[25/5] = LED(&PORTB, 3); // B3
	M[30/5] = LED(&PORTB, 0); // B0
	M[35/5] = LED(&PORTD, 7); // D7
	M[40/5] = LED(&PORTB, 7); // B7
	M[45/5] = LED(&PORTA, 3); // A3
	M[50/5] = LED(&PORTD, 4); // D4
	M[55/5] = LED(&PORTD, 1); // D1
}

void Display::showTime()
{
    showTime(displayTime);	
}

void Display::showTime(Time t)
{
    if(showing)	clear();	// clear old time
    
    displayTime = t;
    H[displayTime.h%12].turnOn();
	M[displayTime.m/5].turnOn();
    
    timer0.resetCounter(); // reset blinking timer
	showing = 1;
}

void Display::clear()
{
	H[displayTime.h%12].turnOff();
	M[displayTime.m/5].turnOff();
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

