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
#include "time.h"
#include "twi.h"

class RTC
{
private:
	const uint8_t _address;
	const uint8_t _reg;

public:
	RTC(uint8_t address = 0xA2, uint8_t reg = 0x0A) : _address(address), _reg(reg) {  }
	~RTC() {  }
	
	Time getTime();
	uint8_t setTime(Time);
};

extern RTC rtc;

#else
#warning "RTC already in use."
#endif
