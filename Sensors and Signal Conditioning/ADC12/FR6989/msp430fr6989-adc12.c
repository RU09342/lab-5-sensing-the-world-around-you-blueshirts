//******************************************************************************
//											
//                 MSP430FR6989							
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
//  Filename : msp430fr6989-adc12.c
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

void TimerAInit(void);
void ADC12Init(void);
void UARTInit(void);
void Temp_GPIOInit(void);

//Variable declaration//

unsigned volatile int	adc_in = 0;
volatile float			tempC = 0;
volatile float			tempF = 0;
volatile float			voltage = 0;
volatile int			adc_time = 0;


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;                // Stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;					// High-Z

	//adc-12 interrupt
	ADC12CTL0 = ADC12SHT0_2 + ADC12ON;			// Set sample time
	ADC12CTL1 = ADC12SHP;						// Enable sample timer
	ADC12CTL2 |= ADC12RES_2;					// 12-bit conversion results
	ADC12MCTL0 = ADC12INCH_5 | ADC12VRSEL_1;	// Vref+ = , Input
	ADC12IER0 |= ADC12IE0;						// Enable ADC conv complete interrupt

	//Initialize timer
	TA0CCTL0 |= CCIE;// Timer B, capture control 0 interrupt enable
	TA0CCR0 = 62500;//Timer B, capture control 0 set to 50000 cycles
	TA0CTL |= TASSEL__SMCLK | MC__UP | (BIT6 + BIT7);//select SMCLK, up mode


	// Startup clock system with DCO setting ~1MHz
	CSCTL0_H = CSKEY_H;                     // Unlock CS registers
	CSCTL1 = DCOFSEL_0 | DCORSEL;           // Set DCO to 8MHz
	CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
	CSCTL3 = DIVA__1 | DIVM__1;				// Set all dividers
	CSCTL0_H = 0;                           // Lock CS registers
											
	//GPIO setup
	Temp_GPIOInit();

	// Configure UART
	UCA1CTLW0 |= UCSWRST;
	UCA1CTLW0 |= UCSSEL__SMCLK;
											 // Baud Rate calculation
											 // 1000000/(16*9600) = 104.1667
											 // Fractional portion = 0.1667
											 // User's Guide Table 17-4: UCBRSx = 0x20
											 // UCBRFx = int ( (52.083-52)*16) = 1
	UCA1BR0 = 6;                             // 1000000/16/9600
	UCA1MCTLW = 0x2081;

	UCA1CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
											  // UCA1IE |= UCRXIE;                         
											  // Enable USCI_A0 RX interrupt

	__bis_SR_register(LPM3_bits | GIE);         // Enter LPM3, interrupts enabled
	__no_operation();                         // For debugger
}

#pragma vector = TIMER0_A0_VECTOR			//toggle LED Timer Interrupt (Beta)
__interrupt void turn_on_ADC(void)
{
	switch (adc_time)
	{
	case 0: adc_time++;
		P1OUT ^= BIT0;		//toggles led and pushes to next case
		break;
	case 1: adc_time = 0;
		ADC12CTL0 |= ADC12SC | ADC12ENC;		//enables that ADC conversation
		break;
	}
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

void Temp_GPIOInit()		//P1.2=>ADC to LM35 
{
	P1SEL0 |= BIT2;		//P1.2 as ADC
	P1SEL1 |= BIT2;
	P1DIR |= BIT0;
}
