/*
 * Nom: main.cpp
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 22 fev 2017
 */

#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>

#include "main.h"

#include "display.h"
#include "time.hpp"
#include "button.h"
#include "rtc.h"

Time time;
RTC rtc;
Display display;

namespace StateCtx
{
static state_t current = InvalidState;

void Init()
{
    //if(current == SleepState)
    //    States::SleepState_init();
    //else
    if (current == WakeUpState)
        States::WakeUpState_init();
    else if (current == DisplayState)
        States::DisplayState_init();
    else if (current == BlinkState)
        States::BlinkState_init();
    else if (current == ChangeTimeState)
        States::ChangeTimeState_init();
}

void Update()
{
    if (current == SleepState)
        States::SleepState_Update();
    else if (current == WakeUpState)
        States::WakeUpState_Update();
    else if (current == DisplayState)
        States::DisplayState_Update();
    else if (current == BlinkState)
        States::BlinkState_Update();
    else if (current == ChangeTimeState)
        States::ChangeTimeState_Update();
}

void Perform()
{
    if (current == SleepState)
        States::SleepState_Perform();
    /*else if (current == WakeUpState)
        States::WakeUpState_Perform();
    else if(current == DisplayState)
        States::DisplayState_Perform();*/
    else if (current == BlinkState)
        States::BlinkState_Perform();
    else if (current == ChangeTimeState)
        States::ChangeTimeState_Perform();
}

void SetState(state_t s)
{
    if (current != s)
    {
        current = s;
        Init();
    }
}

namespace States
{
void SleepState_Update()
{
    //if (Button::current_state)
    {
        StateCtx::SetState(WakeUpState);
    }
}

void SleepState_Perform()
{
    if (time.isDirty())
    {
        rtc.setTime(time);
    }

    display.clear();

    // activate INT1 interrupts for wake-up
    Button::EnableInterrupt();

    //http://www.atmel.com/webdoc/AVRLibcReferenceManual/group__avr__sleep.html
    set_sleep_mode(SLEEP_MODE_PWR_DOWN); // full sleep mode
    cli();
    {
        sleep_enable();
        sleep_bod_disable();
        sei();
        sleep_cpu();
        sleep_disable();
    }
    sei();
}

void WakeUpState_init()
{
    cli();
    {
        power_all_disable();
        power_timer0_enable();

        Button::Init();

        time = rtc.getTime();
        display.showTime(time);
    }
    sei();
}

void WakeUpState_Update()
{
    StateCtx::SetState(DisplayState);
}

static uint16_t sleep_cnt = 5000;
static union cnt_t {
    uint8_t lock;
    uint8_t blink;
    uint8_t change;
} cnt = {1};

void DisplayState_init()
{
    sleep_cnt = 5000;
    cnt.lock = 1;
    display.showTime();
}

void DisplayState_Update()
{
    if (cnt.lock == 0)
    {
        if (Button::pressed_cnt > 2000)
        {
            StateCtx::SetState(BlinkState);
        }
        else if (sleep_cnt == 0)
        {
            StateCtx::SetState(SleepState);
        }
    }
    else
    {
        if (Button::current_state && !Button::previous_state)
        {
            cnt.lock = 0;
        }
    }
}

void DisplayState_Perform()
{
    if (Button::current_state)
    {
        sleep_cnt = 5000;
    }
    else
    {
        sleep_cnt--;
    }
}

void BlinkState_init()
{
    cnt.blink = 250;
    sleep_cnt = 5000;
}

void BlinkState_Update()
{
    if (Button::current_state && !Button::previous_state)
    {
        StateCtx::SetState(ChangeTimeState);
    }
    else if (sleep_cnt == 0)
    {
        StateCtx::SetState(SleepState);
    }
}

void BlinkState_Perform()
{
    cnt.blink--;
    if (cnt.blink == 0)
    {
        display.toggle();
        cnt.blink = 250;
    }

    sleep_cnt--;
}

void ChangeTimeState_init()
{
    display.showTime();
    cnt.change = 250;
}

void ChangeTimeState_Update()
{
    static uint8_t change_cnt = 0;
    if (Button::pressed_cnt > cnt.change)
    {
        Button::pressed_cnt = 0;

        time.next(5);
        display.showTime(time);

        if (change_cnt < 3)
        {
            change_cnt++;
        }
        else
        {
            cnt.change = 100;
        }
    }
}

void ChangeTimeState_Perform()
{
    if (!Button::current_state)
    {
        StateCtx::SetState(BlinkState);
    }
}
} // States

} // StateCtx

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
