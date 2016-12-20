#ifndef __TIMER1_H__
#define __TIMER1_H__

#include <avr/interrupt.h>

#define TIMER_PRESCALER 1024UL

namespace Timer1
{
	void (*onTimerOut)(void) = 0;
	
	void setup(void (*handler)(void) = 0)
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
	
	void start(uint16_t top)
	{
		TCNT1 = 0;
		OCR1A = top * F_CPU / TIMER_PRESCALER / 1000;	//Counter compare value
		PRR &= ~_BV(PRTIM1);
	}
	
	void restart()
	{
		TCNT1 = 0;
		PRR &= ~_BV(PRTIM1);
	}
	
	void stop()
	{
		PRR |= _BV(PRTIM1);
	}
	
	void enableInterrupt(void (*handler)(void) = 0)
	{
		if(handler) onTimerOut = handler;
		TIMSK1 = _BV(OCIE1A); 
	}
	
	void disableInterrupt()
	{
		TIMSK1 = 0;
	}
	
	uint16_t getCounter()
	{
		return TCNT1;
	}
	
	uint16_t getTime()
	{
		return getGounter() * F_CPU / TIMER_PRESCALER / 1000;
	}
}

ISR(TIMER1_COMPA_vect)
{
	if(Timer1::onTimerOut)
	{
		cli();
		Timer1::onTimerOut();
		sei();
	}
}

#endif