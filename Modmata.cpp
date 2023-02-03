/** 
@file Modmata.cpp
@author Sam Hutcherson
@date 01/25/23
@brief Functions to access Modmata commands from arduino .ino files.
*/
#include "Modmata.h"
#include "HardwareSerial.h"
using namespace modmata;

ModmataClass Modmata;

/**
Start the Modbus serial connection with a baud rate of 9600 and slave id of 1.
@return void
*/
void ModmataClass::begin()
{
  callbackFunctions[PINMODE] = &pinMode;
  callbackFunctions[DIGITALWRITE] = &digitalWrite;
  callbackFunctions[DIGITALREAD] = &digitalRead;
  callbackFunctions[ANALOGREFERENCE] = &analogReference;
  callbackFunctions[ANALOGWRITE] = &analogWrite;
  callbackFunctions[ANALOGREAD] = &analogRead;
  
  mb.config(&Serial, 9600, SERIAL_8N1);
  mb.setSlaveId(1);
 
  // Command register
  mb.addHreg(0);

  // Pin register
  mb.addHreg(1);

  // Value register
  mb.addHreg(2);
}

/**
Assign a function to a command number. Standard commands have default functions, but those can be overwritten here, or more commands can be added.
@param command The modbus command being assigned a function
@param fn A pointer to the function to be called when the command is recieved
@return void
*/
void ModmataClass::attach(uint16_t command, int (*fn)(uint8_t *arg1, uint8_t *arg2))
{
  callbackFunctions[command] = fn;
}

/** 
Execute a command when the input is recieved.
@return output A struct containing the command, arguments, and return value for debugging 
*/
debug ModmataClass::processInput()
{
  debug output;
  output.cmd = mb.Hreg(0);
  output.arg1 = mb.Hreg(1);
  output.arg2 = mb.Hreg(2);
  mb.Hreg(2, (callbackFunctions[mb.Hreg(0)])(mb.Hreg(1), mb.Hreg(2)));
  output.result = mb.Hreg(2);
  mb.Hreg(0, 0);
  return output;
}

/**
Update modbus registers and check if a command has been recieved
@return True or false
*/
bool ModmataClass::available()
{
  return mb.task() && mb.Hreg(0);
}

