/*
 * Nom: display.cpp
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 13 fev 2017
 */

#include <avr/io.h>
#include <avr/pgmspace.h>

#include "display.h"

// Workaround for http://gcc.gnu.org/bugzilla/show_bug.cgi?id=34734
#ifdef PROGMEM
#undef PROGMEM
#define PROGMEM __attribute__((section(".progmem.data")))
#endif

#define loc_of(x) (uint16_t)(&x)
#define data_of(x) (*(volatile uint8_t *)((uint16_t)x))

static const uint8_t H = 0;
static const uint8_t M = 1;

typedef struct
{
    uint8_t p;
    uint8_t b;
} led_t;

static led_t MEMDATA_H[] PROGMEM =
    {
        {loc_of(PORTD), _BV(0)},
        {loc_of(PORTC), _BV(2)},
        {loc_of(PORTC), _BV(0)},
        {loc_of(PORTC), _BV(7)},
        {loc_of(PORTB), _BV(5)},
        {loc_of(PORTB), _BV(3)},
        {loc_of(PORTB), _BV(0)},
        {loc_of(PORTD), _BV(7)},
        {loc_of(PORTB), _BV(7)},
        {loc_of(PORTA), _BV(0)},
        {loc_of(PORTD), _BV(4)},
        {loc_of(PORTD), _BV(1)}};

static led_t MEMDATA_M[] PROGMEM =
    {
        {loc_of(PORTC), _BV(3)},
        {loc_of(PORTC), _BV(1)},
        {loc_of(PORTA), _BV(1)},
        {loc_of(PORTA), _BV(0)},
        {loc_of(PORTB), _BV(4)},
        {loc_of(PORTB), _BV(2)},
        {loc_of(PORTB), _BV(1)},
        {loc_of(PORTD), _BV(6)},
        {loc_of(PORTD), _BV(5)},
        {loc_of(PORTB), _BV(6)},
        {loc_of(PORTA), _BV(2)},
        {loc_of(PORTD), _BV(2)}};

static uint8_t showing = 0;
static led_t current[2] = {
    {
        0,
    },
    {
        0,
    }};

namespace Display
{

void showTime()
{
    if (showing)
    {
        for (uint8_t port = loc_of(PORTB); port <= loc_of(PORTA); port += 3)
        {
            uint8_t mask = 0;
            if (port == current[H].p)
            {
                mask |= current[H].b;
            }
            if (port == current[M].p)
            {
                mask |= current[M].b;
            }
            data_of(port) = mask;
        }
    }

    showing = 1;
}

void setTime(Time t)
{
    uint16_t dw = pgm_read_word(&MEMDATA_H[t.h / 2]);
    current[H].p = (uint8_t)(dw >> 8);
    current[H].b = (uint8_t)(dw);

    dw = pgm_read_word(&MEMDATA_M[t.m / 5]);
    current[M].p = (uint8_t)(dw >> 8);
    current[M].b = (uint8_t)(dw);
}

void clear()
{
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTA = 0;

    showing = 0;
}

void toggle()
{
    if (showing)
        clear();
    else
        showTime();
}
}
