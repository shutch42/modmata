/*
Modmata Functions
Copyright © 2023 char* teamName <shutche@siue.edu>
Licensed under LGPL-2.1
*/

/**
 * @file Functions.cpp
 * @author Sam Hutcherson, Chase Wallendorff, Iris Astrid
 * @brief Define standard callback functions to control Arduino I/O
 * @date 2023-04-06
 */

#include "Functions.h"

/** @brief Current number of attached servos */
int servo_count = 0;

/** @brief Array to control attached servos */
Servo servos[14];	// FIXME: Change to pointers so that pins 9/10 can be used as analog when Servos aren't used

/** @brief Singleton to represent the current SPI connection settings */
struct spi_settings settings;

/** 
 * @brief Helper struct that is returned directly when a callback function has no return values,
 * and that is copy-constructed from when a clean struct is needed to populate with values
 */
const registers VOID_STRUCT{0, nullptr};

/**
 * @brief Change the settings of the Arduino I/O pins
 * 
 * @param argc The number of arguments contained within the 'argv' array (2)
 * @param argv The arguments to use within the function	(pin #, mode)
 * @return void (empty struct)
 */
struct registers pinMode(uint8_t argc, uint8_t *argv) {
	if (argc == 2) {
		pinMode(argv[0], argv[1]);
	}

	return VOID_STRUCT;
}

/**
 * @brief Write a digital (HIGH/LOW) value to the Arduino I/O pins
 * 
 * @param argc The number of arguments contained within the 'argv' array (2)
 * @param argv The arguments to use within the function (pin #, value)
 * @return void (empty struct)
 */
struct registers digitalWrite(uint8_t argc, uint8_t *argv) {
	if (argc == 2) {
		digitalWrite(argv[0], argv[1]);
	}

	return VOID_STRUCT;
}

/**
 * @brief Read a digital value (HIGH/LOW) from the Arduino I/O pins
 * 
 * @param argc The number of arguments contained within the 'argv' array (1)
 * @param argv The arguments to use within the function (pin #)
 * @return struct containing the value of pin (1 uint8_t)
 */
struct registers digitalRead(uint8_t argc, uint8_t *argv) {
	struct registers result{VOID_STRUCT};

	if (argc == 1) {
		uint8_t read_val = digitalRead(argv[0]);
		result.count = 1;
		result.value = (uint8_t *)malloc(sizeof(uint8_t));
		result.value[0] = read_val;
	}

	return result;
}

/**
 * @brief Write an analog value (0-255) to the Arduino I/O pins
 * 
 * @param argc The number of arguments contained within the 'argv' array (3)
 * @param argv The arguments to use within the function (pin #, 16-bit value split into 2 8-bit values)
 * @return void (empty struct)
 */
struct registers analogWrite(uint8_t argc, uint8_t *argv) {
	if (argc == 3) {
		analogWrite(argv[0], makeWord(argv[1], argv[2]));
	}

	return VOID_STRUCT;
}

/**
 * @brief Read an analog value (0-1023) from the Arduino I/O pins
 * 
 * @param argc The number of arguments contained within the 'argv' array (1)
 * @param argv The arguments to use within the function (pin #)
 * @return struct containing the value of pin (10-bit unsigned int contained within a uint16_t)
 */
struct registers analogRead(uint8_t argc, uint8_t *argv) {
	struct registers result{VOID_STRUCT};

	if (argc == 1) {
		uint16_t read_val = analogRead(argv[0]);
		result.count = 2;
		result.value = (uint8_t *)malloc(sizeof(uint8_t) * 2);
		result.value[0] = highByte(read_val);
		result.value[1] = lowByte(read_val);
	}

	return result;
}

/**
 * @brief Attach a connected servo to a control interface
 * 
 * @param argc The number of arguments contained within the 'argv' array (1)
 * @param argv The arguments to use within the function (pin #)
 * @return struct containing the ServoIndex (uint8_t), I have no idea what it actually means though (undocumented on Arduino???)
 */
