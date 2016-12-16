//#include "driverlib.h"
//#include <stdint.h>
//#include <stdbool.h>
//#include <math.h>
//
//#define PI 3.14159265f
//static volatile float currentADC = 0;
//static volatile float sine[50];
//
//static float bandstop_g = 0.97547839075053411;
//static float bandstop_num[3] = {1, -1.0719921365629379, 1};
//static float bandstop_den[3] = {1, -1.0457051642716415, 0.95095678150106822};
//
//float bandstop_buf[3] = {0,0,0};
//
//void update_buffer(float *w){
//
//  w[0] = w[1];              //First buffer value replaced by second value
//  w[1] = w[2];              //Second buffer value replaced by third value
//
//}
//
//float filter(float x, float *w, float *b, float *a){
//
//  w[2] = x - (a[1] * w[1]) - (a[2] * w[0]);                     //Last value of bandstop_buf is changed to (input) - (2nd denominator * 2nd of bandstop_buf value) - (3rd denominator * 1st bandstop_buf value)
//  float y = (b[0] * w[2]) + (b[1] * w[1]) + (b[2] * w[0]);      //Output is equal to (first numerator * last bandstop_buf) + (second numerator * second bandstop_buf) + (third numerator * first bandstop_buf)
//
//  return y;
//
//}
//
//float bandstop(float input){
//
//  update_buffer(bandstop_buf);                                              //Update the first and second values of the buffer
//  float output = filter(input, bandstop_buf, bandstop_num, bandstop_den);   //Update the third value of the buffer and acquire final output
//
//  return output * bandstop_g;                                               //Return output multiplied by buffer gain
//
//}
//
//
//int main(void){
//
//    MAP_WDT_A_holdTimer();
//
//    P6DIR |= BIT0; //Set P6 as output
//
//    P5SEL0 |= BIT6 | BIT7; // Set pins P5.6 and P5.7 as external reference voltage
//    P5SEL1 |= BIT6 | BIT7; // P5.6 is V+ and P5.7 is V-
//
//    REFCTL0 |= REFON;     // Turn on reference module.
//    REFCTL0 |= REFOUT;    // Output reference voltage to a pin.
//    REFCTL0 &= ~(REFVSEL_3);              // Clear existing VREF voltage select level setting.
//    REFCTL0 |= REFVSEL_0;                 // Set VREF = 1.2V.
//    while (REFCTL0 & REFGENBUSY);       // Wait until the reference generation is settled.
//
//    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_PJ, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION); //  HFXT is connected to pins 2 and 3 of Port J.
//    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7); /* Set all 8 pins of P2 as output */
//    MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P4, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION); /* Check with a scope on this pin to make sure the clock is indeed 48 MHz */
//    CS_setExternalClockSourceFrequency(32000,48000000); //Tell Launchpad values of external LFXT and HFXT
//    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);  // Starting HFXT in non-bypass mode without a timeout.
//    MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);  //Change number of wait states used by flash controller for read operations.
//    MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);
//    CS_startHFXT(false);  // Initialise the HFXT.  "false" means we are not using the bypass mode but are using the crystal.
//
//    MAP_CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);  // The divider is set to 1 and so the MCLK is the same as the HFXT at 48 MHz
//
//    MAP_SysTick_enableModule();  //Enable the timer SysTick and start counting
//    MAP_SysTick_setPeriod(960);  // 960 MCLK clock cycles per interrupt. Therefore the sampling frequency is 48 MHz / 960 = 50 kHz
//    MAP_Interrupt_enableSleepOnIsrExit();  //Enables the processor to sleep when exiting an ISR. For low power operation, this is ideal as power cycles are not wasted with the processing required for waking up from an ISR and going back to sleep.
//    MAP_SysTick_enableInterrupt();  // Enable timer interrupt.
//
//    MAP_ADC14_enableModule();
//    MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_4, 0); /* Initializing ADC (MCLK/1//4) */
//    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION); /* Configuring GPIOs (P5.0 (A5) as the ADC input pin) */
//    //ADC14_setResolution(ADC_10BIT);
//    MAP_ADC14_configureSingleSampleMode(ADC_MEM0, true);    //Configure ADC Memory for single ADC memory location, repeat causes ADC to resume when inital sample is executed
//    MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A5, false); //Change A5 to A0???
//    MAP_ADC14_enableSampleTimer(ADC_MANUAL_ITERATION); //Configure sample timer
//    MAP_ADC14_enableConversion();           /* Enabling/Toggling Conversion */
//    MAP_ADC14_enableInterrupt(ADC_INT0);
//    MAP_Interrupt_enableInterrupt(INT_ADC14);
//
//    MAP_Interrupt_enableMaster();   /* Enabling processor MASTER interrupts */
//
//    //Enable floating-point unit
//    MAP_FPU_enableModule();
//    MAP_FPU_enableLazyStacking();
//    FPU_setFlushToZeroMode(FPU_FLUSH_TO_ZERO_EN);   //Values treated as zeros
//
//    int s = 0;
//    for (s=0; s<50; s++){
//
//        float x = (2.0f * PI * (s * 7.0f)) / 50.0f;
//        float k = sin(x);
//        sine[s] = k;            //50 sine[s] values varies between -1 and 1
//
//    }
//
//    while (1)
//    {
//        //MAP_PCM_gotoLPM0();
//    }
//}
//
//int N = 0;
//
//void systick_isr(void){
//
//                P6OUT |= BIT0; // set P6.0 high on entering this interrupt service routine
//
//    MAP_ADC14_toggleConversionTrigger();
//    currentADC = MAP_ADC14_getResult(ADC_MEM0);
//printf("%f\n",currentADC);
//    float input = currentADC - 8192.0f;
//
//    float input_stopband = bandstop(input); //Bandstop filter to remove 8khz
//
//    float input_stopband_sine = input_stopband * sine[N];
//
//    float filtered = input_stopband + 8192.0f;
//
//    float output = filtered / 64.0f;
//
//    P2OUT = output;
//
//    N++;
//        if(N > 49){  // 7 full sine waves, 50 samples
//            N = 0;
//    }
//
//                P6OUT &= ~BIT0; // set P6.0 low on exiting this interrupt service routine
//}
//
///* ADC Interrupt Handler. This handler is called whenever there is a conversion that is finished for ADC_MEM0. */
//
//void adc_isr(void){
//
//                uint64_t status = MAP_ADC14_getEnabledInterruptStatus();   //Return status of ADC interrupt register masked with enabled interrupts
//                MAP_ADC14_clearInterruptFlag(status);                      /* This simply clears the ADC14 interrupt and exits this adc_isr. */
//
//    if(status & ADC_INT0){
//        currentADC = MAP_ADC14_getResult(ADC_MEM0);
//    }
//
//}
//
//
//

