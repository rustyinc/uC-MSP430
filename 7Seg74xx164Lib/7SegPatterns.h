/****************************************************************************
 * Project Name:    7Seg74xx164Lib
 * Author:          RUST
 * Contact:         rust@eal.dk
 *
 * Filename:        7SegPatterns.h
 *
 * =========================================================================
 * Version History:
 * 19-05-2014 : Initial Version
 *
 * =========================================================================
 * Description:
 * This project implements the driver for the 74xx164 bitshift register chip.
 *
 * File containing the LED patterns
 * -------------------------------------------------------------------------
 * Result:
 * Link to video: - None -
 ****************************************************************************/

#ifndef __7SEGPATTERNS_H__
#define __7SEGPATTERNS_H__

#include "7SegPatterns.h"

/* Predefined patterns  */
typedef enum
{
   LED_0 = (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F),         /*  0  */
   LED_1 = (SEG_B | SEG_C),                                         /*  1  */
   LED_2 = (SEG_A | SEG_B | SEG_D | SEG_E | SEG_G),                 /*  2  */
   LED_3 = (SEG_A | SEG_B | SEG_C | SEG_D | SEG_G),                 /*  3  */
   LED_4 = (SEG_B | SEG_C | SEG_F | SEG_G),                         /*  4  */
   LED_5 = (SEG_A | SEG_C | SEG_D | SEG_F | SEG_G),                 /*  5  */
   LED_6 = (SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G),         /*  6  */
   LED_7 = (SEG_A | SEG_B | SEG_C),                                 /*  7  */
   LED_8 = (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G), /*  8  */
   LED_9 = (SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G),         /*  9  */
   LED_A = (SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G),         /*  A  */
   LED_B = (SEG_C | SEG_D | SEG_E | SEG_F | SEG_G),                 /*  b  */
   LED_C = (SEG_A | SEG_D | SEG_E | SEG_F),                         /*  C  */
   LED_D = (SEG_B | SEG_C | SEG_D | SEG_E | SEG_G),                 /*  d  */
   LED_E = (SEG_A | SEG_D | SEG_E | SEG_F | SEG_G),                 /*  E  */
   LED_F = (SEG_A | SEG_E | SEG_F | SEG_G),                         /*  F  */
   LED_H = (SEG_B | SEG_C | SEG_E | SEG_F | SEG_G),                 /*  H  */
   LED_DOT = SEG_DP,                                                /* DOT */
   LED_CLEAR = 0                                                    /*Clear*/
}eLEDPatterns;

static const char patternArray[] = {
        LED_0, LED_1, LED_2, LED_3, LED_4, LED_5, LED_6, LED_7, LED_8, LED_9,
        LED_A, LED_B, LED_C, LED_D, LED_E, LED_F, LED_H, LED_DOT, LED_CLEAR
};


#endif /* 7SEGPATTERNS_H_ */
