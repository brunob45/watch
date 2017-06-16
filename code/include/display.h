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

class Display
{
public:
  Display();
  ~Display() {}

  void clear();
  void showTime();
  void showTime(Time t);
  void toggle();
  void setTime(Time t);
};

#else
#warning "display already in use."
#endif