/*
 * Real-time Audio Descrambler Scenario
 * Digital MSP432
 * Daniel Saul, 2015
 *
 * main.c
 *
 */

// #include "driverlib.h"
// #include <stdint.h>
// #include <stdbool.h>
// #include <math.h>
// #include <stdio.h>
// #include "filter.h"

// #define PI 3.14159265f

// //const bool DEBUG = true;

// static volatile float currentADC = 0;
// //static volatile float currentInput;

// static volatile float sine[50];


// int main(void)
// {

//     MAP_WDT_A_holdTimer(); // Turn off watchdog

//     P6DIR |= BIT0; // P6.0 set as output.

//     // VREF = 1.2V P5.6
//     P5SEL0 |= BIT6 | BIT7;
//     P5SEL1 |= BIT6 | BIT7;

//     REFCTL0 |= REFON;
//     REFCTL0 |= REFOUT;
//     REFCTL0 &= ~(REFVSEL_3);
//     REFCTL0 |= REFVSEL_0;
//     while (REFCTL0 & REFGENBUSY);

//     //P5.5 ADC A0
//     P5SEL1 |= BIT5;
//     P5SEL0 |= BIT5;

//     // Output pins
//     P2DIR = 0xFF;
//     P2OUT = 0x00;


//     // Configuring pins for high frequency crystal (HFXT) crystal for 48 MHz clock
//     MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_PJ, GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
//     MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P4, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION); // P4.3 -> 48MHz
//     CS_setExternalClockSourceFrequency(32000,48000000);
//     MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);
//     MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
//     MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);
//     CS_startHFXT(false);  // Initialise the HFXT.

