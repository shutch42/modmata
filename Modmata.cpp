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
  servo_count = 0;
  callbackFunctions[PINMODE] = &pinMode;
  callbackFunctions[DIGITALWRITE] = &digitalWrite;
  callbackFunctions[DIGITALREAD] = &digitalRead;
  callbackFunctions[ANALOGREFERENCE] = &analogReference;
  callbackFunctions[ANALOGWRITE] = &analogWrite;
  callbackFunctions[ANALOGREAD] = &analogRead;
  //callbackFunctions[SERVOATTACH] = (int (*)(uint8_t*, uint8_t*))&servoAttach;
  //callbackFunctions[SERVODETACH] = (int (*)(uint8_t*, uint8_t*))&servoDetach;
  //callbackFunctions[SERVOWRITE] = (int (*)(uint8_t*, uint8_t*))&servoWrite;
  //callbackFunctions[SERVOREAD] = (int (*)(uint8_t*, uint8_t*))&servoRead;
  
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
  callbackFunctions[command] = *fn;
}

/** 
Execute a command when the input is recieved.
@return void
*/
debug ModmataClass::processInput()
{
  debug output;
  output.cmd = mb.Hreg(0);
  output.arg1 = mb.Hreg(1);
  output.arg2 = mb.Hreg(2);
  switch(mb.Hreg(0)) {
    case 7:
      mb.Hreg(2, servoAttach(mb.Hreg(1)));
      break;
    case 8:
      mb.Hreg(2, servoDetach(mb.Hreg(1)));
      break;
    case 9:
      mb.Hreg(2, servoWrite(mb.Hreg(1), mb.Hreg(2)));
      break;
    case 10:
      mb.Hreg(2, servoRead(mb.Hreg(1)));
      break;
    default:
      mb.Hreg(2, (callbackFunctions[mb.Hreg(0)])(mb.Hreg(1), mb.Hreg(2)));
      break;
  }
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

/**
Attach a servo to a digital/PWM pin.
@param pin The pin number that the servo is attached to.
@return 1 on success, 0 on error
*/
static int ModmataClass::servoAttach(int pin)
{
  if (pin >= 0 && pin <= 13 && servo_count < 12)
  {
    Servo curr_servo;
    curr_servo.attach(pin);
    servos[pin] = &curr_servo;
    servo_count++;
    return 1;
  }
  else
  {
    return 0;
  }

}

/**
Detach a servo from a pin
@param pin The pin number that the servo is being detached from
@return 1 on success, 0 on error
*/
static int ModmataClass::servoDetach(int pin)
{
  if (pin >= 0 && pin <= 13 && servos[pin])
  {
    servos[pin]->detach();
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
Write an angle value to a servo
@param pin The pin number that the servo is attached to
@param angle The angle value the user wishes to write
@return 1 on success, 0 on error
*/
static int ModmataClass::servoWrite(int pin, int angle)
{
  if (pin >= 0 && pin <= 13 && servos[pin])
  {
    servos[pin]->write(angle);
    return 1;
  }
  else
  {
    return 0;
  }
}

/**
Read an angle value from a servo (the last value written to the servo)
@param pin The pin number that the servo is attached to
@return The previously written value on success, -1 on error
*/
static int ModmataClass::servoRead(int pin)
{
  if (pin >= 0 && pin <= 13 && servos[pin])
  {
    return servos[pin]->read();
  }
  else
  {
    return -1;
  }
}
