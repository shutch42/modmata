#include "Functions.h"

int servo_count = 0;
Servo servos[14];	// FIXME: Change to pointers so that pins 9/10 can be used as analog when Servos aren't used

struct spi_settings settings;

struct registers pinMode(uint8_t argc, uint8_t *argv) {
	if (argc == 2) {
		pinMode(argv[0], argv[1]);
	}

	struct registers result;
	result.count = 0;
	return result;
}

struct registers digitalWrite(uint8_t argc, uint8_t *argv) {
	if (argc == 2) {
		digitalWrite(argv[0], argv[1]);
	}

	struct registers result;
	result.count = 0;
	return result;
}

struct registers digitalRead(uint8_t argc, uint8_t *argv) {
	struct registers result;

	if (argc == 1) {
		uint8_t read_val = digitalRead(argv[0]);
		result.count = 1;
		result.value = malloc(sizeof(uint8_t));
		result.value[0] = read_val;
	}
	else {
		result.count = 0;
	}

	return result;
}

struct registers analogWrite(uint8_t argc, uint8_t *argv) {
	if (argc == 3) {
		analogWrite(argv[0], argv[1] << 8 | argv[2]);
	}

	struct registers result;
	result.count = 0;
	return result;
}

struct registers analogRead(uint8_t argc, uint8_t *argv) {
	struct registers result;

	if (argc == 1) {
		uint16_t read_val = analogRead(argv[0]);
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

struct registers servoAttach(uint8_t argc, uint8_t *argv) {
	struct registers result;
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

struct registers servoDetach(uint8_t argc, uint8_t *argv) {
	struct registers result;
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

struct registers servoWrite(uint8_t argc, uint8_t *argv) {
	struct registers result;
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

struct registers servoRead(uint8_t argc, uint8_t *argv) {
	struct registers result;
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

struct registers wireBegin(uint8_t argc, uint8_t *argv) {
	Wire.begin();
	struct registers result;
	result.count = 0;
	return result;
}

struct registers wireEnd(uint8_t argc, uint8_t *argv) {
	Wire.end();
	struct registers result;
	result.count = 0;
	return result;
}

struct registers wireSetClock(uint8_t argc, uint8_t *argv) {
	if (argc == 1) {
		Wire.setClock(argv[0]);
	}
	struct registers result;
	result.count = 0;
	return result;
}

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
	
	struct registers result;
	result.count = 0;
	return result;	
}

struct registers wireRead(uint8_t argc, uint8_t *argv) {
	struct registers result;
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

struct registers spiBegin(uint8_t argc, uint8_t *argv) {
	struct registers result;
	result.count = 0;

	if (argc == 0) {
		SPI.begin();
		settings.speed = 4000000;
		settings.order = MSBFIRST;
		settings.mode = SPI_MODE0;
	}

	return result;
}

struct registers spiSettings(uint8_t argc, uint8_t *argv) {
	struct registers result;
	result.count = 0;

	if (argc == 3) {
		settings.speed = argv[0];
		settings.order = argv[1];
		settings.mode = argv[2];
	}

	return result;
}

struct registers spiTransferBuf(uint8_t argc, uint8_t *argv) {
	// | CMD | ARGC | CS_pin |   Transfer bytes   |
	// |<----- 3 words ----->|<-- 29 words max -->|

	// Alternate strategy:
	// | CMD/ARGC | Bytes                 |
	// |<-1 word->|<-31 words / 62 bytes->|

	struct registers result;
	
	if (argc > 1) {
		uint8_t CS_pin = argv[0];

		result.count = argc - 1;
		result.value = malloc(sizeof(uint8_t) * (argc - 1));
		
		SPI.beginTransaction(SPISettings(settings.speed, settings.order, settings.mode));
		digitalWrite((uint8_t)CS_pin, (uint8_t)LOW);
		for(int i = 1; i < argc; i++) {
			result.value[i-1] = SPI.transfer(argv[i]);
		}
		digitalWrite((uint8_t)CS_pin, (uint8_t)HIGH);
		SPI.endTransaction();
	}
	else {
		result.count = 0;
	}

	return result;
}

struct registers spiEnd(uint8_t argc, uint8_t *argv) {
	struct registers result;
	result.count = 0;

	if (argc == 0) {
		SPI.end();
	}

	return result;
}

