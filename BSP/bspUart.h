#ifndef BSP_UART_H
#define BSP_UART_H


#include "stdint.h"


#define UART_SEND_LENGTH 600



void uartInit(uint32_t bound);
void startSendData(void);
void dmaTest(void);
#endif


/**********************(C)  COPYRIGHT 2017 Œ‚‘∂„Ú *********************************************************/
