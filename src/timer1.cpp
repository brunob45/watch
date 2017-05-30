/*
 * Nom: timer1.cpp
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 13 fev 2017
 */

#include <avr/interrupt.h>
#include <avr/power.h>
#include "timer1.h"

#define TIMER_PRESCALER 1024UL

static EventHandler onTimer1Out = 0;

Timer1::Timer1(EventHandler handler)
{
    // mode CTC du timer 1 avec horloge divisee par 1024
    // interruption apres la duree specifiee

    // enable clock to timer1 module
    power_timer1_enable();

    // set interrupt handler method
    if (handler)
		onTimer1Out = handler;

    TCCR1A = 0;					 //No output
    TCCR1B = _BV(CS12) | _BV(CS10) | _BV(WGM12); //CS12 & CS10 for clk/1024 + CTC timer
    TCCR1C = 0;

    power_timer1_disable();
}

void Timer1::start(const uint16_t top)
{
    power_timer1_enable();
    TCNT1 = 0;
    OCR1A = top; //Counter compare value
}

void Timer1::start_ms(const uint16_t top_ms)
{
    start(top_ms * F_CPU / TIMER_PRESCALER / 1000);
}

void Timer1::restart()
{
    power_timer1_enable();
    TCNT1 = 0;
}

void Timer1::stop()
{
    power_timer1_disable();
}

void Timer1::enableInterrupt(EventHandler handler)
{
    uint8_t PRR_old = PRR;
    power_timer1_enable();

    if (handler)
		onTimer1Out = handler;
    TIMSK1 = _BV(OCIE1A);

    PRR = PRR_old;
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

void Timer1::resetCounter(uint16_t cnt)
{
    TCNT1 = cnt;
}

uint16_t Timer1::getTop()
{
    return OCR1A;
}

uint8_t Timer1::isActive()
{
    return PRR & _BV(PRTIM1);
}

ISR(TIMER1_COMPA_vect)
{
    if (onTimer1Out)
		onTimer1Out();
}
