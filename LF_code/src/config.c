#include "config.h"

void configurate()
{
	// aktywacja przerwan
	sei();
	
	// diody LED i przelacznik
	DDRA |= (1<<RedLed) | (1<<BlueLed) | (1<<Sw) | (1<<Stby);

	DDRD |= 0x30; // PWM do mostka H
	DDRC |= 0xF0; // GPIO do mostka H
	DDRB |= 0x00; // czujniki lini
	
	// stan wysoki (diody zgaszone)
	PORTA |= 0x11;

	// stan niski dd GPIO sterownika
	PORTC = 0x00;
	
	// pull-up na port czujnikow
	PORTB = 0xFF;
	
	// ustawienie PWM na silniki
	// 8 bit Fast PWM
	TCCR1A |= (1<<(WGM10));
	TCCR1B |= (1<<(WGM12));
	// ustawienie PWM na portach PD4 i PD5
	TCCR1A |= (1<<(COM1A1));
	TCCR1A |= (1<<(COM1B1));
	// prescaller: 256 czestotliwosc: 62,499 kHz
	TCCR1B |= (1<<(CS12));
	
	//inicjalizacja UART
	UART_Init();

	// przerwanie na TC2 wykonywane co 10ms ( f = 100Hz )
	TCCR2A |= (1<<WGM21);	// Tryb CTC
	TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20);	// Prescaler: 1024 czyli f = 15 624Hz
	OCR2A = 155;	// f dzielone przez 155 da okolo 100Hz
	TIMSK2 |= (1<<OCIE2A);	// zezwolenie na przerwanie Compare Match

	// Przerwania zewnetrzne z enkodera na pinach (PD2 i PD3) (INT0 i INT1)
	EICRA |= (1<<ISC10); //| (1<<ISC10); // przerwanie na Rising Edge dla INT1
	EICRA |= (1<<ISC00); //| (1<<ISC00); // przerwanie na Rising Edge dla INT2
	EIMSK |= (1<<INT1) | (1<<INT0); // aktywacja przerwan

	// konwerter analog-cyfra (ADC)
    ADMUX &= ~(1<<REFS1); // wewnetrze vcc
    ADMUX |= (1<<REFS0) | (1<<ADLAR); // wewnetrze vcc
    ADCSRA |= (1<<ADEN); // wlaczenie ADC

}

void read_voltage(uint16_t * conv_result)
{
    ADMUX = (ADMUX & 0xf0) | 0;
    ADCSRA |= (1<<ADSC); // start konwersji
    while(ADCSRA & (1<<ADSC));
    uint16_t tmp = ADCL>>6;
    conv_result = ADCH<<2 | tmp;
}