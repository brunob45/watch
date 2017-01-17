/*
 * Nom: timer0.h
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 17 jan 2017
 */

#ifndef __TIMER0_H__
#define __TIMER0_H__

#include "util.h"

class Timer0
{
public:
	Timer0(EventHandler handler = 0);
	~Timer0() {  }
	
	void start(const uint8_t top);
	void restart();
	void stop();
	
	void enableInterrupt(EventHandler handler = 0);
	void disableInterrupt();
	
	uint8_t getCounter();
	uint8_t getTime();
};

extern Timer0 timer0;

#else
#warning "timer0 already in use."
#endif
