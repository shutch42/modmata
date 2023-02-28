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
	uint16_t count;
	uint16_t* value;
};

struct spi_settings {
	int speed;
	bool order;
	uint8_t mode;
};

// General Arduino functions
struct registers pinMode(uint16_t argc, uint16_t *argv);
struct registers digitalWrite(uint16_t argc, uint16_t *argv);
struct registers digitalRead(uint16_t argc, uint16_t *argv);
struct registers analogWrite(uint16_t argc, uint16_t *argv);
struct registers analogRead(uint16_t argc, uint16_t *argv);

// Servo functions
struct registers servoAttach(uint16_t argc, uint16_t *argv);
struct registers servoDetach(uint16_t argc, uint16_t *argv);
struct registers servoWrite(uint16_t argc, uint16_t *argv);
struct registers servoRead(uint16_t argc, uint16_t *argv);

// I2C functions
struct registers wireBegin(uint16_t argc, uint16_t *argv);
struct registers wireEnd(uint16_t argc, uint16_t *argv);
struct registers wireSetClock(uint16_t argc, uint16_t *argv);
struct registers wireWrite(uint16_t argc, uint16_t *argv);
struct registers wireRead(uint16_t argc, uint16_t *argv);

// SPI functions
struct registers spiBegin(uint16_t argc, uint16_t *argv);
struct registers spiSettings(uint16_t argc, uint16_t *argv);
struct registers spiTransferBuf(uint16_t argc, uint16_t *argv);
struct registers spiEnd(uint16_t argc, uint16_t *argv);

