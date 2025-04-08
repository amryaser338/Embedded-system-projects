/*
 * HMI_ECU_program.c
 *
 * Author: Amr Yasser
 */
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include <util/delay.h>
#include"DIO_interface.h"
#include"CLCD_interface.h"
#include"USART_interface.h"
#include"KPD_interface.h"
#include"TIMER_interface.h"

// Constants and Macros
#define PASS_SIZE 5
#define ENTER_BUTTON '='
#define READY 0xAA
#define DONE 0xFF
#define OPEN_DOOR '+'
#define CHANGE_PASS '-'
#define OP_MOTOR 0x0A
#define CL_MOTOR 0xA0
#define WAITING 0xCC

// Global Variables
u8 PASS[PASS_SIZE] = "";         // Stores the user-entered password
u8 RE_PASS[PASS_SIZE] = "";      // Stores the re-entered password
u8 time = 0;					 // Timer variable
u8 attempts = 0;				 // Track the number of incorrect attempts

// Function Prototypes
void init(void);
u8 recieve_Byte(void);
void send_Byte(u8 byte);
void send_Password(u8 *Password_ptr1);
void creat_pass(u8 *pass, u8 *message);
void System_Pass(void);
void timer_handler(void);
void lock_system(void);


void main(void) {
	u8 choice;
	init(); 		//Initialization
	// Set up system password
	System_Pass();
	CLCD_voidClearScreen();

	 // Main application loop
	while (1) {
		 // Display options on LCD
		CLCD_voidSetPosition(1, 1);
		CLCD_voidSendString("+ : Open Door");
		CLCD_voidSetPosition(2, 1);
		CLCD_voidSendString("- :Change Pass");

		// Get user choice from keypad
		choice = KPD_u8GetPressed();
		send_Byte(choice);	// Send the choice to the control unit

		switch (choice) {
		case OPEN_DOOR:	// Option to open the door
			for (u8 i = 0; i < 3; i++) {
				CLCD_voidClearScreen();
				creat_pass(PASS, "ENTER PASS");  // Prompt user to enter password
				send_Password(PASS);			 // Send password to control unit

				u8 CONFIRM_KEY = recieve_Byte(); // Receive confirmation
				if (CONFIRM_KEY == OK) {
					attempts=0;
					u8 door = 1;
					while (door) {
						door = recieve_Byte();
						if (door == OP_MOTOR) {
							CLCD_voidSetPosition(1, 1);
							CLCD_voidSendString("Door Unlocking         ");

						} else if (door == CL_MOTOR) {
							CLCD_voidSetPosition(1, 1);
							CLCD_voidSendString("Door locking           ");
							CLCD_voidSetPosition(2, 1);
							CLCD_voidSendString("                       ");

						} else if (door == WAITING) {
							CLCD_voidSetPosition(1, 1);
							CLCD_voidSendString("Wait for people");
							CLCD_voidSetPosition(2, 1);
							CLCD_voidSendString("   to enter ");

						}

					}
					break;
				} else if (CONFIRM_KEY == NOK) {
					attempts++;
					if (attempts >= 3) {
						lock_system();	// Lock the system after 3 incorrect attempts
					}
				}
			}
			break;

		case CHANGE_PASS:
			for (u8 i = 0; i < 3; i++) {
				creat_pass(RE_PASS, "ENTER OLD PASS"); // Prompt user to enter old password
				send_Password(RE_PASS);    			   // Send old password to control unit
				u8 CONFIRM_KEY = recieve_Byte();	   // Receive confirmation
				if (CONFIRM_KEY == OK) {
					attempts=0;
					System_Pass();  // Set up a new password
					break;
				} else if (CONFIRM_KEY == NOK) {
					attempts++;
					if (attempts >= 3) {
						lock_system();	// Lock the system after 3 incorrect attempts
					}
				}
			}
			break;
		}
	}
}
// Initialization for the system
void init(void) {
	CLCD_voidInit();	 //Initialize Buzzer
	KPD_Init();			 // Initialize KPAD
	USART_voidInit();	 // Initialize UART communication
	TIMER1_voidInit();	 // Initialize Timer
	TIMER_u8SetCallBack(timer_handler, TIMER1_CTCA_VECTOR_ID);// Set timer callback
	GIE_VoidEnable();			 // Enable global interrupts
}

/* Function to receive a single byte */
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

/* Function to send a single byte */
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

/* Function to send a password array */
void send_Password(u8 *Password_ptr1) {
	u8 i;
	u8 k;
	USART_u8SendData(READY);
	USART_u8RecevieData(&k);
	while (k != READY) {
		USART_u8RecevieData(&k);
	}
	for (i = 0; i < PASS_SIZE; i++) {
		USART_u8SendData(Password_ptr1[i]);
	}
	USART_u8RecevieData(&k);
	while (k != DONE) {
		USART_u8RecevieData(&k);
	}
}

/* Function to create a password */
void creat_pass(u8 *pass, u8 *message) {
	u8 i = 0;
	u8 number;

	CLCD_voidClearScreen();
	CLCD_voidSendString(message);
	CLCD_voidSetPosition(2, 1);
	while (i < PASS_SIZE) {
		number = KPD_u8GetPressed();
		if (number >= '0' && number <= '9') {
			pass[i++] = number;
			CLCD_voidSendData('*');
			_delay_ms(250);
		}
	}
	while (KPD_u8GetPressed() != ENTER_BUTTON)
		;
	CLCD_voidClearScreen();
}

/* Function to set up a system password */
void System_Pass() {
	while (1) {
		creat_pass(PASS, "Enter Pass:");
		creat_pass(RE_PASS, "RE_Enter Pass:");

		send_Password(PASS);
		send_Password(RE_PASS);

		if (recieve_Byte() == OK) {
			break;
		} else {
			CLCD_voidSendString("TRY AGAIN");
			_delay_ms(250);
		}
	}
}

/* Timer handler function */
void timer_handler(void) {
	time++;
}

/* Function to lock the system */
void lock_system(void) {
	attempts = 0;
	CLCD_voidClearScreen();
	CLCD_voidSendString("System is locked");
	time = 0;
	while (time < 10)
		;
	CLCD_voidClearScreen();

}
