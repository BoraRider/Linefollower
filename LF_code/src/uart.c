/*
 * uart.c
 *
 * Created: 14.12.2021 02:05:48
 *  Author: Maciek
 */
#include "uart.h"


extern volatile uint8_t BlueLed_state EEMEM;


void UART_Init()
{
	
	/*Set baud rate */
	UBRR0H = (unsigned char)(MYUBRR>>8);
	UBRR0L = (unsigned char)MYUBRR;
	/* Enable receiver and transmitter */
	//UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0B |= 0b00011000;
	/* Set frame format: 8data, 1stop bit */
	//UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	UCSR0C = 0b00000110;
	UCSR0B = (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0); 

	uart_buf_counter = 0;
}

void UART_Transmit( char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

unsigned char UART_Receive( void )
{
	while ( !(UCSR0A & (1<<RXC0)) );     //oczekiwanie na dane od odebrania
	return UDR0;                        //zwrocenie zawartosci bufora z odebranymi danymi
}

void UART_Send( char * data )
{
	for(uint8_t i=0; i<strlen(data); i++)
	{
		UART_Transmit(data[i]);
	}
}

void UART_printBits( uint8_t n )
{
    const uint8_t Bits = 8 * sizeof n;
    char tmp[ Bits + 1 ];
   
    for( uint8_t i = 0; i < Bits; ++i )
    {
        tmp[ Bits - i - 1 ] = '0' + n % 2;
        n /= 2;
    }
   
    tmp[ Bits ] = 0;
	UART_Send(tmp);
	UART_Send(" \n\r");
}

SIGNAL( USART0_RX_vect)
{
	unsigned char data;
	data = UART_Receive();
	

	if(data == CR){
		UART_Send("\n\rZnak CR\n\r");
		UART_Send("Odebrane dane:\n\r");

		if(uart_buf_counter > 4){
			if(UART_RxBuf[0] == 'L' && UART_RxBuf[1] == 'E' && UART_RxBuf[2] == 'D')
			{
				if(UART_RxBuf[4] == '1')
				{
					eeprom_write_byte(&BlueLed_state, 1);
				}
				if(UART_RxBuf[4] == '0')
				{
					eeprom_write_byte(&BlueLed_state, 0);
				}
			}
		}

		for(uint8_t i=0; i<uart_buf_counter; i++)
		{
			UART_Transmit(UART_RxBuf[i]);
			UART_RxBuf[i]=' ';
		}
		uart_buf_counter=0;
	}
	else
	{
		UART_RxBuf[uart_buf_counter] = data;
		uart_buf_counter++;
	}
}