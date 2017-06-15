/*
 * Nom: twi.h
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 17 jan 2017
 */

#ifndef __TWI_H__
#define __TWI_H__

namespace TWI
{
static void start();
static void stop();
static void restart();

static void write(uint8_t t);
static uint8_t read(uint8_t ack = 0);

namespace local
{
static void enable();
static void disable();
static void sendStartCondition();
static void sendStopCondition();
}

void start()
{
  local::enable();
  local::sendStartCondition();
}

void stop()
{
  local::sendStopCondition();
  local::disable();
}

void restart()
{
  local::sendStopCondition();
  local::sendStartCondition();
}

uint8_t read(uint8_t ack)
{
  TWCR = _BV(TWINT) | _BV(TWEN) | (ack ? _BV(TWEA) : 0);

  while ((TWCR & _BV(TWINT)) == 0)
    ;
  return TWDR;
}

void write(uint8_t t)
{
  TWDR = t;
  TWCR = _BV(TWINT) | _BV(TWEN);
  while ((TWCR & _BV(TWINT)) == 0)
    ;
}

namespace local
{
void enable()
{
  power_twi_enable(); //enable clock to module
  TWSR = 0;           // Initialisation de l'horloge de l'interface I2C
  TWBR = 10;          //MAX((MAX(F_CPU / F_TWI, 16) - 16) / 2, 10); // prediviseur
  TWHSR = 0;          // _BV(TWIHS); // clock source = F_CPU * 2   /!\ Dont need to /!\ */
}

void disable()
{
  power_twi_disable(); //disable clock to module
}

void sendStartCondition()
{
  TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN); // Condition de depart
  while ((TWCR & _BV(TWINT)) == 0)
    ; // Attente de fin de transmission
}

void sendStopCondition()
{
  TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN); // Condition de depart
  for (uint8_t i = 0; i < TWBR; i++)
    ; // Attendre que la condition soit transmise
}
} // namespace local
} //namespace TWI

#else
#warning "TWI already in use."
#endif
