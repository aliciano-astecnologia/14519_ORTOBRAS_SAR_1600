// Cpu.c
#include "Cpu.h"

void CpuInit(void)
{
	CLK_HSIPrescalerConfig (CLK_PRESCALER_HSIDIV1);
	
	TIM4_TimeBaseInit(TIM4_PRESCALER_128,124); //1ms
	TIM4_ClearFlag(TIM4_FLAG_UPDATE);
	TIM4_ITConfig(TIM4_IT_UPDATE,ENABLE);
	TIM4_Cmd(ENABLE);
   
  IWDG_Enable();
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_128);
	IWDG_SetReload((uint8_t)(128000/512));
	IWDG_ReloadCounter();
}
