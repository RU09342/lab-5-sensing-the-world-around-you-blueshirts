
//  Filename : msp430fr2311-adc10.c
//
//  Created on : November 28, 2017
//  Updated on : November 28, 2017
//  Author : Jack Pedicone
//

#include <msp430.h>

//Function Setup//

void TimerAInit(void);
void ADC10Init(void);
void UARTInit(void);

//Variable declaration//

unsigned volatile int	adc_in = 0;
volatile float			tempC = 0;
volatile float			tempF = 0;
volatile float			voltage = 0;

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;               // Stop WDT
	PM5CTL0 &= ~LOCKLPM5;					//Disable HIGH Z mode

	Temp_GPIOInit();
	TimerAInit();							//Timer Function call
	UARTInit();								//UART Function call


	while (REFCTL0 & REFGENBUSY);           // If ref generator busy, WAIT
	REFCTL0 |= REFVSEL_0 + REFON;           // Enable internal 1.2 reference

	ADC10Init();							// ADC10 Function call

	while (!(REFCTL0 & REFGENRDY));         // Wait for reference generator
	__enable_interrupt();					// Enable interrupts.

	while (1)
	{

		__bis_SR_register(LPM0 + GIE);		// Enter LPM0, interrupts enabled
		__no_operation();					// For debugger
	}
}

void UARTInit(void)
{

	P1SEL0 &= ~(BIT6 | BIT7);                   // Configure pin 1.6 to RXD
	P1SEL1 |= BIT6 + BIT7;                      // Configure pin 1.7 to TXD
												// Configure USCI_A0 for UART mode
	UCA0CTLW0 = UCSWRST;                        // Put eUSCI in reset
	UCA0CTLW0 |= UCSSEL__SMCLK;                 // CLK = SMCLK
	UCA0BRW = 52;                               // 8000000/16/9600
	UCA0MCTLW |= UCOS16 | UCBRF_1 | 0x4900;
	UCA0CTLW0 &= ~UCSWRST;                      // Initialize eUSCI
	UCA0IE |= UCRXIE;                           // Enable USCI_A0 RX interrupt
}
void ADC10Init(void)
{
    P1SEL1 |= ADC10;                         	// Configure P1.0 and P1.1 for ADC
	P1SEL0 |= ADC10;
	ADCCTL0 = ADC10SHT_2 | ADCON | ADCMSC;		// Set sample time, enable multiple conversations
	ADCCTL1 = ADCSHP | ADCCONSEQ_2;				// Enable sample timer and continuous sampling
	ADCCTL2 |= ADC10RES;						// 10-bit conversion results
	ADCMCTL0 = ADC10INCH_1;						// A1 <= ADC input
	ADCCTL0 |= ADCENC | ADCSC;					// Enable and start conversation

}

void TimerAInit(void) {
	TA0CCTL0 = CCIE;				//Disable timer Interrupt
	TA0CCTL1 = OUTMOD_3;			//Set/Reset when the timer counts to the TA0CCR1 value, reset for TA0CCR0
	TA0CCR1 = 256;
	TA0CCR0 = 4096 - 1;				//Set CCR0 for a ~1kHz clock.
	TA0CTL = TASSEL_1 | MC_1 | ID_3;
}

#pragma vector=TIMER_A0_VECTOR			//Timer A Interrupt Vector
__interrupt void TimerA(void)
{
	adc_in = ADCMEM0;					//Put ADCMEM value from LM35 to variable
	voltage = adc_in * 0.00029;         //converts ADC to voltage
	tempC = voltage / 0.01;             //converts voltage to Temp C
	tempF = ((9 * tempC) / 5) + 32;     //Temp C to Temp F
	while (!(UCA0IFG&UCTXIFG));
	UCA0TXBUF = tempF;					//Sends TempF over UART
}