/*
 * uart.c
 *
 * Created: 14.12.2021 02:05:48
 *  Author: Maciek
 */
#include "uart.h"


extern volatile uint8_t BlueLed_state EEMEM;
extern volatile uint8_t motorAspeed EEMEM;
extern volatile uint8_t motorBspeed EEMEM;


volatile char UART_RxBuf[UART_RX_BUF_SIZE];
volatile uint8_t uart_buf_counter;

volatile char numbers_lookup_table[] = {'0','1', '2', '3', '4', '5', '6', '7', '8', '9'};

void UART_Init()
{
	
	// ustawienie baud rate
	UBRR0H = (unsigned char)(MYUBRR>>8);
	UBRR0L = (unsigned char)MYUBRR;
	// aktywacja transmisji i odbioru
	//UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0B |= 0b00011000;
	// format 8 bajtow, 1 bit stopu
	//UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	UCSR0C = 0b00000110;

	// aktywacja przerwan
	UCSR0B = (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0); 

	uart_buf_counter = 0;
}

// Wyslanie pojedynczego znaku
void UART_Transmit( char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

// Odebranie pojedyncego znaku
unsigned char UART_Receive( void )
{
	// oczekiwanie na dane do odebrania
	while ( !(UCSR0A & (1<<RXC0)) );
	return UDR0;
}

// Wyslanie ciagu znakow
void UART_Send( char * data )
{
	for(uint8_t i=0; i<strlen(data); i++)
	{
		UART_Transmit(data[i]);
	}
}

// Wyswietlenie w postaci binarnej
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

// Przerwanie odebrania pczki danych
//
// Wprowadzanie danych:
// P wart I wart D wart
//
// Dla testow predkosci kol
// A wart B wart
SIGNAL( USART0_RX_vect)
{
	unsigned char data;
	data = UART_Receive();
	uint16_t recivedNumber=0;
	uint8_t speedA=0;
	uint8_t speedB=0;
	uint8_t element=0;
	uint8_t temp1=0;
	uint8_t iterator=0;
	char data2[DATA_LENGTH];

	if(data == CR){
		UART_Send("\n\rZnak CR\n\r");
		UART_Send("Odebrane dane:\n\r");

		while(iterator < uart_buf_counter)
		{
			if(UART_RxBuf[iterator] == ' ')
			{
				iterator++;
				recivedNumber=0;
			}
			if(UART_RxBuf[iterator] == 'A')
			{
				element=1;
			}
			if(UART_RxBuf[iterator] == 'B')
			{
				element=2;
			}

			if(UART_RxBuf[iterator] != CR && UART_RxBuf[iterator] != 'A' && UART_RxBuf[iterator] != 'B' && UART_RxBuf[iterator] != ' ')
			{
				recivedNumber = recivedNumber * 10;

				while (temp1<10)
				{
					if(UART_RxBuf[iterator] == numbers_lookup_table[temp1])
					{
						recivedNumber += temp1;
						temp1 = 10;
					}
					else temp1++;
				}
				temp1 = 0;
			}
			if(element == 1){ speedA = recivedNumber; }
			if(element == 2){ speedB = recivedNumber; }

			iterator++;
		}

		eeprom_write_byte(&motorAspeed, speedA);
		eeprom_write_byte(&motorBspeed, speedB);

		// if(recivedNumber == 255)
		// {
		// 	eeprom_write_byte(&BlueLed_state, 1);
		// }
		// else
		// {
		// 	eeprom_write_byte(&BlueLed_state, 0);
		// }
		UART_Send("Speed1: ");
		sprintf(data2, "%5d", speedA);
		UART_Send(data2);
		UART_Send("\n\r");
		UART_Send("Speed2: ");
		sprintf(data2, "%5d", speedB);
		UART_Send(data2);
		UART_Send("\n\r");


		for(uint8_t i=0; i<uart_buf_counter; i++)
		{
			UART_Transmit(UART_RxBuf[i]);
			UART_RxBuf[i]=' ';
		}
		uart_buf_counter = 0;
	}
	else
	{
		UART_RxBuf[uart_buf_counter] = data;
		uart_buf_counter++;
	}
}