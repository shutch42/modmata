#include "Functions.h"
#include <Arduino.h>
#include <Servo.h>


int servo_count = 0;
Servo servos[14];	// FIXME: Change to pointers so that pins 9/10 can be used as analog when Servos aren't used

spi_settings settings;

// Prefix Arduino Library functions so as to specify _exactly_
// which function is being called in the namespace, as if the Modmata
// definition being in a namespace wasn't enough already... *eyerolls*

void (&apinMode)(uint8_t, uint8_t) = 		pinMode; 
void (&adigitalWrite)(uint8_t, uint8_t) = 	digitalWrite;
int  (&adigitalRead)(uint8_t) = 			digitalRead;
void (&aanalogWrite)(uint8_t, int) = 		analogWrite;
int  (&aanalogRead)(uint8_t) = 				analogRead;
void (&aanalogReference)(uint8_t) = 		analogReference;


/**
 * @brief Change the settings of the Arduino I/O pins
 * 
 * @param argc The number of arguments contained within the 'argv' array (2)
 * @param argv The arguments to use within the function	(pin #, mode)
 * @return result: a 'registers' struct holding the return values of the called function (empty, in this case)
 */
registers functions::pinMode(uint8_t argc, uint8_t *argv) {
	if (argc == 2) {
		apinMode(argv[0], argv[1]);
	}

	registers result;
	result.count = 0;
	return result;
}


/**
 * @brief Write a digital (HIGH/LOW) value to the Arduino I/O pins
 * 
 * @param argc The number of arguments contained within the 'argv' array (2)
 * @param argv The arguments to use within the function (pin #, value)
 * @return result: a 'registers' struct holding the return values of the called function (empty, in this case)
 */
registers functions::digitalWrite(uint8_t argc, uint8_t *argv) {
	if (argc == 2) {
		adigitalWrite(argv[0], argv[1]);
	}

	registers result;
	result.count = 0;
	return result;
}


/**
 * @brief Read a digital value (HIGH/LOW) from the Arduino I/O pins
 * 
 * @param argc The number of arguments contained within the 'argv' array (1)
 * @param argv The arguments to use within the function (pin #)
 * @return result: a 'registers' struct holding the return values of the called function (value of pin)
 */
registers functions::digitalRead(uint8_t argc, uint8_t *argv) {
	registers result;

	if (argc == 1) {
		uint8_t read_val = adigitalRead(argv[0]);
		result.count = 1;
		result.value = malloc(sizeof(uint8_t));
		result.value[0] = read_val;
	}
	else {
		result.count = 0;
	}

	return result;
}


/**
 * @brief Write an analog value (0-255) to the Arduino I/O pins
 * 
 * @param argc The number of arguments contained within the 'argv' array (3)
 * @param argv The arguments to use within the function (pin #, 16-bit value split into 2 8-bit values)
 * @return result: a 'registers' struct holding the return values of the called function (empty, in this case)
 */
registers functions::functions::analogWrite(uint8_t argc, uint8_t *argv) {
	if (argc == 3) {
		aanalogWrite(argv[0], argv[1] << 8 | argv[2]);
	}

	registers result;
	result.count = 0;
	return result;
}


/**
 * @brief Read an analog value (0-255) from the Arduino I/O pins
 * 
 * @param argc The number of arguments contained within the 'argv' array (1)
 * @param argv The arguments to use within the function (pin #)
 * @return result: a 'registers' struct holding the return values of the called function (16-bit pin value split into 2 8-bit values)
 */
registers functions::functions::analogRead(uint8_t argc, uint8_t *argv) {
	registers result;

	if (argc == 1) {
		uint16_t read_val = aanalogRead(argv[0]);
		result.count = 2;
		result.value = malloc(sizeof(uint8_t) * 2);
		result.value[0] = read_val >> 8;
		result.value[1] = read_val & 0x00ff;
	}
	else {
		result.count = 0;
	}

	return result;
}


