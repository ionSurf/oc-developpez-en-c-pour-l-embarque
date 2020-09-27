#include "stm32f10x.h"

/**
 * Configuration functions
 */ 
void ConfigureSystemClock();
void ConfigureUSART();
void ConfigureGPIOAlternatePushPull();

/**
 * USART data manipulation
 */ 
void sendToUSART( char );

/**
 * Application qui transmet les lettres de l'alphabet
 */ 
void int main() {
    ConfigureSystemClock();
    ConfigureUSART();
    ConfigureGPIOAlternatePushPull();

    char data = 'A';
    int count = 0;
    while ( count < 26 ) {
        sendToUSART( data + count )
        ++count;
    }

    while(1);
    return;
}

/**
 * Configuration functions
 */ 
void ConfigureSystemClock() {

}
void ConfigureUSART() {
    // Configuration de la horloge USART1
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    // Activation de l'USART
    USART1->CR1 |= USART_CR1_UE;
    // Selection de la taille de la donnée ()8 bits
    USART1->CR1 &= ~USART_M;
    // Selection du nombre de bits de stop
    USART1->CR1 &= USART_CR2_STOP;
    // Selection de la vitesse
    USART1->BRR |= ( 468 << 4 );    // Partie fractionaire
    USART1->BRR |= ( 75 );          // Partie entiere
    // Envoyer la premier trame d'attente
    USART1->CR1 |= USART_CR1_TE;
}
void ConfigureGPIOAlternatePushPull() {

}

/**
 * USART data manipulation
 */ 
void sendToUSART( char data ) {
    // Placer les informations sur le registre DR
    USART1->DR |= data;
    // Attendre la fin de la transmission
    while ( !( USART1->SR & USART_SR_TC ) );
}

/**
 * Function d'outil pour configurer les broches
 */ 
ConfigureGPIOAlternatePushPull( GPIO_typedef *gpio, int pin ) {
    if ( gpio == GPIOA ) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    } else if ( gpio == GPIOB ) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    } else if ( gpio == GPIOC ) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    } else {
        return -1;
    }

    if ( pin < 8 ) {
        gpio->CRL &= ~( (0xF << 4*pin) | (0xF << 4*pin) );
        gpio->CRL |= ( 0xA << 4*pin );
    } else if ( pin < 15 ) {
        pin -= 8;
        gpio->CRH &= ~( (0xF << 4*pin) | (0xF << 4*pin) );
        gpio->CRH |= ( 0xA << 4*pin );
    } else {
        return -1;
    }
    return 0
    
}