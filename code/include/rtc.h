/*
 * Nom: rtc.h
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 17 jan 2017
 */

#ifndef __RTC_H__
#define __RTC_H__

#include <avr/io.h>
#include "time.hpp"
#include "twi.h"
#include "bcd.hpp"

namespace RTC
{
static const uint8_t _address = 0xA2;
static const uint8_t _reg = 0x04;

static __inline__ Time getTime()
{
    Time t;

    TWI::start();
    TWI::write(_address);
    TWI::write(_reg);
    TWI::restart();

    TWI::write(_address + 1);

    t.s = BCDtoBIN(TWI::read(1));
    t.m = BCDtoBIN(TWI::read(1));
    t.h = BCDtoBIN(TWI::read(0));

    TWI::stop();

    t.normalize();
    return t;
}

static __inline__ uint8_t setTime(Time t)
{
    TWI::start();
    TWI::write(_address);
    TWI::write(_reg);

    TWI::write(BINtoBCD(t.s));
    
    TWI::write(BINtoBCD(t.m));
    
    TWI::write(BINtoBCD(t.h));
    
    TWI::stop();

    return 0;
}

};

#else
#warning "RTC already in use."
#endif
