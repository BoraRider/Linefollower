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

// //predkosci zadane
volatile uint8_t motorAspeed;
volatile uint8_t motorBspeed;

// struktury
Motor motorA, motorB;
PID pid;

// zmienne w pamieci EEPROM
volatile uint8_t BlueLed_state EEMEM;
volatile uint8_t Kp EEMEM;
volatile uint8_t Ki EEMEM;
volatile uint8_t Kd EEMEM;
// nastawy regulatora dla silnika
volatile uint8_t sKp EEMEM;
volatile uint8_t sKd EEMEM;

// liczniki przerwaniowe
uint32_t counter_10ms;
uint32_t counter_encoderA;
uint32_t counter_encoderB;

// flagi przerwaniowe
volatile uint8_t encoder_done_flag;
volatile uint8_t uart_send_flag;
volatile uint8_t adc_flag;

// uniwersalna zmienna ciagu znakow
char data[DATA_LENGTH];

// zmienne zwiazane z oczytem napiecia
uint16_t conv_result, adc_read=0;
float real_voltage;


// Przerwanie co 10ms
ISR(TIMER2_COMPA_vect)
{

		if( !( PINA & (1<<Sw) ) )
		{
			startMotor();
			regulator_PID(&pid, PINB);
			pid_interpreter(&motorA, &motorB, &pid);

			//setSpeed(&motorA, motorAspeed);
			//setSpeed(&motorB, motorBspeed);

     		//UART_Send("PIDout: ");
	 		//sprintf(data, "%4d", pid.ctrl );
	 		//UART_Send(data);

		}
		if( PINA & (1<<Sw) )
		{
			stopMotor();
		}

	if(counter_10ms %5 == 0)	// przerwanie co 50ms
	{
		motorA.mot_speed = counter_encoderA;
		motorB.mot_speed = counter_encoderB;
		counter_encoderA = 0;
		counter_encoderB = 0;
		encoder_done_flag = 1;
		

		
	}
	
	if(counter_10ms %100 == 0)	// przerwanie co 1000ms
	{
		tbi(PORTA, RedLed);// swietlna informacja o wyslaniu danych uart
		/*
		UART_Send("  | A: ");
		sprintf(data, "%4d%4d", motorA.mot_speed, motorA.mot_speed_des);
		UART_Send(data);
		UART_Send("  | B: ");
		sprintf(data, "%4d%4d", motorB.mot_speed, motorB.mot_speed_des);
		UART_Send(data);
		UART_Send("\n\r");
		*/
		//uart_send_flag=1;

		// odczyt napiecia akumulatora
		//read_voltage(&conv_result);
        //adc_read = adc_read + conv_result;
		//adc_flag++;

		counter_10ms=0;
	}/*
	if(adc_flag == 2)
	{
		adc_flag=0;

		adc_read=adc_read/3;

    	real_voltage = ((float)adc_read *8.4)/1024;
		

		//UART_Send("Napiecie: ");
		//sprintf(data, "%6d", adc_read);
		//UART_Send(data);
		//UART_Send(" V\n\r");
		adc_read=0;
	}*/
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

	adc_flag=0;
	adc_read=0;

	counter_encoderA = 0;
	counter_encoderB = 0;
	counter_10ms=0;

	configurate();

	PID_init(&pid);

	motorInit( &motorA, 1);
	motorInit( &motorB, 2);
	startMotor();
	
	uint8_t sensors=0x00; // listwa z czujnikami
	//uint8_t SW=0x00; // przelacznik 
	
	sbi(PORTA, BlueLed);
	sbi(PORTA, RedLed);

	setMotor( &motorA,0,1);
	setMotor( &motorB,0,1);
    while (1)  
    {
		sensors = PINB;

		if(encoder_done_flag == 1)
		{
			encoder_done_flag = 0;
			if(uart_send_flag == 1)
			{
				// if( !( SW & (1<<Sw) ) )
				// {
					// UART_printBits(sensors);
					// UART_Send("PID S / A / B : ");
					// sprintf(data, "%4d%4d%4d", pid.ctrl, motorA.ctrl, motorB.ctrl);
					// UART_Send(data);
					// UART_Send("\n\r");

					uart_send_flag = 0;
				//}
				// else
				// {
					// UART_Send("Az: ");
					// sprintf(data, "%3d", motorAspeed );
					// UART_Send(data);
					// UART_Send(" Aa: ");
					// sprintf(data, "%3d", motorA.mot_speed );
					// UART_Send(data);

					// UART_Send(" | Bz: ");
					// sprintf(data, "%3d", motorBspeed );
					// UART_Send(data);
					// UART_Send(" Ba: ");
					// sprintf(data, "%3d", motorB.mot_speed );
					// UART_Send(data);
					// UART_Send(" \n\r");
					// uart_send_flag = 0;
				//}
			}
		}

		
    }
}

