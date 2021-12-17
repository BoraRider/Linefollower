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


int main(void)
{
	
	configurate();
	
	Motor motorA, motorB;
	motorInit( &motorA, 1, 0, 1, 0);
	motorInit( &motorB, 2, 0, 1, 0);
	//startMotor();
	//setMotor( &motorA,0,1);
	//setMotor( &motorB,0,1);
	
	//uint8_t x=0x00;
	//uint8_t pAscan=0x00;
	
	sbi(PORTA, BlueLed);
	sbi(PORTA, RedLed);
	
	//unsigned char text='a';
	char data[DATA_LENGTH];
	sprintf(data, "Dane ");
	
    while (1)  
    {
		//x = PINB;
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
		USART_Send(data);
		tbi(PORTA, RedLed);
    }
}