//     // Initializing the master clock (MCLK) to HFXT (effectively 48MHz)
//     MAP_CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);

//     // Configure ADC14 - pin 5.5
//     MAP_ADC14_enableModule();
//     MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, 0);
//     MAP_ADC14_configureSingleSampleMode(ADC_MEM0, true);
//     MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A5, false);
//     MAP_ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);
//     MAP_ADC14_enableConversion();
//     MAP_ADC14_enableInterrupt(ADC_INT0);
//     MAP_Interrupt_enableInterrupt(INT_ADC14);

//     // Configure SysTick -> 50kHz
//     MAP_SysTick_enableModule();
//     MAP_SysTick_setPeriod(960); // 48 MHz / 960 = 50 kHz
//     MAP_Interrupt_enableSleepOnIsrExit();
//     MAP_SysTick_enableInterrupt();

//     // Enabling MASTER interrupts
//     MAP_Interrupt_enableMaster();

//     // Enable FPU
//     MAP_FPU_enableModule();
//     MAP_FPU_enableLazyStacking();
//     FPU_setFlushToZeroMode(FPU_FLUSH_TO_ZERO_EN);

//     // Generate 7kHz sine wave at 50kHz sampling rate
//     int N = 0;
//     for(N=0;N<50;N++){
//         float a = N*7.0f;
//         float b = a/50.0f;
//         float c = b*2.0f*PI;
//         //float t = N*2*PI*7/50;
//         float n = sin(c);
//         sine[N] = n;
//     }

//     while (1)
//     {
//       //MAP_PCM_gotoLPM0();
//     }

// }

// int M = 0;

// // 50kHz Systick
// void systick_isr(void)
// {
// 	P6OUT |= BIT0; // P6.0 high

//   MAP_ADC14_toggleConversionTrigger(); // ADC Conversion

//   // 0 magnitude value - (600/1200)*16384
//   float input = currentADC;
//   float input2 = input - 8192.0f;

//   // Bandstop filter input, remove 8kHz tone
//   float input_bandstop = bandstop(input2);

//   // Multiply by 7kHz sine wave
//   float input_bandstop_sine = (input_bandstop * sine[M]);

//   // Filter out upper sideband-----
//   float filtered = lowpass(input_bandstop_sine);

//   filtered = input_bandstop_sine + 8192.0f;
//   float output = (input/64.0f);

//   P2OUT = input/4;

//   M = M+1;
//   if(M > 49){  // 7 full sine waves, 50 samples
//     M = 0;
//   }

// 	P6OUT &= ~BIT0; // P6.0 low
// }


// // ADC Interrupt Handler - ADC_MEM0
// void adc_isr(void)
// {

// 	uint64_t status = MAP_ADC14_getEnabledInterruptStatus();
// 	MAP_ADC14_clearInterruptFlag(status);

//   if (status & ADC_INT0){
//     currentADC = MAP_ADC14_getResult(ADC_MEM0);
//     printf("%f\n",currentADC);
//     //currentInput = (currentADC * 1.2) / 16384;
//   }

// }

