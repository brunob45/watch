/*
 * Nom: timer1.h
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 13 fev 2017
 */

#ifndef __TIMER1_H__
#define __TIMER1_H__

#include "util.h"

class Timer1
{
public:
  Timer1(EventHandler handler = 0);
  ~Timer1() {}

  void start(const uint16_t top = 65535);
  void start_ms(const uint16_t top_ms = 65535);
  void restart();
  void stop();

  void enableInterrupt(EventHandler handler = 0);
  void disableInterrupt();

  uint16_t getCounter();
  uint16_t getTime();
  uint8_t isActive();

  void resetCounter(uint16_t cnt = 0);
  uint16_t getTop();
};

#else
#warning "timer1 already in use."
#endif
