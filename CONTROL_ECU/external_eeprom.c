 /******************************************************************************
 *
 * Module: External EEPROM
 *
 * File Name: external_eeprom.c
 *
 * Description: Source file for the External EEPROM Memory
 *
 * Author: Amr Yasser
 *
 *******************************************************************************/
#include "external_eeprom.h"
#include <util/delay.h>
#include "twi.h"
#define PASSWORD_ADDRESS	(0x0311)

u8 EEPROM_writeByte(u16 u16addr, u8 u8data)
{
	/* Send the Start Bit */
    TWI_start();
    if (TWI_getStatus() != TWI_START)
        return ERROR;
		
    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write) */
    TWI_writeByte((u8)(0xA0 | ((u16addr & 0x0700)>>7)));
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return ERROR; 
		 
    /* Send the required memory location address */
    TWI_writeByte((u8)(u16addr));
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;
		
    /* write byte to eeprom */
    TWI_writeByte(u8data);
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;

    /* Send the Stop Bit */
    TWI_stop();
	
    return SUCCESS;
}

u8 EEPROM_readByte(u16 u16addr, u8 *u8data)
{
	/* Send the Start Bit */
    TWI_start();
    if (TWI_getStatus() != TWI_START)
        return ERROR;
		
    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write) */
    TWI_writeByte((u8)((0xA0) | ((u16addr & 0x0700)>>7)));
    if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
        return ERROR;
		
    /* Send the required memory location address */
    TWI_writeByte((u8)(u16addr));
    if (TWI_getStatus() != TWI_MT_DATA_ACK)
        return ERROR;
		
    /* Send the Repeated Start Bit */
    TWI_start();
    if (TWI_getStatus() != TWI_REP_START)
        return ERROR;
		
    /* Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=1 (Read) */
    TWI_writeByte((u8)((0xA0) | ((u16addr & 0x0700)>>7) | 1));
    if (TWI_getStatus() != TWI_MT_SLA_R_ACK)
        return ERROR;

    /* Read Byte from Memory without send ACK */
    *u8data = TWI_readByteWithNACK();
    if (TWI_getStatus() != TWI_MR_DATA_NACK)
        return ERROR;

    /* Send the Stop Bit */
    TWI_stop();

    return SUCCESS;
}
u8 EEPROM_writeString(u16 u16addr, const u8 *str)
{
    while (*str)
    {
        /* Write each character in the string to EEPROM */
        EEPROM_writeByte(u16addr++, *str++);
        _delay_ms(10);
    }
    /* Write null terminator to indicate end of string */
    EEPROM_writeByte(u16addr, '\0');
    _delay_ms(10);
    return SUCCESS;
}

/*
 * Read String from EEPROM at a specific address
 */
u8 EEPROM_readString(u16 u16addr, u8 *str, u8 maxLen)
{
    u8 i = 0;
    u8 data;

    /* Read each character from EEPROM until null terminator or maxLen */
    for (i = 0; i < maxLen - 1; i++)
    {
        if (EEPROM_readByte(u16addr++, &data) != SUCCESS)
        {
            return ERROR;  // Return error if read operation fails
        }
        str[i] = data;
        if (data == '\0')
        {
            return SUCCESS;  // Stop if null terminator is found
        }
    }
    str[i] = '\0';  // Ensure null termination if maxLen limit is reached
    return SUCCESS;
}

