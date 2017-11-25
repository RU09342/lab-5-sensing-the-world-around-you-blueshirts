# Lab 5 - Sensors and Signal Conditioning
#### Joshua Gould
##### Created - 11-12-2017
##### Updated - 11-25-2017

## Library Used
msp430.h - default library

## Dependencies
* Breadboard
* MSP430G2553 and FR6989
* Photoresistor
* Photodiode
* Phototransistor
* Multimeter and oscilloscope

## Set Requirements
### Sensors
"Sensors in a nutshell convert a physical phenomena into an electrical phenomena. For example, and accelerometer converts an acceleration into a change in capacitance (in the case of some MEMS devices). This electrical phenomena does not have to be a voltage, and as such, we need to be able to convert our sensor output to a voltage. From there, we can read in this voltage through our Analog to Digital Converter (ADC)."

### Signal Conditioning
"The signal conditioning chain can be broken up into a few main building blocks. In the beginning you have your sensor which converts something into the electrical world. We then need a block which can convert that resultant electrical phenomena into a voltage. From here, this voltage may be too small/large for our microcontroller, so we need to amplify the signal. From there, we may need to de-noise the signal by filtering. After all of this, we might be at a point where we are ready to read the signal into our processor."

For this part of the lab, you need to focus on two main aspects: utilizing and reading the ADC, and conditioning a sensor to provide you with a decent output. To do this, you will need to build the proper circuitry to take a measurement from sensors which convert a physical phenomena into:
* Voltage
* Current
* Resistance

#### Tasks
* [x] Code for ADC10
* [x] Code for ADC12
* [x] Resistor, Current, and Voltage measurement
* [x] Circuit design and descriptions

## Compatibility
* MSP430G2553
* MSP430FR6989
* MSP430FR5994

## Functionality Description

Create an ADC and being able to pull data from it. This code needs to communicate back to the computer using UART at 9600 Baud and send the ADC contents at 1 reading per second to start. Each type of reading for voltage, current, and resistance is provided below.

## Temperature Reading - LM35 - Voltage
After running ADC code for the G2553, the values transmitted from the LM35 across the UART Serial cable.
The values read are provided below. The LM35 handles each 10mV as one degree celcius and as such the value taken from this circuit must be divided by 0.01V. Other conversions to Fahrenheit were made and the snippet of code in the ADC interrupt is provided below as well.

```C
  adc_value = ADC10MEM;
  voltage = adc_value* 0.0033;         //Converts ADC to voltage. (Vref/2^10) = 0.0033 * ADC = voltage .00029 for ADC 12
  tempC = voltage / 0.01;               //For LM35 each degree C is 10mv (0.01V)
  tempF = ((9 * (tempC)) / 5) + 32;             //converts degrees C to degrees F
```
### Circuit

