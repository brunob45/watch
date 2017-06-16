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

namespace Button
{
static uint8_t current_state = 0;
static uint8_t previous_state = 0;
static uint16_t pressed_cnt = 0;
static uint8_t deb_cnt = 0;

static __inline__ void Update()
{
  previous_state = current_state;

  uint8_t reading = PORTC & _BV(3);
  if (current_state ^ reading)
  {
    deb_cnt--;
    if (deb_cnt == 0)
    {
      current_state = reading;
    }
  }
  else
  {
    deb_cnt = 4;
  }

  if (current_state)
  {
    pressed_cnt++;
  }
  else
  {
    pressed_cnt = 0;
  }
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
  current_state = 0;
  previous_state = 0;
  pressed_cnt = 0;

  // mettre le bouton en entree
  DDRD &= ~_BV(3);
  // desactiver le Pull-Up resistor
  PORTD &= ~_BV(3);

  // low level interrupt by default
  DisableInterrupt();
}
}

#else
#warning "button already in use."
#endif
