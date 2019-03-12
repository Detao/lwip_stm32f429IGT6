
/**
  ******************************************************************************
  * @file    stm32f429_eth.h
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    31-July-2013
  * @brief   This file contains all the functions prototypes for the Ethernet
  *          firmware driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F429_HAL_ETH_H_
#define __STM32F429_HAL_ETH_H_

#ifdef __cplusplus
 extern "C" {
#endif
	
	 
#include "stm32f4xx_hal.h"
	 
#define ETH_RESET_GPIO_Port  GPIOI
#define ETH_RESET_GPIO_Pin    GPIO_PIN_1
	 
	 
int hal_eth_init(void);
int hal_eth_write(char *buf,int buf_len)	;
int eth_get_len(void)	;
void hal_eth_read(uint16_t length,uint8_t **databuf);
void DMA_close(void);
	 
	 
	 
#ifdef __cplusplus
}
#endif
#endif /*__STM32F429_HAL_ETH_H_*/
/**
  * @}
  */ 


/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

