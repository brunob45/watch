/*
 * Nom: 
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: 
 * Version: 1.0
 */

#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <avr/io.h>

class Button
{
public:
	Button();
	~Button() {  }

	uint8_t getDebState();
	uint8_t getState();
	
	void enableInterrupt(void (*handler)(void));
	void disableInterrupt();
};

static Button button;

#endif

