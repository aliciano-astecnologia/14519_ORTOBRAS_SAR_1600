
#ifndef EVENTS_MANAGER_H
#define EVENTS_MANAGER_H

typedef enum {
   EVENT_LOW = 0,	//Nível baixo
   EVENT_HIGH,    //Nível alto
   EVENT_FALLING,	//Borda de descida
   EVENT_RISING 	//Borda de subida
}EventStatus;

typedef struct {
   char State;
   char Level;   
}Event;

void EventInit(Event *ThisEvent);
EventStatus EventProcess(Event *ThisEvent, char In);

#endif
