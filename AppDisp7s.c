// AppDisp7s.c
#include <AppDisp7s.h>

U8 Digito[4];
U8 ContDigito;
U8 FlagDisplayOff;
extern U32 Ciclos;
extern U32 CiclosTotais;
extern U32 CiclosPosManutencao;
extern U8 FlagEnableViewCiclos;

U8 FlagAlarme;
U8 FlagAlarme1;
U8 FlagAlarme2;

extern U8 FlagRun;
extern U8 FlagStart;
U8 FlagTimeoutDisp = 0;

void AppDisp7sInit(void)	
{
	DESLIGA_CD1;DESLIGA_CD2;DESLIGA_CD3;DESLIGA_CD4;
	DESLIGA_ALL_DIG;
	ContDigito = 0;
	FlagDisplayOff = 0;
	FlagAlarme = 0;
	FlagAlarme1 = 0;
	FlagAlarme2 = 0;
}

void AppDispErro(U8 erro) {

	if(erro == 1) {
		FlagAlarme = 1;
		FlagAlarme1 = 0;
		FlagAlarme2 = 0;
	} else if (erro == 2){
		FlagAlarme = 0;
		FlagAlarme1 = 1;
		FlagAlarme2 = 0;
	} else if (erro == 3){
		FlagAlarme = 0;
		FlagAlarme1 = 0;
		FlagAlarme2 = 1;
	}  else {
		FlagAlarme = 0;
		FlagAlarme1 = 0;
		FlagAlarme2 = 0;
	}
}
	
void AppDisp7sUpdate(void)
{
	if(FlagEnableViewCiclos == TOTAIS) Ciclos = CiclosTotais;
	else                               Ciclos = CiclosPosManutencao;

	if(FlagAlarme)
	{ 
		Digito[0] = 'F'; 
		Digito[1] = 1; 
		Digito[2] = Digito[3] = 'N';
		FlagAlarme = 0;
	
	} else if(FlagAlarme1) {
		Digito[0] = 'F'; 
		Digito[1] = 2; 
		Digito[2] = Digito[3] = 'N';
		FlagAlarme1 = 0;
	
	} else if(FlagAlarme2) {
		Digito[0] = 'F'; 
		Digito[1] = 3; 
		Digito[2] = Digito[3] = 'N';
		FlagAlarme2 = 0;
	}
	else 
	{
		Digito[0] = Ciclos / 1000;
		Digito[1] = (Ciclos % 1000) / 100;
		Digito[2] = (Ciclos / 10) % 10;
		Digito[3] = Ciclos % 10;
	}	
	
}

void AppDisp7sSetDigito(char Dig1, char Dig2, char Dig3, char Dig4)
{
	Digito[0] = Dig1;
	Digito[1] = Dig2;
	Digito[2] = Dig3;
	Digito[3] = Dig4;
}

void AppDisp7sTask(void)
{
	if(FlagStart)return;
	DESLIGA_CD1;DESLIGA_CD2;DESLIGA_CD3;DESLIGA_CD4;
	DESLIGA_ALL_DIG;
	
	if(ContDigito < 3) ContDigito++;
	else               ContDigito = 0;

	switch(ContDigito)
	{
		case 0:
			AppDisp7sConfig(Digito[0]);
			LIGA_CD1;
		break;
		case 1:
		
			AppDisp7sConfig(Digito[1]);
			LIGA_CD2;
		break;
		case 2:
			AppDisp7sConfig(Digito[2]);
			LIGA_CD3;
		break;
		case 3:
			AppDisp7sConfig(Digito[3]);
			if(FlagRun) LIGA_SEG_P;			
			LIGA_CD4;
		break;
	}	
}

void AppDisp7sConfig(U8 Dig)
{
	if(FlagDisplayOff) return;
	switch(Dig)
	{
		case 0: LIGA_DIG_0;
		break;
		case 1: LIGA_DIG_1;
		break;
		case 2: LIGA_DIG_2;
		break;
		case 3: LIGA_DIG_3;
		break;
		case 4: LIGA_DIG_4;
		break;
		case 5: LIGA_DIG_5;
		break;
		case 6: LIGA_DIG_6;
		break;
		case 7: LIGA_DIG_7;
		break;
		case 8: LIGA_DIG_8;
		break;
		case 9: LIGA_DIG_9;
		break;
		case 'F': LIGA_DIG_F;
		break;
		case 'N': DESLIGA_ALL_DIG;
		break;
		default: DESLIGA_ALL_DIG;
	}
}

void AppDisplayResetTimeOut(void)
{
	FlagTimeoutDisp = 1;
}
