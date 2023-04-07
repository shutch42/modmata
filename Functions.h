/*
Modmata Functions
Copyright © 2023 char* teamName <shutche@siue.edu>
Licensed under LGPL-2.1
*/

/**
 * @file Functions.h
 * @author Sam Hutcherson, Chase Wallendorff, Iris Astrid
 * @brief Header file for 'Functions.cpp'
 * @date 2023-04-06
 */

#include <stdlib.h>
#include <stdint.h>
#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include <SPI.h>

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

/**
 * @brief Combine four 8-bit integral types into one 32-bit integral type,
 * or in simpler terms, reassemble a uint32_t from four uint8_t's
 */
#define makeDWord(i0, i1, i2, i3) ((uint32_t) (i0 << 24 | i1 << 16 | i2 << 8 | i3))

// Command number associated with each function

#define IDLE 0
#define PINMODE 1
#define DIGITALWRITE 2
#define DIGITALREAD 3
#define ANALOGREFERENCE 4
#define ANALOGWRITE 5
#define ANALOGREAD 6
#define SERVOATTACH 7
#define SERVODETACH 8
#define SERVOWRITE 9
#define SERVOREAD 10
#define WIREBEGIN 11
#define WIREEND 12
#define WIRECLOCK 13
#define WIREWRITE 14
#define WIREREAD 15
#define SPIBEGIN 16
#define SPISETTINGS 17
#define SPITRANSFER 18
#define SPIEND 19

/**
 * @brief A data structure to describe function arguments and return values.
 * @param count The number of arguments contained within the array 'value'.
 * @param value A pointer to an array of bytes (8-bit integral types) that 
 * contains the values of the arguments or return values for a command.
 */
struct registers {
	/** The number of arguments contained within the array 'value'. */
	uint8_t 	count;
	
	/** A pointer to an array of bytes (8-bit integral types) that contains the values of the arguments or return values for a command. */
	uint8_t * 	value;
};

/**
 * @brief A data structure to describe a SPI connection's configuration settings.
 * Essentially the same as Arduino's 'SPIsettings' object
 * @param speed The baud rate/clock speed of the connection.
 * @param order The order of bits sent over the wire 
 * @param mode The clock polarity and phase
 */
struct spi_settings {
	/** The baud rate/clock speed of the connection. */
	uint32_t 	speed;

	/** The order of bits sent over the wire. */
	bool 		order;
	
	/** The clock polarity and phase. */
	uint8_t 	mode;
};


// General Arduino functions

struct registers pinMode(uint8_t argc, uint8_t *argv);
struct registers digitalWrite(uint8_t argc, uint8_t *argv);
struct registers digitalRead(uint8_t argc, uint8_t *argv);
struct registers analogWrite(uint8_t argc, uint8_t *argv);
struct registers analogRead(uint8_t argc, uint8_t *argv);


// Servo functions

struct registers servoAttach(uint8_t argc, uint8_t *argv);
struct registers servoDetach(uint8_t argc, uint8_t *argv);
struct registers servoWrite(uint8_t argc, uint8_t *argv);
struct registers servoRead(uint8_t argc, uint8_t *argv);


// I2C functions

struct registers wireBegin(uint8_t argc, uint8_t *argv);
struct registers wireEnd(uint8_t argc, uint8_t *argv);
struct registers wireSetClock(uint8_t argc, uint8_t *argv);
struct registers wireWrite(uint8_t argc, uint8_t *argv);
struct registers wireRead(uint8_t argc, uint8_t *argv);


// SPI functions

struct registers spiBegin(uint8_t argc, uint8_t *argv);
struct registers spiSettings(uint8_t argc, uint8_t *argv);
struct registers spiTransferBuf(uint8_t argc, uint8_t *argv);
struct registers spiEnd(uint8_t argc, uint8_t *argv);

#endif
