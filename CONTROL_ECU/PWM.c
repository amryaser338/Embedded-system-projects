/******************************************************************************
 *
 * Module: PWM
 *
 * File Name: PWM.c
 *
 * Description: Source file for the ATmega32 PWM driver
 *
 * Author: Amr Yasser
 *
 *******************************************************************************/

#include "PWM.h"

#include <avr/io.h>
#include "DIO_interface.h"

void PWM_Timer0_Start(u8 duty_cycle) {

	DDRB |= (1 << PB3);  /* Set PB3 (OC0) as output for PWM */
		TCCR0 = (1 << WGM00) | (1 << WGM01) | (1 << COM01); /* Set Fast PWM mode and clear on compare match */
		TCCR0 = ((TCCR0 & PWM_RESET_PRESCALER_BITS) | PWM_F_CPU_64); /* Start Timer0 with prescaler of 1024 */
		OCR0 = (u8) (((u16) duty_cycle * 255) / 100); /* Set the duty cycle when the given is percentage (0/100)% */
	}


