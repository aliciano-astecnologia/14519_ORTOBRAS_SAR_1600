#include "AppCiclos.h"
#include <AppDisp7s.h>
#include <Cpu.h>
#include "App.h"
#include "Gpio.h"
#include "EventsManager.h"
#include "App.h"

static U8 FlagEnableTecla;
static U8 FlagDisableTecla;
static U8 FlagSalvaManutencao;
U32 Ciclos;
U32 CiclosTotais;
U32 CiclosPosManutencao;
U8 FlagEnableViewCiclos;
static U8 Mem[8];

static TickTimer TimerTeclaPressionada;
static TickTimer TimerMostraManutencao;
@near static Event EvHandlerSensorFimCursoSubida;

static void AppCiclosSave(void);

void AppCiclosInit(void)
{
	U16 i;

	FLASH_Unlock(FLASH_MEMTYPE_DATA);

	/* Ciclos totais. */
	Mem[7] = FLASH_ReadByte(0x004000);
	Mem[6] = FLASH_ReadByte(0x004001);
	Mem[5] = FLASH_ReadByte(0x004002);
	Mem[4] = FLASH_ReadByte(0x004003);

	/* Ciclos desde a ultima manutencao. */
	Mem[3] = FLASH_ReadByte(0x004004);
	Mem[2] = FLASH_ReadByte(0x004005);
	Mem[1] = FLASH_ReadByte(0x004006);
	Mem[0] = FLASH_ReadByte(0x004007);
	FLASH_Lock(FLASH_MEMTYPE_DATA);

	CiclosTotais = Mem[7];
	CiclosTotais = CiclosTotais << 8;
	CiclosTotais = CiclosTotais + Mem[6];
	CiclosTotais = CiclosTotais << 8;
	CiclosTotais = CiclosTotais + Mem[5];
	CiclosTotais = CiclosTotais << 8;
	CiclosTotais = CiclosTotais + Mem[4];

	CiclosPosManutencao = Mem[3];
	CiclosPosManutencao = CiclosPosManutencao << 8;
	CiclosPosManutencao = CiclosPosManutencao + Mem[2];
	CiclosPosManutencao = CiclosPosManutencao << 8;
	CiclosPosManutencao = CiclosPosManutencao + Mem[1];
	CiclosPosManutencao = CiclosPosManutencao << 8;
	CiclosPosManutencao = CiclosPosManutencao + Mem[0];

	/* Mantem o recurso ja existente de zerar os contadores na energizacao. */
	for(i = 0; i < 10000; i++)
	{
		if(!(GPIO_ReadInputPin(TECLA_PIN)))
		{
			CiclosTotais = 9999;
			CiclosPosManutencao = 9999;
			AppCiclosInc();
			break;
		}
	}

	FlagEnableTecla = 0;
	FlagDisableTecla = 0;
	FlagSalvaManutencao = 0;
	FlagEnableViewCiclos = TOTAIS;
	AppDisp7sUpdate();

	EventInit(&EvHandlerSensorFimCursoSubida);
}

void AppCiclosInc(void)
{
	if(CiclosTotais < 9999) CiclosTotais++;
	else                    CiclosTotais = 0;

	if(CiclosPosManutencao < 9999) CiclosPosManutencao++;
	else                         CiclosPosManutencao = 0;

	disableInterrupts();
	FLASH_Unlock(FLASH_MEMTYPE_DATA);
	FLASH_ProgramWord(0x004000, CiclosTotais);
	FLASH_ProgramWord(0x004004, CiclosPosManutencao);
	FLASH_Lock(FLASH_MEMTYPE_DATA);
	enableInterrupts();
}

static void AppCiclosSave(void)
{
	disableInterrupts();

	FLASH_Unlock(FLASH_MEMTYPE_DATA);
	FLASH_ProgramWord(0x004000, CiclosTotais);
	FLASH_ProgramWord(0x004004, CiclosPosManutencao);
	FLASH_Lock(FLASH_MEMTYPE_DATA);

	enableInterrupts();
}

static void ControleIncrementaCiclos(void)
{
	static EventStatus Evento;
	Evento = EventProcess(&EvHandlerSensorFimCursoSubida,
	                      SENSOR_LIMITE_SUBIDA_ATIVO);

	if ((AppGetState() == APP_STATE_SOBE) &&
	    SINAL_ONIBUS_ATIVO &&
	    (Evento == EVENT_RISING))
	{
		AppDisplayResetTimeOut();
		AppCiclosInc();
		AppDispErro(0);
		AppDisp7sUpdate();
	}
}

void AppCiclosTask(void)
{
	ControleIncrementaCiclos();

	if(TECLA)
	{
		if(!FlagEnableTecla)
		{
			AppDisplayResetTimeOut();
			FlagEnableTecla = 1;
			FlagDisableTecla = 0;
			FlagSalvaManutencao = 0;
			FlagEnableViewCiclos = TOTAIS;
			AppDisp7sUpdate();
			TickTimerSet(&TimerMostraManutencao, 200);
			TickTimerSet(&TimerTeclaPressionada, 10000);
		}

		if(TickTimerExpired(&TimerMostraManutencao) &&
		   (FlagEnableViewCiclos != POS_MANUT))
		{
			FlagEnableViewCiclos = POS_MANUT;
			AppDisp7sUpdate();
		}

		if(TickTimerExpired(&TimerTeclaPressionada) && !FlagSalvaManutencao)
		{
			AppDisplayResetTimeOut();
			FlagSalvaManutencao = 1;
			CiclosPosManutencao = 0;
			AppCiclosSave();
			FlagEnableViewCiclos = POS_MANUT;
			AppDisp7sUpdate();
		}
	}
	else
	{
		if(!FlagDisableTecla)
		{
			AppDisplayResetTimeOut();
			FlagDisableTecla = 1;
			FlagEnableViewCiclos = TOTAIS;
			AppDisp7sUpdate();
		}
		FlagEnableTecla = 0;
		FlagSalvaManutencao = 0;
	}
}
