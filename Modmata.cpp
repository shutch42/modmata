/*
Modmata
Copyright © 2023 char* teamName <shutche@siue.edu>
Licensed under LGPL-2.1
*/

/**
 * @file Modmata.cpp
 * @author Sam Hutcherson, Chase Wallendorff, Iris Astrid
 * @brief Functions to access Modmata commands from arduino .ino files.
 * @date 2023-04-06
 */

/**
 * @example ModmataLCD.ino
 * @example StandardModmata.ino
 */

#include "Modmata.h"
using namespace modmata;

ModmataClass Modmata;

/**
 * @brief Begin listening for a Modmata connection over serial/USB
 * @remark This is configured to be used between a host computer and Arduino Leonardo using a USB connector
 * (hardwired in the case of the LattePanda Delta 3.) The Leonardo is different from most other Arduinos 
 * in the usage of the serial connection, so if you modify this code, please keep that in mind.
 * @param baud Set the baud rate of the listening serial connection
 */
void ModmataClass::begin(int baud) {
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
 * @brief Assign a function to a command number. Standard commands have default functions, 
 * but those can be overwritten here, or more commands can be added.
 * @param command The modbus command being assigned a function
 * @param fn A pointer to the function to be called when the command is recieved
 */
void ModmataClass::attach(uint8_t command, struct registers (*fn)(uint8_t argc, uint8_t *argv)) {
  callbackFunctions[command] = fn;
}

/**
 * @brief Read the command and args sent and execute the corresponding callback function,
 * store the results of which in holding functions to be communicated with the host.
 */
void ModmataClass::processInput() {
  // UNPACK COMMAND/FUNCTION CODE & NUMBER OF ARGS (ARGC)
  uint16_t CMD_ARGC = mb.Hreg(0);
  int cmd = highByte(CMD_ARGC);
  int argc = lowByte(CMD_ARGC);

  // Allocate space for argv to be transferred
  uint8_t *argv = (uint8_t *)malloc(sizeof(uint8_t) * argc);

  // Read Hregs into argv
  for(int i = 0; i < argc; i++) {
    uint16_t thisWord = mb.Hreg(i/2 + 1); // read Hreg()

    // set argv[i] to the low or high byte of the current holding register depending the parity of the current index
    argv[i] = (i % 2 == 0 ? highByte(thisWord) : lowByte(thisWord));
  }

  // EXECUTE CALLBACK FUNCTION
  struct registers result = (callbackFunctions[cmd])(argc, argv);
  
  // RESPOND WITH RESULT
  for(int i = 0; i < result.count; i++) {
    uint16_t thisWord = mb.Hreg(i/2 + 1); // Get the value of the current holding register
    uint8_t curResult = result.value[i];  // Get the value of the current result

    // Write half the holding register at a time, depending on whether 
    // it will be the high or low byte (determined by the parity of i)
    mb.Hreg(i/2 + 1, (i % 2 == 0 ? makeWord(curResult, 0) : thisWord | curResult));
  }
  
  // Deallocate memory
  free(argv);
  if (result.value != nullptr) free(result.value);

  // Save the number of result values, return to idle command
  mb.Hreg(0, result.count);
}

/**
 * Update modbus registers and check if a command has been received
 * @remark Will return false unless there is a Command function code besides IDLE in Hreg 0
 * _and_ there was data received since the last time mb.task() was called 
 * @return True or false
 */
bool ModmataClass::available() {
  return mb.task() && (highByte(mb.Hreg(0)));
}

