// AppCiclos.h
#include <stm8s_gpio.h>
#include <gpio.h>
#include <GenericTypes.h>
#include <TickTimer.h>

#define TOTAIS 		0 
#define POS_MANUT   1 

void AppCiclosInit(void);
void AppCiclosInc(void);
void AppCiclosTask(void);

