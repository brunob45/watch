/*
 * Nom: button.cpp
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 13 fev 2017
 */

#include "button.h"

#include <util/delay.h>
#include <avr/interrupt.h>
#include "util.h"

static uint8_t _state = 0;
static EventHandler onButtonPressed = 0;

Button::Button()
{
    // mettre le bouton en entree
    DDRD &= ~_BV(3);
    // d�sactiver le Pull-Up resistor
    PORTD &= ~_BV(3);

    // il faut sensibiliser les interruptions externes aux
    // changements de niveau (falling edge) du bouton-poussoir
    // en ajustant le registre EICRA
    EICRA = _BV(ISC11);
}

uint8_t Button::getDebState()
{
    //uint8_t new_state = getState();
    _delay_ms(50);
    return getState(); //(_state = getState()) == new_state;
}

uint8_t Button::getState()
{
    return _state = !(PIND & _BV(3));
}

void Button::enableInterrupt(EventHandler handler = 0)
{
    if (handler)
		onButtonPressed = handler;
    EIMSK |= _BV(INT1);
}

void Button::disableInterrupt()
{
    EIMSK &= ~_BV(INT1);
}

ISR(INT1_vect)
{
    if (onButtonPressed)
		onButtonPressed();
}
