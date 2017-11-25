# Lab 5 - Visualizing Data
#### Joshua Gould
##### Created - 11-12-2017
##### Updated - 11-25-2017

## Library Used
msp430.h - default library

## Dependencies
* CCS Software
* Matlab
* RGB LED
* MSP430FR6989 LCD Display 

## Set Requirements
"Focus on LCD display visualization. For this method, you need to pick 1 processor based on the part. As with Milestone 1, you will need to talk in this README about why you picked the processors you did.

#### LCD Display
Now that you are getting sensor data and acting on it, why don't you actually try to display the information the user in actual numbers. Using the MSP430FR6989, convert the information from all three of your sensors to a human readable value on the on-board LCD. Fair warning, *DO NOT TRY TO REINVENT THE WHEEL*. Make sure you give the resource explorer a good looking through to see what TI is going to provide you. You can utilize the provided LCDDriver.c and LCDDriver.h files in your code.

## Tasks
* [x] Code for ADC12
* [x] LCD Display
* [x] LCD Read thermistor
* [ ] Explain the board used

## Compatibility
* MSP430FR6989

## Functionality Description
For the MSP430FR6989 an ADC had to be created and have the ability to pull data from it to put on the LCD. The 6989 was chosen as it was the only board with a built in LCD display. UART transmission was not necessary for this part of the lab as the tempeture ADC reading is displayed on the board itself.

## Temperature Reading - LM35 - Voltage
A similar circuit from the LM35 circuit was used to read temperture from. The circuit is provided below.

### Circuit

![alt text](https://github.com/RU09342/lab-5-sensing-the-world-around-you-blueshirts/blob/master/Sensors%20and%20Signal%20Conditioning/Circuits/Phototransistor.PNG)

## Code

### ADC
Additionally, the ADC conversion is similar to code used before with initalization

```C
void initADC12(void) {

	ADC12CTL0 = ADC12SHT0_2 | ADC12ON;      // Sampling time, S&H=16, ADC12 on
	ADC12CTL1 = ADC12SHP;                   // Use sampling timer
	ADC12CTL2 |= ADC12RES_2;                // 12-bit conversion results
	ADC12MCTL0 |= ADC12INCH_3;              // A3 ADC input select; Vref=AVCC
	ADC12IER0 |= ADC12IE0;                  // Enable ADC conv complete interrupt
}
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)	//ADC Interrupt		
#elif defined(__GNUC__)
void __attribute__((interrupt(ADC12_VECTOR))) ADC12_ISR(void)
#else
#error Compiler not supported!
#endif
{
	switch (__even_in_range(ADC12IV, ADC12IV_ADC12RDYIFG))
	{
	case ADC12IV_ADC12IFG0:             // Vector 12:  ADC12MEM0 Interrupt
		adc_value = ADC12MEM0;
		voltage = ((float)adc_value / 1241.2);
		LCD_1 = voltage * 10;
		//LCD_1 = (9 * (voltage * 10) / 5) + 32; //Fareheit
		LCD_2 = ((int)(voltage * 100)) % 10;
		//LCD_2 = ((9*((int)(voltage * 100))/5)+32) % 10;
		char_1 = '0' + LCD_1;	//convert to char value
		char_2 = '0' + LCD_2;

		showChar(char_1, 1);        //tens digit
		showChar(char_2, 2);        //ones digit
		showChar(' ', 3);           //add space
		showChar('C', 4);           //Celcius or F if fahrenheit

		__bic_SR_register_on_exit(LPM0_bits);
		break;
}
```
### LCD
Calling upon the LCDdriver.h and initialization of the LCD crystal is what separates this code from the others. 

```C
void initLCD(void) {

	LCDCCTL0 &= ~LCDON;                                 // lcd off
	LCDCCTL0 = LCDDIV_2 + LCDPRE__4 + LCD4MUX + LCDLP;  // Flcd = 512 Hz; Fframe = 64 Hz
	LCDCMEMCTL = LCDCLRM;                               // clear memory
	LCDCPCTL0 = 0xFFFF;                                 // enable Segments
	LCDCPCTL1 = 0xFFFF;                                 // enable segments
	LCDCPCTL2 = 0x7FFF;                                 // enable segments
	LCDCCTL0 |= LCDON;                                  // lcd on
}
```

### Extra Work
* [ ] RGB LED
* [x] MATLAB Code
* [ ] Computer Plotting
