/*
 * motor.h
 *
 * Created: 30.11.2021 20:22:33
 *  Author: Maciek
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

#include "bitmanip.h"

struct Motor{
	uint8_t mot_id;
	uint8_t mot_pwm;
	uint8_t mot_direction;
	uint8_t mot_speed;
};


uint8_t setMotor(uint8_t motor, uint8_t speed, uint8_t direction);
uint8_t stopMotor();
uint8_t startMotor();

#endif /* MOTOR_H_ */