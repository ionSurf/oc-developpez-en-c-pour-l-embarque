#ifndef STM32_OCLIB
#define STM32_OCLIB

#include "stm32f10x.h"

struct {
    unsigned short psc, arr;
} typedef TimerConfig;

void configure_gpio_pa5(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIOA->CRL &= ~( 0xF << 4*5 );
    GPIOA->CRL |= ( 0x1 << 4*5 );
}

void configure_gpio_pc13(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    GPIOC->CRH &= ~( 0xF << 4*5 );
    GPIOC->CRH |= ( 0x04 << 4*5 );
}

void set_gpio( GPIO_TypeDef *GPIO, int n ) {
	GPIO->ODR |= ( 1 << n );
}

void reset_gpio( GPIO_TypeDef *GPIO, int n ) {
    GPIO->ODR &= ~( 1 << n );
}

void toggle_gpio( GPIO_TypeDef *GPIO, int n ) {
	GPIOA->ODR = GPIOA->ODR ^ (1 << 5);
}

unsigned int read_gpio( GPIO_TypeDef *GPIO, unsigned short n ) {
    unsigned int value = 0x0;
    return ( GPIO->IDR & ( 0x1 << n ) ) >> n;
}

void configure_timer( TIM_TypeDef *TIM, unsigned int psc, unsigned int arr ) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM->ARR = arr;
    TIM->PSC = psc;
}

void configure_it(void) {
    TIM2->DIER |= TIM_DIER_UIE;
    NVIC->ISER[0] |= NVIC_ISER_SETENA_28;
    NVIC->IP[28] |= ( 7 << 4 ); // Priority set to 7
}

void start_timer( TIM_TypeDef *TIM ) {
    TIM->CR1 |= TIM_CR1_CEN;
}

void stop_timer( TIM_TypeDef *TIM ) {
    TIM->CR1 &= ~TIM_CR1_CEN;
    TIM->SR &= ~TIM_SR_UIF;
}

// void configure_interrupt( IRQ_TypeDef *INT, unisgned int num, unsigned int priority ) {
    
// }

#endif