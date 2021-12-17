#include "config.h"

void configurate()
{
    
    DDRA |= 0xC0; // diody LED i przelacznik
	DDRD |= 0x30; // PWM do mostka H
	DDRC |= 0xF0; // GPIO do mostka H
	DDRB |= 0x00; // czujniki lini
	
	PORTA |= 0x11;
	PORTC = 0x00;
	
	PORTB = 0x00;
	
	// ustawienie PWM na silniki
	TCCR1A |= (1<<(WGM10))|(1<<(WGM11));
	TCCR1B |= (1<<(WGM12));
	// PWM SET ON PB1 NON-INV
	TCCR1A |= (1<<(COM1A1));
	TCCR1A |= (1<<(COM1B1));
	// prescaller: 64
	TCCR1B |= (1<<(CS11))|(1<<(CS10));
	
	//inicjalizacja USART
	USART_Init();
    
}