#ifndef REGULATOR_H_
#define REGULATOR_H_

#include "bitmanip.h"
#include "uart.h"

void calc_err(uint8_t transopt);
void regulator_PID(uint8_t optors, int8_t *PIDout);
void regulator_P(int8_t *Pout);
void regulator_I(int8_t *Iout);
void regulator_D(int8_t *Dout);

#endif