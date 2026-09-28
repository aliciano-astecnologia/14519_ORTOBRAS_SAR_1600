/**
 * @file AppComm.c
 * @author Ricardo Kaderli (engenharia4@tiptronic.com.br)
 * @brief 
 * @version 0.1
 * @date 19-11-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "AppComm.h"
#include <stdlib.h>
#include <stm8s.h>
#include "AppTeclasBotoeira.h"
#include "AppCiclos.h"

const uint8_t     key_base[]  = {'K', 'C', 'Q', 'D', 'F'};
static @tiny uint8_t rolling_key[]  = {'K', 'C', 'Q', 'D', 'F'};
volatile @tiny uint8_t data_buffer[MSG_SIZE] = {0, 0, 0, 0, 0};
volatile @tiny uint8_t crypt_rx_data[MSG_SIZE] = {0, 0, 0, 0, 0};

volatile uint8_t buffer_index = 0;
volatile uint8_t msg_received = 0;
uint8_t comm_ok = 0;

static uint8_t StatusTeclaSobe = 0;
static uint8_t StatusTeclaDesce = 0;
static uint8_t StatusTeclaFecha = 0;
static TickTimer comm_timeout = {0};
static TickTimer comm_cmd = {0};

extern U32 CiclosTotais;
static uint8_t seed = 0;

static void send_crypted_cmd(void);
static void decode_crypted_response(void);
static uint8_t UartEnviaBuffer(const char *str, uint8_t size);
static void modifyKeyWithSeed(uint8_t *res, uint8_t *key, uint8_t keySize, char *seed);
static void encryptDecryptWithModifiedKey(uint8_t *input, uint8_t *output, uint8_t size, uint8_t *key, uint8_t keySize);
static uint8_t calculateChecksum(const uint8_t *data, uint8_t size);
static void debounceKeypad(uint8_t key_up, uint8_t key_down, uint8_t key_close);

typedef enum {
    SENDING_CMD = 0,
    WAITING_RESPONSE
} comm_state_t;

static comm_state_t comm_state = SENDING_CMD;

void AppComm_Init(void) {

	disableInterrupts(); 
	UART1_DeInit();
	UART1_ClearITPendingBit(UART1_IT_RXNE);
	UART1_ITConfig(UART1_IT_RXNE,DISABLE);
	UART1_Cmd(DISABLE);
	UART1_Init((uint32_t)9600,UART1_WORDLENGTH_8D,UART1_STOPBITS_1,UART1_PARITY_NO,UART1_SYNCMODE_CLOCK_DISABLE,UART1_MODE_TXRX_ENABLE);
	UART1_ITConfig(UART1_IT_RXNE,ENABLE);
	UART1_Cmd(ENABLE);
	enableInterrupts();

	TickTimerSet(&comm_cmd, 75);
	TickTimerSet(&comm_timeout, 100);
	
	srand(CiclosTotais & 0xFF);
}

void AppComm_Task(void) {

    BOOL DoDebounce = 0;

    switch (comm_state) {

        case SENDING_CMD:
            /* Envia mensagem a cada 75ms */
            if ((TickTimerExpired(&comm_cmd))) {
                buffer_index = 0;   /* Reinicia o index do buffer para manter a sicronização */
                send_crypted_cmd();
                TickTimerRestart(&comm_cmd);
                TickTimerRestart(&comm_timeout);
                comm_state = WAITING_RESPONSE;
            }
            break;

        case WAITING_RESPONSE:
            /* Aguarda 100ms pela resposta */
            if ((!TickTimerExpired(&comm_timeout))) {

                /* Se recebeu resposta */
                if (msg_received) {
                    decode_crypted_response(); /* Decodar pacote recebido da botoeira */
                    msg_received = 0;          /* Limpa flag de msg recebida */
                    DoDebounce = 1;
                    comm_state = SENDING_CMD;  /* Volta para estado de envio */
                }

            } else {
                comm_ok = 0;              /* Reseta flag de comunicaçao OK */
                StatusTeclaSobe = 0;
                StatusTeclaDesce = 0;
                StatusTeclaFecha = 0;
                DoDebounce = 1;
                comm_state = SENDING_CMD; /* Volta para estado de envio */
            }
            if (DoDebounce) {
                debounceKeypad(StatusTeclaSobe, StatusTeclaDesce, StatusTeclaFecha);
            }
            break;

        default:
            break;
    }
}

void AppComm_Callback(void) {
		
	uint8_t x = 0;
		
    for (x = 0; x < MSG_SIZE; x++) {
        crypt_rx_data[x] = data_buffer[x];
    }
    
    msg_received = 1;
}

