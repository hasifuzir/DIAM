
/*******************************************************************************
This program sets up the MSP432 to generate a 2.5 kHz sine wave.  The output is
taken from the 8 digital pins of PORT_P2 so you need to build an external DAC to
convert these digital outputs to an analogue signal and hence the sine wave.

The master clock is set to 48 MHz and the SysTick timer is set to count to 960.
Therefore, the systick_isr is called at a rate of 48 MHz / 960 = 50 kHz.  Each
cycle of a sine wave is constructed using 20 points.  Therefore, the frequency
of the sine wave is 50 kHz / 20 = 2.5 kHz.
 *
 * Author: Dr Chin-Pang Liu, University College London, 2016
 ******************************************************************************/
/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define PTS 20 // no of points in one cycle of a sine wave
#define PI 3.14159
volatile int sin_value[PTS];

int main(void)
{
    /* Halting the Watchdog */
    MAP_WDT_A_holdTimer();


	int t;

	for( t = 0; t < PTS; t = t + 1)
	{
		sin_value[t] = 255*(sin(2*PI*(t/(float)PTS))+1)/2;
	}


    /* Configuring pins for HFXT crystal */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_PJ,
            GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    MAP_GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN0);


    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7);


    /* Configuring GPIOs (P4.3 MCLK) */
       MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P4, GPIO_PIN3,
       GPIO_PRIMARY_MODULE_FUNCTION);

    /* Setting the external clock frequency. This API is optional, but will
     * come in handy if the user ever wants to use the getMCLK/getACLK/etc
     * functions
     */
    CS_setExternalClockSourceFrequency(32000,48000000);

    /* Starting HFXT in non-bypass mode without a timeout. Before we start
     * we have to change VCORE to 1 to support the 48MHz frequency */
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);
    CS_startHFXT(false);

    /* Initializing MCLK to HFXT (effectively 48MHz) */
    MAP_CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);

       /* Configuring SysTick to trigger at 24000000 (MCLK is 48MHz so this will
     * make it toggle every 0.5s) */
    MAP_SysTick_enableModule();
    MAP_SysTick_setPeriod(960);
    MAP_Interrupt_enableSleepOnIsrExit();
    MAP_SysTick_enableInterrupt();


     /* Enabling MASTER interrupts */
    MAP_Interrupt_enableMaster();

    while (1)
    {
        //MAP_PCM_gotoLPM0();
    }
}

void systick_isr(void)
{
	P6OUT |= BIT0; // set P6 PIN0 High


	static int j = 0;

	P2OUT = sin_value[j];  // This is where the sine wave value is written to the 8 output pins of port 2
	j++;
		if (j == PTS)
		{j = 0;}


	P6OUT &= ~BIT0; // set P6 PIN0 Low

}