/**
 * @brief Attach a connected servo to a control interface
 * 
 * @param argc The number of arguments contained within the 'argv' array (1)
 * @param argv The arguments to use within the function (pin #)
 * @return result: a 'registers' struct holding the return values of the called function
 */
registers functions::servoAttach(uint8_t argc, uint8_t *argv) {
	registers result;
	if (argc == 1) {
		int pin = argv[0];
		result.count = 1;
		result.value = malloc(sizeof(uint8_t));
		if (pin >= 0 && pin <= 13 && servo_count < MAX_SERVOS) {
			result.value[0] = servos[pin].attach(pin);
		}
		else {
			result.value[0] = 0;
		}
	}
	else {
		result.count = 0;
	}

	return result;
}


/**
 * @brief Detach a servo from the the control interface
 * 
 * @param argc The number of arguments contained within the 'argv' array (1)
 * @param argv The arguments to use within the function (pin #)
 * @return result: a 'registers' struct holding the return values of the called function (1 on success)
 */
registers functions::servoDetach(uint8_t argc, uint8_t *argv) {
	registers result;
	if (argc == 1) {
		int pin = argv[0];
		result.count = 1;
		result.value = malloc(sizeof(uint8_t));
		if (pin >= 0 && pin <= 13) {
			servos[pin].detach();
			result.value[0] = 1;
		}
		else {
			result.value[0] = 0;
		}
	}
	else {
		result.count = 0;
	}

	return result;
}


/**
 * @brief Write a value to the connected servo
 * 
 * @param argc The number of arguments contained within the 'argv' array (2)
 * @param argv The arguments to use within the function (pin #, value)
 * @return result: a 'registers' struct holding the return values of the called function (1 on success)
 */
registers functions::servoWrite(uint8_t argc, uint8_t *argv) {
	registers result;
	if(argc == 2) {
		int pin = argv[0];
		int angle = argv[1];
		result.count = 1;
		result.value = malloc(sizeof(uint8_t));
		if (pin >= 0 && pin <= 13) {
			servos[pin].write(angle);
			result.value[0] = 1;
		}
		else {
			result.value[0] = 0;
		}
	}
	else {
		result.count = 0;
	}
	
	return result;
}


/**
 * @brief Read a value from the connected servo
 * 
 * @param argc The number of arguments contained within the 'argv' array
 * @param argv The arguments to use within the function
 * @return result: a 'registers' struct holding the return values of the called function
 */
registers functions::servoRead(uint8_t argc, uint8_t *argv) {
	registers result;
	if (argc == 1) {
		int pin = argv[0];
		if (pin >= 0 && pin <= 13) {
			result.count = 1;
			result.value = malloc(sizeof(uint8_t));
			result.value[0] = servos[pin].read();
		}
		else {
			result.count = 0;
		}
	}
	else {
		result.count = 0;
	}
	
	return result;
}


/**
 * @brief Begin an I2C connection between the Arduino and a peripheral
 * 
 * @param argc The number of arguments contained within the 'argv' array
 * @param argv The arguments to use within the function
 * @return result: a 'registers' struct holding the return values of the called function
 */
registers functions::wireBegin(uint8_t argc, uint8_t *argv) {
	Wire.begin();
	registers result;
	result.count = 0;
	return result;
}


/**
 * @brief End an I2C connection between the Arduino and a peripheral
 * 
 * @param argc The number of arguments contained within the 'argv' array
 * @param argv The arguments to use within the function
 * @return result: a 'registers' struct holding the return values of the called function
 */
registers functions::wireEnd(uint8_t argc, uint8_t *argv) {
	Wire.end();
	registers result;
	result.count = 0;
	return result;
}


/**
 * @brief Change the clock speed settings of the I2C connection
 * 
 * @param argc The number of arguments contained within the 'argv' array
 * @param argv The arguments to use within the function
 * @return result: a 'registers' struct holding the return values of the called function
 */
registers functions::wireSetClock(uint8_t argc, uint8_t *argv) {
	if (argc == 4) {
		// 4 8-bit values combine to make a 32-bit int
		Wire.setClock((uint32_t)argv[0] << 24 | (uint32_t)argv[1] << 16 | (uint32_t)argv[2] << 8 | (uint32_t)argv[3]);
	}
	registers result;
	result.count = 0;
	return result;
}


