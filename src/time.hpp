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
public :
	uint8_t h;
	uint8_t m;
	uint8_t s;
public :
	Time(uint8_t _h = 0, uint8_t _m = 0, uint8_t _s = 0)
        : h(_h), m(_m), s(_s)
    {
        normalize();
    }
    ~Time() {  }
    
	void increment(uint8_t minutes = 5)
	{
		uint8_t m_temp = this->m + minutes; 
		this->m = m_temp %60;
		this->h = (this->h + (m_temp/60)) %24;
	}
	void normalize()
	{
        m += s/60;
        h += m/60;
        
        h %= 24;
        m %= 60;
        s %= 60;
	}
    /*
    Time operator+ (uint8_t r)
    {
        return Time(h, m+r, s);
    }
    Time operator++ ()
    {
        return Time(h, m+1, s);
    }
    
   Time operator+= (uint8_t r)
    {
        return Time(h, m+r, s);
    }*/
};
#endif