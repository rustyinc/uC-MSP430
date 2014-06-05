/****************************************************************************
 * Project Name:    7Seg74xx164Lib
 * Author:          RUST
 * Contact:         rust@eal.dk
 *
 * =========================================================================
 * Version History:
 * 19-05-2014 : Initial Version
 *
 * =========================================================================
 * Description:
 * This project implements the driver for the 74xx164 bitshift register chip.
 *
 * -------------------------------------------------------------------------
 * Result:
 * Link to video: - None -
 ****************************************************************************/

#ifndef __7SEG74XX164LIB_H__
#define __7SEG74XX164LIB_H__

/***
 * enums
 */
typedef enum {
    PORT1,
    PORT2
}ePortDefinitions;

typedef enum {
    DOT_ON,
    DOT_OFF
}eDotDefinitions;

/* Define the different segments */
typedef enum
{
    SEG_B = BIT0,
    SEG_A = BIT1,
    SEG_F = BIT2,
    SEG_G = BIT3,
    SEG_DP = BIT4,
    SEG_C = BIT5,
    SEG_D = BIT6,
    SEG_E = BIT7
}eLEDSegments;

/**
 * Function for init the 74xx164 driver library
 */
void* init7Seg74xx164Lib(ePortDefinitions, unsigned int data,
        unsigned int clock, unsigned int master_reset);

/***
 * Function for writing a number between 0-16 (inclusive A-F) to the
 * 7-segment LED.
 */
void writeNumber74xx164(void*, unsigned char, eDotDefinitions);

/***
 * Function used to write a selfcomponened pattern
 */
void writeSelfPattern74xx164(void*, unsigned char);

#endif /* 7SEG74XX164LIB_H_ */
