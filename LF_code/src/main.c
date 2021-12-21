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
#include "config.h"

volatile uint32_t encoderA_TPS;
volatile uint32_t encoderB_TPS;
volatile uint8_t encoder_done_flag;

uint32_t counter_10ms;
uint32_t counter_encoderA;
uint32_t counter_encoderB;
char data[DATA_LENGTH];

// Przerwanie co 10ms
ISR(TIMER2_COMPA_vect)
{
	counter_10ms++;
	if(counter_10ms > 50){
		tbi(PORTA, RedLed);
		encoderA_TPS = counter_encoderA;
		encoderB_TPS = counter_encoderB;
		counter_encoderA = 0;
		counter_encoderB = 0;
		encoder_done_flag=1;
		counter_10ms=0;
		
	}
}
// przerwanie enkodera A
ISR(INT0_vect)
{
	counter_encoderA++;
}
// przerwanie enkodera B
ISR(INT1_vect)
{
	counter_encoderB++;
}

int main(void)
{
	encoder_done_flag=0;
	counter_encoderA = 0;
	counter_encoderB = 0;
	encoderA_TPS=0;
	encoderB_TPS=0;
	counter_10ms=0;

	configurate();
	
	Motor motorA, motorB;
	motorInit( &motorA, 1, 0, 1, 0);
	motorInit( &motorB, 2, 0, 1, 0);
	//startMotor();
	//setMotor( &motorA,0,1);
	//setMotor( &motorB,0,1);
	
	uint8_t Opto=0x00; // listwa z czujnikami
	uint8_t SW=0x00; // przelacznik 
	
	sbi(PORTA, BlueLed);
	sbi(PORTA, RedLed);
	
	//unsigned char text='a';
	
	

	
    while (1)  
    {
		Opto = PINB;
		SW = PINA;

		if(encoder_done_flag == 1)
		{
			encoder_done_flag = 0;

			if( !( SW & (1<<Sw) ) )
			{
				//UART_printBits(Opto);
			}
			else
			{
				// sprintf(data, "%6d", encoderA_TPS);
				// UART_Send(data);
				// UART_Send("\t");
				// sprintf(data, "%6d", encoderB_TPS);
				// UART_Send(data);
				// UART_Send(" \n\r");
			}
		}

		
    }
}

