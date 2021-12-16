/*
 * Linefollower_1.c
 *
 * Created: 23.11.2021 13:07:47
 * Author : Maciek
 */ 
#include <util/delay.h>
#include "bitmanip.h"
#include "motor.h"
#include "uart.h"


int main(void)
{
	
	DDRA |= 0xC0; // diody LED i przelacznik
	DDRD |= 0x30; // PWM do mostka H
	DDRC |= 0xF0; // GPIO do mostka H
	DDRB |= 0x00; // czujniki lini
	
	PORTA |= 0x11;
	PORTC = 0x00;
	
	PORTB = 0x00;
	
	TCCR1A |= (1<<(WGM10))|(1<<(WGM11));
	TCCR1B |= (1<<(WGM12));
	// PWM SET ON PB1 NON-INV
	TCCR1A |= (1<<(COM1A1));
	TCCR1A |= (1<<(COM1B1));
	// prescaller: 64
	TCCR1B |= (1<<(CS11))|(1<<(CS10));
	
	USART_Init();
	
	//startMotor();
	
	//setMotor(1,0,2);
	//setMotor(2,0,1);
	
	uint8_t x=0x00;
	//uint8_t pAscan=0x00;
	
	sbi(PORTA, BlueLed);
	sbi(PORTA, RedLed);
	
	//unsigned char text='a';
	char data[DATA_LENGTH];
	sprintf(data, "Dane ");
	
    while (1)  
    {
		x = PINB;
		//pAscan = PINA;
		//if( x == 0x01 )
		//{
		//	cbi(PORTA, BlueLed);
		//}
		//if(x == 0x10)
		//{
		//	cbi(PORTA, RedLed);
		//}
		//else
		//{
		//	sbi(PORTA, BlueLed);
		//	sbi(PORTA, RedLed);
		//}
		
		_delay_ms(1000);
		
		//USART_Transmit(text);
		USART_Send(data);
		tbi(PORTA, RedLed);
    }
}

