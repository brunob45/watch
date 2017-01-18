/*
 * Nom: twi.cpp
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 17 jan 2017
 */

#include "twi.h"
#include "util.h"

TWI::TWI()
{
	disable();
}

TWI::~TWI()
{
	disable();
}

void TWI::start()
{
	enable();
	sendStartCondition();
}

void TWI::stop()
{
	sendStopCondition();
	disable();
}

void TWI::restart()
{
	sendStopCondition();
	sendStartCondition();
}

void TWI::read(uint8_t& t, uint8_t ack)
{
	if(ack)
		TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
	else
		TWCR = _BV(TWINT) | _BV(TWEN);

	while ((TWCR & _BV(TWINT)) == 0);
    t = TWDR;
}

void TWI::write(uint8_t t)
{
	TWDR = t;
	TWCR = _BV(TWINT) | _BV(TWEN);
	while ((TWCR & _BV(TWINT)) == 0);
}

void TWI::enable()
{
	PRR &= ~_BV(PRTWI); //enable clock to module
	TWSR = 0;  // Initialisation de l'horloge de l'interface I2C
	TWBR = MAX((F_CPU  / F_TWI - 16) / 2, 10); // prediviseur
	TWHSR = 0; // _BV(TWIHS); // clock source = F_CPU * 2   /!\ Dont need to /!\
}

void TWI::disable()
{
	PRR |= _BV(PRTWI); //disable clock to module
}

void TWI::sendStartCondition()
{
	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN); // Condition de depart
	while ((TWCR & _BV(TWINT)) == 0);      		// Attente de fin de transmission
}

void TWI::sendStopCondition()
{
	TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN); // Condition de depart
	while ((TWCR & _BV(TWINT)) == 0);      		// Attente de fin de transmission
}
