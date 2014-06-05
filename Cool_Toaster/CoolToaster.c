/****************************************************************************
 * Project Name:    Cool toaster
 * Project Created: 07-02-14
 * Author:          RUST
 *
 * ------------------------------------------------------------------------
 * Version history:
 * 07-02-14: Initial version
 *
 * =========================================================================
 * Description:
 * The CoolToaster project implements a statemachine than can control a
 * toasters toast-function and defrost-function. The buttons are controlled
 * by a pull-scheme
 *
 * State / Event diagram
 *                          ___________                      __________
 *                         /           \  BUTTON_ON_OFF     /          \
 *                   /--->| State:      |----------------->| State:     |
 * BUTTON_DEFROSTING |    | OFF         | BUTTON_ON_OFF    | TOASTING   |
 *                   \___/|             |<-----------------|            |
 *                         \___________/                    \__________/
 *                                     ^                    /
 *                                      \                  / BUTTON_DEFROST
 *                         BUTTON_ON_OFF \  ___________   /
 *                                         /           \ V
 *                                        | State:      |<---\
 *                                        | DEFROSTING  |     | BUTTON_DEFROST
 *                                        |             |     |
 *                                         \___________/ ----/
 *
 * State / Event table.
 * |================================================|
 * |STATE / EVENT|| BUTTON_ON_OFF  | BUTTON_DEFROST |
 * |================================================|
 * | OFF         || TOASTING       | --             |
 * |------------------------------------------------|
 * | TOASTING    || OFF            | DEFROSTING     |
 * |------------------------------------------------|
 * | DEFROSTING  || OFF            | --             |
 * |------------------------------------------------|
 *
 ****************************************************************************/

#include <msp430.h> 

/* TODO: Add your states and events here. Remember to use good names.*/
enum States {STATE_TOASTER_ON, STATE_TOASTER_OFF, STATE_TOASTER_DEFROSTING};
enum Events {BUTTON_ON_OFF, BUTTON_DEFROSTING};
/* TODO: Set the current state to the initial state.*/
int current_state = STATE_TOASTER_OFF;

/* Forward declarations (don't mind yet) */
void SM_toaster();
void doState(int);
void doOnEnter(int);
void doOnExit(int);

/**
 * Entering point for the program.
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	int event; /* Used to hold the received button event */

	/* TODO: SETUP THE OUTPUT PORTS (LED PORTS): e.g. P2DIR = .... */
	P2DIR  = 0xFF; // Setup all pins in port 2 to output direction
	P2OUT  = 0x00; // Turn off all LEDs initially
	P2SEL  = 0;    // Mandatory in order to use the P6 and P7 of port 2
	P2SEL2 = 0;
	
	/* Setup the input ports (Button ports) */
	P1DIR = 0x00;

	while(1) // Loop forever
    {
    	/*
    	 * Test if buttons are pressed (pulling), and generate the event from the button press
    	 */
		event = 0xFF;
		while(event == 0xFF)
		{
			if((P1IN & BIT4) == 0) // Test if port 4 is zero (button pressed)
			{
				event = BUTTON_ON_OFF;
				__delay_cycles(250000); // sleep 250k clock cycles (necessary sleep)
			}
			else if((P1IN & 0x20) == 0) // Test if port 5 is zero (button pressed)
			{
				event = BUTTON_DEFROSTING;
			}
			else
			{
				/* No buttons pressed - do nothing */
			}
		}
		SM_toaster(event);
    }
}

/**
 * Function to handle the toaster state machine
 */
void SM_toaster(int event)
{
	int next_state;

	next_state = current_state;

	/* TODO: ADD YOUR STATE MACHINE HERE!! */
	switch(current_state)
	{
		/* Testing state STATE_TOASTER_OFF */
		case STATE_TOASTER_OFF:
		{
			switch(event)
			{
				case BUTTON_ON_OFF:
					next_state = STATE_TOASTER_ON;
				break;

				case BUTTON_DEFROSTING:
					/* Stay in state - do nothing */
				break;
			}
		}
		break;

		/* Testing state STATE_TOASTER_ON */
		case STATE_TOASTER_ON:
		{
			switch(event)
			{
				case BUTTON_ON_OFF:
					next_state = STATE_TOASTER_OFF;
				break;

				case BUTTON_DEFROSTING:
					next_state = STATE_TOASTER_DEFROSTING;
				break;
			}
		}
		break;

		/* Testing state STATE_TOASTER_DEFROSTING */
		case STATE_TOASTER_DEFROSTING:
		{
			switch(event)
			{
				case BUTTON_ON_OFF:
					next_state = STATE_TOASTER_OFF;
				break;

				case BUTTON_DEFROSTING:
					/*Stay in state STATE_TOASTER_DEFROSTIN - do nothing*/
				break;
			}
		}
		break;
	}

	/* Test if the states are not equal - and handle the*/
	if(next_state != current_state)
	{
		doOnExit(current_state);
		doOnEnter(next_state, event);
		current_state = next_state;
	}
	doState(current_state); // Handle the state functionality.
}

/**
 * Function doState handles the functionality inside the state
 */
void doState(int state)
{
	/* TODO: TEST THE NEXT STATE, AND DO THE REQUIRED FUNCTIONALITY */
	switch(state)
	{
		case STATE_TOASTER_OFF:
		{
			P2OUT = 0; // Turn off all the LEDs
		}
		break;

		case STATE_TOASTER_ON:
		{
			/* Turn on the three LEDs in each side of the board
			 * (ON ON ON OFF OFF ON ON ON)
			 *   1  1  1  0   0   1  1  1 (0xE7 hexadecimal) (231 decimal)
			 */
			P2OUT = 0xE7;
		}
		break;

		case STATE_TOASTER_DEFROSTING:
		{
			/* Turn on the two LEDs in the middle of the board
			 * (OFF OFF OFF ON ON OFF OFF OFF)
			 *   0   0   0   1  1  0   0   0  (0x18 hexadecimal) (24 decimal)
			 */
			P2OUT = 0x18;
		}
		break;
	}
}

/**
 * Handles the functionality before entering a specific state, when a specific event happens
 * Probably not used on the toaster assignment
 */
void doOnEnter(int state, int event)
{
	switch(state)
	{
		case STATE_TOASTER_OFF:
		{
			switch(event)
			{
				case TIMEROUT:
					/* Say Ding */
				break;
			}
		}
		break;
	}
}


/**
 * Handles the functionality before leaving a specific state, when a specific event happens
 * Probably not used on the toaster assignment
 */
void doOnExit(int state)
{
	/* Not part of the assignment toaster. Do nothing here. */
	switch(state)
	{
		/* ... */
	}
}
