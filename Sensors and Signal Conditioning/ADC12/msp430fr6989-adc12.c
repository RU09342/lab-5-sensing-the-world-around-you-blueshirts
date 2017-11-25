#include <msp430.h>

void Init_GPIO();

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;                // Stop watchdog timer


	PM5CTL0 &= ~LOCKLPM5;

	ADC12CTL0 = ADC12SHT0_2 | ADC12ON;      // Sampling time, S&H=16, ADC12 on
	ADC12CTL1 = ADC12SSEL_3 | ADC12SHP; ;                   // Use sampling timer
	ADC12CTL2 |= ADC12RES_3;                // 12-bit conversion results
	ADC12MCTL0 |= ADC12INCH_5;              // A1 ADC input select; Vref=AVCC
	ADC12IER0 |= ADC12IE0;                  // Enable ADC conv complete interrupt


											//Initialize timer
	TA0CCTL0 |= CCIE;// Timer B, capture control 0 interrupt enable
	TA0CCR0 = 62500;//Timer B, capture control 0 set to 50000 cycles
	TA0CTL |= TASSEL__SMCLK | MC__UP | (BIT6 + BIT7);//select SMCLK, up mode


													 // Startup clock system with DCO setting ~1MHz
	CSCTL0_H = CSKEY_H;                     // Unlock CS registers
	CSCTL1 = DCOFSEL_0 | DCORSEL;           // Set DCO to 8MHz
	CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
	CSCTL3 = DIVA__1 | DIVM__1;   // Set all dividers
	CSCTL0_H = 0;                           // Lock CS registers

	Init_GPIO();
	P1OUT = BIT0;
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
											  //UCA1IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

	__bis_SR_register(LPM3_bits | GIE);         // Enter LPM3, interrupts enabled
	__no_operation();                         // For debugger
}
int timing = 0;
#pragma vector = TIMER0_A0_VECTOR
__interrupt void turn_on_ADC(void)
{
	switch (timing)
	{
	case 0: timing++;
		//UCA1TXBUF = 0xff;
		P1OUT ^= BIT0;
		break;
	case 1: timing = 0;
		ADC12CTL0 |= 0x03;
		break;
	}
}
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
#elif defined(__GNUC__)
void __attribute__((interrupt(ADC12_VECTOR))) ADC12_ISR(void)
#else
#error Compiler not supported!
#endif
{
	switch (__even_in_range(ADC12IV, ADC12IV_ADC12RDYIFG))
	{
	case ADC12IV_NONE:        break;    // Vector  0:  No interrupt
	case ADC12IV_ADC12OVIFG:  break;    // Vector  2:  ADC12MEMx Overflow
	case ADC12IV_ADC12TOVIFG: break;    // Vector  4:  Conversion time overflow
	case ADC12IV_ADC12HIIFG:  break;    // Vector  6:  ADC12BHI
	case ADC12IV_ADC12LOIFG:  break;    // Vector  8:  ADC12BLO
	case ADC12IV_ADC12INIFG:  break;    // Vector 10:  ADC12BIN
	case ADC12IV_ADC12IFG0:             // Vector 12:  ADC12MEM0 Interrupt
		while (!(UCA0IFG&UCTXIFG));       //Use this line in ADC interrupt
		UCA1TXBUF = ADC12MEM0;
		//ADC12IFG=0x00;
		break;
	case ADC12IV_ADC12IFG1:   break;    // Vector 14:  ADC12MEM1
	case ADC12IV_ADC12IFG2:   break;    // Vector 16:  ADC12MEM2
	case ADC12IV_ADC12IFG3:   break;    // Vector 18:  ADC12MEM3
	case ADC12IV_ADC12IFG4:   break;    // Vector 20:  ADC12MEM4
	case ADC12IV_ADC12IFG5:   break;    // Vector 22:  ADC12MEM5
	case ADC12IV_ADC12IFG6:   break;    // Vector 24:  ADC12MEM6
	case ADC12IV_ADC12IFG7:   break;    // Vector 26:  ADC12MEM7
	case ADC12IV_ADC12IFG8:   break;    // Vector 28:  ADC12MEM8
	case ADC12IV_ADC12IFG9:   break;    // Vector 30:  ADC12MEM9
	case ADC12IV_ADC12IFG10:  break;    // Vector 32:  ADC12MEM10
	case ADC12IV_ADC12IFG11:  break;    // Vector 34:  ADC12MEM11
	case ADC12IV_ADC12IFG12:  break;    // Vector 36:  ADC12MEM12
	case ADC12IV_ADC12IFG13:  break;    // Vector 38:  ADC12MEM13
	case ADC12IV_ADC12IFG14:  break;    // Vector 40:  ADC12MEM14
	case ADC12IV_ADC12IFG15:  break;    // Vector 42:  ADC12MEM15
	case ADC12IV_ADC12IFG16:  break;    // Vector 44:  ADC12MEM16
	case ADC12IV_ADC12IFG17:  break;    // Vector 46:  ADC12MEM17
	case ADC12IV_ADC12IFG18:  break;    // Vector 48:  ADC12MEM18
	case ADC12IV_ADC12IFG19:  break;    // Vector 50:  ADC12MEM19
	case ADC12IV_ADC12IFG20:  break;    // Vector 52:  ADC12MEM20
	case ADC12IV_ADC12IFG21:  break;    // Vector 54:  ADC12MEM21
	case ADC12IV_ADC12IFG22:  break;    // Vector 56:  ADC12MEM22
	case ADC12IV_ADC12IFG23:  break;    // Vector 58:  ADC12MEM23
	case ADC12IV_ADC12IFG24:  break;    // Vector 60:  ADC12MEM24
	case ADC12IV_ADC12IFG25:  break;    // Vector 62:  ADC12MEM25
	case ADC12IV_ADC12IFG26:  break;    // Vector 64:  ADC12MEM26
	case ADC12IV_ADC12IFG27:  break;    // Vector 66:  ADC12MEM27
	case ADC12IV_ADC12IFG28:  break;    // Vector 68:  ADC12MEM28
	case ADC12IV_ADC12IFG29:  break;    // Vector 70:  ADC12MEM29
	case ADC12IV_ADC12IFG30:  break;    // Vector 72:  ADC12MEM30
	case ADC12IV_ADC12IFG31:  break;    // Vector 74:  ADC12MEM31
	case ADC12IV_ADC12RDYIFG: break;    // Vector 76:  ADC12RDY
	default: break;
	}
}
void Init_GPIO() //P1.5=>A5     P3.4 txd     a5=>
{
	P3SEL0 |= BIT4;
	P8SEL0 |= BIT6;
	P8SEL1 |= BIT6;
	P1DIR |= BIT0;
}
