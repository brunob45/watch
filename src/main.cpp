/*
 * Nom: 
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: 
 * Version: 1.0
 */

#include "main.h"

#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

#include "button.h"
#include "timer1.h"
#include "display.h"
#include "rtc.h"

void buttonHandler(void)
{
	timer1.stop();
	
	if( Button::getDebState() &&       // button is pressed
		timer1.getTime() < 500)       // pressed fast enough (t < 500ms)
	{
		// enter time set mode
		setTime();
		rtc.setTime(now);
	}
	
	timer1.restart();
}

void timer1Handler( void )
{
	display.clear();
	sleepNow();
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

void setClock()
{
	CLKPR = _BV(CLKPCE); // enable clock prescaler change
	CLKPR = _BV(CLKPS0); // set clock prescaler to f/2;
}

void setup()
{
	// disable all peripherals.
	PRR = 0xFF;

	// set sleep mode
	set_sleep_mode(SM1); // full sleep mode
}

void setTime()
{
	while(TCNT1 < OCR1A/2)
	{
		if(Button::getState())
		{
			now.increment();
			display.showTime(now);
			TCNT1 = 0;
			_delay_ms(750);
		}
	}
}

int main()
{
	cli(); // disable interrupt
	setup();
	
	Button::setup();
	Button::enableInterrupt(buttonHandler);
	
	timer1.enableInterrupt(timer1Handler);
	timer1.start(5000);       //minuterie de 5 secondes
	sei();
	
	for(;;)	{	}
	return 0; 
}

