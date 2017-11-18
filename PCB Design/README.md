# Lab 5 - PCB Design
#### Joshua Gould & John Pedicone 
##### Created - 11-12-2017
##### Updated - 11-18-2017

## Dependencies
* BOARD.sch
* BOARD.pcb
* GERBERS (Folder)
* Diptrace Program

## Set Requirements
"You will be required to design a simple breakout board for the MSP430FR2311. This is one of the simplest processors you have to design for, requiring you to only place a few capacitors to get it to work. You might think wait, but isn't there all that stuff up at the top of the board? Don't we need to also include that? All of those parts are part of the programmer/debugger and it is very easy to program your chip using just a few wires."

#### Tasks
* [x] BOARD.sch
* [x] BOARD.pcb
* [x] GERBER folder

## Compatibility
* MSP430FR2311

# Functionality Description
Your board should be a 2 layer board which can conveniently fit on a breadboard and contains the required passives for this processor to work in a standalone implementation. This means that the board should contain bypass capacitors, and the proper reset circuitry as laid out in the MSP430FR2311 datasheet. As per PCBWAY manufacturing restrictions, you should not have any traces smaller than 6mil, and your board should be smaller than 4 inches per side (which is maybe 4 times larger than your board should be.)

### PCB & Circuit
![Alt Text](https://github.com/RU09342/lab-5-sensing-the-world-around-you-blueshirts/blob/master/PCB%20Design/images/PCBPNG.PNG)
![Alt Text](https://github.com/RU09342/lab-5-sensing-the-world-around-you-blueshirts/blob/master/PCB%20Design/images/CircuitPNG.PNG)

### Bill of Materials
|Part     |Manufacturer Part #|Brand|Value|Sizes|Package      |Unit Price ($)|Website|
|------------|-------------------|-----|-------------------|-------------------|-------------|--------------|----|
|MSP430FR2311|MSP430FR2311IPW16R |TI   | N/A | 0.173" W| Tape and Reel | 0.63 | [Link](https://www.digikey.com/product-detail/en/texas-instruments/MSP430FR2311IPW16R/MSP430FR2311IPW16R-ND/6098078)|
|Resistor |RC0805JR-0747KL    |Yageo| 47k |0.079" L x 0.049" W|Cut Tape     |0.00245       |[Link](https://www.digikey.com/product-detail/en/yageo/RC0805JR-0747KL/311-47KARCT-ND/731283)|
|Capacitor|CC0805KKX5R6BB106  |Yageo|10uF |0.079" L x 0.049" W|Tape and Reel|0.19          |[Link](https://www.digikey.com/product-detail/en/yageo/CC0805KKX5R6BB106/311-1460-2-ND/2833625)|      
|Capacitor|C0805C102K5RACTU   |KEMET|1nF  |0.079" L x 0.049" W|Cut Tape     |0.00784       |[Link](https://www.digikey.com/product-detail/en/kemet/C0805C102K5RACTU/399-1147-1-ND/411422)|
|Capacitor|C0805C104K5RACTU   |KEMET|100nF|0.079" L x 0.049" W|Cut Tape     |0.01012       |[Link](https://www.digikey.com/product-detail/en/kemet/C0805C104K5RACTU/399-1170-1-ND/411445)|
