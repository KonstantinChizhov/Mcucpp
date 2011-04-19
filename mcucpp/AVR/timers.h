#pragma once

#ifdef PLLCSR
#define USE_TINNY_TIMER_1
#endif

#ifdef TCNT0
#define USE_TIMER_0
#endif

#if defined(TCNT1) && !defined(USE_TINNY_TIMER_1)
#define USE_TIMER_1
#endif

#ifdef TCNT2
#define USE_TIMER_2
#endif

#ifdef USE_TIMER_0
#include "AVR/timer0.h"
#endif

#ifdef USE_TIMER_1
#include "AVR/timer1.h"
#endif

#ifdef USE_TIMER_2
#include "AVR/timer2.h"
#endif

#ifdef USE_TINNY_TIMER_1
#include "AVR/tinny_timer1.h"
#endif