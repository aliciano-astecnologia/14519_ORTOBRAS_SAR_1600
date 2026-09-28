/*
 * AppTeclasBotoeira.c
 *
 *  Created on: 30 de jun de 2020
 *      Author: engenharia4
 */

#include "AppTeclasBotoeira.h"
#include <AppDisp7s.h>

#define NUMERO_CONT_DEBOUNCE   5 //Num*40ms = tempo
_Tecla Tecla[NUMERO_TECLAS];

void AppTeclasBotoeiraInit(void)
{	
	uint8_t i;

	for(i = 0; i < NUMERO_TECLAS; i++)
	{
		Tecla[i].Debounce = 0;
		Tecla[i].StatusTeclas = 0;
	}
}

U8 AppTeclasBotoeiraGet(U8 NumTecla)
{
	if(NumTecla < NUMERO_TECLAS)
	{
		return Tecla[NumTecla].StatusTeclas;
	}
	return 0;
}

void AppTeclasContDebounce(U8 NumTecla, U8 Cmd)
{
	if(NumTecla < NUMERO_TECLAS)
	{				
		if(Cmd == 1)
		{
			AppDisplayResetTimeOut();
			if(Tecla[NumTecla].Debounce < NUMERO_CONT_DEBOUNCE)
			{
				Tecla[NumTecla].Debounce = Tecla[NumTecla].Debounce + 1; 
			}
			else
			{
				Tecla[NumTecla].StatusTeclas = 1;
			}
		}
		else
		{
			Tecla[NumTecla].Debounce = 0;
			Tecla[NumTecla].StatusTeclas = 0;
		}
	}
}






























