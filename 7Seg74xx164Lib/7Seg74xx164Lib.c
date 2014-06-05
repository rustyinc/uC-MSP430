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
 * C file, that implements the 7Seg74xx164Lib driver
 *
 * -------------------------------------------------------------------------
 * Result:
 * Link to video: - None -
 ****************************************************************************/
#include <msp430.h>
#include <stdlib.h>
#include "7Seg74xx164Lib.h"
#include "7SegPatterns.h"

/***
 * instance data struct struct
 */
typedef struct
{
    unsigned char *PORTx;
    unsigned int CLOCK;
    unsigned int DATA;
    unsigned int MASTER_RST;
}instance_data_t;

/***
 * Forward declarations
 */
static void writeLED(instance_data_t* inst, unsigned char data);

/****************************************************************************
 * Function for initiating the 74xx164 driver
 ****************************************************************************/
void* init7Seg74xx164Lib(ePortDefinitions port, unsigned int data, unsigned int clock, unsigned int master_reset)
{
    instance_data_t* inst = (instance_data_t*) malloc(sizeof(instance_data_t));
    memset(inst, 0, sizeof(instance_data_t));

    if (port == PORT1)
    {
        /* Setup port 1 direction and set select bits */
        P1DIR  |=  (clock | data | master_reset);
        P1SEL  &= ~(clock | data | master_reset);
        P1SEL2 &= ~(clock | data | master_reset);
        inst->PORTx = (unsigned char*)&P1OUT;
    }
    else
    {
        /* Setup port 1 direction and set select bits */
        P2DIR  |=  (clock | data | master_reset);
        P2SEL  &= ~(clock | data | master_reset);
        P2SEL2 &= ~(clock | data | master_reset);
        inst->PORTx = (unsigned char*)&P2OUT;
    }

    /* Make sure Master Reset is set high */
    *inst->PORTx |= master_reset;
    /* Set clock initially to low */
    *inst->PORTx &= ~(clock);

    /* Save the registers */
    inst->DATA       = data;
    inst->CLOCK      = clock;
    inst->MASTER_RST = master_reset;

    return (void*) inst;
}

/******************************************************************************
 * Write a number to the 7-segment LED
 *****************************************************************************/
void writeNumber74xx164(void* inst, unsigned char number, eDotDefinitions dot)
{
    if (number <= 0x0F)
    {
        number = patternArray[number];
        number = (dot == DOT_ON) ? (number | LED_DOT) : (number & ~LED_DOT);
        writeLED((instance_data_t*)inst, number);
    }
    else
    {
        /* do nothing - number out of range */
    }
}

/******************************************************************************
 * Write a bitpattern to the 74xx164 which is generated from the user
 *****************************************************************************/

void writeSelfPattern74xx164(void* inst, unsigned char data)
{
    writeLED((instance_data_t*)inst, data);
}

/******************************************************************************
 * Write the bitpattern to the 74xx164
 *****************************************************************************/
static void writeLED(instance_data_t* inst, unsigned char data)
{
    unsigned int i;
    /* Clear display */
    for(i = 8; i > 0; i--)
    {
        /* Check if bit to transmit is high or low */
        if(data & 0x01)
        {
            *inst->PORTx |= inst->DATA;
        }
        else
        {
            *inst->PORTx &= ~inst->DATA;
        }

        *inst->PORTx |= inst->CLOCK;
        *inst->PORTx &= ~(inst->CLOCK);
        data >>= 1;
    }
}
