#include "stm32f10x.h"

void TIM2_IRQHandler() {
	TIM2->SR &= ~TIM_SR_UIF;
	GPIOA->ODR ^= ( 1 << 5 );
}