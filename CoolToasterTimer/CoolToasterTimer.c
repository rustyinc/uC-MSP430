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
 * by a pull-scheme. Besides the buttons, toaster do also implement the
 * timer functionality
 *
 * State / Event diagram               ________TIMEOUT________
 *                          __________V                      _\________
 *                         /           \  BUTTON_ON_OFF     /          \
 *                   /--->| State:      |----------------->| State:     |
 * BUTTON_DEFROSTING |    | OFF         | BUTTON_ON_OFF    | TOASTING   |
 *                   \___/|             |<-----------------|            |
 *                         \___________/                    \__________/
 *                        ^            ^                    /
 *                       /              \                  / BUTTON_DEFROST
 *                      |  BUTTON_ON_OFF \  ___________   /
 *                      |                  /           \ V
 *                       \   TIMEOUT      | State:      |<---\
 *                        \---------------| DEFROSTING  |     | BUTTON_DEFROST
 *                                        |             |     |
 *                                         \___________/ ----/
 *
 * State / Event table.
 * |==============================================================|
 * |STATE / EVENT|| BUTTON_ON_OFF  | BUTTON_DEFROST | TIMEOUT     |
 * |==============================================================|
 * | OFF         || TOASTING       | --             | --          |
 * |--------------------------------------------------------------|
 * | TOASTING    || OFF            | DEFROSTING     | OFF         |
 * |--------------------------------------------------------------|
 * | DEFROSTING  || OFF            | --             | OFF         |
 * |--------------------------------------------------------------|
 *
 ****************************************************************************/

#include <msp430.h>

#define HALF_SECOND 62500 // Half second (1 MHZ divide by 8)

/* TODO: Add your states and events here. Remember to use good names.*/
enum States {STATE_TOASTER_ON, STATE_TOASTER_OFF, STATE_TOASTER_DEFROSTING};
enum Events {BUTTON_ON_OFF, BUTTON_DEFROSTING, TIMER_TIMEOUT};
/* TODO: Set the current state to the initial state.*/
static int current_state = STATE_TOASTER_OFF;
static int TimerValueSeconds;

/* Forward declarations (don't mind yet) */
void SM_toaster();
void doState(int);
void doOnEnter(int, int);
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
	
	P1DIR |= BIT0;  // Enable counter LED P1.0
	P1OUT &= ~BIT0; // Turn off counter LED.

	/* Setup the input ports (Button ports) */
	P1DIR &= ~(BIT4 | BIT5);

	/* Setup The ADC to handle the input */
	ADC10CTL1 = ADC10DIV_3 + INCH_7; 			// No division, Input A7
	ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON;  // Sample and hold 16, REF Vref+ and Vref-, Enable conversion

	/* Setup timer from the ADC reading. 0x000 -> 0 sec and 0x3FF -> 60 sec. */
    TA0CCR0 = 0;
    CCTL0 = CCIE;
    TA0CTL = TASSEL_2 | ID_3 | MC_1; // SMCLK and DIVIDE BY 8 and UP
    _enable_interrupts();

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
			else if(TimerValueSeconds == 0)
			{
				event = TIMER_TIMEOUT;
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

				case TIMER_TIMEOUT:
					/* Do nothing */
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

				case TIMER_TIMEOUT:
					next_state = STATE_TOASTER_OFF;
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

				case TIMER_TIMEOUT:
					next_state = STATE_TOASTER_OFF;
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
			TA0CCR0 = 0; // Stop timer
			TimerValueSeconds = 0; // Stop decrementing
			P1OUT &= ~BIT0; // Turn off counter LEDs p1.0
		}
		break;

		case STATE_TOASTER_ON:
		{
			/* Readin ADC */
			ADC10CTL0 |= ADC10SC + ENC;     // Start ADC conversion
			while(!(ADC10IFG & ADC10CTL0))
				;
			ADC10CTL0 &= ~ADC10IFG; // Clear flag
			TimerValueSeconds = ADC10MEM;
			TimerValueSeconds >>= 3; // Calculate value to a number between 0 and 128 (half seconds)

			/* Turn on the three LEDs in each side of the board
			 * (ON ON ON OFF OFF ON ON ON)
			 *   1  1  1  0   0   1  1  1 (0xE7 hexadecimal) (231 decimal)
			 */
			P2OUT = 0xE7;
			TA0CCR0 = HALF_SECOND;
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
				case TIMER_TIMEOUT:
					/**
					 * Write code that sings hallelujah (or just a ding)
					 */
				break;

				case BUTTON_ON_OFF:     // Fall through (no break)
				case BUTTON_DEFROSTING:
					/* Do nothing*/
				break;
			}
		}

		case STATE_TOASTER_ON:
		{
			switch(event)
			{
				case TIMER_TIMEOUT: // Fall through (no break)
				case BUTTON_ON_OFF: // Fall through (no break)
				case BUTTON_DEFROSTING:
					/* Do nothing*/
				break;
			}
		}

		case STATE_TOASTER_DEFROSTING:
		{
			switch(event)
			{
				case TIMER_TIMEOUT: // Fall through (no break)
				case BUTTON_ON_OFF: // Fall through (no break)
				case BUTTON_DEFROSTING:
					/* Do nothing*/
				break;
			}
		}
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

#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer_interrupt(void)
{
	if(TimerValueSeconds > 0)
	{
		TimerValueSeconds--;
		P1OUT ^= BIT0;
	}
	// else - do nothing
}
