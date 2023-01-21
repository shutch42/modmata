/** 
@file Modmata.cpp
@author Sam Hutcherson
@date 01/20/23
@brief Functions to access Modmata commands from arduino .ino files.
*/
#include "Modmata.h"
#include "HardwareSerial.h"
using namespace modmata;

ModmataClass Modmata;

/**
Access the first holding register from Modbus containing the command value.
@return The command register value
*/
int ModmataClass::getCommand()
{
  return mb.Hreg(0);
}

/**
Reset the command register to 0, completing the transaction.
@return void
*/
void ModmataClass::complete()
{
  //reset command register to 0
  mb.Hreg(0, 0);
}

/**
Access the holding register containing the pin value of the current command.
@return The pin number
*/
int ModmataClass::getPin()
{
  return mb.Hreg(1);
}
/**
Access the holding register containing the read or write value of the command.
@return The command's read or write value
*/
int ModmataClass::getValue()
{
  return mb.Hreg(2);
}

/**
Set the holding register in case the command will be returning a value. (ex: digitalRead(), analogRead())
@param value A 16 bit value to return to the client, typically an arduino function's return value
@return void
*/
void ModmataClass::setValue(uint16_t value)
{
  mb.Hreg(2, value);
}

/**
Start the Modbus serial connection with a baud rate of 9600 and slave id of 1.
@return void
*/
void ModmataClass::begin()
{
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
Check if a command has been sent from the client.
@return True or False
*/
bool ModmataClass::commandRecieved()
{
  // Update modbus registers and poll for command
  return mb.task() && getCommand();
}

