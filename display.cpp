/*
 * Nom: 
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: 
 * Version: 1.0
 */

#include "display.h"
#include <avr/interrupt.h>

Time displayTime;

void Display::setup()
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
	                 
	M[0]  = LED(&PORTD, 0); // D0
	M[5]  = LED(&PORTC, 2); // C2
	M[10] = LED(&PORTC, 0); // C0
	M[15] = LED(&PORTC, 7); // C7
	M[20] = LED(&PORTB, 5); // B5
	M[25] = LED(&PORTB, 3); // B3
	M[30] = LED(&PORTB, 0); // B0
	M[35] = LED(&PORTD, 7); // D7
	M[40] = LED(&PORTB, 7); // B7
	M[45] = LED(&PORTA, 3); // A3
	M[50] = LED(&PORTD, 4); // D4
	M[55] = LED(&PORTD, 1); // D1
	
	// setup TIM0
	TCCR0A = _BV(CTC0) | _BV(CS02) | _BV(CS00); //CS12 & CS10 for clk/1024, CTC0 for clear on compare match
	
	PRR |= _BV(PRTIM0); // disable timer 0
}

void Display::showTime(Time t)
{
	if(showing)	clear();	// clear old time
	displayTime = t;	// save time for future use
	
	H[displayTime.h%12].turnOn();
	H[displayTime.m/5].turnOn();
	showing = 1;
}

void Display::clear()
{
	H[displayTime.h%12].turnOff();
	H[displayTime.m/5].turnOff();
	showing = 0;
}

void Display::startFlash(uint8_t delay)
{
	PRR &= ~_BV(PRTIM0);
	TCNT0 = 0;						//Reset counter value
	OCR0A = 0xFE;					//Counter compare value
	TIMSK0 = _BV(OCIE0A);			//enable TIM0 interrupt
}

void Display::stopFlash()
{
	// disable TIM0 interrupts
	PRR |= _BV(PRTIM0);
}