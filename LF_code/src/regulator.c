#include "regulator.h"

int8_t Err = 0;
int8_t lastErr = 0;
uint8_t BaseTime = 50;
uint8_t halfBaseTime = 25;
int8_t ErrSum = 0;
int8_t DErr = 0;

extern uint8_t Kp EEMEM;
extern uint8_t Ki EEMEM;
extern uint8_t Kd EEMEM;


void calc_err(uint8_t transopt)
{
    //uint8_t sensor[ 8 ];
    int8_t weights[] = { -4, -3, -2, -1, 1, 2, 3, 4};
    Err = 0;

    for( uint8_t i = 0; i < 8; ++i )
    {
        //sensor[ i ] = transopt % 2;
        Err += ( transopt % 2 ) * weights[ i ];
        transopt /= 2;
    }

}

void regulator_PID(uint8_t optors, int8_t *PIDout)
{
    int8_t Pout, Iout, Dout;

    lastErr = Err;
    calc_err(optors);
    
    regulator_P(&Pout);
    regulator_I(&Iout);
    regulator_D(&Dout);
    
    PIDout = Pout + Iout + Dout;
}


void regulator_P(int8_t *Pout)
{
    uint8_t kp = eeprom_read_byte(&Kp);
    Pout = kp * Err;
}

void regulator_I(int8_t *Iout)
{
    uint8_t ki = eeprom_read_byte(&Ki);
    ErrSum += (Err + lastErr) * halfBaseTime;
    Iout = ki * ErrSum;
}

void regulator_D(int8_t *Dout)
{
    uint8_t kd = eeprom_read_byte(&Kd);
    DErr += (Err - lastErr) * BaseTime;
    Dout = kd * DErr;
}