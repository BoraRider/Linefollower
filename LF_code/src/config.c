#include "config.h"

uint32_t counter_10ms;

void configurate()
{
	sei();
    
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

	counter_10ms=0;

	// przerwanie na TC2 wykonywane co 10ms ( f = 100Hz )
	TCCR2A |= (1<<WGM21);	// Tryb CTC
	TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20);	// Prescaler: 1024 czyli f=15624Hz
	OCR2A = 155;	// f dzielone przez 155 da okolo 100Hz
	TIMSK2 |= (1<<OCIE2A);	// zezwolenie na przerwanie Compare Match
}

// Przerwanie co 10ms
ISR(TIMER2_COMPA_vect)
{
	counter_10ms++;
	if(counter_10ms > 50){
		tbi(PORTA, BlueLed);
		counter_10ms=0;
	}
}