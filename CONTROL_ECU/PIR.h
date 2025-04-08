 /******************************************************************************
 *
 * Module: PIR
 *
 * File Name: PIR.h
 *
 * Description: Header file for the AVR PIR driver
 *
 * Author: Amr Yasser
 *
 *******************************************************************************/
#ifndef PIR_H_
#define PIR_H_

#include "STD_TYPES.h"

/*******************************************************************************
 *                      Type Definitions                                       *
 *******************************************************************************/

/* Structure to hold PIR sensor configuration */
typedef struct {
    u8 port;   /* PIN Register to read the port state */
    u8 pin;                 /* Pin number for the PIR sensor */
} PIR_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void PIR_init(const PIR_ConfigType *Config_Ptr_PIR);
u8 PIR_getState(const PIR_ConfigType *Config_Ptr_PIR);
extern PIR_ConfigType Config_Ptr_PIR;
#endif /* PIR_H_ */
