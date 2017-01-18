/*
 * Nom: main.cpp
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 17 jan 2017
 */

#include "main.h"

#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

#include "button.h"
#include "timer1.h"
#include "display.h"
#include "rtc.h"

Button button;
Timer1 timer1;
Display display;
RTC rtc;

void buttonHandler(void)
{
	timer1.stop();
	
	if( button.getDebState() &&       // button is pressed
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



void setTime()
{
	while(timer1.getCounter() < timer1.getTop()/2)
	{
		if(button.getState())
		{
			now.increment();
			display.showTime(now);
			timer1.resetCounter();
			_delay_ms(750);
		}
	}
}

void setup()
{
	// disable all peripherals.
	PRR = 0xFF;
	
	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xFF;
	
	// set sleep mode
	set_sleep_mode(SM1); // full sleep mode
}

int main()
{
	setup();
	for(;;);
	
	
	/*cli(); // disable interrupt
	setup();
	
	button.enableInterrupt(buttonHandler);
	
	timer1.enableInterrupt(timer1Handler);
	timer1.start(5000);       //minuterie de 5 secondes
	sei();
	
	for(;;)	{	}*/
	return 0; 
}

