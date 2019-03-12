#ifndef __STM32F4XX_HAL_SYSTICK_H_
#define __STM32F4XX_HAL_SYSTICK_H_

#include "stm32f4xx.h"

/**
  *@Brief
	*@CALL   Internal or External
	*@Param  None
	*@Note   None
	*@RetVal None
	*/
void SysTick_Init(void);

/**
  *@Brief
  *@CALL   Internal or External
  *@Param  None
  *@Note   None
  *@RetVal None
  */
	
void TimingDelay_Decrement(void);

void Delay_10ms(__IO u32 nTime);


#endif /*__STM32F4XX_HAL_SYSTICK_H_*/

