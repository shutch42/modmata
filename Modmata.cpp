#include "Modmata.h"
#include "HardwareSerial.h"
using namespace modmata;

ModmataClass Modmata;

int ModmataClass::getCommand()
{
  return mb.Hreg(0);
}

void ModmataClass::complete()
{
  //reset command register to 0
  mb.Hreg(0, 0);
}

int ModmataClass::getPin()
{
  return mb.Hreg(1);
}

int ModmataClass::getValue()
{
  return mb.Hreg(2);
}

void ModmataClass::setValue(uint16_t value)
{
  mb.Hreg(2, value);
}

void ModmataClass::begin(void)
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

bool ModmataClass::commandRecieved()
{
  // Update modbus registers and poll for command
  return mb.task() && getCommand();
}