static void send_crypted_cmd(void) {
   
    uint8_t uncrypt_tx_data[MSG_SIZE] = {0, 0, 0, 0, 0};
    uint8_t crypt_tx_data[MSG_SIZE] = {0, 0, 0, 0, 0};
    uint8_t chksum = 0;
    uint8_t dummy_read = 0;
    

    seed = (uint8_t)(rand() % 256);                                    /* Gerando Nova Seed */
    modifyKeyWithSeed(rolling_key, key_base, sizeof(key_base), &seed); /* Gera a rooling key baseada na nova seed */

    uncrypt_tx_data[0] = seed;                                                        /* Coloca Seed não criptografada na primeira posição, para gerar checksum */
    uncrypt_tx_data[1] = 0xBA;                                                        /* Comando Botões */
    uncrypt_tx_data[2] = 0xFF;                                                        /* Não Usado */
    uncrypt_tx_data[3] = 0xFF;                                                        /* Não Usado */
    uncrypt_tx_data[4] = (uint8_t)calculateChecksum(uncrypt_tx_data, (MSG_SIZE - 1)); /* Calcula checksum com a seed incluida, Adiciona na ultima posição do buffer */

    encryptDecryptWithModifiedKey(uncrypt_tx_data, crypt_tx_data, sizeof(uncrypt_tx_data), rolling_key, sizeof(rolling_key)); /* Encripta o pacote com a rooling key */

    crypt_tx_data[0] = seed; /* Envia seed descriptografada */

    UART1_ITConfig(UART1_IT_RXNE, DISABLE);
    dummy_read = UART1->SR;
    dummy_read = UART1->DR;

    (void)UartEnviaBuffer(crypt_tx_data, MSG_SIZE);

    dummy_read = UART1->SR;
    dummy_read = UART1->DR;
    UART1_ITConfig(UART1_IT_RXNE, ENABLE);
}

static void decode_crypted_response(void) {

    uint8_t uncrypt_rx_data[MSG_SIZE] = {0, 0, 0, 0, 0};
    uint8_t calc_checksum = 0;

    encryptDecryptWithModifiedKey(crypt_rx_data, uncrypt_rx_data, sizeof(crypt_rx_data), rolling_key, sizeof(rolling_key)); /* Gera pacote com a rooling key */

    calc_checksum = calculateChecksum(uncrypt_rx_data, (MSG_SIZE - 1));

    StatusTeclaDesce = 0;
    StatusTeclaSobe = 0;
    StatusTeclaFecha = 0;

    /* Verifica se o comando e checksum vieram corretos */
    if ((0xAB == uncrypt_rx_data[0]) &&(calc_checksum == uncrypt_rx_data[MSG_SIZE - 1])) {

            /* uncrypt_data[1]  não usado */
            /* uncrypt_data[2]  não usado */

            comm_ok = 1; /* Seta flag de comunicaçao OK */

            if (uncrypt_rx_data[3] & 0x01) {
               StatusTeclaDesce = 1;
            }

            if (uncrypt_rx_data[3] & 0x02) {
               StatusTeclaSobe = 1;
            }

            if (uncrypt_rx_data[3] & 0x04) {
               StatusTeclaFecha = 1;
            }

        } else {
            comm_ok = 0;
        }
}

static uint8_t UartEnviaBuffer(const char *str, uint8_t size)
{
    uint8_t i;

    for (i = 0; i < size; i++)
    {
        while (!(UART1->SR & UART1_SR_TXE));
        UART1->DR = str[i];
    }

    /* Aguarda o último byte terminar fisicamente */
    while (!(UART1->SR & UART1_SR_TC));

    return(i);
}

static void modifyKeyWithSeed(uint8_t *res, uint8_t *key, uint8_t keySize, uint8_t *seed) {

    uint8_t i;

    for (i = 0; i < keySize; i++) {
        res[i] = key[i] ^ (*seed); /* Modifica a chave com XOR utilizando a seed */
    }
}

static void encryptDecryptWithModifiedKey(uint8_t *input, uint8_t *output, uint8_t size, uint8_t *key, uint8_t keySize) {

    uint8_t i;

    for (i = 0; i < size; i++) {
        output[i] = input[i] ^ rolling_key[i % keySize]; /* Realiza a operação XOR com a chave modificada */
    }
}

static uint8_t calculateChecksum(const uint8_t *data, uint8_t size) {

    uint8_t checksum = 0;
    uint8_t i;

    for (i = 0; i < size; i++) {
        checksum += (uint8_t)data[i];
    }
    return checksum;
}

static void debounceKeypad(uint8_t key_up, uint8_t key_down, uint8_t key_close) {

    if ((key_down == 1) && (key_up == 0) && (key_close == 0)) {
        AppTeclasContDebounce(TECLA_DESCE, 1);
    } else {
        AppTeclasContDebounce(TECLA_DESCE, 0);
    }

    if ((key_down == 0) && (key_up == 1) && (key_close == 0)) {
        AppTeclasContDebounce(TECLA_SOBE, 1);
    } else {
        AppTeclasContDebounce(TECLA_SOBE, 0);
    }

    if ((key_down == 0) && (key_up == 0) && (key_close == 1)) {
        AppTeclasContDebounce(TECLA_FECHA, 1);
    } else {
        AppTeclasContDebounce(TECLA_FECHA, 0);
    }

    if ((key_down == 1) && (key_up == 0) && (key_close == 1)) {
        AppTeclasContDebounce(TECLAS_DESCE_E_FECHA, 1);
    } else {
        AppTeclasContDebounce(TECLAS_DESCE_E_FECHA, 0);
    }

    if ((key_down == 0) && (key_up == 1) && (key_close == 1)) {
        AppTeclasContDebounce(TECLAS_SOBE_E_FECHA, 1);
    } else {
        AppTeclasContDebounce(TECLAS_SOBE_E_FECHA, 0);
    }
}
