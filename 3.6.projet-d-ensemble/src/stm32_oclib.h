#ifndef STM32_OCLIB
#define STM32_OCLIB

struct {
    unsigned int psc, arr
} typedef TimerConfig;

void configure_gpio_pa5(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIOA->CRL &= ~( 0xF << 4*5 );
    GPIOA->CRL |= ( 0x1 << 4*5 );
}

void configure_gpio_pa13(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    GPIOA->CRH &= ~( 0xF << 4*5 );
    GPIOA->CRH |= ( 0x1 << 4*5 );
}

void set_gpio( GPIO_TypeDef *GPIO, int n ) {
    if ( n > 7 ) {
        GPIO->CRH |= ( 0x1 << n%8 );
    } else {
        GPIO->CRL |= ( 0x1 << n );
    }
}

void reset_gpio( GPIO_TypeDef *GPIO, int n ) {
    if ( n > 7 ) {
        GPIO->CRH &= ~( 0x1 << n%8 );
    } else {
        GPIO->CRL &= ~( 0x1 << n );
    }
}

void toggle_gpio( GPIO_TypeDef *GPIO, int n ) {

}

void read_gpio( GPIO_TypeDef *GPIO, int n ) {
    unsigned int value = 0x0;
    value = GPIO->IDR & ( 0x1 << n );

    return ( value >> n );
}

void blink( GPIO_TypeDef *GPIO, unsigned int cycles, unsigned int freq )  {
    // Calculer le temps pour attendre
    unsigned int wait_time = freq_to_sec( freq );
    
    // Mettre à boucle
    for (unsigned int i = 0; i < cycles; i++) {
        // Turn off the light
        reset_gpio( GPIOA, 5 );
        
        // Wait
        sleep(wait_time);

        // turn on the light
        set_gpio( GPIOA, 5 );

        // Wait
        sleep(wait_time);
    }

    // Mettre la lumière à 0
    reset_gpio( GPIOA, 5 );
}

void configure_timer( TIM_TypeDef *TIM, unsigned int psc, unsigned int arr ) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM->ARR = arr;
    TIM->PSC = psc;
}

void configure_it(void) {
    TIM2->DIER |= TIM_DIER_UIE;
    NVIC->ISER[0] |= NVIC_ISER_28;
    NVIC->IP |= ( 7 << 4 ); // Priority set to 7
}

void start_timer( TIM_TypeDef *TIM ) {
    TIM->CR1 |= TIM_CR1_CEN;
}

void stop_timer( TIM_TypeDef *TIM ) {
    TIM->CR1 &= ~TIM_CR1_CEN;
    // UIF to 0
}

// void configure_interrupt( IRQ_TypeDef *INT, unisgned int num, unsigned int priority ) {
    
// }

#endif