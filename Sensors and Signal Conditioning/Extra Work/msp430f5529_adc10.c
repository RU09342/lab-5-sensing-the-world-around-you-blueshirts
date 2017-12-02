
//  Filename : msp430f5529-adc12.c
//
//  Created on : November 28, 2017
//  Updated on : November 28, 2017
//  Author : Jack Pedicone
//

#include <msp430.h>

//Function Setup//

void TimerAInit(void);
void ADC12Init(void);
void UARTInit(void);

//Variable declaration//

unsigned volatile int	adc_in = 0;
volatile float			tempC = 0;
volatile float			tempF = 0;
volatile float			voltage = 0;

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;               // Stop WDT

	TimerAInit();							//Timer Function call
	UARTInit();								//UART Function call
	ADC12Init();							// ADC12 Function call

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
	P3SEL &= ~(BIT3 | BIT4);                    // Configure pin 3.4 to RXD
	P3SEL |= BIT3 + BIT4;                      	// Configure pin 3.3 to TXD
												// Configure USCI_A0 for UART mode											
	UCA0CTL1 = UCSWRST | UCSSEL_2;              // Put eUSCI in reset, SMCLK
	UCA0BR0 = 6;                               	// Baud Rate
	UCA0BR1 = 0;                               	// Baud Rate 
	UCA0MCTL |= UCOS16 | UCBRS_0 | UCBRF_13;	// Modulation
	UCA0CTL10 &= ~UCSWRST;                      // Initialize eUSCI
	UCA0IE |= UCRXIE;                           // Enable USCI_A0 RX interrupt
}
void ADC12Init(void)
{
    P6SEL |= BIT0;                         			// Configure P6.0 for ADC
	ADCCTL0 = ADC12SHT0_8 | ADC12ON | ADC12MSC;		// Turn on ADC12, set sample time, enable multiple conversations
	ADC12CTL1 = ADC12SHP | ADC12CONSEQ_2;			// Enable sample timer and continuous sampling
	ADCCTL0 |= ADCENC | ADCSC;						// Enable and start conversation
}

void TimerAInit(void) {
	TA0CCTL0 = CCIE;				// Enables timer Interrupt
	TA0CCTL1 = OUTMOD_3;			// Set/Reset when the timer counts to the TA0CCR1 value, reset for TA0CCR0
	TA0CCR1 = 256;
	TA0CCR0 = 4096 - 1;				//Set CCR0 for a ~1kHz clock.
	TA0CTL = TASSEL_1 | MC_1 | ID_3;
}

#pragma vector=TIMER_A0_VECTOR			//Timer A Interrupt Vector
__interrupt void TimerA(void)
{
	adc_in = ADC12MEM0;					//Put ADCMEM value from LM35 to variable
	voltage = adc_in * 0.00029;         //converts ADC to voltage
	tempC = voltage / 0.01;             //converts voltage to Temp C
	tempF = ((9 * tempC) / 5) + 32;     //Temp C to Temp F
	while (!(UCA0IFG&UCTXIFG));
	UCA0TXBUF = tempF;					//Sends TempF over UART
}