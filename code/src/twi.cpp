/*
 * Nom: twi.cpp
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 17 jan 2017
 */

#include "twi.h"
#include <avr/power.h>

void TWI::init()
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

uint8_t TWI::read(uint8_t ack)
{
    TWCR = _BV(TWINT) | _BV(TWEN) | (ack ? _BV(TWEA) : 0);

    while ((TWCR & _BV(TWINT)) == 0)
	;
    return TWDR;
}

void TWI::write(uint8_t t)
{
    TWDR = t;
    TWCR = _BV(TWINT) | _BV(TWEN);
    while ((TWCR & _BV(TWINT)) == 0)
	;
}

void TWI::enable()
{
    power_twi_enable();           //enable clock to module
    TWSR = 0;				      // Initialisation de l'horloge de l'interface I2C
    TWBR = 10;/*MAX((F_CPU / F_TWI - 16) / 2, 10);*/ // prediviseur
    TWHSR = 0;				      // _BV(TWIHS); // clock source = F_CPU * 2   /!\ Dont need to /!\ */
}

void TWI::disable()
{
    power_twi_disable(); //disable clock to module
}

void TWI::sendStartCondition()
{
    TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN); // Condition de depart
    while ((TWCR & _BV(TWINT)) == 0)
	; // Attente de fin de transmission
}

void TWI::sendStopCondition()
{
    TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN); // Condition de depart
    for (uint8_t i = 0; i < TWBR; i++)
	; // Attendre que la condition soit transmise
}
