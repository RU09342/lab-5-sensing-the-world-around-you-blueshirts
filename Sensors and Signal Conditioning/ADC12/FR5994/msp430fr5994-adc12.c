//******************************************************************************
//											
//                 MSP430FR5994							
//              ---------------								 
//         /|\  |          VCC|- 3.3V				
//          |   |             |						
//          --  |GND      P1.5|--< adc temp		
//              |			  |
//              |			  |			
//              |             |			
//				|		  P1.0|--> LED
//              -------------------
//  adc_value (P1.5) to LM35
//	
//
//  Filename : msp430fr5994-adc12.c
//
//  Created on : November 25, 2017
//  Updated on : November 25, 2017
//  Author : Joshua Gould
//
//******************************************************************************
//
//Reference/Mad Help from Jessica Wozniak & Russell Trafford
//
//******************************************************************************

#include <msp430.h>

//Function Setup//

void TimerAInit(void);
void ADC12Init(void);
void UARTInit(void);
void Temp_GPIOInit(void);

//Variable declaration//

unsigned volatile int	adc_in = 0;
volatile float			tempC = 0;
volatile float			tempF = 0;
volatile float			voltage = 0;

void Temp_GPIOInit()
{
	P1OUT &= ~BIT0;                         // Clear LED to start
	P1DIR |= BIT0;                          // P1.0 output
	P1SEL1 |= ADC12;                         // Configure P1.5 for ADC
	P1SEL0 |= ADC12;
}

int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;               // Stop WDT
	PM5CTL0 &= ~LOCKLPM5;					//Disable HIGH Z mode

	Temp_GPIOInit();
	TimerAInit();							//Timer Function call
	UARTInit();								//UART Function call


	while (REFCTL0 & REFGENBUSY);           // If ref generator busy, WAIT
	REFCTL0 |= REFVSEL_0 + REFON;           // Enable internal 1.2 reference

	ADC12Init();							//ADC12 Function call

	while (!(REFCTL0 & REFGENRDY));         // Wait for reference generator
	__enable_interrupt();					//Enable interrupts.

	while (1)
	{

		__bis_SR_register(LPM0 + GIE);		// Enter LPM0, interrupts enabled
		__no_operation();					// For debugger
	}
}

void UARTInit(void)
{

	CSCTL0_H = CSKEY_H;                         // Unlock CS registers
	CSCTL1 = DCOFSEL_3 | DCORSEL;               // Set DCO to 8MHz
	CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
	CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;       // Set all dividers
	CSCTL0_H = 0;                               // Lock CS registers

	P2SEL0 &= ~(BIT0 | BIT1);                   //Configure pin 2.0 to RXD
	P2SEL1 |= BIT0 + BIT1;                        //Configure pin 2.1 to TXD

												  // Configure USCI_A0 for UART mode
	UCA0CTLW0 = UCSWRST;                        // Put eUSCI in reset
	UCA0CTLW0 |= UCSSEL__SMCLK;                 // CLK = SMCLK
	UCA0BRW = 52;                               // 8000000/16/9600
	UCA0MCTLW |= UCOS16 | UCBRF_1 | 0x4900;
	UCA0CTLW0 &= ~UCSWRST;                      // Initialize eUSCI
	UCA0IE |= UCRXIE;                           // Enable USCI_A0 RX interrupt
}
void ADC12Init(void)
{
	ADC12CTL0 = ADC12SHT0_2 + ADC12ON;			// Set sample time
	ADC12CTL1 = ADC12SHP;						// Enable sample timer
	ADC12CTL2 |= ADC12RES_2;					// 12-bit conversion results
	ADC12MCTL0 = ADC12INCH_5 | ADC12VRSEL_1;	// Vref+ = , Input
	ADC12IER0 |= ADC12IE0;						// Enable ADC conv complete interrupt

}

void TimerAInit(void) {
	TA0CCTL0 = CCIE;				//Disable timer Interrupt
	TA0CCTL1 = OUTMOD_3;			//Set/Reset when the timer counts to the TA0CCR1 value, reset for TA0CCR0
	TA0CCR1 = 256;
	TA0CCR0 = 4096 - 1;				//Set CCR0 for a ~1kHz clock.
	TA0CTL = TASSEL_1 + MC_1 + ID_3;
}
#pragma vector=TIMER0_A0_VECTOR			//Timer for ADC Interrupt
__interrupt void TIMER0_A0_ISR(void)
{
	ADC12CTL0 |= ADC12SC | ADC12ENC;
}

#pragma vector=ADC12_B_VECTOR			//ADC Interrupt Vector
__interrupt void ADC12ISR(void)
{
	adc_in = ADC12MEM0;					//Put ADC12MEM value from LM35 to variable
	voltage = adc_in * 0.00029;         //converts ADC to voltage
	tempC = voltage / 0.01;             //converts voltage to Temp C
	tempF = ((9 * tempC) / 5) + 32;     //Temp C to Temp F
	while (!(UCA0IFG&UCTXIFG));
	P1OUT ^= BIT0;						//toggles LED when sending
	UCA0TXBUF = tempF;					//Sends TempF over UART
}
