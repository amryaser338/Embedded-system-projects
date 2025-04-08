/******************************************************************************
 *
 * Module: PIR
 *
 * File Name: PIR.c
 *
 * Description: Source file for the AVR PIR driver
 *
 * Author: Amr Yasser
 *
 *******************************************************************************/
#include "PIR.h"

#include <avr/io.h>

#include "BIT_MATH.h"
#include "DIO_interface.h"
PIR_ConfigType Config_Ptr_PIR = {DIO_PORTC,DIO_PIN2 };
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void PIR_init(const PIR_ConfigType *Config_Ptr_PIR) {
	/* Configure PIR sensor pin as input */
	DIO_enumSetPinDirection(Config_Ptr_PIR->port, Config_Ptr_PIR->pin,DIO_PIN_OUTPUT);
}

u8 PIR_getState(const PIR_ConfigType *Config_Ptr_PIR) {
	/* Return the PIR sensor state (HIGH if motion detected, LOW otherwise) */
	u8 k;
	DIO_enumGetPinValue(Config_Ptr_PIR->port,Config_Ptr_PIR->pin,&k);
	return k;
}

