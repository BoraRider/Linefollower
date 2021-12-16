/*
 * uart.c
 *
 * Created: 14.12.2021 02:05:48
 *  Author: Maciek
 */#include "uart.h"
void USART_Init()
{
	
	/*Set baud rate */
	UBRR0H = (unsigned char)(MYUBRR>>8);
	UBRR0L = (unsigned char)MYUBRR;
	/* Enable receiver and transmitter */
	//UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0B |= 0b00011000;
	/* Set frame format: 8data, 1stop bit */
	//UCSR0C = (1<<USBS0)|(3<<UCSZ00);	UCSR0C = 0b00000110;
}

void USART_Transmit( char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char USART_Receive( void )
{
	while ( !(UCSR0A & (1<<RXC0)) );     //oczekiwanie na dane od odebrania
	return UDR0;                        //zwrócenie zawartoœci bufora z odebranymi danymi
}

void USART_Send( char * data )
{
	for(uint8_t i=0; i<strlen(data); i++)
	{
		USART_Transmit(data[i]);
	}
}