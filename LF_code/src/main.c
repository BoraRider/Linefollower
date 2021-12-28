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
#include "regulator.h"

//predkosci zadane
volatile uint8_t motorAspeed;
volatile uint8_t motorBspeed;

volatile uint8_t encoder_done_flag;
Motor motorA, motorB;

// zmienne w pamieci EEPROM
volatile uint8_t BlueLed_state EEMEM;

uint32_t counter_10ms;
uint32_t counter_encoderA;
uint32_t counter_encoderB;
volatile char data[DATA_LENGTH];

// Przerwanie co 10ms
ISR(TIMER2_COMPA_vect)
{
	counter_10ms++;
	if(counter_10ms > 5){
		tbi(PORTA, RedLed);
		motorA.mot_speed = counter_encoderA;
		motorB.mot_speed = counter_encoderB;
		counter_encoderA = 0;
		counter_encoderB = 0;
		encoder_done_flag = 1;
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
	counter_10ms=0;

	configurate();

	PID pid;
	
	motorInit( &motorA, 1, 0, 1, 0, 255, 100, 40);
	motorInit( &motorB, 2, 0, 1, 0, 255, 100, 40);
	startMotor();
	setMotor( &motorA,0,1);
	setMotor( &motorB,0,1);
	
	uint8_t Opto=0x00; // listwa z czujnikami
	uint8_t SW=0x00; // przelacznik 
	
	sbi(PORTA, BlueLed);
	sbi(PORTA, RedLed);
	
	
	
    while (1)  
    {
		Opto = PINB;
		SW = PINA;		

		if(encoder_done_flag == 1)
		{
			encoder_done_flag = 0;

			if( !( SW & (1<<Sw) ) )
			{
				// UART_printBits(Opto);
			}
			else
			{
				setSpeed(&motorA, motorBspeed);
				setSpeed(&motorB, motorAspeed);

				UART_Send("Az: ");
				sprintf(data, "%3d", motorAspeed );
				UART_Send(data);
				UART_Send(" Aa: ");
				sprintf(data, "%3d", motorA.mot_speed );
				UART_Send(data);

				UART_Send(" | Bz: ");
				sprintf(data, "%3d", motorBspeed );
				UART_Send(data);
				UART_Send(" Ba: ");
				sprintf(data, "%3d", motorB.mot_speed );
				UART_Send(data);
				UART_Send(" \n\r");
			}
		}

		
    }
}

