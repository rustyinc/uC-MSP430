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
 * A library that controls the 7-segment display using a 4094. Ports are
 * setup by calling the 7Seg4094Init function.
 *
 * -------------------------------------------------------------------------
 * Result:
 * Link to video: - None -
 ****************************************************************************/
#include <msp430.h>
#include <ctype.h>
#include "7seg4094Lib.h"

static unsigned int  PxOUT;
static unsigned char DATA;
static unsigned char CLOCK;
static unsigned char STROBE;

/**
 * Macros
 */
#define TOGGLE_CLOCK(port, clock)  \
{                                  \
    ((port) |= (clock));           \
    __delay_cycles(1000);          \
    ((port) &= ~(clock));          \
    __delay_cycles(1000);          \
}


/**
 * Arrays for the numbers
 */
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

/**
 * Array for the letters
 */
static const char LEDAryLetters[] = {
        L_A | L_B | L_C | L_E | L_F | L_G, /* Letter A */
        L_C | L_D | L_E | L_F | L_G,       /* Letter B */
        L_A | L_D | L_E | L_F,             /* Letter C */
        L_B | L_C | L_D | L_E | L_G,       /* Letter D */
        L_A | L_D | L_E | L_F | L_G,       /* Letter E */
        L_A | L_E | L_F | L_G,             /* Letter F */
};

/*************************************************************************
 * Forward declarations:
 */
static void writeLED(unsigned char led, eDotState dot);

/*************************************************************************
 * Function to initialize the 4094 communication
 *************************************************************************/
void SevenSeg4094Init(ePortDefinitions port, unsigned char data,
                  unsigned char clock, unsigned char strobe,
                  unsigned char latch)
{
    unsigned int port_direction;
    unsigned int port_select;
    unsigned int port_select2;
    unsigned int port_out;
    if(port == PORT1)
    {
        port_direction = P1DIR;
        port_select    = P1SEL;
        port_select2   = P1SEL2;
        port_out       = P1OUT;
    }
    else
    {
        port_direction = P2DIR;
        port_select    = P2SEL;
        port_select2   = P2SEL2;
        port_out       = P2OUT;
    }

    port_direction |= (data | clock | strobe | latch);
    port_select &= ~(data | clock | strobe | latch);
    port_select2 &= ~(data | clock | strobe | latch);

    /* Setup Latch high - always */
    port_out |= latch;
    /* Clear the clock bit */
    port_out &= ~clock;

    /* Save the data pins setup for later use */
    DATA   = data;
    CLOCK  = clock;
    STROBE = strobe;
    PxOUT  = port_out;
}

/***********************************************************************
 * Function to write numbers to the 7-segment LED
 ***********************************************************************/
void writeNumberLED(unsigned char number, eDotState dot)
{
    unsigned char out = LEDAryNumbers[number];

    if(number >= sizeof(LEDAryNumbers))
        return;

    writeLED(out, dot);
}

/************************************************************************
 * Function to write a letter to the 7-segment LED
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
 * Function to write any combination to 7-segment LED
 ************************************************************************/
void writeAnyCombinationLED(unsigned char combi, eDotState dot)
{
    writeLED(combi, dot);
}

/************************************************************************
 * static function to update the LED
 ************************************************************************/
static void writeLED(unsigned char led, eDotState dot)
{
    unsigned char i;

    /* Clear strobe */
    PxOUT &= ~STROBE;

    /* Set or clear the dot */
    led &= ~L_DP;
    led |= dot;

    /* Write out data */
    for(i = 8; i > 0; i--)
    {
        /* Toggle out the zero */
        if(led & 0x01)
        {
            /* Set bit */
            PxOUT |= DATA;
        }
        else
        {
            /* Clear Bit */
            PxOUT &= ~DATA;
        }
        led >>= 1; /* Bitshift one to left */
        TOGGLE_CLOCK(PxOUT, CLOCK);
    }
    /* Strobe data - set data out */
    PxOUT |= STROBE;
}

