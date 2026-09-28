
#ifndef TICK_TIMER_H
#define TICK_TIMER_H

#include "GenericTypes.h" 

typedef struct TickTimers {
   U32 Start;
   U32 Interval;   
}TickTimer;

void TickTimerReset(TickTimer *T);
void TickTimerRestart(TickTimer *T);
void TickTimerSet(TickTimer *T, U32 Interval);
char TickTimerExpired(TickTimer *T);
U32  TickTimerGet(void);
void TickTimerCounter(void);

#endif
