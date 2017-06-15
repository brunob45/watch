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

#include <avr/io.h>

namespace TWI
{
  void init();
  void start();
  void stop();
  void restart();
  void write(uint8_t t);
  uint8_t read(uint8_t ack = 0);

  void enable();
  void disable();
  void sendStartCondition();
  void sendStopCondition();
};

#else
#warning "TWI already in use."
#endif
