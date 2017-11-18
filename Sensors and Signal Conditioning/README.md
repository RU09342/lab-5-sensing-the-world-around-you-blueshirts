# Lab 5 - Sensors and Signal Conditioning
#### Joshua Gould
##### Created - 11-12-2017
##### Updated - 11-18-2017

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
* [ ] Resistor, Current, and Voltage measurement
* [x] Circuit design and descriptions

## Compatibility
* MSP430G2553
* MSP430FR6989

## Functionality Description

ADC and being able to pull data from it. Your code need to communicate back to your computer using UART at 9600 Baud and send the ADC contents at 1 reading per second to start. This really is meant for you to see whether or not your signal conditioning is actually working and you can see changes in your sensor reading. This code should be very very similar to code you have written before and should be simple to port between your processors.

## Temperature Reading - LM35 - Voltage
After running ADC code for the G2553, the values transmitted from the LM35 across the UART Serial cable.
The values read are provided below. The LM35 handles each 10mV as one degree celcius and as such the value taken from this circuit must be divided by 0.01V. Other conversions to Fahrenheit were made and the snippet of code in the ADC interrupt is provided below as well.

## Phototransistor - Current
As for the phototransistor, with increased light, current increases and inversely so with less light. With the added resistor, the phototransistor's sensitivity can be controlled. (Higher resistance = more sensistivity)

The OP805SL was used for this circuit. The datasheet is provided in repository
Code for the phototransistor UART measurements are provided below

## Photoresistor - Resistance
In this photoresistance circuit, the photoresistor was used with ADC and voltage division.
The ADC code reads the resistance of the photoresistor and the resistor in the circuit. The constant reisistor was selected as a 1kΩ reistor at 3.3V. As for the photoresistor, the resistance increases when the light is lower and decreases as light increases.

Code for the photoresistor UART measurements are provided below
## Code
The requirements for this lab were modified to provide only a working ADC10 and ADC12 code for the lab. Each ADC is provided below.

#### ADC-10
##### Board Used: MSP430G2553
For the ADC-10, the process requires a GPIO, Timer, ADC, and UART setup.
##### GPIO
Output was set to P1.3 on the G2 and the UART pins were set.

##### Timer
Timer was set to allow a constant read from the ADC to read once per second

##### ADC
ADC settings (given by TI) allowed for the ADC to turn on and set registers

#### ADC-12
##### Board Used: MSP430FR6989

#### Code description


