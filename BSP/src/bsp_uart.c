#include "bsp_uart.h"


UART_HandleTypeDef uart_handle;

int fputc(int ch, FILE *f)
{ 
  int ret;	
	ret = HAL_UART_Transmit(&uart_handle, (uint8_t *)&ch, 1, 30000);   
	return ret;
}



uint8_t aRxBuffer[RXBUFFERSIZE];


int32_t uart_init()
{
	int32_t ret = -1;
	
	uart_handle.Instance = USART1;
	uart_handle.Init.BaudRate = DEBUG_USART_BAUDRATE;
	uart_handle.Init.WordLength = UART_WORDLENGTH_8B;
	uart_handle.Init.StopBits = UART_STOPBITS_1;
	uart_handle.Init.Parity = UART_PARITY_NONE;
	uart_handle.Init.Mode = UART_MODE_TX_RX;
	uart_handle.Init.HwFlowCtl=UART_HWCONTROL_NONE;
	
	HAL_UART_Init(&uart_handle);
	
	//HAL_UART_Recevie_IT(&uart_handle,(uint8_t *)aRxBuffer,RXBUFFERSIZE);
	
	
	return 0;		
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)//如果是串口1，进行串口1 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
		__HAL_RCC_USART1_CLK_ENABLE();			//使能USART1时钟
	//	__HAL_RCC_AFIO_CLK_ENABLE();
	
		GPIO_Initure.Pin=GPIO_PIN_9|GPIO_PIN_10;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		 GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Initure.Alternate = GPIO_AF7_USART1;
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
		GPIO_Initure.Mode=GPIO_MODE_INPUT;	//模式要设置为复用输入模式！	
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA10
		
//#if EN_USART1_RX
//		HAL_NVIC_EnableIRQ(USART1_IRQn);				//使能USART1中断通道
//		HAL_NVIC_SetPriority(USART1_IRQn,3,3);			//抢占优先级3，子优先级3
//#endif	
	}
}



//串口1中断服务程序
//void USART1_IRQHandler(void)                	
//{ 
//	u32 timeout=0;

//	
//	HAL_UART_IRQHandler(&UART1_Handler);	//调用HAL库中断处理公用函数
//	
//	timeout=0;
//    while (HAL_UART_GetState(&UART1_Handler) != HAL_UART_STATE_READY)//等待就绪
//	{
//	 timeout++;////超时处理
//     if(timeout>HAL_MAX_DELAY) break;		
//	
//	}
//     
//	timeout=0;
//	while(HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)//一次处理完成之后，重新开启中断并设置RxXferCount为1
//	{
//	 timeout++; //超时处理
//	 if(timeout>HAL_MAX_DELAY) break;	
//	}

//} 

