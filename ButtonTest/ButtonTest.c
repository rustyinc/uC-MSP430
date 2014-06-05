/****************************************************************************
 * Name: ButtonTest.c
 * Created: 03/02-2014
 * Version: 1.0
 *
 * Author RUST:
 * Erhvervsakademiet Lillebaelt. (www.eal.dk)
 * ------------------------------------------------------------------------
 * Version history:
 * 03/02-2014: Initial version (1.0)
 *
 * =========================================================================
 * Description:
 * Library to hide monitoring of buttons and assigning events to button
 * presses.
 ***************************************************************************/

#include <msp430.h> 
#include "ButtonTest.h"

/* Definitions */
#define FALSE 0
#define TRUE !FALSE
#define MAX_BUTTONS_PORT 16

/* Define the boolean value */
typedef int bool;

typedef struct
{
	portSelection port;
	pinSelection pin;
	int event;
	int active;
	bool pressed;
}pinData_t;

static pinData_t pinDataAry[MAX_BUTTONS_PORT];
static unsigned int nextEmptyInAry = 0;

void setupButton(portSelection port, pinSelection pin, int event, edgeDirection dir)
{
    /* Check the params */
	if(pin < BIT0 || pin > BIT7) return;
	if(nextEmptyInAry >= MAX_BUTTONS_PORT) return; // No more room the the array

	/* Setup Input Port*/
	if(port == PORT1)
	{
		P1DIR &= ~pin; // Set pin to input.
		P1REN |= pin;  // Set resistor on.
		if(dir == HIGH_TO_LOW)
		{
			P1IES |= pin;  // Set pin to interrupt high-to-low. (Interrupt edge selection)
			P1OUT |= pin;  // Pull up resistor
		}
		else
		{
			P1IES &= ~pin;  // Set pin til interrupt low-to-high. (Interrupt edge selection)
			P1OUT &= ~pin;  // Pull down resistor
		}
		P1IFG &= ~pin; // Pin IFG cleared. (Interrupt flag)
		P1IE  |= pin;  // Enable interrupt on Pin
	}
	else
	{
		P2DIR &= ~pin; // Set pin to input.
		P2REN |= pin;  // Set resistor on.
		if(dir == HIGH_TO_LOW)
		{
			P2IES |= pin;  // Set pin to interrupt high-to-low. (Interrupt edge selection)
			P2OUT |= pin;  // Pull up resistor
		}
		else
		{
			P2IES &= ~pin;  // Set pin til interrupt low-to-high. (Interrupt edge selection)
			P2OUT &= ~pin;  // Pull down resistor
		}

		P2IFG &= ~pin; // Pin IFG cleared. (Interrupt flag)
		P2IE  |= pin;  // Enable interrupt on Pin
	}

    /* Save the information in the array */
    pinDataAry[nextEmptyInAry].port = port;
	pinDataAry[nextEmptyInAry].pin = pin;
	pinDataAry[nextEmptyInAry].event = event;
	pinDataAry[nextEmptyInAry].active = (dir == LOW_TO_HIGH) ? 1 : 0; // Set the button active high / low
	pinDataAry[nextEmptyInAry].pressed = FALSE;
    nextEmptyInAry++;

    _BIS_SR(/*LPM4_bits + */GIE); // Enter LPM4 w/ Global interrupt enable
}

int getButtonEvent()
{
	unsigned int i;
	int event;
	bool breakLoop = FALSE;

	while(!breakLoop)
	{
		for(i = 0; i < nextEmptyInAry; i++)
		{
			if(pinDataAry[i].pressed == TRUE)
			{
				event = pinDataAry[i].event;
				pinDataAry[i].pressed = FALSE;
				breakLoop = TRUE;
			    i = nextEmptyInAry;
			}
		}
	}
	return event;
}

// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
	unsigned int i;
	/* look for the interrupted pin */
	for(i = 0; i < nextEmptyInAry; i++)
	{
		if(pinDataAry[i].port == PORT1 &&
		   P1IFG & pinDataAry[i].pin)
		{
			pinDataAry[i].pressed = TRUE;
			P1IFG &= ~pinDataAry[i].pin; // Clear the interrupt
		} /* else: Do nothing - we are only looking at port 1 */
	}
}

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
	unsigned int i;
	/* look for the interrupted pin */
	for(i = 0; i < nextEmptyInAry; i++)
	{
		if(pinDataAry[i].port == PORT2 &&
		   P2IFG & pinDataAry[i].pin)
		{
			pinDataAry[i].pressed = TRUE;
			P2IFG &= ~pinDataAry[i].pin; // Clear the interrupt
		} /* else: Do nothing - we are only looking at port 2 */
	}
}
