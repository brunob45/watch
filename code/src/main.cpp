/*
 * Nom: main.cpp
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 22 fev 2017
 */

#ifndef F_CPU
#warning "F_CPU not defined in <main.h>"
#define F_CPU 128000UL
#endif

#include <avr/io.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

#include "time.hpp"
#include "display.h"
#include "button.h"
#include "rtc.h"
#include "states.h"

#define tick GPIOR0
static __inline__ void set_tick()
{
    tick |= _BV(0);
}
static __inline__ void clear_tick()
{
    tick &= ~_BV(0);
}
static __inline__ void wait_tick()
{
    while (~tick & _BV(0))
    {
        // wait
    }
}

ISR(TIMER0_COMPA_vect)
{
    set_tick();
}

int main()
{
    StateCtx::SetState(WakeUpState);

    uint8_t deb_cnt = 10;
    for (;;)
    {
        wait_tick();
        clear_tick();

        Button::previous_state = Button::current_state;
        deb_cnt--;
        if (deb_cnt == 0)
        {
            deb_cnt = 10;
            Button::Update();
        }

        StateCtx::Update();
        StateCtx::Perform();
    }
}
