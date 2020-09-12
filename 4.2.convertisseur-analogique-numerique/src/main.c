#include "stm32f10x.h"

/**
 * L'ADC1 du STM32F103 porte une résolution 12 bits avec 16 voies INx.
 * L'horloge doit se mettre à niveau de 72MHz à 14MHz.
 * Il doit se configurer pour utiliser la voie 8 venant du broche GPIOB.0
 * 
 * Le PWM doit se configurer pour utiliser le CH
 */

void configure_gpio_pa6_alternate_push_pull() {
    RCC->APB2ENR = RCC_APB2ENR_IOPAEN;
    RCC->CR1 &= ~( 0xF << 24 );
    RCC->CR1 |= ( 0xA << 24 );
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

void configure_gpio_pb0_analog_input() {
    // Paramétrer l'horloge
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    
    // Paramétrer la broche GPIOB.0
    //GPIOB->CR1 &= ~( (0x1 << 0) | (0x1 << 1) | (0x1)   )
    GPIOB->CR1 &= ~( 0XFF  );
}

void configure_adc_in8() {
    // Paramétrer l'horloge et son prescaler
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;
    
    // Allumer l'ADC / lancement de la conversion
    ADC1->CR2 |= ADC_CR2_ADON;

    // Sequènce de conversion ( de voies )
    ADC1->SQR1 &= ADC_SQR1_L;
    ADC1->SQR3 |= 8;
    
    // Initialiser la calibration
    ADC1->CR2 |= ADC1_CR2_CAL;
    // Scrutin de la calibration
    while( ADC1->CR2 & ADC_CR2_CAL );
}

int convert_single() {
    // Allumer l'ADC / lancement de la conversion
    ADC1->CR2 |= ADC_CR2_ADON;

    // Scrutin / attente de la fin de la conversion
    while( !(ADC1->SR & ADC1_SR_EOC) );

    // Redemarrage de la conversion
    ADC1->SR &= ~ADC1_SR_EOC;

    // Retour de la conversion
    return ADC1->DR & ~( 0x0F << 12 );
}

int main(void) {
    int res = 0;

    // configuration du PWM - potentiomètre
    configure_gpio_pb0_analog_input();
    configure_pwm_ch1_20khz( TIM3 );
    set_pulse_percentage( TIM3, 0 );
    
    // configuration de l'ADC
    configure_gpio_pb0_analog_input();
    configure_adc_in8();

    // demarrage du PWM
    start_timer(TIM3);
    while(1) {
        res = convert_single();
        // Resolution de 12 bits == 2**12 == 16***3 == 0xFFF
        set_pulse_percentage( TIM3, 100 * res / 0xFFF )
    }
}