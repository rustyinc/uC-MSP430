/*
 * 24AA04_EEPROM.c
 *
 *  Created on: 10/04/2014
 *      Author: rust
 */
#include <msp430.h>
#include "usi_i2c.h"

#define ADDRESS     0xA0
#define BLOCK_BIT   0x02
#define RW_BIT      0x01
#define BLOCK_SIZE  16  // Bytes (maximum in each write cycle)
#define EEPROM_SIZE 512 // Bytes

typedef enum writeArrayEnum
{
    ADDRESS_IDX = 0,
    EEPROM_IDX = 1,
    DATA_IDX = 2
}eWriteArrayEnum;

/* Forward declerations */
void waitI2CToComplete();

/* This function writes data to the Microchip 24AA04 EEPROM
 * All writes are performed from index zero in block 0 and up.
 */
unsigned int writeEEPROM(const char* data, unsigned int length)
{
    unsigned int data_idx = 0;
    unsigned int writeAry[18] = {0, 0, 0};
    unsigned char dumme_read;
    if(length > 0 && length <= EEPROM_SIZE)
    {
        while (data_idx < length)
        {
            // Set address with block bit
            writeAry[ADDRESS_IDX] = (data_idx < (EEPROM_SIZE >> 1)) ?
                    ADDRESS :
                    (ADDRESS | BLOCK_BIT);

            // Set data
            writeAry[DATA_IDX] = *(data + data_idx);

            // Write to the I2C device
            i2c_send_sequence(writeAry, sizeof (writeAry), &dumme_read, 0);

                // Wait for I2C to complete
            while(!i2c_done()) ;
            __delay_cycles(10000); // Dummy wait 25 ms.

            // Update control data
            data_idx++;
            writeAry[EEPROM_IDX] = (data_idx == EEPROM_SIZE >> 1) ?
                    0 :
                    writeAry[EEPROM_IDX] + 1;
        }
    }
    /* else - do nothing */

    return data_idx;
}


unsigned int readEEPROM(char* data, unsigned int length)
{
    return 0;
}
