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

namespace Button
{
	void setup();
	uint8_t getDebState();
	uint8_t getState();
	
}

#endif

