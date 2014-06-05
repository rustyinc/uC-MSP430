/****************************************************************************
 * Project Name:    7SegTest4094
 * Author:          RUST
 * Contact:         rust@eal.dk
 *
 * =========================================================================
 * Version History:
 * 17-03-2014 : Initial Version
 *
 * =========================================================================
 * Description:
 * This project implements the driver for the 4094 bitshift register chip.
 *
 * Port setup:
 * P2.0: Clock
 * P2.1: Data
 * P2.2: Strobe
 * P2.3: Output enable
 *
 * -------------------------------------------------------------------------
 * Result:
 * Link to video: - None -
 ****************************************************************************/
#include <msp430.h>
#include <ctype.h>

/* Definition of signals */
#define DATA          BIT0
#define CLOCK         BIT1
#define STROBE        BIT2
#define OUTPUT_ENABLE BIT3

#define TOGGLE_CLOCK        \
{                           \
    P2OUT |= CLOCK;         \
    __delay_cycles(1000);   \
    P2OUT &= ~CLOCK;         \
    __delay_cycles(1000);   \
}

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

static const char LEDAryNumbers[] = {
        L_A | L_B | L_C | L_D | L_E | L_F,        /* 0 */
              L_B | L_C,                          /* 1 */
        L_A | L_B |       L_D | L_E |        L_G, /* 2 */
        L_A | L_B | L_C | L_D |              L_G, /* 3 */
              L_B | L_C |             L_F  | L_G, /* 4 */
        L_A |       L_C | L_D |       L_F  | L_G, /* 5 */
        L_A |       L_C | L_D | L_E | L_F  | L_G, /* 6 */
        L_A | L_B | L_C,                          /* 7 */
        L_A | L_B | L_C | L_D | L_E | L_F  | L_G, /* 8 */
        L_A | L_B | L_C | L_D |       L_F  | L_G  /* 9 */
};

static const char LEDAryLetters[] = {
        L_A | L_B | L_C | L_E | L_F | L_G, /* Letter A */
        L_C | L_D | L_E | L_F | L_G,       /* Letter B */
        L_A | L_D | L_E | L_F,             /* Letter C */
        L_B | L_C | L_D | L_E | L_G,       /* Letter D */
        L_A | L_D | L_E | L_F | L_G,       /* Letter E */
        L_A | L_E | L_F | L_G,             /* Letter F */
};

typedef enum
{
    DOT_OFF = 0,
    DOT_ON = L_DP
}eDotState;

/* Forward declaration */
void writeNumberLED(unsigned char number, eDotState dot);
void writeLetterLED(unsigned char letter, eDotState dot);
void writeLED(unsigned char led, eDotState dot);

/************************************************************************
 * Main function
 ************************************************************************/
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    /* Port 1 initializering */
    P1DIR = 0x00;

    /*
     * P2.0 : Data
     * P2.1 : Clock
     * P2.2 : Strobe
     * P2.3 : Output Enable
     */
    P2DIR |= CLOCK | DATA | STROBE | OUTPUT_ENABLE;
    P2SEL = 0;
    P2SEL2 = 0;

    /* Always enable output */
    P2OUT |= OUTPUT_ENABLE;
    /* Clear the clock bit */
    P2OUT &= ~CLOCK;

    unsigned int i;
    unsigned char letter = 'a';
    eDotState dot = DOT_ON;
    for(i = 0; i < sizeof(LEDAryNumbers); i++)
    {
        writeNumberLED(i, dot);
        __delay_cycles(1000000);
        dot = (dot == DOT_ON) ? DOT_OFF : DOT_ON;
    }

	return 0;
}

/************************************************************************
 * WriteLetterLED
 ************************************************************************/
void writeLetterLED(unsigned char letter, eDotState dot)
{
    unsigned char out;

    letter = toupper(letter);
    out = letter - 0x41; /* Get index in table */

    if(out > sizeof(LEDAryLetters))
        return;

    out = LEDAryLetters[out];
    writeLED(out, dot);
}

/************************************************************************
 * WriteNumberLED
 ************************************************************************/
void writeNumberLED(unsigned char number, eDotState dot)
{
    unsigned char out = LEDAryNumbers[number];

    if(number >= sizeof(LEDAryNumbers))
        return;

    writeLED(out, dot);
}

/************************************************************************
 * WriteLED
 ************************************************************************/
void writeLED(unsigned char led, eDotState dot)
{
    unsigned char i;

    /* Clear strobe */
    P2OUT &= ~STROBE;

    /* Set or clear the dot */
    led &= ~L_DP;
    led |= dot;

    /* Write out data */
    for(i = 0; i < 8; i++)
    {
        /* Toggle out the zero */
        if(led & 0x01)
        {
            /* Set bit */
            P2OUT |= DATA;
        }
        else
        {
            /* Clear Bit */
            P2OUT &= ~DATA;
        }
        led >>= 1; /* Bitshift one to left */
        TOGGLE_CLOCK;
    }
    /* Strobe data - set data out */
    P2OUT |= STROBE;
}
