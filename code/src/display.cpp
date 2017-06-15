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
        {loc_of(PORTD), _BV(1)}
};

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
        {loc_of(PORTD), _BV(2)}
};

static uint8_t showing = 0;
static led_t current[2] = {
    {
        0,
    },
    {
        0,
    }};
//static Time displayTime;

Display::Display()
{ /*
    outerRing[0] = LED(&PORTC, 3);  // C3
    outerRing[1] = LED(&PORTC, 1);  // C1
    outerRing[2] = LED(&PORTA, 1);  // A1
    outerRing[3] = LED(&PORTA, 0);  // A0
    outerRing[4] = LED(&PORTB, 4);  // B4
    outerRing[5] = LED(&PORTB, 2);  // B2
    outerRing[6] = LED(&PORTB, 1);  // B1
    outerRing[7] = LED(&PORTD, 6);  // D6
    outerRing[8] = LED(&PORTD, 5);  // D5
    outerRing[9] = LED(&PORTB, 6);  // B6
    outerRing[10] = LED(&PORTA, 2); // A2
    outerRing[11] = LED(&PORTD, 2); // D2

    innerRing[0] = LED(&PORTD, 0);  // D0
    innerRing[1] = LED(&PORTC, 2);  // C2
    innerRing[2] = LED(&PORTC, 0); // C0
    innerRing[3] = LED(&PORTC, 7); // C7
    innerRing[4] = LED(&PORTB, 5); // B5
    innerRing[5] = LED(&PORTB, 3); // B3
    innerRing[6] = LED(&PORTB, 0); // B0
    innerRing[7] = LED(&PORTD, 7); // D7
    innerRing[8] = LED(&PORTB, 7); // B7
    innerRing[9] = LED(&PORTA, 3); // A3
    innerRing[10] = LED(&PORTD, 4); // D4
    innerRing[11] = LED(&PORTD, 1); // D1*/
}

void Display::showTime()
{
    if (showing)
    {
        for (uint8_t port = loc_of(PORTB); port <= loc_of(PORTA); port += 3)
        {
            if (current[H].p == port || current[M].p == port)
            {
                if (current[H].p == current[M].p)
                {
                    data_of(current[H].p) = current[H].b | current[M].b;
                }
                else
                {
                    data_of(current[H].p) = current[H].b;
                    data_of(current[M].p) = current[M].b;
                }
            }
            else
            {
                data_of(port) = 0;
            }
        }
    }

    showing = 1;
}

void Display::showTime(Time t)
{
    setTime(t);

    /*if (showing)
        clear(); // clear old time*/

    showTime();
}

void Display::setTime(Time t)
{
    uint16_t dw = pgm_read_word(&MEMDATA_H[t.h / 2]);
    current[H].p = (uint8_t)(dw >> 8);
    current[H].b = (uint8_t)(dw);

    dw = pgm_read_word(&MEMDATA_M[t.m / 5]);
    current[M].p = (uint8_t)(dw >> 8);
    current[M].b = (uint8_t)(dw);
}

void Display::clear()
{
    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    PORTA = 0;

    showing = 0;
}

void Display::toggle()
{
    if (showing)
        clear();
    else
    {
        //clear();
        showTime();
    }
}
