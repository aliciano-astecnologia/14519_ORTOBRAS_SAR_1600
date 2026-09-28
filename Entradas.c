// Entradas.c
#include <Entradas.h>

@near static U16 ContEntradasSet[NUM_ENTRADAS];
@near static U16 ContEntradasClear[NUM_ENTRADAS];
@near BOOL Entradas[NUM_ENTRADAS];
U8 FlagTecla;

void EntradasInit(void)
{
	U8 i;

	for(i = 0; i < NUM_ENTRADAS; i ++)
	{ 
		ContEntradasSet[i] = ContEntradasClear[i] = 0; 
		Entradas[i] = 0;
	}

	FlagTecla = 0;
}

void EntradasTask(void)
{
	if(!GPIO_ReadInputPin(ENTRADA_4_PIN))
	{	
		ContEntradasSet[ENTRADA_4]++;
		if(ContEntradasSet[ENTRADA_4] >= DEBOUNCE_ENTRADAS)
		{ 
			ContEntradasClear[ENTRADA_4] = 0; 
			Entradas[ENTRADA_4] = 1; 
		}
	}
	else 
	{ 
		ContEntradasClear[ENTRADA_4]++;
		if(ContEntradasClear[ENTRADA_4] >= DEBOUNCE_ENTRADAS)
		{ 
			ContEntradasSet[ENTRADA_4] = 0; 
			Entradas[ENTRADA_4] = 0;
		}
	}
	
	if(!GPIO_ReadInputPin(ENTRADA_5_PIN))
	{	
		ContEntradasSet[ENTRADA_5]++;
		if(ContEntradasSet[ENTRADA_5] >= DEBOUNCE_ENTRADAS)
		{ 
			ContEntradasClear[ENTRADA_5] = 0; 
			Entradas[ENTRADA_5] = 1;
		}
	}
	else 
	{ 
		ContEntradasClear[ENTRADA_5]++;
		if(ContEntradasClear[ENTRADA_5] >= DEBOUNCE_ENTRADAS)
		{ 
			ContEntradasSet[ENTRADA_5] = 0; 
			Entradas[ENTRADA_5] = 0;
		}
	}

	if(!GPIO_ReadInputPin(ENTRADA_6_PIN))
	{	
		ContEntradasSet[ENTRADA_6]++;
		if(ContEntradasSet[ENTRADA_6] >= DEBOUNCE_ENTRADAS)
		{ 
			ContEntradasClear[ENTRADA_6] = 0; 
			Entradas[ENTRADA_6] = 1;
		}
	}
	else 
	{ 
		ContEntradasClear[ENTRADA_6]++;
		if(ContEntradasClear[ENTRADA_6] >= DEBOUNCE_ENTRADAS)
		{ 
			ContEntradasSet[ENTRADA_6] = 0; 
			Entradas[ENTRADA_6] = 0;
		}
	}

	if(!GPIO_ReadInputPin(ENTRADA_7_PIN))
	{	
		ContEntradasSet[ENTRADA_7]++;
		if(ContEntradasSet[ENTRADA_7] >= DEBOUNCE_ENTRADAS)
		{ 
			ContEntradasClear[ENTRADA_7] = 0; 
			Entradas[ENTRADA_7] = 1;
		}
	}
	else 
	{ 
		ContEntradasClear[ENTRADA_7]++;
		if(ContEntradasClear[ENTRADA_7] >= DEBOUNCE_ENTRADAS)
		{ 
			ContEntradasSet[ENTRADA_7] = 0; 
			Entradas[ENTRADA_7] = 0;
		}
	}

	if(GPIO_ReadInputPin(ENTRADA_8_PIN)) //Configurada como negativa
	{	
		ContEntradasSet[ENTRADA_8]++;
		if(ContEntradasSet[ENTRADA_8] >= DEBOUNCE_ENTRADAS)
		{ 
			ContEntradasClear[ENTRADA_8] = 0; 
			Entradas[ENTRADA_8] = 1;
		}
	}
	else 
	{ 
		ContEntradasClear[ENTRADA_8]++;
		if(ContEntradasClear[ENTRADA_8] >= DEBOUNCE_ENTRADAS)
		{ 
			ContEntradasSet[ENTRADA_8] = 0; 
			Entradas[ENTRADA_8] = 0;
		}
	}	

	if(!GPIO_ReadInputPin(TECLA_PIN))
	{	
		ContEntradasSet[ENTRADA_9]++;
		if(ContEntradasSet[ENTRADA_9] >= DEBOUNCE_ENTRADAS)
		{ 
			ContEntradasClear[ENTRADA_9] = 0;
			Entradas[ENTRADA_9] = 1;
		}
	}
	else 
	{ 
		ContEntradasClear[ENTRADA_9]++;
		if(ContEntradasClear[ENTRADA_9] >= DEBOUNCE_ENTRADAS)
		{ 
			ContEntradasSet[ENTRADA_9] = 0; 
			Entradas[ENTRADA_9] = 0;
		}
	}
	FlagTecla = Entradas[ENTRADA_9];
}
