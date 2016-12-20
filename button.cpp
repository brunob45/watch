/*
 * Nom: 
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: 
 * Version: 1.0
 */

#include "button.h"
#include "util/delay.h"
uint8_t _state = 0;

void Button::setup()
{
	// Cette procédure ajuste le registre EIMSK
	// de ATmega324PA pour permettre les interruptions externes.
	EIMSK |= _BV(INT1);
	// il faut sensibiliser les interruptions externes aux
	// changements de niveau du bouton-poussoir
	// en ajustant le registre EICRA
	EICRA = _BV(ISC10) | _BV(ISC11);
}

uint8_t Button::getDebState()
{
	uint8_t new_state = Button::getState();
	_delay_ms(20);
	return Button::getState() == new_state;
}

uint8_t Button::getState()
{
	return PIND & _BV(BUTTON);
}