///*******************************************************************************
// * Template for Real-time Audio Descrambler, Second Year Scenario.
// *
// * This template is provided for your use in the Real-time Audio Descrambler Scenario.
// *
// * The template sets up all the important peripherals on the MSP432 for you.  This includes setting up the 48 MHz
// * high-frequency crystal (HFXT), the master clock MCLK to run at 48 MHz, the internal core voltage for 48 MHz
// * operation, the ADC reference voltages and outputting them to P5.6 and P5.7.
// * However, you need to set up the ADC yourself.
// *
// * Most importantly, the template sets up the timer SysTick to count 960 clock cycles.  Every time 960 clock cycles
// * or periods have passed, SysTick will generate an interrupt and the interrupt service routine systick_isr will be called.
// * Since the master clock is 48 MHz, systick_isr will be called 50,000 times a second (48 MHz / 960 = 50,000) or 50 kHz.
// * Any commands or functions inside systick_isr will therefore be executed at 50 kHz which can be used as
// * your sampling frequency.
// *
// * The descrambling and filtering functions should be implemented inside the systick_isr as these are
// * related to and run at the sampling frequency, i.e. 50 kHz.
// *
// * Anything else that only needs to be run once but not repeatedly should be implemented inside the function main{}.
// *
// * You need to make your codes as efficient as possible.  Any codes you put in systick_isr must be completed
// * before the next interrupt is generated.  To see how long it will take to perform all the functions inside systick_isr,
// * in this template, P6.0 is set to high at the start of systick_isr.  At the end of systick_isr, P6.0 is set to low.
// * You can therefore use an oscilloscope to measure the output at P6.0 to see how long it takes the systick_isr to
// * complete all the codes there.
// *
// * You should save the whole template folder in the workspace folder of the Code Composer Studio (CCS).  Then within CCS,
// * "import" this project.
// *
// *
// *
// * By Dr Chin-Pang Liu, University College London, 2015
// ******************************************************************************/
//
//
///* DriverLib Includes */
//#include "driverlib.h" // This library is required if you want to use the library functions such as "MAP_GPIO_setAsPeripheralModuleFunctionInputPin"
//
///* Standard Includes already here from the example*/
//#include <stdint.h>
//#include <stdbool.h>
//#include <stdio.h>
//
///* Maths Includes */
//#include <math.h>  // This library is required to use the "sin" function
//
///* Filter Includes */
//#include "filter.h"
//
///* Define variables */
//#define PTS 50 // no of points in one cycle of a sine wave
//#define PI 3.14159265f
//volatile float sin_value[PTS];
//
//
//int main(void)
//{
//    /* Halting the Watchdog */
//    MAP_WDT_A_holdTimer();  // This command is used in almost all programs to stop the MSP from stopping automatically.
//
//    /* P6.0 set as output.  This is used for timing the duration of the systick_isr.  P6.0 is set high on entering the systick_isr and
//     * off on exiting.  An oscilloscope can be used to monitor how much time the interrupt requires to complete all the operations.
//     * It can also measure the frequency at which the isr is called.  This should be 50 kHz*/
//    P6DIR |= BIT0;
//    //P1DIR |= BIT0;
//
//    /* Configure P5.6 to its analog function to output VREF.  VREF is set to 1.2V below and so the ADC input voltage should be between 0 V and 1.2 V */
//	P5SEL0 |= BIT6 | BIT7; // Set pins P5.6 and P5.7 as external reference voltage. See section 10.2.6 in slau356a.pdf, tables 4.1 and 6.45 of msp432p401r.pdf, slau596.pdf.
//	P5SEL1 |= BIT6 | BIT7; // Set pins P5.6 and P5.7 as external reference voltage. See section 10.2.6 in slau356a.pdf, tables 4.1 and 6.45 of msp432p401r.pdf, slau596.pdf.
//
//	REFCTL0 |= REFON;                     // Turn on reference module.  Section 19.3.1 in slau365a.pdf.
//	REFCTL0 |= REFOUT;                    // Output reference voltage to a pin.  Section 19.3.1 in slau365a.pdf.
//
//     /* Output VREF = 1.2V */
//	REFCTL0 &= ~(REFVSEL_3);              // Clear existing VREF voltage level setting. Table 19-2 in slau365a.pdf.
//	REFCTL0 |= REFVSEL_0;                 // Set VREF = 1.2V. Table 19-2 in slau365a.pdf.
//	while (REFCTL0 & REFGENBUSY);         // Wait until the reference generation is settled.  Table 19-2 in slau365a.pdf.
//
//     /* Generate 7kHz sine wave */
//	int t;
//	for( t = 0; t < PTS; t = t + 1)
//	{
//		float a = 7.0;
//		//sin_value[t] = 255*(sin(2*PI*(t/((float)PTS)))+1)/2; // original example code
//		sin_value[t] = 255*(sin(2*PI*a*(t/((float)PTS)))+1)/2;
//	}
//
//    /* Configuring pins for high frequency crystal (HFXT) crystal for 48 MHz clock */
//    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_PJ,GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION); //  Section 10.4.2.13 in MSP432_DriverLib_Users_Guide.  In Figure 4.1 in msp432p401r.pdf, you can see that the HFXT is conneccted to pins 2 and 3 of Port J.
//
//	/* Set P6.0 as output */
//	MAP_GPIO_setAsOutputPin(GPIO_PORT_P6, GPIO_PIN0);
//
//	/* Set all 8 pins of P2 as output */
//	MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2 | GPIO_PIN3 | GPIO_PIN4 | GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7);
//
//    /* Set P4.3 to output the internal 48 MHz master clock as (MCLK) its primary function so you can check with a scope on this pin to make sure the clock is indeed 48 MHz */
//	MAP_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P4, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);
//
//
//    /* There is a 32 kHz low frequency crystal (LFXT) and a 48 MHz high frequency crystal (HFXT) on the Launchpad. This command tells the MSP432 what frequencies these two external crystals are*/
//    /* Section 6.6.2.4 in MSP432_DriverLib_Users_Guide*/
//	CS_setExternalClockSourceFrequency(32000,48000000);
//
//
//    /* Starting HFXT in non-bypass mode without a timeout.
//     * Before increasing MCLK to a higher speed, it is necessary for software to ensure that the CPU voltage or core voltage (VCORE level) is
//     * sufficiently high for the chosen frequency.  This is done through the Power Control Manager (PCM).  See Chapter 7 in slau356a.pdf.
//     * To run the CPU at the maximum frequency of 48 MHz, the core voltage must be set to VCORE1.  See section 7.4.1 in slau356a.pdf.
//     * It is much easier to use the library commands below to set the core voltage to VCORE1.  */
//    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);  // Section 14.7.2.16 in MSP432_DriverLib_Users_Guide.
//    MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
//    MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);
//    CS_startHFXT(false);  // Initialise the HFXT.  "false" means we are not using the bypass mode but are using the crystal.  Section 6.6.2.27 in MSP432_DriverLib_Users_Guide.
//
//
//    /* Initializing the master clock (MCLK) to HFXT (effectively 48MHz) */
//    MAP_CS_initClockSignal(CS_MCLK, CS_HFXTCLK_SELECT, CS_CLOCK_DIVIDER_1);  // The divider is set to 1 and so the MCLK is the same as the HFXT at 48 MHz.  Section 6.6.2.18 in in MSP432_DriverLib_Users_Guide.
//
//
//    /* Configuring the timer SysTick to trigger at 50 kHz which will be the sampling frequency for sampling and processing the audio signal.
//     * See Chapter 22 in MSP432_DriverLib_Users_Guide.
//     * The other two timers "Timer32" and "Timer_A" can also be used instead */
//    MAP_SysTick_enableModule();  //Enable the timer SysTick
//    MAP_SysTick_setPeriod(960);  // SysTick will count 960 MCLK cycle before generating and calling an interrupt.  Therefore the sampling frequency is 48 MHz / 960 = 50 kHz
//    MAP_Interrupt_enableSleepOnIsrExit();  //Enables the processor to sleep when exiting an ISR. For low power operation, this is ideal as power cycles are not wasted with the processing required for waking up from an ISR and going back to sleep.
//    MAP_SysTick_enableInterrupt();  // Enable timer interrupt.
//
//    /* Initializing ADC (MCLK/1//1) */
//    MAP_ADC14_enableModule();
//    MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1, 0);
//
//    /* Configuring GPIOs (P5.0 (A5) as the ADC input pin) */
//    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);
//
//    /* Configure ADC Resolution */
//    ADC14_setResolution(ADC_10BIT);
//
//
//    /* Configuring ADC Memory */
//    MAP_ADC14_configureSingleSampleMode(ADC_MEM0, true);
//    MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_INTBUF_VREFNEG_VSS, ADC_INPUT_A5, false);
//    /* Configuring Sample Timer */
//	MAP_ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);
//
//     /* Enabling MASTER interrupts */
//    MAP_Interrupt_enableMaster();
//
//    while (1)
//    {
//        //MAP_PCM_gotoLPM0();
//    }
//
//}
//
//int j = 0;
//
//
///* The following systick_isr is called 50000 times a second, i.e. 50 kHz, controlled by the master clock and SysTick  */
//void systick_isr(void)
//{
//	//static int i = 0;
//	P6OUT |= BIT0; // set P6.0 high on entering this interrupt service routine (isr). Include your codes below
//
//	//MAP_ADC14_toggleConversionTrigger();
//
//	//i = i+1;
//	static long temp_ADC = 0;
//	float factor = 4.0;
//	temp_ADC = ADC14->MEM[0];  //Get the conversion result.  Alternatively, you can use temp_ADC = ADC14_getResult(ADC_MEM0)
//	float con_ADC = temp_ADC/factor;  // We do this because the ADC is set to use 10 bits but P2OUT is only 8 bits.
//
////	    if(i==2){
//	//    	float voltage = temp_ADC/1023 * 3.3;
////	    	printf("%ld\n",temp_ADC);
////	    	i=0;
////	    }
//
//	/* Enabling/Toggling Conversion */
//	MAP_ADC14_enableConversion();
//	MAP_ADC14_toggleConversionTrigger();
//	MAP_ADC14_toggleConversionTrigger();
//
//	/* Descrambling functions*/
//	float input_bandstop = bandstop(con_ADC);
//	float multiply = input_bandstop*sin_value[j];
//
//	/* DAC */
//	float output = multiply;
//	P2OUT = input_bandstop;  // This is where our output is written to the 8 output pins of port 2 (we hope!)
//
//	j = j+1;
//	if (j >= PTS)
//	{
//		j = 0; // reset j
//	}
//
//	P6OUT &= ~BIT0; // set P6.0 low on exiting this interrupt service routine (isr). Include yours codes above
//
//}
//
//
//
//
///* ADC Interrupt Handler. This handler is called whenever there is a conversion
//* that is finished for ADC_MEM0.
//*/
//void adc_isr(void)
//{
//
//	uint64_t status = MAP_ADC14_getEnabledInterruptStatus();
//	MAP_ADC14_clearInterruptFlag(status);
//	/* This simply clears the ADC14 interrupt and exits this adc_isr. */
//
//}
//
//







/*
 This example demonstrates using the internal analogue-to-digital converter (ADC)
 to digitise an input analogue signal and then reproducing it as an 8-bit digital
 output.  To obtain an actual analogue output signal from this 8-bit digital
 output, an external digital-to-analogue converter (DAC) must be built.  So what
 this example does is that if you supply an analogue signal such as sine wave to
 the ADC input of the MSP432 with proper DC biasing and AC coupling, then with a
 correctly constructed external DAC, the same sine wave will be produced by the
 external DAC.

The SysTick timer has been set up so that the sampling is carried out at 50 kHz.
All the peripheral pins have also been set up.  P5.0 (A5) is set up to be the
input ADC pin.  An ADC reference voltage of 1.2 V is set and is output via P5.6
and P5.7.  The 8-bit digital output is given by the 8 pins of P2.


 * Author: Dr Chin-Pang Liu, University College London, 2016
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
    
    // float input_bandstop2_sine = bandstop(input_bandstop_sine);
    
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

