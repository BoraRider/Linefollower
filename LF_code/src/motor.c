/*
 * motor.c
 *
 * Created: 30.11.2021 20:22:48
 *  Author: Maciek
 */ 
#include "motor.h"

uint8_t setMotor(uint8_t motor, uint8_t speed, uint8_t direction)
{
	if(motor==1){
		if(direction==1){
			sbi(PORTC, m_ctrl_A1);
			cbi(PORTC, m_ctrl_A2);
		}
		if(direction==2){
			cbi(PORTC, m_ctrl_A1);
			sbi(PORTC, m_ctrl_A2);
		}
		OCR1B = speed;
	}
	if(motor==2){
		if(direction==1){
			sbi(PORTC, m_ctrl_B1);
			cbi(PORTC, m_ctrl_B2);
		}
		if(direction==2){
			cbi(PORTC, m_ctrl_B1);
			sbi(PORTC, m_ctrl_B2);
		}
		OCR1A = speed;
	}
	
	
	return 0;
}


uint8_t stopMotor(){
	cbi(PORTA, Stby);
}

uint8_t startMotor(){
	sbi(PORTA, Stby);
}