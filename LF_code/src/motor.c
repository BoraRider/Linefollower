/*
 * motor.c
 *
 * Created: 30.11.2021 20:22:48
 *  Author: Maciek
 */ 
#include "motor.h"

void setMotor(Motor *motor, uint8_t pwm, uint8_t direction)
{

	if(motor->mot_id == 1)
	{
		if(direction == 1)
		{
			cbi(PORTC, m_ctrl_A1);
			sbi(PORTC, m_ctrl_A2);

		}
		if(direction == 2)
		{
			sbi(PORTC, m_ctrl_A1);
			cbi(PORTC, m_ctrl_A2);

		}
		OCR1B = pwm;
	}

	if(motor->mot_id == 2)
	{
		if(direction == 2)
		{
			sbi(PORTC, m_ctrl_B1);
			cbi(PORTC, m_ctrl_B2);

		}
		if(direction == 1)
		{
			cbi(PORTC, m_ctrl_B1);
			sbi(PORTC, m_ctrl_B2);

		}
		OCR1A = pwm;
	}
	
	motor->mot_pwm = pwm;
	motor->mot_direction = direction;
}

void motorInit(Motor *motor, uint8_t id, uint8_t pwm, uint8_t direct, uint8_t speed)
{
	motor->mot_id = id;
	motor->mot_direction = direct;
	motor->mot_pwm = pwm;
	motor->mot_speed = speed;

}


uint8_t stopMotor(){
	cbi(PORTA, Stby);
}

uint8_t startMotor(){
	sbi(PORTA, Stby);
}