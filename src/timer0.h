/*
 * Nom: timer0.h
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 13 fev 2017
 */

#ifndef __TIMER0_H__
#define __TIMER0_H__

#include "util.h"

class Timer0
{
  public:
    Timer0(EventHandler handler = 0);
    ~Timer0() {}

    void start(const uint8_t top = 255);
    void start_ms(const uint8_t top_ms = 255);
    void restart();
    void stop();

    void enableInterrupt(EventHandler handler = 0);
    void disableInterrupt();

    uint8_t getCounter();
    uint8_t getTime();
    uint8_t isActive();

    void resetCounter(uint8_t cnt = 0);
    uint16_t getTop();
};

extern Timer0 timer0;

#else
#warning "timer0 already in use."
#endif
