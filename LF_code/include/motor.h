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

    int16_t p;
    int16_t d;
    uint8_t p_max;
    uint8_t d_max;
	uint8_t pd_max;
    uint8_t dt;
    int8_t err;
    int8_t last_err;
    int16_t ctrl;

    uint8_t mot_speed_des;
}Motor;


void setMotor(Motor *motor, uint8_t pwm, uint8_t direction);
void setPWM(Motor *motor, uint8_t pwm);
void motorInit(Motor *motor, uint8_t id);
void stopMotor();
void startMotor();
void setSpeed(Motor *motor, uint8_t desSpeed);
void pid_interpreter(Motor *motorA, Motor *motorB, PID *pid);

#endif /* MOTOR_H_ */