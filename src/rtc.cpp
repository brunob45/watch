/*
 * Nom: 
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: 
 * Version: 1.0
 */

#include "rtc.h"

Time RTC::getTime()
{
	Time t;

	twi.start();
	twi.write(_address);
	twi.write(_reg);
	twi.restart();
	
	twi.write(_address + 1);
	twi.read(t.s, 1);
	twi.read(t.m, 1);
	twi.read(t.h, 0);
	
	twi.stop();
	
	return t;
}

uint8_t RTC::setTime(Time t)
{
	twi.start();
	twi.write(_address);
	twi.write(_reg);
	
	twi.write(t.s);
	twi.start();
	
	twi.write(t.m);
	twi.start();
	
	twi.write(t.h);
	twi.stop();
	
	return 0;
}

