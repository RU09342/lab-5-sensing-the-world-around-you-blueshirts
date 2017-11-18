//******************************************************************************
//
//				 MSP430FR6989
//			      -------------------
//			   /|\|              VCC|- 3.3V
//			    | |                 |
//			    --|GND		|
//			      |			|
//		              |                	|
//		 adc_value <--|P1.3/A1      P1.0|--> LED
//			      |			|
//			      |			|
//			      |   LCD DISPLAY	|
//			      -------------------
//
//	Description: adc_value to LM35, displays temperture on LCD Display
//
//	Filename : msp430fr6989-LCD.c
//  
//	Created on : October 22, 2017
//	Updated on : November 15, 2017
//	Author : Joshua Gould
//
//******************************************************************************
/* --COPYRIGHT--,BSD_EX
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 *
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//	Configured P9.1 for ADC
//

//LIBRARY INIT & DEFINITIONS//////////////////////////////////////////////////

#include <msp430.h>
#include "LCDDriver.h"

void initADC12(void);
void initLCD(void);
void initGPIO(void);

float tempC = 0;
float tempF = 0;
float voltage = 0;
unsigned int adc_value;
int LCD_1, LCD_2;
char char_1, char_2

//LIBRARY INIT & DEFINITIONS////////////////////////////////////////////////////

//MAIN FUNCTION/////////////////////////////////////////////////////////////////

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;               // Stop WDT
	
	initGPIO();
	initADC12();
	initLCD();

	// Disable the GPIO power-on default high-impedance mode to activate
	// previously configured port settings
	PM5CTL0 &= ~LOCKLPM5;

	while (1)
	{
		__delay_cycles(1000);
		ADC12CTL0 |= ADC12ENC | ADC12SC;    // Start sampling/conversion

		__bis_SR_register(LPM0_bits | GIE); // LPM0, ADC12_ISR will force exit
		__no_operation();                   // For debugger
	}

}

//MAIN FUNCTION/////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////
	/*						*/
	/*		INITIALIZATIONS			*/
	/*						*/
	//////////////////////////////////////////////////	


//GPIO INITiALIZATION///////////////////////////////////////////////////////////

void initGPIO() {
	
	P1OUT &= ~BIT0;                         // Clear LED to start
	P1DIR |= BIT0;                          // Set P1.0/LED to output
	
	P1DIR |= BIT3;							// Configure P1.3 for ADC
	P1SEL1 |= BIT3;
	P1SEL0 |= BIT3;

	//All out reference from TI
	P1DIR = 0xFF;
	P1REN = 0xFF;
	P1OUT = 0x00;

	P2DIR = 0xFF;
	P2REN = 0xFF;
	P2OUT = 0x00;

}
//GPIO INITiALIZATION///////////////////////////////////////////////////////////

//ADC INITIALIZATION////////////////////////////////////////////////////////////
void initADC12() {
	// Configure ADC12
	ADC12CTL0 = ADC12SHT0_2 | ADC12ON;      // Sampling time, S&H=16, ADC12 on
	ADC12CTL1 = ADC12SHP;                   // Use sampling timer
	ADC12CTL2 |= ADC12RES_2;                // 12-bit conversion results
	ADC12MCTL0 |= ADC12INCH_3;              // A3 ADC input select; Vref=AVCC
	ADC12IER0 |= ADC12IE0;                  // Enable ADC conv complete interrupt


}
//ADC INITIALIZATION/////////////////////////////////////////////////////////////

//LCD INITIALIZE///////////////////////////////////////////////////////////

void LCDInit()
{
	LCDCTL0 &= ~LCDON;					//lcd off
	LCDCCTL0 = LCDDIV__2 + LCDPRE__4 + LCD4MUX + LCDLP;	//Flcd = 512 Hz. Fframe = 64Hz
	LCDCCPCTL = LCDCPCLKSYNC;				// Clock synchronization enabled
	LCDCMEMCTL = LCDCLRM;					// Clear LCD memory
	LCDCPCTL0 = 0xFFFF;
	LCDCPCTL1 = 0xFFFF;
	LCDCPCTL2 = 0x7FFF;
	LCDCCTL0 |= LCDON;                                	// lcd on

	// Disable the GPIO power-on default high-impedance mode
	// to activate previously configured port settings
	// You have to do this again the interrupt
	PM5CTL0 &= ~LOCKLPM5;

}

//LCD INITIALIZE///////////////////////////////////////////////////////////

	//////////////////////////////////////////////////
	/*						*/
	/*		INTERRUPTS			*/
	/*						*/
	//////////////////////////////////////////////////	

//ADC INTERRUPT////////////////////////////////////////////////////////////

##if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
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
		adc_value = ADC12MEM0;
		voltage = ((float)adc_value/1241.2)
		LCD_1 = voltage*10;
		LCD_2 = ((int)(voltage * 100)) % 10;
		char_1 = '0' + LCD_1;
		char_2 = '0' + LCD_2;

		showChar(char_1, 1);		//tens digit
		showChar(char_2, 2);		//ones digit
		showChar(' ', 3);			//add space
		showChar('C', 4);			//Celcius

		__bic_SR_register_on_exit(LPM0_bits);
		break;	
	case ADC12IV_ADC12IFG1:   break;    // Vector 14:  ADC12MEM1
	case ADC12IV_ADC12IFG2:	  break;    // Vector 16:  ADC12MEM2
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
	default:break;
	}
}
//ADC INTERRUPT////////////////////////////////////////////////////////////
