/*
 * bitmanip.h
 *
 * Created: 30.11.2021 20:06:19
 *  Author: Maciek
 */ 


#ifndef BITMANIP_H_
#define BITMANIP_H_

#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <string.h>


#ifndef _BV
#define _BV(bit)				(1<<(bit))
#endif
#ifndef sbi
#define sbi(reg,bit)		reg |= (_BV(bit))
#endif

#ifndef cbi
#define cbi(reg,bit)		reg &= ~(_BV(bit))
#endif

#ifndef tbi
#define tbi(reg,bit)		reg ^= (_BV(bit))
#endif

// def PORTA
#define RedLed PA7
#define BlueLed PA6
#define Sw PA5
#define Stby PA4

// def PORTC
#define m_ctrl_B1 PC4
#define m_ctrl_B2 PC5
#define m_ctrl_A1 PC6
#define m_ctrl_A2 PC7




#endif /* BITMANIP_H_ */