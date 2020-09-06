#include "stm32_ocjeu.h"

bool set_blinkers_on = false;

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

    // Get random wait time
    TimerConfig timer_config
    rand( 800, 1800, &timer_conf );

    // Config Timers
    configure_timer( TIM2, timer_config->psc, timer_config->arr );

    // Config Interrupts
    configure_it();

    // Start timer
    start_timer(TIM2);

    while(1) {
        if ( set_blinkers_on ) {
            blink();

            // Reset clock and start over
            rand( 800, 1800, &timer_config );
            configure_timer(TIM2, timer_config->psc, timer_config->arr );
            start_timer(TIM2);
        }
    }
}