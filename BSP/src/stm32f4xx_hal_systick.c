#include "stm32f4xx_hal_systick.h"

static __IO uint32_t TimingDelay;


void SysTick_Init(void)
{
		if(SysTick_Config(SystemCoreClock/100)){
			while(1);
		}
}

void Delay_10ms(__IO u32 nTime)
{
		TimingDelay = nTime;
		while(TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{
		if(TimingDelay != 0x00){
				TimingDelay--;
		}
}