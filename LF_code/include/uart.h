/*
 * uart.h
 *
 * Created: 14.12.2021 02:05:31
 *  Author: Maciek
 */ 
#ifndef UART_H_
#define UART_H_

#include "bitmanip.h"

#define FOSC 16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR (FOSC/16/BAUD-1)

#define DATA_LENGTH 100
#define UART_RX_BUF_SIZE 100

#define CR 0x0D
#define LF 0x0A

void UART_Init();
void UART_Transmit( char data );
unsigned char UART_Receive( void );
void UART_Send( char * data );
void UART_printBits( uint8_t n );

#endif /* UART_H_ */