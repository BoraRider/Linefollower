/*
 * uart.h
 *
 * Created: 14.12.2021 02:05:31
 *  Author: Maciek
 */ 
#ifndef UART_H_
#define UART_H_

#include "bitmanip.h"

#define FOSC 16000000//1843200 // Clock Speed
#define BAUD 9600
#define MYUBRR (FOSC/16/BAUD-1)

#define DATA_LENGTH 100

void USART_Init();
void USART_Transmit( char data );
unsigned char USART_Receive( void );
void USART_Send( char * data );


#endif /* UART_H_ */