/**
 * @brief Write data to the connected I2C peripheral
 * 
 * @param argc The number of arguments contained within the 'argv' array
 * @param argv The arguments to use within the function
 * @return result: a 'registers' struct holding the return values of the called function
 */
registers functions::wireWrite(uint8_t argc, uint8_t *argv) {
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
	
	registers result;
	result.count = 0;
	return result;	
}



/**
 * @brief Read data from the connected I2C peripheral
 * 
 * @param argc The number of arguments contained within the 'argv' array
 * @param argv The arguments to use within the function
 * @return result: a 'registers' struct holding the return values of the called function
 */
registers functions::wireRead(uint8_t argc, uint8_t *argv) {
	registers result;
	if(argc == 3) {
		// Argument format: addr | reg | num_bytes
		uint8_t addr = argv[0];
		uint8_t reg = argv[1];
		uint8_t num_bytes = argv[2];
		result.value = malloc(sizeof(uint8_t) * num_bytes);
		
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
		else {
			result.count = 0;
		}
	}
	else {
		result.count = 0;
	}

	return result;	
}


/**
 * @brief Begin a SPI connection between the Arduino and a peripheral
 * 
 * @param argc The number of arguments contained within the 'argv' array
 * @param argv The arguments to use within the function
 * @return result: a 'registers' struct holding the return values of the called function
 */
registers functions::spiBegin(uint8_t argc, uint8_t *argv) {
	registers result;
	result.count = 0;

	if (argc == 0) {
		SPI.begin();
		settings.speed = 4000000;
		settings.order = MSBFIRST;
		settings.mode = SPI_MODE0;
	}

	return result;
}


/**
 * @brief Change the settings of the SPI connection
 * 
 * @param argc The number of arguments contained within the 'argv' array
 * @param argv The arguments to use within the function
 * @return result: a 'registers' struct holding the return values of the called function
 */
registers functions::spiSettings(uint8_t argc, uint8_t *argv) {
	registers result;
	result.count = 0;

	if (argc == 6) {
		settings.speed = (uint32_t)argv[0] << 24 | (uint32_t)argv[1] << 16 | (uint32_t)argv[2] << 8 | (uint32_t)argv[3];
		settings.order = argv[4];
		settings.mode = argv[5];
	}

	return result;
}


/**
 * @brief Exchange data over the SPI connection (Read + Write)
 * 
 * @param argc The number of arguments contained within the 'argv' array
 * @param argv The arguments to use within the function
 * @return result: a 'registers' struct holding the return values of the called function
 */
registers functions::spiTransferBuf(uint8_t argc, uint8_t *argv) {
	// Register format:
	// | CMD/ARGC | Bytes                 |
	// |<-1 word->|<-31 words / 62 bytes->|

	registers result;
	
	if (argc > 1) {
		uint8_t CS_pin = argv[0];

		result.count = argc - 1;
		result.value = malloc(sizeof(uint8_t) * (argc - 1));
		
		SPI.beginTransaction(SPISettings(settings.speed, settings.order, settings.mode));
		adigitalWrite((uint8_t)CS_pin, (uint8_t)LOW);
		for(int i = 1; i < argc; i++) {
			result.value[i-1] = SPI.transfer(argv[i]);
		}
		adigitalWrite((uint8_t)CS_pin, (uint8_t)HIGH);
		SPI.endTransaction();
	}
	else {
		result.count = 0;
	}

	return result;
}


/**
 * @brief End a SPI connection between the Arduino and a peripheral
 * 
 * @param argc The number of arguments contained within the 'argv' array (0)
 * @param argv The arguments to use within the function (None)
 * @return result: a 'registers' struct holding the return values of the called function (empty, in this case)
 */
registers functions::spiEnd(uint8_t argc, uint8_t *argv) {
	registers result;
	result.count = 0;

	if (argc == 0) {
		SPI.end();
	}

	return result;
}
