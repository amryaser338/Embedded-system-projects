/******************************************************************************
 *
 * Module: DCmotor
 *
 * File Name: DCmotor.h
 *
 * Description: header file for the ATmega32 DC MOTOR driver
 *
 * Author: Amr Yasser
 *
 *******************************************************************************/
#ifndef DCMOTOR_H_
#define DCMOTOR_H_
#include "std_types.h"
#define DCMOTOR_ENABLE_EN TRUE

/* Enumeration for Motor States*/
typedef enum {
    MOTOR_CW,     // Clockwise rotation
    MOTOR_ACW     // Anti-Clockwise rotation
} DcMotor_State;

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
/* Struct for motor configuration: direction and enable pins */
typedef struct {
	u8 DcMotor_DIR_PORT :2;   /* Direction port */
	u8 DcMotor_DIR_PINA :3;   /* Direction pin A */
	u8 DcMotor_DIR_PINB :3;   /* Direction pin B */
#if(DCMOTOR_ENABLE_EN == TRUE)
	u8 DCMotor_EN_PORT :2;    /* Enable port */
	u8 DcMotor_EN_PIN :3;     /* Enable pin */
#endif
} DcMotor;
#define MAX_SPEED	(100)
#define MIN_SPEED	(0)
extern DcMotor motor;
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/* Initialize motor pins */
void DcMotor_init(DcMotor *Motor);

/* Rotate motor in the specified direction with speed */
void DcMotor_Rotate(DcMotor *Motor, DcMotor_State state, u8 speed);
void DcMotor_STOP(DcMotor *Motor);


#endif /* DCMOTOR_H_ */
