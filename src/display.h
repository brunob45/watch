/*
 * Nom: 
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: 
 * Version: 1.0
 */

#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "led.h"
#include "time.h"
#include <avr/io.h>

namespace Display
{
	void setup();
	void clear();
	void showTime();
	void showTime(Time);
	void startFlash(uint8_t delay);
	void stopFlash();
	
	static LED H[12];
	static LED M[12];
	
	void onTimerOut();
};

#endif