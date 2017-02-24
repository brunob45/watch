/*
 * Nom: main.h
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 13 fev 2017
 */

#ifndef __MAIN_H__
#define __MAIN_H__

#ifndef F_CPU
#warning "F_CPU not defined in <main.h>"
#define F_CPU 4000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void wakeUpSequence();
void sleepSequence();
void sleepNow();
void setTime();
void onButtonPressed();
void setup();


#endif