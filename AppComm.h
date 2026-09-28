/**
 * @file AppComm.h
 * @author Ricardo Kaderli (engenharia4@tiptronic.com.br)
 * @brief 
 * @version 0.1
 * @date 19-11-2024
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef APPCOMM_H_
#define APPCOMM_H_

#include <stm8s.h>
#include "TickTimer.h"

#define MSG_SIZE (5)

extern volatile uint8_t data_buffer[MSG_SIZE];
extern volatile uint8_t crypt_rx_data[MSG_SIZE];
extern volatile uint8_t buffer_index;
extern uint8_t comm_ok;

void AppComm_Init(void);
void AppComm_Task(void);
void AppComm_Callback(void);

#endif