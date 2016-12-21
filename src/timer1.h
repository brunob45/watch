/*
 * Nom: 
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: 
 * Version: 1.0
 */

#ifndef __TIMER1_H__
#define __TIMER1_H__

#include "util.h"

class Timer1
{
public:
	EventHandler onTimerOut = 0;

	Timer1(EventHandler handler = 0);
	~Timer1() {  }
	
	void start(const uint16_t top);
	void restart();
	void stop();
	
	void enableInterrupt(EventHandler handler = 0);
	void disableInterrupt();
	
	uint16_t getCounter();
	uint16_t getTime();
};

static Timer1 timer1;

#endif
