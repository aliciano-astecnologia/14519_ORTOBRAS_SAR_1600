#include <App.h>
#include <AppTeclasBotoeira.h>
#include <EventsManager.h>
#include <AppCiclos.h>

typedef enum ElevadorCommandTypedef {
	ELEVADOR_SOBE = 0,
	ELEVADOR_DESCE,
	ELEVADOR_PARA
} ElevadorCommandTypedef;

@near static AppStateTypedef AppState;
@near static ElevadorCommandTypedef ComandoAnterior;
@near static U8 SaidaComandoHabilitada;
@near static TickTimer TimerTrocaEstado;

static void ControleElevador(ElevadorCommandTypedef Comando);
static void ControleIncrementaCiclos(void);

void AppInit(void)
{
	DESLIGA_RELE_FECHA();
	DESLIGA_BOBINA_DESCE();
	DESLIGA_BOBINA_SOBE();

	AppState = APP_STATE_OFF;
	ComandoAnterior = ELEVADOR_SOBE;
	SaidaComandoHabilitada = 0;
	TickTimerSet(&TimerTrocaEstado, TEMPO_TROCA_ESTADO_MS);
}

void AppTask(void)
{
	switch (AppState)
	{
		case APP_STATE_OFF:
			ControleElevador(ELEVADOR_PARA);
			DESLIGA_RELE_FECHA();

			if (SINAL_ONIBUS_ATIVO)
			{
				AppState = APP_STATE_SELECAO;
				LIGA_RELE_FECHA();
			}
			break;

		case APP_STATE_SELECAO:
			if (!SINAL_ONIBUS_ATIVO)
			{
				AppState = APP_STATE_OFF;
			}
			else if ((!SENSOR_LIMITE_LATERAL_ATIVO) ||
			         (!SENSOR_LIMITE_GUARDADO_ATIVO))
			{
				AppState = APP_STATE_ESTACIONADO;
			}
			else if (AppTeclasBotoeiraGet(TECLA_DESCE))
			{
				AppState = APP_STATE_DESCE;
			}
			else if (AppTeclasBotoeiraGet(TECLA_SOBE) &&
			         (!SENSOR_LIMITE_SUBIDA_ATIVO))
			{
				AppState = APP_STATE_SOBE;
			}
			break;

		case APP_STATE_ESTACIONADO:
			if (!SINAL_ONIBUS_ATIVO)
			{
				AppState = APP_STATE_OFF;
			}
			else if (AppTeclasBotoeiraGet(TECLA_DESCE))
			{
				AppState = APP_STATE_DESCE;
			}
			else if (AppTeclasBotoeiraGet(TECLA_SOBE) &&
			         (!SENSOR_LIMITE_SUBIDA_ATIVO))
			{
				AppState = APP_STATE_SOBE;
			}
			break;

		case APP_STATE_DESCE:
			if (!SINAL_ONIBUS_ATIVO)
			{
				AppState = APP_STATE_OFF;
			}
			else if (AppTeclasBotoeiraGet(TECLA_DESCE))
			{
				ControleElevador(ELEVADOR_DESCE);
			}
			else
			{
				ControleElevador(ELEVADOR_PARA);
				AppState = APP_STATE_SELECAO;
			}
			break;

		case APP_STATE_SOBE:
			if (!SINAL_ONIBUS_ATIVO)
			{
				AppState = APP_STATE_OFF;
			}
			else if (AppTeclasBotoeiraGet(TECLA_SOBE))
			{
				ControleElevador(ELEVADOR_SOBE);
			}
			else
			{
				ControleElevador(ELEVADOR_PARA);
				AppState = APP_STATE_SELECAO;
			}
			break;

		default:
			ControleElevador(ELEVADOR_PARA);
			DESLIGA_RELE_FECHA();
			AppState = APP_STATE_OFF;
			break;
	}
}

AppStateTypedef AppGetState(void)
{
	return AppState;
}

static void ControleElevador(ElevadorCommandTypedef Comando)
{
	if (Comando != ComandoAnterior)
	{
		DESLIGA_BOBINA_SOBE();
		DESLIGA_BOBINA_DESCE();
		SaidaComandoHabilitada = 0;
		TickTimerRestart(&TimerTrocaEstado);
		ComandoAnterior = Comando;
	}

	if ((!SaidaComandoHabilitada) && TickTimerExpired(&TimerTrocaEstado))
	{
		SaidaComandoHabilitada = 1;
	}

	if (!SaidaComandoHabilitada)
	{
		return;
	}

	switch (Comando)
	{
		case ELEVADOR_SOBE:
			DESLIGA_BOBINA_DESCE();
			if (!SENSOR_LIMITE_SUBIDA_ATIVO)
			{
				LIGA_BOBINA_SOBE();
			}
			else
			{
				DESLIGA_BOBINA_SOBE();
			}
			break;

		case ELEVADOR_DESCE:
			DESLIGA_BOBINA_SOBE();
			LIGA_BOBINA_DESCE();
			break;

		case ELEVADOR_PARA:
		default:
			DESLIGA_BOBINA_SOBE();
			DESLIGA_BOBINA_DESCE();
			break;
	}
}


