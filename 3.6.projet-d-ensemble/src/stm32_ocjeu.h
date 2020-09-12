#ifndef STM32_OCJEU
#define STM32_OCJEU

#include "stm32_oclib.h"

unsigned int blink_cycles = 0;

#define BLINK_CYCLES 10;

typedef enum {
    PUSH_WAIT,
    RANDOM_WAIT,
    BLINK_WAIT
} WaitState;

unsigned int rand( unsigned int min, unsigned int max, TimerConfig* tc ) {
    // Calculate waiting time
    //unsigned int wait_time = 

    // Calculate prescale and autoreload based on wait_time
    // tc->psc = 
    // tc->arr = 
}

// void TIM2_IRQHandler(void) {
//     //Stop timer
//     stop_timer(TIM2);

//     // Check if button was pressed, 
//     if ( !read_gpio( GPIOB, 13 ) ) {
//         set_blinkers_on = true;
//     }
// }

unsigned int get_clock_freq() {
    return 72000000;
}

unsigned int get_min_prescaler( unsigned int clock_freq, unsigned int ms ) {
    // 2^16 = 65,535 c'est le plafond du compteur du TIM2, 
    // calculé par le nombre de bits, soit 8, 16 ou 32.
    return (int)clock_freq/65535;
}

void time_to_timer_config( unsigned int time, TimerConfig* tc ) {
    unsigned int clock_freq = get_clock_freq();
    tc->psc = get_min_prescaler( clock_freq, (int)(clock_freq*time/1000) );
    tc->arr = (int)( ( 1000/time ) / ( tc->psc * clock_freq ) - 1 );
}

void freq_to_timer_config( unsigned int freq, TimerConfig* tc ) {
    unsigned int clock_freq = get_clock_freq();
    tc->psc = get_min_prescaler( clock_freq, (int)(clock_freq/freq) );
    tc->arr = (int)( ( freq ) / ( tc->psc * clock_freq ) - 1 );
}

void TIM2_IRQHandler(void) {
    //Stop timer
    stop_timer(TIM2);

    TimerConfig* p_timer_config = &timer_config;

    switch (wait_state) {
    case RANDOM_WAIT: {
        // Next state is PUSH_WAIT

        // Turn light on
        set_gpio( GPIOA, 5 );

        // Calculate prescaler and autoreload based on 300ms
        time_to_timer_config( 300, p_timer_config );
        
        wait_state = PUSH_WAIT;
        break;
    }
    case PUSH_WAIT: {
        // Next state is either BLINK or RANDOM

        if ( !read_gpio( GPIOB, 13 ) ) {
            // Next state is BLINK

            // Calculate prescaler and autoreload based on 4Hz
            freq_to_timer_config( 4, p_timer_config );

            wait_state = BLINK_WAIT;
        } else {
            // Next state is RANDOM

            // Turn light off
            reset_gpio( GPIOA, 5 );

            // Calculate rand between 800 and 1800
            rand( 800, 1800, p_timer_config );

            wait_state = RANDOM_WAIT;
        }
        break;
    }
    case BLINK_WAIT: {
        if ( cycles_remaining > 0 ) {
            // Next state is BLINK

            // Toggle light
            toggle_gpio( GPIOA, 5 );

            // Reset timer
            // Use the same timer_config values
        } else {
            // Next state is RANDOM

            // Calculate rand between 800 and 1800
            rand( 800, 1800, p_timer_config );

            wait_state = RANDOM_WAIT;
        }
        break;
    }
    default:
        break;
    }

    TIM2->PSC = p_timer_config->psc;
    TIM2->ARR = p_timer_config->arr;
    start_timer( TIM2 );
}

#endif