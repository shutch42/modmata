/** 
@file Modmata.cpp
@author Sam Hutcherson, Chase Wallendorff, Iris Astrid
@date 02/16/23
@brief Functions to access Modmata commands from arduino .ino files.
*/

#include "Modmata.h"
using namespace modmata;

ModmataClass Modmata;

/**
Start the Modbus serial connection with a baud rate of 9600 and slave id of 1.
@return void
*/
void ModmataClass::begin(int baud)
{
  mb.config(&Serial, baud, SERIAL_8N1);
  mb.setSlaveId(1);
  
  callbackFunctions[PINMODE] = &pinMode;
  callbackFunctions[DIGITALWRITE] = &digitalWrite;
  callbackFunctions[DIGITALREAD] = &digitalRead;
  callbackFunctions[ANALOGWRITE] = &analogWrite;
  callbackFunctions[ANALOGREAD] = &analogRead;
  
  callbackFunctions[SERVOATTACH] = &servoAttach;
  callbackFunctions[SERVODETACH] = &servoDetach;
  callbackFunctions[SERVOWRITE] = &servoWrite;
  callbackFunctions[SERVOREAD] = &servoRead;

  callbackFunctions[WIREBEGIN] = &wireBegin;
  callbackFunctions[WIREEND] = &wireEnd;
  callbackFunctions[WIRECLOCK] = &wireSetClock;
  callbackFunctions[WIREWRITE] = &wireWrite;
  callbackFunctions[WIREREAD] = &wireRead;

  callbackFunctions[SPIBEGIN] = &spiBegin;
  callbackFunctions[SPISETTINGS] = &spiSettings;
  callbackFunctions[SPITRANSFER] = &spiTransferBuf;
  callbackFunctions[SPIEND] = &spiEnd;

  // Command register
  for(int i = 0; i < MAX_REG_COUNT; i++) {
    mb.addHreg(i);
  }
}

/**
Assign a function to a command number. Standard commands have default functions, but those can be overwritten here, or more commands can be added.
@param command The modbus command being assigned a function
@param fn A pointer to the function to be called when the command is recieved
@return void
*/
void ModmataClass::attach(uint8_t command, struct registers (*fn)(uint8_t argc, uint8_t *argv))
{
  callbackFunctions[command] = fn;
}

/** 
Execute a command when the input is recieved.
@return output A struct containing the command, arguments, and return value for debugging 
*/
void ModmataClass::processInput()
{
  // Unpack all of the modbus registers as 8-bit values
  int cmd = mb.Hreg(0) >> 8;
  int argc = mb.Hreg(0) & 0x00ff;

  uint8_t *argv = malloc(sizeof(uint8_t) * argc);

  for(int i = 0; i < argc; i++) {
  	if (i % 2 == 0) {
		argv[i] = mb.Hreg(i/2 + 1) >> 8;
	}
	else {
		argv[i] = mb.Hreg(i/2 + 1) & 0x00ff;
	}
  }

  struct registers result = (callbackFunctions[cmd])(argc, argv);
  
  // Set all of the result values
  for(int i = 0; i < result.count; i++) {
	if (i % 2 == 0) {
		mb.Hreg(i/2 + 1, (uint16_t)result.value[i] << 8); 
	}
	else {
		// uint16_t curr_reg = mb.Hreg(i/2 + 1);
		mb.Hreg(i/2 + 1, mb.Hreg(i/2 + 1) | result.value[i]); 
	}
  }
  
  free(argv);
  free(result.value);

  // Save the number of result values, return to idle command
  mb.Hreg(0, result.count);
}

/**
Update modbus registers and check if a command has been recieved
@return True or false
*/
bool ModmataClass::available()
{
  return mb.task() && (mb.Hreg(0) >> 8);
}

