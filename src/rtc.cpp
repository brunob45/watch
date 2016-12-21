/*
 * Nom: 
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: 
 * Version: 1.0
 */

#include "rtc.h"
#include <avr/io.h>


const uint8_t address = 0xA2;
	
void RTC::setup()
{
   // Initialisation de l'horloge de l'interface I2C
   TWSR = 0;
   
   // prediviseur
   TWBR =  (F_CPU * 2 / F_TWI - 16) / 2 > 10 ? (F_CPU * 2 / F_TWI - 16) / 2 : 10; 
   
   // clock source = F_CPU * 2
   TWHSR = _BV(TWIHS);
}

void RTC::start()
{
	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN); // Condition de depart
	while ((TWCR & _BV(TWINT)) == 0);      		// Attente de fin de transmission
}

void RTC::stop()
{
	TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN); // Condition de depart
	while ((TWCR & _BV(TWINT)) == 0);      		// Attente de fin de transmission
}

void RTC::restart()
{
	stop();
	start();
}

void RTC::read(uint8_t& t, uint8_t ack)
{
	if(ack)
		TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA);
	else 
		TWCR = _BV(TWINT) | _BV(TWEN);
	
	while ((TWCR & _BV(TWINT)) == 0);
    t = TWDR;
}

void RTC::write(uint8_t t)
{
	TWDR = t;
	TWCR = _BV(TWINT) | _BV(TWEN);
	while ((TWCR & _BV(TWINT)) == 0);
}

Time RTC::getTime()
{
	Time t;

	start();
	write(address);
	write(0x0A);
	restart();
	
	write(address + 1);
	read(t.s, 1);
	read(t.m, 1);
	read(t.h, 0);
	
	stop();
	
	return t;
}

uint8_t RTC::setTime(Time t)
{
	start();
	write(address);
	write(0x0A);
	
	write(t.s);
	start();
	
	write(t.m);
	start();
	
	write(t.h);
	stop();
	
	return 0;
}

