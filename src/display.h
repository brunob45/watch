/*
 * Nom: display.h
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 13 fev 2017
 */

#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "led.h"
#include "time.hpp"
#include <avr/io.h>

class Display
{
  private:
    LED H[12];
    LED M[12];

  public:
    Display();
    ~Display() {}

    void clear();
    void showTime();
    void showTime(Time t);
    void startFlash(uint8_t delay = 255);
    void stopFlash();
    void toggle();
};

extern Display display;

#else
#warning "display already in use."
#endif