struct registers servoAttach(uint8_t argc, uint8_t *argv) {
	struct registers result{VOID_STRUCT};

	if (argc == 1) {
		int pin = argv[0];
		result.count = 1;
		result.value = (uint8_t *)malloc(sizeof(uint8_t));
		result.value[0] = 0;
		if (pin >= 0 && pin <= 13 && servo_count < MAX_SERVOS) {
			result.value[0] = servos[pin].attach(pin);
		}
	}

	return result;
}

/**
 * @brief Detach a servo from the the control interface
 * 
 * @param argc The number of arguments contained within the 'argv' array (1)
 * @param argv The arguments to use within the function (pin #)
 * @return struct containing the boolean value of the operation's success (uint8_t)
 */
struct registers servoDetach(uint8_t argc, uint8_t *argv) {
	struct registers result{VOID_STRUCT};
	if (argc == 1) {
		int pin = argv[0];
		result.count = 1;
		result.value = (uint8_t *)malloc(sizeof(uint8_t));
		result.value[0] = 0;
		if (pin >= 0 && pin <= 13) {
			servos[pin].detach();
			result.value[0] = 1;
		}
	}

	return result;
}

/**
 * @brief Write a value to the connected servo
 * 
 * @param argc The number of arguments contained within the 'argv' array (2)
 * @param argv The arguments to use within the function (pin #, value)
 * @return struct containing the boolean value of the operation's success (uint8_t)
 */
struct registers servoWrite(uint8_t argc, uint8_t *argv) {
	struct registers result{VOID_STRUCT};
	if(argc == 2) {
		int pin = argv[0];
		int angle = argv[1];
		result.count = 1;
		result.value = (uint8_t *)malloc(sizeof(uint8_t));
		result.value[0] = 0;
		if (pin >= 0 && pin <= 13) {
			servos[pin].write(angle);
			result.value[0] = 1;
		}
	}

	return result;
}

/**
 * @brief Read a value from the connected servo
 * 
 * @param argc The number of arguments contained within the 'argv' array
 * @param argv The arguments to use within the function
 * @return struct containing the last value written to the servo (uint8_t)
 */
struct registers servoRead(uint8_t argc, uint8_t *argv) {
	struct registers result{VOID_STRUCT};
	if (argc == 1) {
		int pin = argv[0];
		if (pin >= 0 && pin <= 13) {
			result.count = 1;
			result.value = (uint8_t *)malloc(sizeof(uint8_t));
			result.value[0] = servos[pin].read();
		}
	}
	
	return result;
}

/**
 * @brief Begin an I2C connection between the Arduino and a peripheral
 * 
 * @param argc The number of arguments contained within the 'argv' array
 * @param argv The arguments to use within the function
 * @return void (empty struct)
 */
struct registers wireBegin(uint8_t argc, uint8_t *argv) {
	Wire.begin();
	return VOID_STRUCT;
}

/**
 * @brief End an I2C connection between the Arduino and a peripheral
 * 
 * @param argc The number of arguments contained within the 'argv' array
 * @param argv The arguments to use within the function
 * @return void (empty struct)
 */
struct registers wireEnd(uint8_t argc, uint8_t *argv) {
	Wire.end();
	return VOID_STRUCT;
}

/**
 * @brief Change the clock speed settings of the I2C connection
 * 
 * @param argc The number of arguments contained within the 'argv' array
 * @param argv The arguments to use within the function
 * @return void (empty struct)
 */
struct registers wireSetClock(uint8_t argc, uint8_t *argv) {
	if (argc == 4) {
		// 4 8-bit values combine to make a 32-bit int
		uint32_t clockspeed = makeDWord(argv[0], argv[1], argv[2], argv[3]);
		Wire.setClock(clockspeed);
	}

	return VOID_STRUCT;
}

/**
 * @brief Write data to the connected I2C peripheral
 * 
 * @param argc The number of arguments contained within the 'argv' array
 * @param argv The arguments to use within the function
 * @return void (empty struct)
 */
