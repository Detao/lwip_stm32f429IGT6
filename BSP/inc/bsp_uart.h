#ifndef __BSP_UART_H_
#define __BSP_UART_H_

#include "stm32f4xx.h"

#define RXBUFFERSIZE  1
#define DEBUG_USART_BAUDRATE 115200

typedef struct{
	uint8_t port;
	UART_InitTypeDef config;
	void *priv;
	
}uart_dev_t;

int32_t uart_init(void );

//int32_t hal_uart_send();

//int32_t hal_uart_recv();

//int32_t hal_uart_finalize();


#endif /*__BSP_UART_H_*/

