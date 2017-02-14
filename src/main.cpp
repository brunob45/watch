/*
 * Nom: main.cpp
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 13 fev 2017
 */

#include "main.h"
#include "display.h"
#include "timer1.h"
#include "time.hpp"
#include "button.h"
#include <util/delay.h>


volatile uint8_t ledState = 0;
volatile uint8_t wakeUpDone = 0;
static Time t;

Timer1 timer1;
Display display;
Button button;

/*
void setLowClockSpeed()
{
	CLKPR = _BV(CLKPCE);
	CLKPR = _BV(CLKPS2) | _BV(CLKPS0); // set clock prescaler to 32 (8MHz /32 = 250kHz)
}*/


void onTimerOut(void)
{
	display.stopFlash();
    timer1.stop();
    display.clear();
    wakeUpDone = 0;
}

void wakeUpSequence()
{
    display.showTime(t);
    wakeUpDone = 1;
}

void onButtonPressed(void)
{
    if(!wakeUpDone)
    {
        wakeUpSequence();
        return;
    }    
    
    if(!button.getDebState())
        return;
    
    display.startFlash();
    while(button.getState())
    {
        t.increment(5);
        display.showTime(t); 
        _delay_ms(250);
    }
    timer1.start_ms(2000);
    
    /*
    
    switch(state)
    {
        case OFF:
            {
                display.showTime(t);
                timer1.start_ms(5000);
                state = DISP;
                break;
            }
            
        case DISP:
            {
                timer1.start_ms(6000);
                while(button.getState())
                {
                    if(state == PROG)
                    {
                        t += (5);
                        display.showTime(t);
                        _delay_ms(250);
                    }
                    else if(timer1.getTop() > 5000)
                    {
                        timer1.stop();
                        state = PROG;
                        break;
                    }
                }
                display.startFlash(255);
                timer1.start_ms(2000);
                break;
            }
            
        case PROG:
        {
            timer1.stop();
            while(button.getState())
            {
                t.increment(5);
                display.showTime(t);
                _delay_ms(250);
            }
            timer1.start_ms(2000);
            break;
        }
    }*/
}


void setup()
{
	// disable all peripherals.
	PRR = 0xFF;
	
    //setLowClockSpeed();
    
    /*
	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xFF;*/
    
    button.enableInterrupt(onButtonPressed);
    display.showTime(t);
    
    timer1.enableInterrupt(onTimerOut);
    
}



int main()
{
	cli();
	setup();
	sei();
	
	for(;;);
	return 0; 
}

