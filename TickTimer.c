#include "TickTimer.h"

volatile U32 Ticks = 0;
volatile U32 Ticks1 = 0;
U8 IncrementTicks = 0;

void TickTimerReset(TickTimer *T)
{
   T->Start = T->Start + T->Interval;      
}

void TickTimerRestart(TickTimer *T)
{
   T->Start = TickTimerGet();      
}

void TickTimerSet(TickTimer *T, U32 Interval)
{
   T->Start = TickTimerGet();
   T->Interval = Interval;
}

char TickTimerExpired(TickTimer *T)
{  
   if (TickTimerGet() - T->Start >= T->Interval)
   {
      return 1;
   }
   return 0;
}

U32 TickTimerGet(void)
{
   U32 _Ticks;
   
   IncrementTicks = 0; //Atomic access 
   _Ticks = Ticks1; 
   IncrementTicks = 1;
   
   return _Ticks;
}

void TickTimerCounter(void)
{
   Ticks ++; 

   if (IncrementTicks)
   {
    Ticks1 = Ticks;
   }
}



/*#include "TickTimer.h"

volatile U32 Ticks = 0;

void TickTimerReset(TickTimer *T)
{
   T->Start = T->Start + T->Interval;      
}

void TickTimerRestart(TickTimer *T)
{
   T->Start = Ticks;      
}

void TickTimerSet(TickTimer *T, U32 Interval)
{
   T->Start = Ticks;
   T->Interval = Interval;
}

char TickTimerExpired(TickTimer *T)
{
   if (Ticks - T->Start >= T->Interval)
   {
      return 1;
   }
   return 0;
}

U32 TickTimerGet(void)
{
   return Ticks;
}

void TickTimerCounter(void)
{
   Ticks ++;   
}*/
