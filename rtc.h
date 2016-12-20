/*
 * Nom: 
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: 
 * Version: 1.0
 */

#ifndef __RTC_H__
#define __RTC_H__

#ifndef F_CPU
#define F_CPU 4000000UL
#warning "F_CPU non defini dans <rtc.h>"
#endif

#define F_TWI 100000UL

#include <avr/io.h>
//#include <util/twi.h>
#include "time.h"

namespace RTC
{
	void setup();
	
	Time getTime();
	uint8_t setTime(Time);
	
	void start();
	void stop();
	void restart();
	void write(uint8_t t);
	void read(uint8_t& t, uint8_t ack = 0);
};

#endif