/******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: Buzzer.c
 *
 * Description: Source file for the Buzzer driver
 *
 * Author: Amr Yasser
 *
 *******************************************************************************/
#include "Buzzer.h"

#include "DIO_interface.h"
BUZZER_conf BUZZER={DIO_PORTC, DIO_PIN7,BUZZER_ACTIVE_HIGH };
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
void Buzzer_init(BUZZER_conf *config) {
	DIO_enumSetPinDirection(config->PORT, config->PIN, DIO_PIN_OUTPUT);
    Buzzer_off(config);
}


/* Turn on the buzzer */
void Buzzer_on(BUZZER_conf *config) {
	DIO_enumSetPinValue(config->PORT, config->PIN, (DIO_PIN_HIGH ^ config->state));
}

/* Turn off the buzzer */
void Buzzer_off(BUZZER_conf *config) {
	DIO_enumSetPinValue(config->PORT, config->PIN, (DIO_PIN_LOW ^ config->state));
}
