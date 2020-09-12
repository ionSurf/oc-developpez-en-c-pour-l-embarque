#include "stm32f10x.h"

void configure_gpio_pa6_alternate_push_pull(){
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIOA->CRL &= ~(0xF << 24);
    GPIOA->CRL |= (0xA << 24);
}

void configure_pwm_ch1_20khz(TIM_TypeDef *TIMER){
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    TIMER->PSC = 0; 
    TIMER->ARR = 0xE0F;
    TIMER->CCMR1 &= ~TIM_CCMR1_OC1M_0;
    TIMER->CCMR1 |= TIM_CCMR1_OC1M_1| TIM_CCMR1_OC1M_2;
    TIMER->CCER |= TIM_CCER_CC1E;
}

void start_timer(TIM_TypeDef *TIMER){
    TIMER->CR1 |= TIM_CR1_CEN;
}

void set_pulse_percentage(TIM_TypeDef *TIMER, int pulse){
    TIMER->CCR1 = TIMER->ARR*pulse/100;
}

void configure_timer2_with_IT(){
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->ARR = 999;
    TIM2->PSC = 7199;
    TIM2->DIER |= TIM_DIER_UIE;
    NVIC->ISER[0] |= NVIC_ISER_SETENA_28;
    NVIC->IP[28] |= (7 << 4);
}

int main(void) {

    configure_gpio_pa6_alternate_push_pull();
    configure_pwm_ch1_20khz( TIM3 );
    set_pulse_percentage( TIM3, 0x100 );
		configure_timer2_with_IT();
    start_timer( TIM3 );
		start_timer( TIM2 );

    while(1);
    return 0;
}

void TIM2_IRQHandler(void) { 
    static int pulse = 0; 
	TIM3->SR &= ~TIM_SR_UIF;
    pulse = (pulse + 5) % 101;
	set_pulse_percentage(TIM3, pulse);
}