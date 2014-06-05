/****************************************************************************
 * Project Name:    7SegTest4094Lib
 * Author:          RUST
 * Contact:         rust@eal.dk
 *
 * =========================================================================
 * Version History:
 * 18.05-2014 : Initial Version
 *
 * =========================================================================
 * Description:
 * A library that controls the 7-segment display using a 4094
 *
 * -------------------------------------------------------------------------
 * Result:
 * Link to video: - None -
 ****************************************************************************/
#ifndef __7SEG4094LIB_H__
#define __7SEG4094LIB_H__

/***************
 *      A
 *     ---
 *  F |   | B
 *    | G |
 *     ---
 *  E |   | C
 *    |   |
 *     ---  o DP
 *      D
 *
 * BIT0 : DP
 * BIT1 : C
 * BIT2 : D
 * BIT3 : E
 * BIT4 : B
 * BIT5 : A
 * BIT6 : G
 * BIT7 : F
 */
#define L_DP BIT0
#define L_C BIT1
#define L_D BIT2
#define L_E BIT3
#define L_B BIT4
#define L_A BIT5
#define L_G BIT6
#define L_F BIT7

/**
 * Enums to be used by the caller
 */
typedef enum {
    PORT1,
    PORT2
}ePortDefinitions;

typedef enum
{
    DOT_OFF = 0,
    DOT_ON = L_DP
}eDotState;

/***
 * Function to initiate the 7-Segment communication with the 4094.
 */
void SevenSeg4094Init(ePortDefinitions port, unsigned char data,
                  unsigned char clock, unsigned char strobe,
                  unsigned char latch);


/***
 * Function to write a number to the the 7-segment.
 */
void writeNumberLED(unsigned char number, eDotState dot);

/***
 * Function to write a letter to the 7-segment.
 */
void writeLetterLED(unsigned char letter, eDotState dot);

/***
 * Function to write any combination of a,b,c,d,e,f,g and dot
 */
void writeAnyCombinationLED(unsigned char letter, eDotState dot);

#endif /* 7SEG4094LIB_H_ */
