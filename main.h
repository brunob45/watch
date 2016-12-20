/*
 * Nom: 
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: 
 * Version: 1.0
 */

#ifndef __MAIN_H__
#define __MAIN_H__

#ifndef F_CPU
#warning "F_CPU not defined in <main.h>"
#define F_CPU 4000000UL
#endif

 
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