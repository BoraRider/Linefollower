#ifndef REGULATOR_H_
#define REGULATOR_H_

#include "bitmanip.h"
#include "uart.h"

typedef struct{
    int16_t p;
    int16_t i;
    int16_t d;

    uint8_t p_max;
    uint8_t i_max;
    uint8_t d_max;
    uint8_t pid_max;

    uint8_t dt;

    int8_t err; // obliczony blad (messured value)
    int8_t last_err;

    int16_t i_sum;

    int16_t ctrl;
}PID;

void PID_init(PID *pid);
int8_t calc_err(uint8_t transopt);
void regulator_PID(PID *pid, uint8_t optors);
void regulator_P(PID *pid);
void regulator_I(PID *pid);
void regulator_D(PID *pid);

#endif