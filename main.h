/*
 * Nom: 
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: 
 * Version: 1.0
 */

#ifndef __MAIN_H__
#define __MAIN_H__

#define F_CPU 8000000UL
#define TIMER_PRESCALER 1024UL
 
#include <avr/io.h> 
#include "display.h"
#include "rtc.h"

Time now;


void sleepNow();
void partirMinuterie ( uint16_t duree_ms );
void setup();
void sleepNow();

uint8_t getTime();
void setTime();
uint8_t saveTime();


#endif