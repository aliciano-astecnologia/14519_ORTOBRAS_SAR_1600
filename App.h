#ifndef APP_H_
#define APP_H_

#include "Gpio.h"
#include <GenericTypes.h>
#include <TickTimer.h>
#include <AppDisp7s.h>
#include "Entradas.h"

typedef enum AppStateTypedef {
	APP_STATE_OFF = 0,
	APP_STATE_SELECAO,
	APP_STATE_ESTACIONADO,
	APP_STATE_DESCE,
	APP_STATE_SOBE
} AppStateTypedef;

/*
 * Mapeamento funcional do produto 11928.
 *
 * EntradasTask ja normaliza as entradas 4 a 7: valor 1 significa
 * entrada positiva acionada. Os aliases preservam essa leitura.
 */
#define SENSOR_CONTADOR_ATIVO          (Entradas[ENTRADA_4]) /* CN2.5  */
#define SENSOR_LIMITE_LATERAL_ATIVO    (Entradas[ENTRADA_5]) /* CN2.11 */
#define SENSOR_LIMITE_GUARDADO_ATIVO   (Entradas[ENTRADA_6]) /* CN2.10 */
#define SENSOR_LIMITE_SUBIDA_ATIVO     (Entradas[ENTRADA_7]) /* CN2.9  */
#define SINAL_ONIBUS_ATIVO              (Entradas[ENTRADA_8]) /* CN2.8  */
#define TECLA                            (Entradas[ENTRADA_9])

/* A terceira saida da placa passa a exercer a funcao do rele FECHA. */
#define LIGA_RELE_FECHA()                LIGA_ELEVADOR_ATIVO()
#define DESLIGA_RELE_FECHA()             DESLIGA_ELEVADOR_ATIVO()

#define TEMPO_TROCA_ESTADO_MS            50

void AppInit(void);
void AppTask(void);
AppStateTypedef AppGetState(void);

#endif
