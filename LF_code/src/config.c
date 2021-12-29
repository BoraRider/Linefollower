#include "config.h"

void configurate()
{
	sei();
    
    //DDRA |= 0xC0; // diody LED i przelacznik
	DDRA |= (1<<RedLed) | (1<<BlueLed) | (1<<Sw) | (1<<Stby);

	DDRD |= 0x30; // PWM do mostka H
	DDRC |= 0xF0; // GPIO do mostka H
	DDRB |= 0x00; // czujniki lini
	
	PORTA |= 0x11;
	PORTC = 0x00;
	
	PORTB = 0xFF;
	
	// ustawienie PWM na silniki
	TCCR1A |= (1<<(WGM10))|(1<<(WGM11));
	TCCR1B |= (1<<(WGM12));
	// PWM SET ON PB1 NON-INV
	TCCR1A |= (1<<(COM1A1));
	TCCR1A |= (1<<(COM1B1));
	// prescaller: 64
	TCCR1B |= (1<<(CS11))|(1<<(CS10));
	
	//inicjalizacja USART
	UART_Init();



	// przerwanie na TC2 wykonywane co 10ms ( f = 100Hz )
	TCCR2A |= (1<<WGM21);	// Tryb CTC
	TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20);	// Prescaler: 1024 czyli f=15624Hz
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
    ADCSRA |= (1<<ADSC); // start conversion
    while(ADCSRA & (1<<ADSC));
    uint16_t tmp = ADCL>>6;
    conv_result = ADCH<<2 | tmp;
}