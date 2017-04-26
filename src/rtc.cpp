/*
 * Nom: rtc.cpp
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 17 jan 2017
 */

#include "rtc.h"
#include "twi.h"
#include "bcd.hpp"

TWI twi;

Time RTC::getTime()
{
    Time t;

    twi.start();
    twi.write(_address);
    twi.write(_reg);
    twi.restart();

    twi.write(_address + 1);

    t.s = BCDtoBIN(twi.read(1));
    t.m = BCDtoBIN(twi.read(1));
    t.h = BCDtoBIN(twi.read(0));

    twi.stop();

    t.normalize();
    return t;
}

uint8_t RTC::setTime(Time t)
{
    twi.start();
    twi.write(_address);
    twi.write(_reg);

    twi.write(BINtoBCD(t.s));
    
    twi.write(BINtoBCD(t.m));
    
    twi.write(BINtoBCD(t.h));
    
    twi.stop();

    return 0;
}
