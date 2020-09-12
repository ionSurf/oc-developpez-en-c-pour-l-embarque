#include "stm32f10x.h"

void configure_gpio_pa6_alternate_push_pull() {
    // Broche PA.6
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIOA->CRL &= ~(0xF << 24);
    GPIOA->CRL |= (0xA << 24);
}

void configure_pwm_ch1_20khz(TIM_TypeDef * TIM) {
    // Timer
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    TIM3->PSC = 0;
    TIM3->ARR = 0xE0F;

    // Configuration canal CH1 en mode PWM - 0b110
    TIM3->CCMR1 &= ~TIM_CCMR1_OC1M_0;
    TIM3->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;

    // Bit de validation pour le CH1
    TIM3->CCER |= TIM_CCER_CC1E;
}

void start_timer( TIM_TypeDef * TIM ) {
    // Lancer le timer
    TIM3->CR1 |= TIM_CR1_CEN;
}

void set_pulse_percentage( TIM_TypeDef * TIM, unsigned short pulse ) {
    // Fixer la durée d'impulsion à 20%
    // TIM3->CCR1 = 99;

    TIM->CCR1 = TIM->ARR * pulse / 100;
}

void configure_timer3_with_IT() {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->ARR = 999;
    TIM2->PSC = 7199;
    TIM2->DIER |= TIM_DIER_UIE;
    NVIC->ISER[0] |= NVIC_ISER_SETENA_28;
    NVIC->IP[28] |= ( 7 << 4 );
}

int main(void) {

    configure_gpio_pa6_alternate_push_pull();
    configure_pwm_ch1_20khz( TIM3 );
    set_pulse_percentage( TIM3, 0x100 );
    start_timer();

    while(1);
    return 0;
}

void TIM2_IRQHandler(void) {
    static int pulse = 0;
    TIM3->SR &| ~TIM_SR_UIF;
    pulse = (pulse + 5) % 101;
    set_pulse_percentage( TIM3, pulse );
}