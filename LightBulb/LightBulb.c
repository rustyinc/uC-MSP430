/****************************************************************************
 * Name: LightBulb
 * Created: 29/01-2014
 * Version: 1.0
 *
 * Author RUST:
 * Erhvervsakademiet Lillebaelt. (www.eal.dk)
 * ------------------------------------------------------------------------
 * Version history:
 * 29/01-2014: Initial version (1.0)
 *
 * =========================================================================
 *
 * This lightBulb project demonstrates a state machine design on a single
 * lightbulb. The light bulb is simulated by using a single LED on the
 * LaunchPad evaulation kit.
 *
 *             _____________                    ___________________
 *            /             \    Press P1.4    /                   \
 * Press  /-->| State: Off  | ---------------> | State: Illuminate | <--\ Press
 * P1.5   |   |             |    Press P1.5    |                   |    | P1.4
 *        \__/| P2.0 Off    | <--------------  |  P2.0 On          |\___/
 *            \_____________/                  \___________________/
 *
 * Button - Active low on P1.4
 * Button - Active low on P1.5
 * LED    - Active high on P2.0
 *
 *****************************************************************************/

#include <msp430.h>

// Definitions og TRUE and FALSE. Can become handy.
#define FALSE 0
#define TRUE !FALSE

enum eStates {
	STATE_BULB_OFF,
	STATE_BULB_ILLUMINATES
};

enum eEvents {
	EVENT_TURN_BULB_ON,  // Press on P1.4
	EVENT_TURN_BULB_OFF, // Press on P1.5
	NO_SUCH_EVENT
};

// Forward decelerations
void handle_state_bulb_off(int event);
void handle_state_bulb_on(int event);
int  read_in_button();

// Global definition of the state
int state = STATE_BULB_OFF; // Set initial state


int main(void) {
	// Setup the microcontroller
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    P1DIR &= ~0x30;             // Set port P1.4 og P1.5 to inputs. (Clear bits for P1.4 og P1.5)
                                // Tilde (~) changes all the bits, so 1 -> 0 og 0 -> 1.
                                // Exameple: 1: 0011 is the same as ~1100.
                                //           2: 11001111 is the same as ~00110000
                                //              (0xCF    is the same as ~0x30)

    P2DIR |= 0x01;              // Set port P2.0 til output.


    // Enter the state machine
    int event;                  // Variable used to hold the event.

    for(;;)                     // Loop forever
    {
    	event = read_in_button();
    	switch(state)
    	{
    	   case STATE_BULB_OFF:
    		   handle_state_bulb_off(event);
    	   break;

    	   case STATE_BULB_ILLUMINATES:
    		   handle_state_bulb_on(event);
    	   break;
    	}
    }
    return 0;
}

// Function to handle state bulb off
void handle_state_bulb_off(int event)
{
	// Test the events - and perform the appropriate action
	switch(event)
	{
		case EVENT_TURN_BULB_ON:
			P2OUT |= 0x01;       // Turn on the LED
			state = STATE_BULB_ILLUMINATES;
		break;

		case EVENT_TURN_BULB_OFF:
			// Do nothing - The LED is already in a state where it is turned off
		break;
	}
}

// Function to handle state bulb on
void handle_state_bulb_on(int event)
{
	// Test the events - and perform the appropriate action
	switch(event)
	{
		case EVENT_TURN_BULB_OFF:
			P2OUT &= ~0x01;      // Turn off the LED
			state = STATE_BULB_OFF;
	    break;

		case EVENT_TURN_BULB_ON:
			// Do nothing - The LED is already in a state where it illuminates.
		break;
	}
}

// Function that monitors and read in the buttons, and returns the button event.
int read_in_button()
{
	int event;
	int loop = TRUE;

	event = NO_SUCH_EVENT;
	do
	{
		if((P1IN & 0x10) == 0)
		{
			event = EVENT_TURN_BULB_ON; // Button on port P1.4 pressed, which is the "on" button.
			loop = FALSE;               // Stop looping.
		}
		else if((P1IN & 0x20) == 0)
		{
			event = EVENT_TURN_BULB_OFF; // Button on port 1.5 pressed, which is the "off" button.
			loop = FALSE;                // Stop looping.
		}
		else
		{
			// Continue looping (do nothing)
		}
	}while(event == NO_SUCH_EVENT);

	// Find the button pressed and return the event.
	return event;
}
