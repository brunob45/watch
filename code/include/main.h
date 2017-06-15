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

enum state_t
{
    SleepState,
    WakeUpState,
    DisplayState,
    BlinkState,
    ChangeTimeState,
    InvalidState
};

namespace StateCtx
{
static __inline__ void Init();
static __inline__ void Update();
static __inline__ void Perform();
static __inline__ void SetState(state_t s);

namespace States
{
static __inline__ void SleepState_Update();
static __inline__ void SleepState_Perform();

static __inline__ void WakeUpState_init();
static __inline__ void WakeUpState_Update();

static __inline__ void DisplayState_init();
static __inline__ void DisplayState_Update();
static __inline__ void DisplayState_Perform();

static __inline__ void BlinkState_init();
static __inline__ void BlinkState_Update();
static __inline__ void BlinkState_Perform();

static __inline__ void ChangeTimeState_init();
static __inline__ void ChangeTimeState_Update();
static __inline__ void ChangeTimeState_Perform();
} // States
} // StateCtx

#endif