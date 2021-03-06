/*
 * Nom: button.h
 * Copyright (C) 2016 Bruno Bousquet
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description:
 * Version: 1.0
 * Date : 17 jan 2017
 */

#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "gpior.h"

namespace Button
{
static const uint8_t current_index = 1;
static const uint8_t previous_index = 2;

static void set_current()
{
  GPIOReg::set(current_index);
}
static void clear_current()
{
  GPIOReg::clear(current_index);
}
static uint8_t get_current()
{
  return GPIOReg::get(current_index);
}
static void set_previous()
{
  GPIOReg::set(previous_index);
}
static void clear_previous()
{
  GPIOReg::clear(previous_index);
}
static uint8_t get_previous()
{
  return GPIOReg::get(previous_index);
}

static uint8_t get_pressed()
{
  return get_current() && !get_previous();
}

static uint16_t state_cnt = 0;
static uint8_t deb_cnt = 4;

static __inline__ void Update()
{
  uint8_t reading = (PIND & _BV(3)) == 0;
  if (get_current() != reading)
  {
    deb_cnt--;
    if (deb_cnt == 0)
    {
      reading ? set_current() : clear_current();
      state_cnt = 0;
    }
  }
  else
  {
    deb_cnt = 4;
  }

  state_cnt += 10;
}

static __inline__ void EnableInterrupt()
{
  EIMSK |= _BV(INT1);
}

static __inline__ void DisableInterrupt()
{
  EIMSK &= ~_BV(INT1);
}

static __inline__ void Init()
{
  // this way, DisplayState won't unlock until the button is released, then pressed
  set_current();
  set_previous();
  state_cnt = 0;

  // mettre le bouton en entree
  DDRD &= ~_BV(3);
  // desactiver le Pull-Up resistor
  PORTD &= ~_BV(3);

  // insure low level interrupt (default)
  EICRA = 0;

  // insure interrupt disabled (default)
  DisableInterrupt();
}
}

#else
#warning "button already in use."
#endif
