#include <stm8s.h>
#include <stdio.h>
#include <stdlib.h>
#include <Cpu.h>
#include <GenericTypes.h>
#include <TickTimer.h>
#include <Gpio.h>
#include <Entradas.h>
#include <App.h>
#include <AppDisp7s.h>
#include <AppCiclos.h>
#include <AppTeclasBotoeira.h>
#include "AppFirmwareCode.h"
#include "AppComm.h"

U8 FlagRun = 0;
U8 FlagStart;
extern U8 FlagComOk;
extern U8 FlagTimeoutDisp;
extern U8 FlagDisplayOff;
extern U8 FlagAtividadeLin;

TickTimer T1ms;
TickTimer T10ms;
TickTimer TimeoutCom;
TickTimer T1s;
TickTimer TimeoutDisp; //30s

void main(void)
{
	CpuInit();
	GpioInit();
	
	EntradasInit();
	AppDisp7sInit();
	AppCiclosInit();
	AppInit();
    AppTeclasBotoeiraInit();
	AppComm_Init();
	
	enableInterrupts();

	TickTimerSet(&T1ms,1);
	TickTimerSet(&T10ms,10);
	TickTimerSet(&T1s,1000);
	TickTimerSet(&TimeoutCom,1000);
	TickTimerSet(&TimeoutDisp,30000);
	
	LIGA_CD1;LIGA_CD2;LIGA_CD3;LIGA_CD4;
	LIGA_SEG_A;LIGA_SEG_B;LIGA_SEG_C;LIGA_SEG_D;LIGA_SEG_E;LIGA_SEG_F;LIGA_SEG_G;LIGA_SEG_P;
	FlagStart = 1;

	AppFirmwareCodeCheckEnter();
	
	while(1)
	{
		AppComm_Task();
				
		if(TickTimerExpired(&T1ms))
		{
			TickTimerReset(&T1ms);
			EntradasTask();
		}

		if(TickTimerExpired(&T10ms))
		{
			TickTimerReset(&T10ms);
			IWDG_ReloadCounter();
			AppTask();
			AppCiclosTask();
		}
		
		if(TickTimerExpired(&T1s))
		{
			TickTimerReset(&T1s);
			if(FlagStart) 
			{
				FlagStart = 0;
			}
			
			if(FlagTimeoutDisp)
			{ 
				FlagTimeoutDisp = FlagDisplayOff = 0; 
				TickTimerRestart(&TimeoutDisp); 
			}
		}
		
		if(TickTimerExpired(&TimeoutCom))
		{
			if (comm_ok) {
				TickTimerSet(&TimeoutCom, 1000);
			} else {
				TickTimerSet(&TimeoutCom, 200);
			}
			FlagRun = !FlagRun;
		}
		
		if(TickTimerExpired(&TimeoutDisp))
		{
			TickTimerReset(&TimeoutDisp);
			AppDisp7sInit();
			FlagDisplayOff = 1;
		}
	}
}


