/*
 * Nom: display.h
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 13 fev 2017
 */

#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "time.hpp"
#include "gpior.h"

namespace Display
{
static __inline__ void set_show()
{
    GPIOReg::set(3);
}
static __inline__ void clear_show()
{
    GPIOReg::clear(3);
}
static __inline__ uint8_t get_show()
{
    return GPIOReg::get(3);
}

void init();
void clear();
void showTime();
void toggle();
void setTime(Time t);
}

#else
#warning "display already in use."
#endif
