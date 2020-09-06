#ifndef STM32_OCJEU
#define STM32_OCJEU

#include "stm32_oclib.h"

unsigned int rand( unsigned int min, unsigned int max, TimerConfig* tc ) {
    // Calculate waiting time
    //unsigned int wait_time = 

    // Calculate prescale and autoreload based on wait_time
    // tc->psc = 
    // tc->arr = 
}

void TIM2_IRQHandler(void) {
    //Stop timer
    stop_timer(TIM2);

    // Check if button was pressed, 
    if ( !read_gpio( GPIOB, 13 ) ) {
        set_blinkers_on = true;
    }
}

#endif