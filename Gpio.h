// Gpio.h
#ifndef GPIO_H_
#define GPIO_H_

#include <stm8s_gpio.h>

// ENTRADAS
#define	ENTRADA_4_PIN 							GPIOB,GPIO_PIN_1 
#define	ENTRADA_5_PIN 							GPIOB,GPIO_PIN_0 
#define	ENTRADA_6_PIN 							GPIOF,GPIO_PIN_4 
#define	ENTRADA_7_PIN 							GPIOA,GPIO_PIN_3
#define	ENTRADA_8_PIN 							GPIOD,GPIO_PIN_7
#define	TECLA_PIN			 					GPIOA,GPIO_PIN_1 

// SAIDAS
#define	SAIDA_1								GPIOB,GPIO_PIN_7 
#define	SAIDA_2								GPIOB,GPIO_PIN_2 
#define	SAIDA_3								GPIOB,GPIO_PIN_3
#define	CONFIG_ENTRADA_8				    GPIOA,GPIO_PIN_2 
#define	CD1									GPIOC,GPIO_PIN_2 
#define	CD2									GPIOE,GPIO_PIN_5 
#define	CD3									GPIOC,GPIO_PIN_1 
#define	CD4									GPIOC,GPIO_PIN_3 
#define	SEG_A								GPIOC,GPIO_PIN_4 
#define	SEG_B								GPIOD,GPIO_PIN_4 
#define	SEG_C								GPIOD,GPIO_PIN_2 
#define	SEG_D								GPIOC,GPIO_PIN_7 
#define	SEG_E								GPIOC,GPIO_PIN_6 
#define	SEG_F								GPIOC,GPIO_PIN_5 
#define	SEG_G								GPIOD,GPIO_PIN_3 
#define	SEG_P								GPIOD,GPIO_PIN_0 

#define	BOBINA_SOBE						    SAIDA_1 /* CN2.2 */
#define	BOBINA_DESCE						SAIDA_2 /* CN2.3 */
#define	ELEVADOR_ATIVO						SAIDA_3 /* CN2.4 */

#define	LIGA_ELEVADOR_ATIVO() 				GPIO_WriteHigh(ELEVADOR_ATIVO)
#define	DESLIGA_ELEVADOR_ATIVO() 			GPIO_WriteLow(ELEVADOR_ATIVO)

#define	LIGA_BOBINA_DESCE() 			    GPIO_WriteHigh(BOBINA_DESCE)
#define	DESLIGA_BOBINA_DESCE() 		        GPIO_WriteLow(BOBINA_DESCE)

#define	LIGA_BOBINA_SOBE() 			        GPIO_WriteHigh(BOBINA_SOBE)
#define	DESLIGA_BOBINA_SOBE() 		        GPIO_WriteLow(BOBINA_SOBE)

#define	CONFIG_ENT8_POS					    GPIO_WriteHigh(CONFIG_ENTRADA_8)
#define	CONFIG_ENT8_NEG 				    GPIO_WriteLow(CONFIG_ENTRADA_8)

#define	LIGA_CD1 							GPIO_WriteHigh(CD1)
#define	DESLIGA_CD1 						GPIO_WriteLow(CD1)

#define	LIGA_CD2 							GPIO_WriteHigh(CD2)
#define	DESLIGA_CD2 						GPIO_WriteLow(CD2)

#define	LIGA_CD3 							GPIO_WriteHigh(CD3)
#define	DESLIGA_CD3 						GPIO_WriteLow(CD3)

#define	LIGA_CD4 							GPIO_WriteHigh(CD4)
#define	DESLIGA_CD4 						GPIO_WriteLow(CD4)

#define	LIGA_SEG_A 							GPIO_WriteHigh(SEG_A)
#define	DESLIGA_SEG_A 					    GPIO_WriteLow(SEG_A)

#define	LIGA_SEG_B 							GPIO_WriteHigh(SEG_B)
#define	DESLIGA_SEG_B 					    GPIO_WriteLow(SEG_B)

#define	LIGA_SEG_C 							GPIO_WriteHigh(SEG_C)
#define	DESLIGA_SEG_C 					    GPIO_WriteLow(SEG_C)

#define	LIGA_SEG_D 							GPIO_WriteHigh(SEG_D)
#define	DESLIGA_SEG_D 					    GPIO_WriteLow(SEG_D)

#define	LIGA_SEG_E 							GPIO_WriteHigh(SEG_E)
#define	DESLIGA_SEG_E 					    GPIO_WriteLow(SEG_E)

#define	LIGA_SEG_F 							GPIO_WriteHigh(SEG_F)
#define	DESLIGA_SEG_F 					    GPIO_WriteLow(SEG_F)

#define	LIGA_SEG_G 							GPIO_WriteHigh(SEG_G)
#define	DESLIGA_SEG_G 					    GPIO_WriteLow(SEG_G)

#define	LIGA_SEG_P 							GPIO_WriteHigh(SEG_P)
#define	DESLIGA_SEG_P 					    GPIO_WriteLow(SEG_P)

void GpioInit(void);

#endif