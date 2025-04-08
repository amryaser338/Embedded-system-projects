/*
 * CONTROL_ECU_program.c
 *
 * Author: Amr Yasser
 */
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include <util/delay.h>

#include"DIO_interface.h"
#include"DCmotor.h"
#include"CLCD_interface.h"
#include"USART_interface.h"
#include"external_eeprom.h"
#include"TIMER_interface.h"
#include"PIR.h"
#include"Buzzer.h"

// Constants and Macros
#define EEPROM_ADDRESS_PASSWORD 0x0311
#define PASS_SIZE 5
#define READY 0xAA
#define DONE 0xFF
#define OPEN_DOOR '+'
#define CHANGE_PASS '-'
#define OP_MOTOR 0x0A
#define CL_MOTOR 0xA0
#define WAITING 0xCC

// Global Variables
u8 IS_PASS_CREATE = 0;           // Flag indicating if the password is created
u8 PASS[PASS_SIZE] = "";         // Stores the user-entered password
u8 RE_PASS[PASS_SIZE] = "";      // Stores the re-entered password
u8 SAVED_PASS[PASS_SIZE] = "";   // Stored password read from EEPROM
u8 attempts = 0;                 // Counter for failed attempts
u8 time = 0;                     // Timer variable

// Function Prototypes
void init(void);
u8 recieve_Byte(void);
void send_Byte(u8 byte);
void receive_Password(u8 *a_PasswordArray);
u8 check_pass(u8 *pass1, u8 *pass2);
void change_pass(void);
void timer_handler(void);
void lock_system(void);

/* Main Function */
void main() {
	init();			//Initialization

	change_pass();  // Ensure a password is set

	// Main control loop
	while (1) {
		u8 operation = recieve_Byte();	// Receive operation command from HMI ECU

		switch (operation) {
		case OPEN_DOOR:	// Handle door opening functionality
			for (u8 i = 0; i < 3; i++) {

				receive_Password(PASS);	// Receive password from HMI ECU

				if (check_pass(SAVED_PASS, PASS) == true) {	// Check if password is correct
					attempts = 0;
					send_Byte(OK);	// Send confirmation
					time = 0;

					// Rotate motor to open door
					while (time < 15) {
						send_Byte(OP_MOTOR);
						DcMotor_Rotate(&motor, MOTOR_CW, 100);

					}

					// Handle PIR sensor for detecting people
					if (PIR_getState(&Config_Ptr_PIR)) {

						while (PIR_getState(&Config_Ptr_PIR)) {
							send_Byte(WAITING);
							DcMotor_STOP(&motor);

						}
					}

					// Rotate motor to close door
					time = 0;
					while (time < 15) {
						send_Byte(CL_MOTOR);
						DcMotor_Rotate(&motor, MOTOR_ACW, 100);
					}
					DcMotor_STOP(&motor);	// Stop motor after operation
					send_Byte(0);			// Indicate door operation is complete
					break;

				} else {
					send_Byte(NOK);			// Send rejection
					attempts++;
					if (attempts >= 3) {
						lock_system();		 // Lock system after 3 failed attempts
					}
				}
			}
			break;

		case CHANGE_PASS:	 // Handle password change functionality
			for (u8 i = 0; i < 3; i++) {
				receive_Password(RE_PASS);	// Receive the old password

				if (check_pass(SAVED_PASS, RE_PASS) == true) {// Check if old password matches
					send_Byte(OK);
					attempts=0;
					change_pass();	// Allow user to change password
					break;
				} else {
					send_Byte(NOK);	// Send rejection
					attempts++;
					if (attempts >= 3) {
						lock_system();    // Lock system after 3 failed attempts
						break;
					}
				}

			}
			break;

		}

	}
}
// Initialization for the system
void init() {
		 //Initialize Buzzer
	USART_voidInit();			 // Initialize UART communication
	TIMER1_voidInit();			 // Initialize Timer
	TIMER_u8SetCallBack(timer_handler, TIMER1_CTCA_VECTOR_ID);	// Set timer callback
	PIR_init(&Config_Ptr_PIR);	 // Initialize PIR sensor
	DcMotor_init(&motor);  		 // Initialize DC motor
	Buzzer_init(&BUZZER);        // Initialize Buzzer
	GIE_VoidEnable();			 // Enable global interrupts
}

/* Function to receive a single byte over UART */
u8 recieve_Byte(void) {
	u8 byte;
	u8 k;
	USART_u8RecevieData(&k);
	while (k != READY) {
		USART_u8RecevieData(&k);
	}
	USART_u8SendData(READY);
	USART_u8RecevieData(&byte);
	USART_u8SendData(DONE);
	return byte;
}

/* Function to send a single byte over UART */
void send_Byte(u8 byte) {
	u8 k;
	USART_u8SendData(READY);
	USART_u8RecevieData(&k);
	while (k != READY) {
		USART_u8RecevieData(&k);
	}
	USART_u8SendData(byte);
	USART_u8RecevieData(&k);
	while (k != DONE) {
		USART_u8RecevieData(&k);
	}
}

/* Function to receive a password array over UART */
void receive_Password(u8 *a_PasswordArray) {
	u8 i = 0;
	u8 k;
	USART_u8RecevieData(&k);
	while (k != READY) {
		USART_u8RecevieData(&k);

	}
	USART_u8SendData(READY);
	while (i < PASS_SIZE) {
		USART_u8RecevieData(&a_PasswordArray[i]);
		i++;
	}
	USART_u8SendData(DONE);
}

/* Function to check if two passwords match */
u8 check_pass(u8 *pass1, u8 *pass2) {
	u8 i;
	for (i = 0; i < PASS_SIZE; i++) {
		if (pass1[i] != pass2[i]) {
			return false;  // Passwords do not match
		}
	}
	return true;
}

/* Function to change the password */
void change_pass(void) {
	while (IS_PASS_CREATE == 0) {
		receive_Password(PASS);      // Receive new password
		receive_Password(RE_PASS);   // Receive re-entered password
		if (check_pass(PASS, RE_PASS)) {
			IS_PASS_CREATE = 1;
			EEPROM_writeString(EEPROM_ADDRESS_PASSWORD, PASS); // Save password to EEPROM
			send_Byte(OK);
		} else {
			send_Byte(NOK);
		}
	}
	EEPROM_readString(EEPROM_ADDRESS_PASSWORD, SAVED_PASS, PASS_SIZE + 1); // Load saved password
	IS_PASS_CREATE = 0;
}

/* Timer handler function */
void timer_handler(void) {
	time++;
}

/* Function to lock the system after multiple failed attempts */
void lock_system(void){
	attempts=0;
	Buzzer_on(&BUZZER);
	time=0;
	while(time<10)
		;
	Buzzer_off(&BUZZER);

}

