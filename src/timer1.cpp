/*
 * Nom:
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 */

#include <avr/interrupt.h>
#include "timer1.h"

#define TIMER_PRESCALER 1024UL

Timer1::Timer1(EventHandler handler)
{
	// mode CTC du timer 1 avec horloge divisee par 1024
	// interruption apres la duree specifiee

	// disable clock to timer1 module
	PRR &= ~_BV(PRTIM1);

	// set interrupt handler method
	if(handler)	onTimerOut = handler;

	TCCR1A = 0; 										//No output
	TCCR1B = _BV(CS12) | _BV(CS10) | _BV(WGM12); 		//CS12 & CS10 for clk/1024 + CTC timer
	TCCR1C = 0;
}

void Timer1::start(const uint16_t top)
{
	TCNT1 = 0;
	OCR1A = top * F_CPU / TIMER_PRESCALER / 1000;	//Counter compare value
	PRR &= ~_BV(PRTIM1);
}

void Timer1::restart()
{
	TCNT1 = 0;
	PRR &= ~_BV(PRTIM1);
}

void Timer1::stop()
{
	PRR |= _BV(PRTIM1);
}

void Timer1::enableInterrupt(EventHandler handler)
{
	if(handler) onTimerOut = handler;
	TIMSK1 = _BV(OCIE1A);
}

void Timer1::disableInterrupt()
{
	TIMSK1 = 0;
}

uint16_t Timer1::getCounter()
{
	return TCNT1;
}

uint16_t Timer1::getTime()
{
	return getCounter() * F_CPU / TIMER_PRESCALER / 1000;
}

ISR(TIMER1_COMPA_vect)
{
	if(timer1.onTimerOut) timer1.onTimerOut();
}