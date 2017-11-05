 * --/COPYRIGHT--*/
//******************************************************************************
//  MSP430G2x33/G2x53 Demo - ADC10, Sample A1, AVcc Ref, Set P1.0 if > 0.5*AVcc
//
//  Description: A single sample is made on A1 with reference to AVcc.
//  Software sets ADC10SC to start sample and conversion - ADC10SC
//  automatically cleared at EOC. ADC10 internal oscillator times sample (16x)
//  and conversion. In Mainloop MSP430 waits in LPM0 to save power until ADC10
//  conversion complete, ADC10_ISR will force exit from LPM0 in Mainloop on
//  reti. If A1 > 0.5*AVcc, P1.0 set, else reset.
//
//                MSP430G2x33/G2x53
//             -----------------
//         /|\|              XIN|-
//          | |                 |
//          --|RST          XOUT|-
//            |                 |
//        >---|P1.1/A1      P1.0|-->LED
//
//  D. Dang
//  Texas Instruments Inc.
//  December 2010
//   Built with CCS Version 4.2.0 and IAR Embedded Workbench Version: 5.10
//******************************************************************************

/*
 * P1.1 -> RXD P1.2 -> TXD
 * ADC input to P1.3
 * MSP430G2553
 *
 * */

 #include <msp430.h>
 unsigned int ADC_value = 0;

 void adc_str(void);

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  //constant erase adjustment
  if (CALBC1_1MHZ==0xFF)
  {
      while(1);
  }
  //select lowest DC0x and MODx settings

  // UART Stuff
  DCOCTL = 0;                               // Select lowest DCOx and MODx settings
  BCSCTL1 = CALBC1_1MHZ;                    // Set DCO
  DCOCTL = CALDCO_1MHZ;
  P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
  P1SEL2 = BIT1 + BIT2 ;                    // P1.1 = RXD, P1.2=TXD
  UCA0CTL1 |= UCSSEL_2;                     // SMCLK
  UCA0BR0 = 104;                            // 1MHz 9600
  UCA0BR1 = 0;                              // 1MHz 9600
  UCA0MCTL = UCBRS0;                        // Modulation UCBRSx = 1
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
  //UART End

  //ADC Pins and interrupts
  BCSCTL2 &= ~(DIVS_3);     //DC0 = 1MHz
  P1SEL |= BIT3;            //ADC input to P1.3
  adc_str();                //ADC function setup
  __enable_interrupt();     //enable int
  //ADC End

  while(1)
  {
      __delay_cycles(1000);
      ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
      __bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
      ADC_value = ADC10MEM;
  }

  /*
   * Removed Code
  ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
  ADC10CTL1 = INCH_1;                       // input A1
  ADC10AE0 |= 0x02;                         // PA.1 ADC option select
  P1DIR |= 0x01;                            // Set P1.0 to output direction

  for (;;)
  {


    if (ADC10MEM < 0x1FF)
      P1OUT &= ~0x01;                       // Clear P1.0 LED off
    else
      P1OUT |= 0x01;                        // Set P1.0 LED on
  }
  */
}

// ADC10 interrupt service routine
 //ADC10 interrupt taken from Matt Mammarelli
 #pragma vector=ADC10_VECTOR
 __interrupt void ADC10_ISR (void)
 {
     // Return to active mode
     __bic_SR_register_on_exit(CPUOFF);
 }

 //ADC initialize set-up
 void adc_str(void)
 {
     ADC10CTL1 = INCH_3 + ADC10DIV_3 ;
     ADC10CTL0 = SREF_0 + ADC10SHT_3 + ADC10ON + ADC10IE;
     // ADC input enable P1.3
     ADC10AE0 |= BIT3;
 }

 //  Echo back RXed character, confirm TX buffer is ready first
 #if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
 #pragma vector=USCIAB0RX_VECTOR
 __interrupt void USCI0RX_ISR(void)
 #elif defined(__GNUC__)
 void __attribute__ ((interrupt(USCIAB0RX_VECTOR))) USCI0RX_ISR (void)
 #else
 #error Compiler not supported!
 #endif
 {
   while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?

   //set input start value S = 83
   if (UCA0TXBUF){
       UCA0TXBUF = ADC10MEM; //adds ADC value and skips clockcycle apparently
   }
   else{
       UCA0TXBUF = 0;       //else code to print stop perhaps
   }

 }
