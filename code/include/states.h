#ifndef __STATES_H__
#define __STATES_H__

static Time time;

enum state_t
{
    SleepState,
    WakeUpState,
    DisplayState,
    BlinkState,
    ChangeTimeState,
    InvalidState
};

namespace StateCtx
{
static __inline__ void Init();
static __inline__ void Update();
static __inline__ void Perform();
static __inline__ void SetState(state_t s);

namespace States
{
static __inline__ void SleepState_Update();
static __inline__ void SleepState_Perform();

static __inline__ void WakeUpState_init();
static __inline__ void WakeUpState_Update();

static __inline__ void DisplayState_init();
static __inline__ void DisplayState_Update();

static __inline__ void BlinkState_init();
static __inline__ void BlinkState_Update();
static __inline__ void BlinkState_Perform();

static __inline__ void ChangeTimeState_init();
static __inline__ void ChangeTimeState_Update();
static __inline__ void ChangeTimeState_Perform();
} // States

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
        RTC::setTime(time);
    }

    Display::shutDown();

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

        TCCR0A = _BV(CTC0) | _BV(CS00);
        TCNT0 = 0;
        OCR0A = 0xff / 2;
        TIMSK0 = _BV(OCIE0A);

        Button::Init();

        time = RTC::getTime();
        Display::init();
        Display::setTime(time);
        Display::showTime();
    }
    sei();
}

void WakeUpState_Update()
{
    StateCtx::SetState(DisplayState);
}

static union cnt_t {
    uint8_t lock;
    uint8_t blink;
    uint8_t change;
} cnt = {0};

void DisplayState_init()
{
    cnt.lock = 1;
    Display::showTime();
}

void DisplayState_Update()
{
    if (cnt.lock == 0)
    {
        if (Button::get_current() && Button::state_cnt > 2000)
        {
            StateCtx::SetState(BlinkState);
        }
        else if (Button::state_cnt > 5000)
        {
            StateCtx::SetState(SleepState);
        }
    }
    else
    {
        if (Button::get_pressed())
        {
            cnt.lock = 0;
        }
    }
}

void BlinkState_init()
{
    cnt.blink = 250;
}

void BlinkState_Update()
{
    if (Button::get_pressed())
    {
        StateCtx::SetState(ChangeTimeState);
    }

    if (Button::state_cnt > 5000)
    {
        StateCtx::SetState(SleepState);
    }
}

void BlinkState_Perform()
{
    cnt.blink--;
    if (cnt.blink == 0)
    {
        Display::toggle();
        cnt.blink = 250;
    }
}

void ChangeTimeState_init()
{
    Display::showTime();
    cnt.change = 250;
}

void ChangeTimeState_Update()
{
    if (!Button::get_current())
    {
        StateCtx::SetState(BlinkState);
    }
}

void ChangeTimeState_Perform()
{
    static uint8_t change_cnt = 0;

    if (Button::state_cnt > cnt.change)
    {
        Button::state_cnt = 0;

        time.next(5);
        Display::setTime(time);
        Display::showTime();

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
} // States

} // StateCtx

#endif // __STATES_H__