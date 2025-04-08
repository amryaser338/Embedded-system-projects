 /******************************************************************************
 *
 * Module: TWI(I2C)
 *
 * File Name: twi.h
 *
 * Description: Header file for the TWI(I2C) AVR driver
 *
 * Author: Amr Yasser
 *
 *******************************************************************************/ 

#ifndef TWI_H_
#define TWI_H_

#include "STD_TYPES.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
#define MASTER_ADDRESS    (0b00000010)    /* Master Address = 1 */
#define BIT_RATE          (400000UL)      /* 400 Kbs */

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */
/*******************************************************************************
 *                      Type Definitions                                       *
 *******************************************************************************/
typedef u8 TWI_AddressType;
typedef u32 TWI_BaudRateType;

typedef struct {
    TWI_AddressType address;
    TWI_BaudRateType bit_rate;
} TWI_ConfigType;
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void TWI_init(const TWI_ConfigType * Config_Ptr);
void TWI_start(void);
void TWI_stop(void);
void TWI_writeByte(u8 data);
u8 TWI_readByteWithACK(void);
u8 TWI_readByteWithNACK(void);
u8 TWI_getStatus(void);

extern TWI_ConfigType TWI_conf;

#endif /* TWI_H_ */
