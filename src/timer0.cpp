/*
 * Nom: timer0.cpp
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 17 jan 2017
 */

#include <avr/interrupt.h>
#include "timer0.h"

#define TIMER_PRESCALER 1024UL

static EventHandler onTimer0Out = 0;

Timer0::Timer0(EventHandler handler)
{
	// mode CTC du timer 1 avec horloge divisee par 1024
	// interruption apres la duree specifiee

	// disable clock to timer1 module
	PRR &= ~_BV(PRTIM0);

	// set interrupt handler method
	if(handler)	onTimer0Out = handler;

	TCCR0A = _BV(CTC0) | _BV(CS02) | _BV(CS00); //CS02 & CS00 for clk/1024 + CTC timer
}

void Timer0::start(const uint8_t top)
{
	TCNT0 = 0;
	OCR0A = top * F_CPU / TIMER_PRESCALER / 1000;	//Counter compare value
	PRR &= ~_BV(PRTIM0);
}

void Timer0::restart()
{
	TCNT1 = 0;
	PRR &= ~_BV(PRTIM1);
}

void Timer0::stop()
{
	PRR |= _BV(PRTIM0);
}

void Timer0::enableInterrupt(EventHandler handler)
{
	if(handler) onTimer0Out = handler;
	TIMSK0 = _BV(OCIE0A);
}

void Timer0::disableInterrupt()
{
	TIMSK0 = 0;
}

uint8_t Timer0::getCounter()
{
	return TCNT0;
}

uint8_t Timer0::getTime()
{
	return getCounter() * F_CPU / TIMER_PRESCALER / 1000;
}

ISR(TIMER0_COMPA_vect)
{
	if(onTimer0Out) onTimer0Out();
}
