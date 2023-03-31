#include <stdlib.h>
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

struct registers {
	uint8_t count;
	uint8_t* value;
};

struct spi_settings {
	uint32_t speed;
	bool order;
	uint8_t mode;
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

