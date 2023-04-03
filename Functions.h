#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include <SPI.h>

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
typedef struct registers {
	uint8_t 	count;
	uint8_t * 	value;
};


/**
 * @brief A data structure to describe a SPI connection's configuration settings.
 * Essentially the same as Arduino's SPIsettings object
 * @param speed The baud rate/clock speed of the connection.
 * @param order The order of bits sent over the wire 
 * @param mode The clock polarity and phase
 */
struct spi_settings {
	uint32_t 	speed;
	bool 		order;
	uint8_t 	mode;
} settings;


// General Arduino functions

/**
 * @brief Distinguish Modmata functions from standard Arduino functions with a namespace
 * 
 */
namespace functions {

	// Pin I/O functions

	registers pinMode(uint8_t argc, uint8_t *argv);
	registers digitalWrite(uint8_t argc, uint8_t *argv);
	registers digitalRead(uint8_t argc, uint8_t *argv);
	registers analogWrite(uint8_t argc, uint8_t *argv);
	registers analogRead(uint8_t argc, uint8_t *argv);

	// Servo functions

	registers servoAttach(uint8_t argc, uint8_t *argv);
	registers servoDetach(uint8_t argc, uint8_t *argv);
	registers servoWrite(uint8_t argc, uint8_t *argv);
	registers servoRead(uint8_t argc, uint8_t *argv);

	// Wire/I2C functions

	registers wireBegin(uint8_t argc, uint8_t *argv);
	registers wireEnd(uint8_t argc, uint8_t *argv);
	registers wireSetClock(uint8_t argc, uint8_t *argv);
	registers wireWrite(uint8_t argc, uint8_t *argv);
	registers wireRead(uint8_t argc, uint8_t *argv);

	// SPI functions

	registers spiBegin(uint8_t argc, uint8_t *argv);
	registers spiSettings(uint8_t argc, uint8_t *argv);
	registers spiTransferBuf(uint8_t argc, uint8_t *argv);
	registers spiEnd(uint8_t argc, uint8_t *argv);

};