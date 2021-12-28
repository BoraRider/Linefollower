#include "regulator.h"

//int8_t Err = 0;
int8_t lastErr = 0;
uint8_t BaseTime = 50;
uint8_t halfBaseTime = 25;
int8_t ErrSum = 0;
int8_t DErr = 0;

extern uint8_t Kp EEMEM;
extern uint8_t Ki EEMEM;
extern uint8_t Kd EEMEM;


void PID_init(PID *pid)
{
    pid->p = 0;
    pid->i = 0;
    pid->d = 0;
    pid->p_max = 100;
    pid->i_max = 100;
    pid->d_max = 100;
    pid->pid_max = 200;
    pid->dt = 50;
    pid->err = 0;
    pid->i_sum = 0;
    pid->last_err = 0;
    pid->ctrl = 0;
}

int8_t calc_err(uint8_t transopt)
{
    //uint8_t sensor[ 8 ];
    int8_t weights[] = { -4, -3, -2, -1, 1, 2, 3, 4};
    int8_t Err = 0;

    for( uint8_t i = 0; i < 8; ++i )
    {
        //sensor[ i ] = transopt % 2;
        Err += ( transopt % 2 ) * weights[ i ];
        transopt /= 2;
    }

    return Err;
}

void regulator_PID(PID *pid, uint8_t optors)
{
    int16_t PIDout;

    pid->err = calc_err(optors);
    
    regulator_P(pid);
    regulator_I(pid);
    regulator_D(pid);
    
    PIDout = pid->p + pid->i + pid->d;

    if( PIDout > pid->pid_max)
    {
        pid->ctrl = pid->pid_max;
    }
    else if( PIDout < (-pid->pid_max) )
    {
        pid->ctrl = (-pid->pid_max);
    }
    else
    {
        pid->ctrl = PIDout;
    }
    
    pid->last_err = pid->err;
}


void regulator_P(PID *pid)
{
    uint8_t kp = eeprom_read_byte(&Kp);
    int8_t pout;
    pout = pid->err*kp;
    if( pout > pid->p_max)
    {
        pid->p = pid->p_max;
    }
    else if( pout < (-pid->p_max) )
    {
        pid->p = (-pid->p_max);
    }
    else
    {
        pid->p = pout;
    }
}

void regulator_I(PID *pid)
{
    uint8_t ki = eeprom_read_byte(&Ki);
    int8_t iout;

    // suma do calki
    pid->i_sum += ( pid->err + pid->last_err ) * pid->dt / 2;

    iout = pid->i_sum * ki;

    if( iout > pid->i_max)
    {
        pid->i = pid->i_max;
    }
    else if( iout < (-pid->i_max) )
    {
        pid->i = (-pid->i_max);
    }
    else
    {
        pid->i = iout;
    }

}

void regulator_D(PID *pid)
{
    uint8_t kd = eeprom_read_byte(&Kd);
    int8_t dout;

    dout = kd * ( pid->err - pid->last_err ) / pid->dt;

    if( dout > pid->d_max)
    {
        pid->d = pid->d_max;
    }
    else if( dout < (-pid->d_max) )
    {
        pid->d = (-pid->d_max);
    }
    else
    {
        pid->d = dout;
    }

}