struct registers wireWrite(uint8_t argc, uint8_t *argv) {
	if (argc >= 2) {
		// Argument format: addr | reg | bytes
		uint8_t addr = argv[0];
		uint8_t reg = argv[1];

		Wire.beginTransmission(addr);
		Wire.write(reg);
		for(int i = 2; i < argc; i++) {
			Wire.write(argv[i]);
		}
		Wire.endTransmission();
	}
	
	return VOID_STRUCT;
}

/**
 * @brief Read data from the connected I2C peripheral
 * 
 * @param argc The number of arguments contained within the 'argv' array
 * @param argv The arguments to use within the function
 * @return struct containing the bytes read (num_bytes * uint8_t)
 */
struct registers wireRead(uint8_t argc, uint8_t *argv) {
	struct registers result{VOID_STRUCT};
	
	if(argc == 3) {
		// Argument format: addr | reg | num_bytes
		uint8_t addr = argv[0];
		uint8_t reg = argv[1];
		uint8_t num_bytes = argv[2];
		result.value = (uint8_t *)calloc(num_bytes, sizeof(uint8_t));
		
		Wire.beginTransmission(addr);
		Wire.write(reg);
		Wire.endTransmission();

		Wire.requestFrom(addr, num_bytes);
		if(Wire.available() == num_bytes) {
			result.count = num_bytes;
			for(int i = 0; i < num_bytes; i++) {
				result.value[i] = Wire.read();
			}
		}
	}

	return result;	
}

/**
 * @brief Begin a SPI connection between the Arduino and a peripheral
 * 
 * @param argc The number of arguments contained within the 'argv' array
 * @param argv The arguments to use within the function
 * @return void (empty struct)
 */
struct registers spiBegin(uint8_t argc, uint8_t *argv) {
	if (argc == 0) {
		SPI.begin();
		settings.speed = 4000000;
		settings.order = MSBFIRST;
		settings.mode = SPI_MODE0;
	}

	return VOID_STRUCT;
}

/**
 * @brief Change the settings of the SPI connection
 * 
 * @param argc The number of arguments contained within the 'argv' array
 * @param argv The arguments to use within the function
 * @return void (empty struct)
 */
struct registers spiSettings(uint8_t argc, uint8_t *argv) {
	if (argc == 6) {
		uint32_t clockspeed = makeDWord(argv[0], argv[1], argv[2], argv[3]);
		settings.speed = clockspeed;
		settings.order = argv[4];
		settings.mode = argv[5];
	}

	return VOID_STRUCT;
}

/**
 * @brief Exchange data over the SPI connection (Read + Write)
 * 
 * @param argc The number of arguments contained within the 'argv' array
 * @param argv The arguments to use within the function
 * @return struct containing the response to the command written (argc - 1 * uint8_t)
 */
struct registers spiTransferBuf(uint8_t argc, uint8_t *argv) {
	// Register format:
	// | CMD/ARGC | Bytes                 |
	// |<-1 word->|<-31 words / 62 bytes->|

	struct registers result{VOID_STRUCT};
	
	if (argc > 1) {
		uint8_t CS_pin = argv[0];

		result.count = argc - 1;
		result.value = (uint8_t *)malloc(sizeof(uint8_t) * result.count);
		
		SPI.beginTransaction(SPISettings(settings.speed, settings.order, settings.mode));
		digitalWrite((uint8_t)CS_pin, (uint8_t)LOW);
		for(int i = 1; i < argc; i++) {
			result.value[i-1] = SPI.transfer(argv[i]);
		}
		digitalWrite((uint8_t)CS_pin, (uint8_t)HIGH);
		SPI.endTransaction();
	}

	return result;
}

/**
 * @brief End a SPI connection between the Arduino and a peripheral
 * 
 * @param argc The number of arguments contained within the 'argv' array (0)
 * @param argv The arguments to use within the function (None)
 * @return void (empty struct)
 */
struct registers spiEnd(uint8_t argc, uint8_t *argv) {
	if (argc == 0) {
		SPI.end();
	}

	return VOID_STRUCT;
}

