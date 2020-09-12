#include "STM32f10x.h"
int main (void) {
	RCC->APB2ENR = RCC->APB2ENR | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN;
	GPIOA->CRL = GPIOA->CRL & ~(0xF << 20); // output push-pull b0001
	GPIOA->CRL = GPIOA->CRL | (0x01 << 20); // output push-pull b0001
	
	GPIOC->CRH = GPIOC->CRH & ~(0xF << 20);  // input floating b0100
	GPIOC->CRH = GPIOC->CRH | (0x04 << 20); // input floating b0100

	int state = 0;
	while(1) {
    if (GPIOC->IDR & (1 << 13)){
	    GPIOA->ODR ^= (1 << 5);
    }
	//state = GPIOC->IDR & (1 << 13);
}
	return 0; 
} 