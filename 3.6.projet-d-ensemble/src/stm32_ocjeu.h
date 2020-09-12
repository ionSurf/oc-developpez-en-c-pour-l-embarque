#ifndef STM32_OCJEU
#define STM32_OCJEU

#include "stm32_oclib.h"

#define BLINK_CYCLES 10

#define MAX_CLOCK 72000000

typedef enum {
    PUSH_WAIT,
    RANDOM_WAIT,
    BLINK_WAIT
} WaitState;

unsigned short gen_random_time( unsigned int min, unsigned int max ) {
    // Calculate waiting time
    //unsigned short wait_time = rand()%( max - min ) + min;
	static int randomseed = 0;
	randomseed = (randomseed * 9301 + 49297) % 233280;
	return 800 + (randomseed % 1000);
	
	

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

unsigned short get_min_prescaler( unsigned int clock_freq, unsigned int ms ) {
    // 2^16 = 65,535 c'est le plafond du compteur du TIM2, 
    // calculé par le nombre de bits, soit 8, 16 ou 32.
	// prescaler = ( ms / 1000 ) / ( clock_freq * ( 65535 + 1 ) )
    //return (unsigned short)(clock_freq * ms / 1000)/65535;
	//unsigned short a = ( ms * MAX_CLOCK ) / 1000;
	unsigned short res;
		res = ( ms * MAX_CLOCK / 1000 );
		res = res / ( 65535 + 1 );
	return ( ms * MAX_CLOCK / 1000 ) / ( 65535 + 1 );
	//return 65535;
}

void time_to_timer_config( unsigned short ms, TimerConfig* tc ) {
    unsigned int clock_freq = MAX_CLOCK; //get_clock_freq();
    //tc->psc = get_min_prescaler( clock_freq, (unsigned int)(clock_freq*time/1000) );
    //tc->arr = (unsigned short)( ( 1000/time ) / ( tc->psc * clock_freq ) - 1 );
	tc->arr = 65535;
	tc->psc = get_min_prescaler( clock_freq, ms );
}

void freq_to_timer_config( unsigned int freq, TimerConfig* tc ) {
    unsigned int clock_freq = MAX_CLOCK;//get_clock_freq();
    //tc->psc = get_min_prescaler( clock_freq, (int)(clock_freq/freq) );
    //tc->arr = (unsigned short)( ( freq ) / ( tc->psc * clock_freq ) - 1 );
	tc->arr = 65535;
	tc->psc = get_min_prescaler( clock_freq, ( unsigned int )( 1000 / freq ) );
}

void config_push_wait_timer( TimerConfig* tc ) {
	tc->arr = 65535;
	tc->psc = 381;
}

void config_blink_wait_timer( TimerConfig* tc ) {
	tc->arr = 65535;
	tc->psc = 286;
}

#endif