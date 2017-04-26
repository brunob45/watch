/*
 * Nom: time.hpp
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 13 fev 2017
 */

#ifndef __TIME_H__
#define __TIME_H__

#include <avr/io.h>

class Time
{
  public:
    uint8_t h;
    uint8_t m;
    uint8_t s;

  public:
    Time(uint8_t _h = 0, uint8_t _m = 0, uint8_t _s = 0)
	    : h(_h), m(_m), s(_s)
    {
		  normalize();
    }
    ~Time() {}

    void normalize()
    {
      m += s / 60;
      h += m / 60;

      h %= 24;
      m %= 60;
      s %= 60;
    }

    void next(uint8_t minutes)
    {
      m += minutes - (m % minutes);
      s = 0;
      normalize();
    }
};

#endif