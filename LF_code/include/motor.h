/*
 * motor.h
 *
 * Created: 30.11.2021 20:22:33
 *  Author: Maciek
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

#include "bitmanip.h"
#include "regulator.h"

typedef struct {
	uint8_t mot_id;
	uint8_t mot_pwm;
	uint8_t mot_direction;
	uint8_t mot_max_pwm;
	uint8_t mot_speed;
	uint8_t mot_max_speed;
	uint8_t mot_min_speed;
}Motor;


void setMotor(Motor *motor, uint8_t pwm, uint8_t direction);
void motorInit(Motor *motor, uint8_t id, uint8_t pwm, uint8_t direct, uint8_t speed, uint8_t max_pwm, uint8_t max_speed, uint8_t min_speed);
void stopMotor();
void startMotor();
void setSpeed(Motor *motor, uint8_t desSpeed);

#endif /* MOTOR_H_ */