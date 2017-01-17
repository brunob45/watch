/*
 * Nom: led.h
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 17 jan 2017
 */

#ifndef __LED_H__
#define __LED_H__

#include <avr/io.h>

#define BUTTON  3              // D3

#define SCL     PORTC5		      // C5
#define SDA     PORTC4		      // C4

#define MISO	PORTB4	
#define MOSI	PORTB3	
#define SCK 	PORTB5

#define RST		PORTC7			  // C7


class LED
{
	volatile uint8_t *_port;
	uint8_t _bit;
public:
	LED() : _port(0), _bit(0){}
	LED(volatile uint8_t *port, uint8_t bit) : _port(port), _bit(bit) 
	{
		// set LED pin as output
		*(_port-1) |= _BV(_bit);
	}
	void turnOn()
	{
		if(_port) *_port |= _BV(_bit);
	}
	void turnOff()
	{
		if(_port) *_port &= ~_BV(_bit);
	}
};

#endif