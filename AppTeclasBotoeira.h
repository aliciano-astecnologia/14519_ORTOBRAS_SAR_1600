/*
 * AppTeclasBotoeira.h
 *
 *  Created on: 30 de jun de 2020
 *      Author: engenharia4
 */
#include "GenericTypes.h"
#include "TickTimer.h"

enum
{
	TECLA_SOBE,
	TECLA_DESCE,
	TECLA_FECHA,
	TECLAS_SOBE_E_FECHA,
	TECLAS_DESCE_E_FECHA,
	NUMERO_TECLAS
};

typedef struct Teclas {
	U8 Debounce;
	U8 StatusTeclas;
	TickTimer EvTeclaSolta;
}_Tecla;

void AppTeclasBotoeiraInit(void);
U8 AppTeclasBotoeiraGet(U8 NumTecla);
void AppTeclasBotoeiraAtualiza(void);
void AppTeclasContDebounce(U8 NumTecla, U8 Cmd);