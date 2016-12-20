/*
 * Nom: 
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: 
 * Version: 1.0
 */

#include "main.h"
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include "Button.h"
#include "timer/timer1.h"

ISR(INT1_vect)
{
	//cli(); // disable interrupt
	PRR |= _BV(PRTIM1); // disable Timer 1
	//TIMSK1 = 0; //disable TIM1 interrupts
	
	if( Button::getDebState() &&               // button is pressed
		TCNT1 < 5000) // pressed fast enough
	{
		// enter time set mode
		setTime();
		RTC::setTime(now);
	}
	
	TCNT1 = 0;	// reset timer counter
	PRR &= ~_BV(PRTIM1); // enable Timer 1
	//TIMSK1 = _BV(OCIE1A); // re-enable TIM1 interrupts
	//sei();		// enable interrupt
}

void timer1Handler( void )
{
	Display::clear();
	sleepNow();
}

ISR(TIMER0_COMPA_vect)
{
	Display::onTimerOut();
}

void sleepNow()
{
	//http://www.atmel.com/webdoc/AVRLibcReferenceManual/group__avr__sleep.html
	cli();
	//if(sleep)
	{
		sleep_enable();
		sei();
		sleep_cpu();
		sleep_disable();
	}
	sei();
}

void partirMinuterie ( uint16_t duree_ms ) 
{
	// mode CTC du timer 1 avec horloge divisee par 1024
	// interruption apres la duree specifiee
	
	//INITIALISATION RANDOM A FIN DE TEST
	TCNT1 = 0;							      			//Reset counter value
	OCR1A = duree_ms * F_CPU / TIMER_PRESCALER / 1000;	//Counter compare value
	TCCR1A = 0; 										//No output
	TCCR1B = _BV(CS12) | _BV(CS10); 					//CS12 & CS10 for clk/1024
	TCCR1C = 0;
	TIMSK1 = _BV(OCIE1A);								//timer mask
}

void setClock()
{
	CLKPR = _BV(CLKPCE); // enable clock prescaler change
	CLKPR = _BV(CLKPS0); // set clock prescaler to f/2;
}

void setup()
{
	cli(); // disable interrupt
	
	PRR = _BV(PRTIM0) | _BV(PRSPI) | _BV(PRADC);
	
	// Tous les ports sont en sortie.
	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xFF & ~_BV(3); //sauf l'interrupteur

	
	RTC::setup();
	Display::setup();
	Button::setup();
	Timer1::setup(timer1Handler);
	
	// set sleep mode
	set_sleep_mode(SM1); // full sleep mode
	
	// setup timer1.
	partirMinuterie(5000); //minuterie de 5 secondes
		
	sei(); // enable interrupt
}

void setTime()
{
	while(TCNT1 < OCR1A/2)
	{
		if(Button::getState())
		{
			now.increment();
			Display::showTime(now);
			TCNT1 = 0;
			_delay_ms(750);
		}
	}
}

int main()
{
	setup();
	
	for(;;)	{	}
	return 0; 
}

