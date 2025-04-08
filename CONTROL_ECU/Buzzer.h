/******************************************************************************
 *
 * Module: BUZZER
 *
 * File Name: Buzzer.h
 *
 * Description: header file for the ATmega32 Buzzer driver
 *
 * Author: Amr Yasser
 *
 *******************************************************************************/


#ifndef BUZZER_H_
#define BUZZER_H_

#include "STD_TYPES.h"

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/
typedef enum {
	BUZZER_ACTIVE_HIGH,
	BUZZER_ACTIVE_LOW
} BUZZER_state;

/* Struct for buzzer configuration: port, pin, and active state */
typedef struct {
	u8 PORT :2;  /* Port number (0-3) */
	u8 PIN :3;   /* Pin number (0-7) */
	BUZZER_state state :1;  /* Active state (high/low) */
} BUZZER_conf;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/
void Buzzer_init(BUZZER_conf *config);  // Initializes the buzzer pin
void Buzzer_on(BUZZER_conf *config);    // Activates the buzzer
void Buzzer_off(BUZZER_conf *config);   // Deactivates the buzzer

extern BUZZER_conf BUZZER;  // External declaration of the buzzer

#endif /* BUZZER_H_ */
