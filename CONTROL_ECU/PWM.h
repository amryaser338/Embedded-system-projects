/******************************************************************************
 *
 * Module: 	PWM
 *
 * File Name: PWM.h
 *
 * Description: header file for the ATmega32 PWM driver
 *
 * Author: Amr Yasser
 *
 *******************************************************************************/


#ifndef PWM_H_
#define PWM_H_
#include "STD_TYPES.h"
#define PWM_RESET_PRESCALER_BITS 0XF8

typedef enum{
	PWM_NO_CLK,
	PWM_F_CPU_CLK,
	PWM_F_CPU_8,
	PWM_F_CPU_64,
	PWM_F_CPU_256,
	PWM_F_CPU_1024
}PWM_PreScaler_Type;
// Function to initialize Timer0 and start PWM

void PWM_Timer0_Start(u8 duty_cycle);

#endif /* PWM_H_ */
