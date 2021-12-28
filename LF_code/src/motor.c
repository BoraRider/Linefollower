/*
 * motor.c
 *
 * Created: 30.11.2021 20:22:48
 *  Author: Maciek
 */ 
#include "motor.h"

#define KP 5

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
		// Short brake
		if(direction == 0)
		{
			sbi(PORTC, m_ctrl_A2);
			sbi(PORTC, m_ctrl_A1);
		}
		// STOP
		if(direction == 3)
		{
			cbi(PORTC, m_ctrl_A2);
			cbi(PORTC, m_ctrl_A1);
		}
		OCR1B = pwm;
	}

	if(motor->mot_id == 2)
	{
		if(direction == 1)
		{
			sbi(PORTC, m_ctrl_B1);
			cbi(PORTC, m_ctrl_B2);
		}
		if(direction == 2)
		{
			cbi(PORTC, m_ctrl_B1);
			sbi(PORTC, m_ctrl_B2);
		}
		// Short brake
		if(direction == 0)
		{
			sbi(PORTC, m_ctrl_B2);
			sbi(PORTC, m_ctrl_B1);
		}
		// STOP
		if(direction == 3)
		{
			cbi(PORTC, m_ctrl_B2);
			cbi(PORTC, m_ctrl_B1);
		}

		OCR1A = pwm;
	}
	
	motor->mot_pwm = pwm;
	motor->mot_direction = direction;
}

void motorInit(Motor *motor, uint8_t id, uint8_t pwm, uint8_t direct, uint8_t speed, uint8_t max_pwm, uint8_t max_speed, uint8_t min_speed)
{
	motor->mot_id = id;
	motor->mot_direction = direct;
	motor->mot_pwm = pwm;
	motor->mot_speed = speed;
	motor->mot_max_pwm = max_pwm;
	motor->mot_max_speed = max_speed;
	motor->mot_min_speed = min_speed;
}

void stopMotor(){
	cbi(PORTA, Stby);
}

void startMotor(){
	sbi(PORTA, Stby);
}

void setSpeed(Motor *motor, uint8_t desSpeed)
{
	int16_t err, U;
	err = desSpeed - motor->mot_speed;
	U = err * KP;
	if(U>255)U=255;
	if(U<0)U=0;

	setMotor(motor, U, 1);

}

void pid_interpreter(Motor *motorA, Motor *motorB, PID *pid)
{
	if(pid->ctrl > 0) // silnik A
	{
		
	}
}