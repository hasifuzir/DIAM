/*
 This is our code for an audio descrambler.
 * Author: A bunch of idiots, University College London, 2016
 *****************************************************************************
 */

#define PI 3.14159265f
/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>

#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "filter.h"

static volatile float sine[50];

int main(void)
{
    /* Halting the Watchdog */
    MAP_WDT_A_holdTimer();
    
    /* Configure P5.6 and P5.7 to their analog functions to output VREF */
    	P5SEL0 |= BIT6 | BIT7;
        P5SEL1 |= BIT6 | BIT7;

        REFCTL0 |= REFON;                     // Turn on reference module
        REFCTL0 |= REFOUT;                    // Output reference voltage to a pin

     /* Output VREF = 1.2V */
        REFCTL0 &= ~(REFVSEL_3);              // Clear existing VREF voltage level setting
        REFCTL0 |= REFVSEL_0;                 // Set VREF = 1.2V
        while (REFCTL0 & REFGENBUSY);       // Wait until the reference generation is settled

    /* Configuring pins for HFXT crystal */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_PJ,
            GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Set P6.0 as output */
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN0);

    /* Set all 8 pins of P2 as output */
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7);

    /* Configuring GPIOs (4.3 MCLK) */
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

    /* Configuring SysTick to trigger at 50kHz (MCLK is 48MHz so this will
     * make it toggle every 0.5s) */
    MAP_SysTick_enableModule();
    MAP_SysTick_setPeriod(960);
    MAP_Interrupt_enableSleepOnIsrExit();
    MAP_SysTick_enableInterrupt();

    /* Initializing ADC (MCLK/1//1) */
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1,
    0);

    /* Configuring GPIOs (P5.0 (A5) as the ADC input pin) */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN0,
    GPIO_TERTIARY_MODULE_FUNCTION);

    /* Configure ADC Resolution */
    ADC14_setResolution(ADC_10BIT);

    /* Configuring ADC Memory */
    MAP_ADC14_configureSingleSampleMode(ADC_MEM0, true);
    MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_INTBUF_VREFNEG_VSS,
    ADC_INPUT_A5, false);

    /* Configuring Sample Timer */
    MAP_ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);

     /* Enabling MASTER interrupts */
    MAP_Interrupt_enableMaster();

    int N = 0;
    for(N=0;N<50;N++){
        float a = N*7.0f;
        float b = a/50.0f;
        float c = b*2.0f*PI;
        //float t = N*2*PI*7/50;
        float n = sin(c);
        sine[N] = n;
    }

    while (1)
    {
        //MAP_PCM_gotoLPM0();
    }
}
int M = 0;
void systick_isr(void)
{
	P6OUT |= BIT0; // set P6 PIN0 High

	static long temp_ADC = 0;
	temp_ADC = ADC14_getResult(ADC_MEM0);  //Get the conversion result.
	float y_bandstop = bandstop(temp_ADC);		
	float input_bandstop_sine = y_bandstop * sine[M];
	P2OUT = input_bandstop_sine / 4;  //We do this because the ADC is set to use 10 bits but P2OUT is only 8 bits.
	M = M+1;
	if(M > 49){  // 7 full sine waves, 50 samples
	M = 0;
}
	/* Enabling/Toggling Conversion */
	MAP_ADC14_enableConversion();
	MAP_ADC14_toggleConversionTrigger();
	MAP_ADC14_toggleConversionTrigger();

	P6OUT &= ~BIT0; // set P6 PIN0 Low
}

/* ADC Interrupt Handler. This handler is called whenever there is a conversion
* that is finished for ADC_MEM0.
*/
void adc_isr(void)
{
	uint64_t status = MAP_ADC14_getEnabledInterruptStatus();
	MAP_ADC14_clearInterruptFlag(status);
}
