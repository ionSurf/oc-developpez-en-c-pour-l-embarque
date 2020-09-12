#include "stm32_ocjeu.h"

// Global variables
bool set_blinkers_on;
WaitState wait_state;
TimerConfig timer_config;

/**
  * TimerConfig poiner / ref
  * Random function
  * Freq to sec/time unit
**/

void main( void ) {
    // Config GPIOs
    configure_gpio_pa5();
    configure_gpio_pa13();
    reset_gpio( GPIOA, 5 );

    TimerConfig* p_timer_config = &timer_config;

    // Get random wait time
    rand( 800, 1800, &timer_config );

    // Config Timers
    configure_timer( TIM2, p_timer_config->psc, p_timer_config->arr );

    // Config Interrupts
    configure_it();

    // Start timer
    start_timer(TIM2);

    // Global variables
    set_blinkers_on = false;
    wait_state = WaitState->RANDOM_WAIT;

    while(1) {
        /*if ( set_blinkers_on ) {
            blink();

            // Reset clock and start over
            rand( 800, 1800, p_timer_config );
            configure_timer(TIM2, p_timer_config->psc, p_timer_config->arr );
            start_timer(TIM2);
        }*/
    }
}