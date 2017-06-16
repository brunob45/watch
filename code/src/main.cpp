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
#include "gpior.h"

static const uint8_t tick_index = 0;
static __inline__ void set_tick()
{
    GPIOReg::set(tick_index);
}
static __inline__ void clear_tick()
{
    GPIOReg::clear(tick_index);
}
static __inline__ void wait_tick()
{
    while (!GPIOReg::get(tick_index))
    {
        // wait
    }
}

ISR(TIMER0_COMPA_vect)
{
    set_tick();
}

ISR(INT1_vect)
{
    Button::DisableInterrupt();
}

int main()
{
    StateCtx::SetState(WakeUpState);

    uint8_t deb_cnt = 10;
    for (;;)
    {
        wait_tick();
        clear_tick();

        Button::get_current() ? Button::set_previous() : Button::clear_previous();

        deb_cnt--;
        if (deb_cnt == 0)
        {
            // do every 10 ms
            deb_cnt = 10;
            Button::Update();
        }

        StateCtx::Update();
        StateCtx::Perform();
    }
}
