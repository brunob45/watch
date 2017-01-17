/*
 * Nom: timer1.h
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 17 jan 2017
 */

#ifndef __TIMER1_H__
#define __TIMER1_H__

#include "util.h"

class Timer1
{
public:

	Timer1(EventHandler handler = 0);
	~Timer1() {  }
	
	void start(const uint16_t top);
	void restart();
	void stop();
	
	void enableInterrupt(EventHandler handler = 0);
	void disableInterrupt();
	
	uint16_t getCounter();
	uint16_t getTime();
	void resetCounter(uint16_t cnt = 0);
	uint16_t getTop();
};

extern Timer1 timer1;

#else
#warning "timer1 already in use."
#endif
