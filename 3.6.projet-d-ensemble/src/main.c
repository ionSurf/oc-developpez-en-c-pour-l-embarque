#include "stm32_ocjeu.h"

// Global variables
static WaitState wait_state;
static TimerConfig timer_config;
static unsigned short blink_cycles;

/**
  * TimerConfig poiner / ref
  * Random function
  * Freq to sec/time unit
**/

int main( void ) {
    // Config GPIOs
    configure_gpio_pa5();
    configure_gpio_pc13();
    reset_gpio( GPIOA, 5 );

	// Variables
  TimerConfig* p_timer_config = &timer_config;
	wait_state = RANDOM_WAIT;
	blink_cycles = 0;

    // Get random wait time
    time_to_timer_config(10*gen_random_time( 800, 1800 ), p_timer_config);

    // Config Timers
    configure_timer( TIM2, p_timer_config->psc, p_timer_config->arr );

    // Config Interrupts
    configure_it();

    // Start timer
    start_timer(TIM2);

    // Global variables
    wait_state = RANDOM_WAIT;

    while(1);
		
		return 0;
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
					//time_to_timer_config( 300, p_timer_config );
				config_push_wait_timer( p_timer_config );
					
					wait_state = PUSH_WAIT;
					break;
			}
			case PUSH_WAIT: {
					// Next state is either BLINK or RANDOM

					if ( !read_gpio( GPIOC, 13 ) ) {
				//if ( GPIOC->ODR & ( 0x1 << 13 ) ) {
							// Next state is BLINK

							// Calculate prescaler and autoreload based on 4Hz
							//freq_to_timer_config( 4, p_timer_config );
						config_blink_wait_timer( p_timer_config );
							blink_cycles = 10;
							wait_state = BLINK_WAIT;
					} else {
							// Next state is RANDOM

							// Turn light off
							reset_gpio( GPIOA, 5 );

							// Calculate rand between 800 and 1800
							time_to_timer_config(10*gen_random_time( 800, 1800 ), p_timer_config);
							
							wait_state = RANDOM_WAIT;
					}
					break;
			}
			case BLINK_WAIT: {
					if ( blink_cycles > 0 ) {
							// Next state is BLINK

							// Toggle light
							toggle_gpio( GPIOA, 5 );

							// Start timer
						start_timer(TIM2);
						
						--blink_cycles;
							// Use the same timer_config values
					} else {
							// Next state is RANDOM
						
						// Turn GPIOA.5 off
					  reset_gpio( GPIOA, 5);

							// Calculate rand between 800 and 1800
							time_to_timer_config(15*gen_random_time( 800, 1800 ), p_timer_config);

							wait_state = RANDOM_WAIT;
					}
					break;
			}
    }

    TIM2->PSC = p_timer_config->psc;
    TIM2->ARR = p_timer_config->arr;
    start_timer( TIM2 );
}
