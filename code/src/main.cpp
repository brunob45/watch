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

volatile bool tick = false;
ISR(TIMER0_COMPA_vect)
{
    tick = true;
}

int main()
{
    StateCtx::SetState(WakeUpState);

    for (;;)
    {
        while (tick == false)
        {
            // wait
        }
        tick = false;

        Button::Update();

        StateCtx::Update();
        StateCtx::Perform();
    }

    return 0;
}