![alt text](https://github.com/RU09342/lab-5-sensing-the-world-around-you-blueshirts/blob/master/Sensors%20and%20Signal%20Conditioning/Circuits/Phototransistor.PNG)

## Phototransistor - Current
As for the phototransistor, with increased light, current increases and inversely so with less light. With the added resistor, the phototransistor's sensitivity can be controlled. (Higher resistance = more sensistivity)

The OP805SL was used for this circuit. The datasheet is provided in repository
Code for the phototransistor UART measurements are provided below

```C
  adc_value = ADC10MEM;
  voltage = adc_value * 0.0033;          //Converts ADC to voltage. (Vref/2^10) = 0.0033 * ADC = voltage
  current = voltage / 1000;               //Ohms law. Constant resisitor is 1kΩ
```
### Circuit

![alt text](https://github.com/RU09342/lab-5-sensing-the-world-around-you-blueshirts/blob/master/Sensors%20and%20Signal%20Conditioning/Circuits/Photodiode_circuit.PNG)

## Photoresistor - Resistance
In this photoresistance circuit, the photoresistor was used with ADC and voltage division.
The ADC code reads the resistance of the photoresistor and the resistor in the circuit. The constant reisistor was selected as a 1kΩ reistor at 3.3V. As for the photoresistor, the resistance increases when the light is lower and decreases as light increases.

Code for the photoresistor UART measurements are provided below

```C
  adc_value = ADC10MEM;
  voltage = adc_value * 0.0033;          //Converts ADC to voltage. (Vref/2^10) = 0.0033 * ADC = voltage
  resistance = (3300.0/voltage) - 1000;          //use voltage division equation
```
### Circuit

![alt text](https://github.com/RU09342/lab-5-sensing-the-world-around-you-blueshirts/blob/master/Sensors%20and%20Signal%20Conditioning/Circuits/Photoresistor_circuit.PNG)

## Code
The requirements for this lab were modified to provide only a working ADC10 and ADC12 code for the lab. Each ADC code explaination is provided below.

#### ADC-10
##### Board Used: MSP430G2553
For the ADC-10, the process requires a to set the GPIO, Timer, ADC, and UART setup.

##### Timer
Timer was set to allow a constant read from the ADC to read once per second

```C
  TACCTL0 = CCIE;                           // Enable interrupt
  TACCR0 = 4096 - 1;                        // PWM Period
  TACCTL1 = OUTMOD_3;                       // TACCR1 set/reset
  TACCR1 = 256;                             // TACCR1 PWM Duty Cycle
  TACTL = TASSEL_1 + MC_1 + ID_3;           // ACLK, UP MODE, DIV 4
```

##### ADC
ADC settings (given by TI) allowed for the ADC to turn on and set registers

```C
  ADC10CTL1 = INCH_7 + SHS_1;            // P1.7, TA1 trigger sample start
  ADC10AE0 = BIT7;                       // ADC10 on P1.7
  P1DIR |= BIT0;                         // set LED1 to output
```
##### UART
Example code was given in LAB-1 
```C
  DCOCTL = 0;                             // Select lowest DCOx and MODx settings
  BCSCTL1 = CALBC1_1MHZ;                  // Set DCO
  DCOCTL = CALDCO_1MHZ;
  P1SEL = BIT1 + BIT2;                    // P1.1 = RXD, P1.2=TXD
  P1SEL2 = BIT1 + BIT2;                   // P1.1 = RXD, P1.2=TXD
  UCA0CTL1 |= UCSSEL_2;                   // SMCLK
  UCA0BR0 = 104;                          // 1MHz 9600
  UCA0BR1 = 0;                            // 1MHz 9600
  UCA0MCTL = UCBRS0;                      // Modulation UCBRSx = 1
  UCA0CTL1 &= ~UCSWRST;                   // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                        // Enable USCI_A0 RX interrupt
```

Complete code provided in msp430g2553-ADC10.c

#### ADC-12
##### Board Used: MSP430FR5994 & MSP430FR6989
For the ADC-12, the process requires a similar set-up of the GPIO, Timer, ADC, and UART setup.

##### Timer
Timer was set to allow a constant read from the ADC to read once per second

```C
  TACCTL0 = CCIE;                           // Enable interrupt
  TACCR0 = 4096 - 1;                        // PWM Period
  TACCTL1 = OUTMOD_3;                       // TACCR1 set/reset
  TACCR1 = 256;                             // TACCR1 PWM Duty Cycle
  TACTL = TASSEL_1 + MC_1 + ID_3;           // ACLK, UP MODE, DIV 4
```
The Timer interrupt starts the ADC conversation
```C
  #pragma vector=TIMER0_A0_VECTOR			//Timer for ADC Interrupt
  __interrupt void TIMER0_A0_ISR(void)
  {
    ADC12CTL0 |= ADC12SC | ADC12ENC;		//start conversation
  }
```
##### ADC
ADC settings (given by TI) allowed for the ADC to turn on and set registers

```C
  ADC12CTL0 = ADC12SHT0_2 + ADC12ON;			// Set sample time
  ADC12CTL1 = ADC12SHP;						// Enable sample timer
  ADC12CTL2 |= ADC12RES_2;					// 12-bit conversion results
  ADC12MCTL0 = ADC12INCH_5 | ADC12VRSEL_1;	// Vref+ = , Input
  ADC12IER0 |= ADC12IE0;						// Enable ADC conv complete interrupt
```
The ADC interrupt recieves the ADC value from the LM35 (or other device) to manipulate it into a readable value over uart
```C
  adc_in = ADC12MEM0;					//Put ADC12MEM value from LM35 to variable
  voltage = adc_in * 0.00029;           //converts ADC to voltage
  tempC = voltage / 0.01;               //converts voltage to Temp C
  tempF = ((9 * tempC) / 5) + 32;       //Temp C to Temp F
  while (!(UCA0IFG&UCTXIFG));
  P1OUT ^= BIT0;		 	//toggles LED when sending
  UCA0TXBUF = tempF;			//Sends TempF over UART
```
##### UART
Example code was given in LAB-1 
```C
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
```


