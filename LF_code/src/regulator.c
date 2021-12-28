#include "regulator.h"

extern volatile uint8_t Kp EEMEM;
extern volatile uint8_t Ki EEMEM;
extern volatile uint8_t Kd EEMEM;

volatile char data[DATA_LENGTH];

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
    // UART_printBits(transopt);

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
    
    // UART_Send("Err: ");
	// sprintf(data, "%6d", pid->err );
	// UART_Send(data);
	// UART_Send("\n\r");

    regulator_P(pid);
    //regulator_I(pid);
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
    // UART_Send("PIDout: ");
	// sprintf(data, "%6d", pid->ctrl );
	// UART_Send(data);
	// UART_Send("\n\r");
    pid->last_err = pid->err;
}


void regulator_P(PID *pid)
{
    uint8_t kp = eeprom_read_byte(&Kp);

    // UART_Send("\n\rkp: ");
	// sprintf(data, "%6d", kp );
	// UART_Send(data);

    int16_t pout;
    pout = pid->err*kp;// / 10;

    // UART_Send("\n\rpout: ");
	// sprintf(data, "%6d", pout );
	// UART_Send(data);

    if( pout > pid->p_max)
    {
        pid->p = pid->p_max;
    // UART_Send("\n\rWarunek > \n\r");
    }
    else if( pout < (-pid->p_max) )
    {
        pid->p = (-pid->p_max);
        // UART_Send("\n\rWarunek < \n\r");
    }
    else
    {
        pid->p = pout;
        // UART_Send("\n\rWarunek else \n\r");
    }
    // UART_Send("Pout: ");
	// sprintf(data, "%6d", pid->p );
	// UART_Send(data);
	// UART_Send("\n\r");
}

void regulator_I(PID *pid)
{
    uint8_t ki = eeprom_read_byte(&Ki);
    int16_t iout;

    // suma do calki
    pid->i_sum += ( pid->err + pid->last_err ) * pid->dt / 2;

    iout = pid->i_sum * ki;// / 10;

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
    // UART_Send("Iout: ");
	// sprintf(data, "%6d", pid->i );
	// UART_Send(data);
	// UART_Send("\n\r");
}

void regulator_D(PID *pid)
{
    uint8_t kd = eeprom_read_byte(&Kd);
    int16_t dout;

    dout = (( pid->err - pid->last_err ) / pid->dt) * kd;// / 10;

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
    // UART_Send("Dout: ");
	// sprintf(data, "%6d", pid->d );
	// UART_Send(data);
	// UART_Send("\n\r");
}