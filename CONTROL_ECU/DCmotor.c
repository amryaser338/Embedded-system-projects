/******************************************************************************
 *
 * Module: DCmotor
 *
 * File Name: DCmotor.c
 *
 * Description: Source file for the ATmega32 DC MOTOR  driver
 *
 * Author: Amr Yasser
 *
 *******************************************************************************/
#include "DCmotor.h"

#include<avr/io.h>

#include "DIO_interface.h"
#include "PWM.h"
// Initialize the Fan motor settings
DcMotor motor = { .DcMotor_DIR_PORT = DIO_PORTD, .DcMotor_DIR_PINA = DIO_PIN6,
		.DcMotor_DIR_PINB = DIO_PIN7, .DCMotor_EN_PORT = DIO_PORTB,
		.DcMotor_EN_PIN = DIO_PIN3 };

/*
 * Function: DcMotor_init
 * Description: Set up motor direction pins and enable pin.
 * Parameters: Pointer to structure of the Fan motor.
 * Return: void
 */
void DcMotor_init(DcMotor *Motor) {
	DIO_enumSetPinDirection(Motor->DcMotor_DIR_PORT, Motor->DcMotor_DIR_PINA, DIO_PIN_OUTPUT);
	DIO_enumSetPinDirection(Motor->DcMotor_DIR_PORT, Motor->DcMotor_DIR_PINB, DIO_PIN_OUTPUT);
#if(DCMOTOR_ENABLE_EN == TRUE)
	DIO_enumSetPinDirection(Motor->DCMotor_EN_PORT, Motor->DcMotor_EN_PIN, DIO_PIN_OUTPUT);
#endif
}

/*
 * Function: DcMotor_Rotate
 * Description: Control motor rotation direction and speed.
 * Parameters: Pointer to structure of the Fan motor.
 * Return: void
 */
void DcMotor_Rotate(DcMotor *Motor, DcMotor_State state, u8 speed) {
	PWM_Timer0_Start(speed); // Start PWM with specified speed

	switch (state) {
	case MOTOR_CW:
		DIO_enumSetPinValue(Motor->DcMotor_DIR_PORT, Motor->DcMotor_DIR_PINA, DIO_PIN_HIGH);
		DIO_enumSetPinValue(Motor->DcMotor_DIR_PORT, Motor->DcMotor_DIR_PINB, DIO_PIN_LOW);
		break;
	case MOTOR_ACW:
		DIO_enumSetPinValue(Motor->DcMotor_DIR_PORT, Motor->DcMotor_DIR_PINA, DIO_PIN_LOW);
		DIO_enumSetPinValue(Motor->DcMotor_DIR_PORT, Motor->DcMotor_DIR_PINB, DIO_PIN_HIGH);
		break;
	default:
		DcMotor_STOP(Motor); // Stop motor if state is invalid
		break;
	}
}

/*
 * Function: DcMotor_STOP
 * Description: Stop the motor by setting direction pins to low.
 * Parameters: Pointer to structure of the Fan motor.
 * Return: void
 */
void DcMotor_STOP(DcMotor *Motor) {
	DIO_enumSetPinValue(Motor->DcMotor_DIR_PORT, Motor->DcMotor_DIR_PINA, DIO_PIN_LOW);
	DIO_enumSetPinValue(Motor->DcMotor_DIR_PORT, Motor->DcMotor_DIR_PINB, DIO_PIN_LOW);
}
