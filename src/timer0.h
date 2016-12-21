/*
 * Nom: 
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: 
 * Version: 1.0
 */

#ifndef __TIMER0_H__
#define __TIMER0_H__

#include <avr/interrupt.h>
#include <avr/io.h>
#include "util.h"

#define TIMER_PRESCALER 1024UL

namespace Timer0
{
	EventHandler onTimerOut = 0;
	
	void setup(EventHandler handler = 0)
	{
		// mode CTC du timer 1 avec horloge divisee par 1024
		// interruption apres la duree specifiee
		
		// disable clock to timer1 module
		PRR &= ~_BV(PRTIM0);
		
		// set interrupt handler method
		if(handler)	onTimerOut = handler;
		
		TCCR0A = _BV(CTC0) | _BV(CS02) | _BV(CS00); //CS02 & CS00 for clk/1024 + CTC timer
	}
	
	void start(const uint8_t top)
	{
		TCNT0 = 0;
		OCR0A = top * F_CPU / TIMER_PRESCALER / 1000;	//Counter compare value
		PRR &= ~_BV(PRTIM0);
	}
	
	void restart()
	{
		TCNT1 = 0;
		PRR &= ~_BV(PRTIM1);
	}
	
	void stop()
	{
		PRR |= _BV(PRTIM0);
	}
	
	void enableInterrupt(EventHandler handler = 0)
	{
		if(handler) onTimerOut = handler;
		TIMSK0 = _BV(OCIE0A); 
	}
	
	void disableInterrupt()
	{
		TIMSK0 = 0;
	}
	
	uint8_t getCounter()
	{
		return TCNT0;
	}
	
	uint8_t getTime()
	{
		return getCounter() * F_CPU / TIMER_PRESCALER / 1000;
	}
}

ISR(TIMER0_COMPA_vect)
{
	if(Timer0::onTimerOut) Timer0::onTimerOut();
}

#endif
