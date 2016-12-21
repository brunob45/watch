/*
 * Nom:
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 */

#ifndef __TWI_H__
#define __TWI_H__

#include <avr/io.h>

class TWI
{
public:
	TWI();
	~TWI();
	void start();
	void stop();
	void restart();
	void write(uint8_t t);
	void read(uint8_t& t, uint8_t ack = 0);
private:
	void enable();
	void disable();
	void sendStartCondition();
	void sendStopCondition();
};

static TWI twi;

#endif
