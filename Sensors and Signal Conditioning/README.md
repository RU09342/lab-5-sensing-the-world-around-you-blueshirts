# Lab 5 - Sensors and Signal Conditioning
#### Joshua Gould
##### Created - 11-12-2017
##### Updated - 11-14-2017

## Library Used
msp430.h - default library

## Dependencies
* BOARD.sch
* BOARD.pcb
* GERBERS (Folder)
* Diptrace Program

## Set Requirements
##### Sensors
"Sensors in a nutshell convert a physical phenomena into an electrical phenomena. For example, and accelerometer converts an acceleration into a change in capacitance (in the case of some MEMS devices). This electrical phenomena does not have to be a voltage, and as such, we need to be able to convert our sensor output to a voltage. From there, we can read in this voltage through our Analog to Digital Converter (ADC)."

##### Signal Conditioning
"The signal conditioning chain can be broken up into a few main building blocks. In the beginning you have your sensor which converts something into the electrical world. We then need a block which can convert that resultant electrical phenomena into a voltage. From here, this voltage may be too small/large for our microcontroller, so we need to amplify the signal. From there, we may need to de-noise the signal by filtering. After all of this, we might be at a point where we are ready to read the signal into our processor."

For this part of the lab, you need to focus on two main aspects: utilizing and reading the ADC, and conditioning a sensor to provide you with a decent output. To do this, you will need to build the proper circuitry to take a measurement from sensors which convert a physical phenomena into:
* Voltage
* Current
* Resistance

#### Tasks
* [x] Code for ADC10
* [x] Code for ADC12
* [x] Circuit design and descriptions

## Compatibility
* MSP430G2553
* MSP430FR6989

# Functionality Description

ADC and being able to pull data from it. Your code need to communicate back to your computer using UART at 9600 Baud and send the ADC contents at 1 reading per second to start. This really is meant for you to see whether or not your signal conditioning is actually working and you can see changes in your sensor reading. This code should be very very similar to code you have written before and should be simple to port between your processors.

### Code

#### Code description


