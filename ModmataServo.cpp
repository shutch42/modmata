#include "ModmataServo.h"

int servo_count = 0;
Servo servos[14];

int servoAttach(int pin) {
	if (pin >= 0 && pin <= 13 && servo_count < MAX_SERVOS) {
		servos[pin].attach(pin);
		servo_count++;
		return 1;
	}
	else {
		return 0;
	}
}

int servoDetach(int pin) {
	if (pin >= 0 && pin <= 13) {
		servos[pin].detach();
		servo_count--;
		return 1;
	}
	else {
		return 0;
	}
}

int servoWrite(int pin, int angle) {
	if (pin >= 0 && pin <= 13) {
		servos[pin].write(angle);
		return 1;
	}
	else {
		return 0;
	}
}

int servoRead(int pin) {
	if (pin >= 0 && pin <= 13) {
		return servos[pin].read();
	}
	else {
		return -1;
	}
}
