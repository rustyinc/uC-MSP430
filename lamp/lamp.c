/****************************************************************************
 * Project Name:    Lamb
 * Project Created: 28-01-14
 * Author:          RUST
 *
 * ------------------------------------------------------------------------
 * Version history:
 * 28-01-14: Initial version
 *
 * =========================================================================
 * Description:
 * A simple state machine describing a lamb that can be turned on / off
 *
 *
 ****************************************************************************/

#include <msp430.h>

enum states {
	LAMP_ON,
	LAMP_OFF
};

enum events {
	BUTTON_ON,
	BUTTON_OFF
};

int currentState;
int currentEvent;
int nextState;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    currentState = LAMP_OFF;
    currentEvent = BUTTON_OFF;

    switch(currentState)
    {
        // STATE LAMP ON
        case LAMP_ON:
            switch(currentEvent)
            {
                case BUTTON_OFF:
                    nextState = LAMP_OFF;
				break;

    			case BUTTON_ON:
    				// Do nothing
    			break;
    		}
    	break;

    	// STATE LAMP OFF
    	case LAMP_OFF:
    		switch(currentEvent)
    		{
    			case BUTTON_ON:
    				nextState = LAMP_ON;
    			break;

    			case BUTTON_OFF:
    			break;
    		}
    	break;
    }

	return 0;
}
