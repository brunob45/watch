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

typedef struct
{
    uint8_t p;
    uint8_t b;
} led_t;

static struct
{
    led_t m;
    led_t h;
} current;

static led_t MEMDATA_H[] PROGMEM =
    {
        {loc_of(PORTD), _BV(0)},  // 0
        {loc_of(PORTC), _BV(2)},  // 1
        {loc_of(PORTC), _BV(0)},  // 2
        {loc_of(PORTC), _BV(7)},  // 3
        {loc_of(PORTB), _BV(5)},  // 4
        {loc_of(PORTB), _BV(3)},  // 5
        {loc_of(PORTB), _BV(0)},  // 6
        {loc_of(PORTD), _BV(7)},  // 7
        {loc_of(PORTB), _BV(7)},  // 8
        {loc_of(PORTA), _BV(3)},  // 9
        {loc_of(PORTD), _BV(4)},  // 10
        {loc_of(PORTD), _BV(1)}}; // 11

static led_t MEMDATA_M[] PROGMEM =
    {
        {loc_of(PORTC), _BV(3)},  // 0
        {loc_of(PORTC), _BV(1)},  // 1
        {loc_of(PORTA), _BV(1)},  // 2
        {loc_of(PORTA), _BV(0)},  // 3
        {loc_of(PORTB), _BV(4)},  // 4
        {loc_of(PORTB), _BV(2)},  // 5
        {loc_of(PORTB), _BV(1)},  // 6
        {loc_of(PORTD), _BV(6)},  // 7
        {loc_of(PORTD), _BV(5)},  // 8
        {loc_of(PORTB), _BV(6)},  // 9
        {loc_of(PORTA), _BV(2)},  // 10
        {loc_of(PORTD), _BV(2)}}; // 11

namespace Display
{

void init()
{
    DDRB = 0xFF;
    DDRC = 0x8F;
    DDRD = 0xF7;
    DDRA = 0x0F;
}

void showTime()
{
    for (uint8_t port = loc_of(PORTB); port <= loc_of(PORTA); port += 3)
    {
        uint8_t mask = 0;
        if (port == current.h.p)
        {
            mask |= current.h.b;
        }
        if (port == current.m.p)
        {
            mask |= current.m.b;
        }
        data_of(port) = mask;
    }

    set_show();
}

void setTime(Time t)
{
    uint16_t dw = pgm_read_word(&MEMDATA_H[t.h % 12]);
    current.h.b = (uint8_t)(dw >> 8);
    current.h.p = (uint8_t)(dw);

    dw = pgm_read_word(&MEMDATA_M[t.m / 5]);
    current.m.b = (uint8_t)(dw >> 8);
    current.m.p = (uint8_t)(dw);
}

void clear()
{
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTA = 0;

    clear_show();
}

void toggle()
{
    get_show() ? clear() : showTime();
}
}
