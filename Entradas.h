// Entradas.h
#ifndef ENTRADAS_H_
#define ENTRADAS_H_

#include <stm8s_gpio.h>
#include <GenericTypes.h>
#include <gpio.h>

enum {
    ENTRADA_4,
    ENTRADA_5,
    ENTRADA_6,
    ENTRADA_7,
    ENTRADA_8,
    ENTRADA_9,
    NUM_ENTRADAS
};

#define	DEBOUNCE_ENTRADAS	50
#define DEBOUNCE_TECLA    1000

extern BOOL Entradas[NUM_ENTRADAS];

void EntradasInit(void);
void EntradasTask(void);

#endif