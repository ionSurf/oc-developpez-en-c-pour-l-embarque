#include <stm32f10x.h>
int main (void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA->CRL &= ~(0xF << 20); // PA.5 output push-pull
	GPIOA->CRL |= (0x01 << 20); // PA.5 output push-pull
  
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->ARR = 9999;
	TIM2->PSC = 7199;
	TIM2->CR1 = TIM2->CR1 | TIM_CR1_CEN;
	
  while(1) {
		if (TIM2->SR & TIM_SR_UIF) { // Scrutation de UIF
			TIM2->SR = TIM2->SR & ~TIM_SR_UIF; // Remise à zéro de UIF
			GPIOA->ODR = GPIOA->ODR ^ (1 << 5);
		}
			
  }
  return 0;
}
