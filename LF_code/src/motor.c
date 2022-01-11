/*
 * motor.c
 *
 * Created: 30.11.2021 20:22:48
 *  Author: Maciek
 */ 
#include "motor.h"

#define KP 1
#define KD 100
#define LS 120

extern volatile uint8_t sKp EEMEM;
extern volatile uint8_t sKd EEMEM;

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

void motorInit(Motor *motor, uint8_t id)
{
	motor->mot_id = id;
	motor->mot_direction = 1;
	motor->mot_pwm = 0;
	motor->mot_speed = 0;
	motor->mot_max_pwm = 160;
	motor->mot_max_speed = 100;
	motor->mot_min_speed = 70;

	motor->p = 0;
    motor->d = 0;
    motor->p_max = 120;
    motor->d_max = 120;
	motor->pd_max = 120;
    motor->dt = 50;
    motor->err = 0;
    motor->last_err = 0;
    motor->ctrl = 0;

	motor->mot_speed_des=0;
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

	uint8_t kp = eeprom_read_byte(&sKp);
	uint8_t kd = eeprom_read_byte(&sKd);

	motor->err = desSpeed - motor->mot_speed; //60 40
												//60 50

	Pout = motor->err * kp;

	if(Pout>motor->p_max){
		Pout=motor->p_max;
	}
	if(Pout< (-motor->p_max) ){
		Pout = (-motor->p_max);
	}

	Dout = ( ( motor->err - motor->last_err ) * kd / motor->dt) ;

	if(Dout>motor->d_max){
		Dout=motor->d_max;
	}
	if(Dout< (-motor->d_max) ){
		Dout = (-motor->d_max);
	}
	PDout = Pout + Dout;

	if(PDout>motor->pd_max) Dout = motor->pd_max;
	if(PDout<0) PDout = 0;

	motor->p = Pout;
	motor->d = Dout;
	motor->ctrl = PDout;

	motor->last_err = motor->err;

	setPWM(motor, motor->ctrl);
}

// funkcja interpretujaca wynik regulatora PID dla wykrywania lini
void pid_interpreter(Motor *motorA, Motor *motorB, PID *pid)
{
	uint8_t control;

	if(pid->ctrl > 10) // silnik B
	{
		// zakladajac ze min speed to 40
		// PID daje regulacje 20
		// na silnik B idze 60
		// na silnik A idzie 40
		// nastepuje skret w strone A
		control = motorA->mot_min_speed + pid->ctrl;

		setSpeed(motorB, motorB->mot_min_speed);

		if(control > motorA->mot_max_speed) // ustawia max speed//if(control > 200)
		{
			motorA->mot_speed_des = motorA->mot_max_speed;
			setSpeed(motorA, motorA->mot_max_speed);
			
		}
		else // ustawia predkosc bezposrednio z PID'a
		{
			motorA->mot_speed_des = control;
			setSpeed(motorA, control);

		}
	}
	else if(pid->ctrl < -10) // silnik A
	{
		// zakladajac ze min speed to 40
		// PID daje regulacje 20
		// na silnik A idze 60
		// na silnik B idzie 40
		// nastepuje skret w strone B
		control = motorB->mot_min_speed - pid->ctrl; // wartosc (pid->ctrl) jest ujemna wiec -(-) = +

		setSpeed(motorA, motorA->mot_min_speed);

		if(control > motorB->mot_max_speed) // ustawia max speed//if(control > 200)
		{
			motorB->mot_speed_des = motorB->mot_max_speed;
			setSpeed(motorB, motorB->mot_max_speed);
		}
		else // ustawia predkosc bezposrednio z PID'a
		{
			motorB->mot_speed_des = control;
			setSpeed(motorB, control);
		}

	}
	else
	{
		setSpeed(motorB, motorB->mot_min_speed);
		setSpeed(motorA, motorA->mot_min_speed);
	}
}