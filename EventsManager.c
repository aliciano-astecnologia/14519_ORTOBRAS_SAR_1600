
#include "EventsManager.h"

void EventInit(Event *ThisEvent)
{
	ThisEvent->State = EVENT_LOW;
}

EventStatus EventProcess(Event *ThisEvent, char In)
{
	switch (ThisEvent->State)
	{
		case EVENT_LOW:
			if (In)
			{
				ThisEvent->State = EVENT_RISING;
			}
			break;
			
		case EVENT_HIGH:
			if (!In)
			{
				ThisEvent->State = EVENT_FALLING;
			}
			break;			
			
		case EVENT_FALLING:
			ThisEvent->State = EVENT_LOW;
			break;
			
		case EVENT_RISING:
			ThisEvent->State = EVENT_HIGH;
			break;
	}
	return ThisEvent->State;
}
