# Modmata
This library is based on the [modbus-arduino library](https://github.com/andresarmento/modbus-arduino), and is formatted to work in a way that is similar to the [firmata arduino library](https://github.com/firmata/arduino).  
  
The goal is to be able to write applications to communicate between a computer and arduino as you would with firmata, 
but using modbus communication instead of MIDI/Sysex messages.  
  
This has the advantage of better error checking, faster communication, and more readable code.  

The library is specificially designed for use on an [Arduino Leonardo](https://docs.arduino.cc/hardware/leonardo), or any Arduino IDE compatible microcontroller that uses an Atmega 32u4 chip. Specifically, this library was made to simplify the programming on the [Lattepanda 3 Delta](https://www.lattepanda.com/lattepanda-3-delta), but it can be used on any computer with an Arduino Leonardo plugged in via USB.

### Functionality  
By default, the library has functions to work with digital and analog I/O, servos, I2C, and SPI. These functions are listed in [Functions.h](https://github.com/shutch42/modmata/blob/main/Functions.h).  
  
If you wish to add a function that is not supported by default, you can do so using the attach() function. *FIXME: Add an example*  

### Usage  
To use the library, clone this repo to your Arduino IDE libraries folder. Once it is saved there, open up the Example program, [StandardModmata.ino](https://github.com/shutch42/modmata/blob/main/examples/StandardModmata/StandardModmata.ino). 
This simple sketch is all that is needed to use Modmata on your Arduino Leonardo. Upload the sketch, and from there, you can program your arduino to do whatever you wish from our [ModmataC library](https://github.com/shutch42/ModmataC).
