/*
 * Nom: button.h
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 17 jan 2017
 */

#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <avr/io.h>

class Button
{
  public:
    Button();
    ~Button() {}

    uint8_t getDebState();
    uint8_t getState();

    void enableInterrupt(void (*handler)(void));
    void disableInterrupt();
};

#else
#warning "button already in use."
#endif
