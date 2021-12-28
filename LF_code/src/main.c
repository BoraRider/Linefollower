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
volatile uint8_t uart_send_flag;
Motor motorA, motorB;

// zmienne w pamieci EEPROM
volatile uint8_t BlueLed_state EEMEM;
volatile uint8_t Kp EEMEM;
volatile uint8_t Ki EEMEM;
volatile uint8_t Kd EEMEM;

uint32_t counter_10ms;
uint32_t counter_encoderA;
uint32_t counter_encoderB;
volatile char data[DATA_LENGTH];

// Przerwanie co 10ms
ISR(TIMER2_COMPA_vect)
{

	if(counter_10ms %5 == 0)	// przerwanie co 50ms
	{
		motorA.mot_speed = counter_encoderA;
		motorB.mot_speed = counter_encoderB;
		counter_encoderA = 0;
		counter_encoderB = 0;
		encoder_done_flag = 1;
	}
	if(counter_10ms > 25)	// przerwanie co 500ms
	{
		tbi(PORTA, RedLed); // swietlna informacja o wyslaniu danych uart
		uart_send_flag=1;
		counter_10ms=0;
	}
	counter_10ms++;
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
	uart_send_flag=0;
	counter_encoderA = 0;
	counter_encoderB = 0;
	counter_10ms=0;

	configurate();

	PID pid;
	PID_init(&pid);

	motorInit( &motorA, 1, 0, 1, 0, 255, 100, 40);
	motorInit( &motorB, 2, 0, 1, 0, 255, 100, 40);
	startMotor();
	setMotor( &motorA,0,1);
	setMotor( &motorB,0,1);
	
	uint8_t sensors=0x00; // listwa z czujnikami
	uint8_t SW=0x00; // przelacznik 
	
	sbi(PORTA, BlueLed);
	sbi(PORTA, RedLed);
	
    while (1)  
    {
		sensors = PINB;
		SW = PINA;

		regulator_PID(&pid, sensors);

		if(encoder_done_flag == 1)
		{
			setSpeed(&motorA, motorBspeed);
			setSpeed(&motorB, motorAspeed);
			encoder_done_flag = 0;
			if(uart_send_flag == 1)
			{
				if( !( SW & (1<<Sw) ) )
				{
					

					UART_printBits(sensors);
					UART_Send("PID ctrl: ");
					sprintf(data, "%6d", pid.ctrl );
					UART_Send(data);
					UART_Send("\n\r");

					uart_send_flag = 0;
				}
				else
				{
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
					uart_send_flag = 0;
				}
			}
		}

		
    }
}

