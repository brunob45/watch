/*
 * Nom: util.h
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 17 jan 2017
 */

#ifndef __UTIL_H__
#define __UTIL_H__

#include <avr/io.h>

typedef void (*EventHandler) (void);

#define MIN(x,y) (x < y ? x : y)
#define MAX(x,y) (x > y ? x : y)

#endif
