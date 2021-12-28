/*
 * motor.c
 *
 * Created: 30.11.2021 20:22:48
 *  Author: Maciek
 */ 
#include "motor.h"

#define KP 5
#define KD 5

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

// szybszy sposob ustawienia PWM
void setPWM(Motor *motor, uint8_t pwm)
{
	if(motor->mot_id == 1)
	{
		OCR1B = pwm;
	}
	if(motor->mot_id == 2)
	{
		OCR1A = pwm;
	}
	motor->mot_pwm = pwm;
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

	motor->p = 0;
    motor->d = 0;
    motor->p_max = 200;
    motor->d_max = 200;
	motor->pd_max = 255;
    motor->dt = 50;
    motor->err = 0;
    motor->last_err = 0;
    motor->ctrl = 0;
}

void stopMotor(){
	cbi(PORTA, Stby);
}

void startMotor(){
	sbi(PORTA, Stby);
}

// regulator PD na poszczegolne kolo
void setSpeed(Motor *motor, uint8_t desSpeed)
{
	
	int16_t Pout, Dout, PDout;

	motor->err = desSpeed - motor->mot_speed;

	Pout = motor->err * KP;

	if(Pout>motor->p_max){
		Pout=motor->p_max;
	}
	if(Pout< (-motor->p_max) ){
		Pout = (-motor->p_max);
	}

	Dout = KD * ( motor->err - motor->last_err ) / motor->dt;

	if(Dout>motor->d_max){
		Dout=motor->d_max;
	}
	if(Dout< (-motor->d_max) ){
		Dout = (-motor->d_max);
	}
	PDout = Dout + Pout;

	if(PDout>motor->pd_max)Dout=motor->pd_max;
	if(PDout<0)PDout=0;

	motor->p = Pout;
	motor->d = Dout;
	motor->ctrl = PDout;

	setPWM(motor, motor->ctrl);
}

// funkcja interpretujaca wynik regulatora PID dla wykrywania lini
void pid_interpreter(Motor *motorA, Motor *motorB, PID *pid)
{
	if(pid->ctrl > 0) // silnik A
	{
		if(pid->ctrl > motorA->mot_max_speed) // ustawia max speed
		{
			setSpeed(motorA, motorA->mot_max_speed);
		}
		else if(pid->ctrl < motorA->mot_min_speed) // ustawia min speed
		{
			setSpeed(motorA, motorA->mot_min_speed);
		}
		else // ustawia predkosc bezposrednio z PID'a
		{
			setSpeed(motorA, pid->ctrl); 
		}
	}
	if(pid->ctrl < 0) // silnik B
	{
		if((-pid->ctrl) > motorB->mot_max_speed) // ustawia max speed
		{
			setSpeed(motorB, motorB->mot_max_speed);
		}
		else if((-pid->ctrl) < motorB->mot_min_speed) // ustawia min speed
		{
			setSpeed(motorB, motorB->mot_min_speed);
		}
		else // ustawia predkosc bezposrednio z PID'a
		{
			setSpeed(motorB, (-pid->ctrl)); 
		}
	}
}