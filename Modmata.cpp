/** 
@file Modmata.cpp
@author Sam Hutcherson
@date 02/16/23
@brief Functions to access Modmata commands from arduino .ino files.
*/
#include "Modmata.h"
#include "HardwareSerial.h"
#include "ModmataServo.h"
#include "Wire.h"		// These functions (mostly) seem to translate directly into Modmata
using namespace modmata;

ModmataClass Modmata;

/**
Start the Modbus serial connection with a baud rate of 9600 and slave id of 1.
@return void
*/
void ModmataClass::begin()
{
  mb.config(&Serial, 9600, SERIAL_8N1);
  mb.setSlaveId(1);
 
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
void ModmataClass::attach(uint16_t command, struct registers (*fn)(uint16_t argc, uint16_t *argv))
{
  callbackFunctions[command] = fn;
}

/** 
Execute a command when the input is recieved.
@return output A struct containing the command, arguments, and return value for debugging 
*/
void ModmataClass::processInput()
{
  int cmd = mb.Hreg(0);
  uint16_t rgc = mb.Hreg(1);
  uint16_t *argv = malloc(sizeof(uint16_t) * argc);
  for(int i = 0; i < argc; i++) {
	argv[i] = mb.Hreg(2+i);
  }
  struct registers result = (callbackFunctions[mb.Hreg(0)])(argc, argv);
  mb.Hreg(1, result.count);
  for(int i = 0; i < result.count; i++) {
    mb.Hreg(2+i, result.value[i]);
  }
  free(argv);
  free(result.value);
  mb.Hreg(0, 0);
}

/**
Update modbus registers and check if a command has been recieved
@return True or false
*/
bool ModmataClass::available()
{
  return mb.task() && mb.Hreg(0);
}

