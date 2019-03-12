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
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)//����Ǵ���1�����д���1 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_USART1_CLK_ENABLE();			//ʹ��USART1ʱ��
	//	__HAL_RCC_AFIO_CLK_ENABLE();
	
		GPIO_Initure.Pin=GPIO_PIN_9|GPIO_PIN_10;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		 GPIO_Initure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_Initure.Alternate = GPIO_AF7_USART1;
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
		GPIO_Initure.Mode=GPIO_MODE_INPUT;	//ģʽҪ����Ϊ��������ģʽ��	
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA10
		
//#if EN_USART1_RX
//		HAL_NVIC_EnableIRQ(USART1_IRQn);				//ʹ��USART1�ж�ͨ��
//		HAL_NVIC_SetPriority(USART1_IRQn,3,3);			//��ռ���ȼ�3�������ȼ�3
//#endif	
	}
}



//����1�жϷ������
//void USART1_IRQHandler(void)                	
//{ 
//	u32 timeout=0;

//	
//	HAL_UART_IRQHandler(&UART1_Handler);	//����HAL���жϴ����ú���
//	
//	timeout=0;
//    while (HAL_UART_GetState(&UART1_Handler) != HAL_UART_STATE_READY)//�ȴ�����
//	{
//	 timeout++;////��ʱ����
//     if(timeout>HAL_MAX_DELAY) break;		
//	
//	}
//     
//	timeout=0;
//	while(HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)//һ�δ������֮�����¿����жϲ�����RxXferCountΪ1
//	{
//	 timeout++; //��ʱ����
//	 if(timeout>HAL_MAX_DELAY) break;	
//	}

//} 

