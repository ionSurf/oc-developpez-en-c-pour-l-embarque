#include "stm32f10x.h"

int main (void) {
	
	// Configuration de la broche en sortie
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	GPIOA->CRL &= ~(0xF << 4*5);
	GPIOA->CRL |= (0x1 << 4*5);
	
	// Configuration du timer
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->ARR = 9999;
	TIM2->PSC = 7199;
	
	// Activer les interruptions
	TIM2->DIER |= TIM_DIER_UIE; // position 0x1
	
	// Configuration de l'interruption sur le coeur
	NVIC->ISER[0] |= NVIC_ISER_SETENA_28;
	NVIC->IP[28] |= ( 7 << 4 );
	
	// Lancement du timer
	TIM2->CR1 |= TIM_CR1_CEN;
	
	while(1) {
	}
}