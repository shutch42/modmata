/**
 * @file Modmata.cpp
 * @author Sam Hutcherson, Chase Wallendorff, Iris Astrid
 * @date 04/02/23
 * @brief Functions to access Modmata commands from arduino .ino files.
*/

#include "Modmata.h"
#include <cstdlib>  // for 'calloc', 'free'
#include <cmath>    // for 'ceil'
#include <cstring>  // for 'memcpy'

#ifndef byte
/** 
 * @brief unsigned 8-bit integral data type
 * @remark has a type definition within Arduino, but not within 'standard' C++
 */
typedef unsigned char byte;
#endif

using namespace modmata;

ModmataClass Modmata; // Singleton instance


/**
 * @brief Begin listening for a Modmata connection over serial
 * @param baud Set the baud rate of the listening serial connection
 * @remark This is configured to be used between a host computer and Arduino
 * Leonardo using a USB connector (hardwired in the case of the LattePanda Delta 3)
 * The Leonardo is different from most other Arduinos in the usage of the serial connection,
 * so if you modify this code, please keep that in mind.
 */
void ModmataClass::begin(int baud) {
  // Set host serial/modbus connection settings
  mb.config(&Serial, baud, SERIAL_8N1);
  mb.setSlaveId(1);
  
  // Map callback functions to associated function codes
  callbackFunctions[PINMODE] =      &(functions::pinMode);
  callbackFunctions[DIGITALWRITE] = &(functions::digitalWrite);
  callbackFunctions[DIGITALREAD] =  &(functions::digitalRead);
  callbackFunctions[ANALOGWRITE] =  &(functions::analogWrite);
  callbackFunctions[ANALOGREAD] =   &(functions::analogRead);
  
  callbackFunctions[SERVOATTACH] =  &(functions::servoAttach);
  callbackFunctions[SERVODETACH] =  &(functions::servoDetach);
  callbackFunctions[SERVOWRITE] =   &(functions::servoWrite);
  callbackFunctions[SERVOREAD] =    &(functions::servoRead);

  callbackFunctions[WIREBEGIN] =    &(functions::wireBegin);
  callbackFunctions[WIREEND] =      &(functions::wireEnd);
  callbackFunctions[WIRECLOCK] =    &(functions::wireSetClock);
  callbackFunctions[WIREWRITE] =    &(functions::wireWrite);
  callbackFunctions[WIREREAD] =     &(functions::wireRead);

  callbackFunctions[SPIBEGIN] =     &(functions::spiBegin);
  callbackFunctions[SPISETTINGS] =  &(functions::spiSettings);
  callbackFunctions[SPITRANSFER] =  &(functions::spiTransferBuf);
  callbackFunctions[SPIEND] =       &(functions::spiEnd);

  // Command register - create sufficient (100) Holding Registers
  for(int i = 0; i < MAX_REG_COUNT; i++) {
    mb.addHreg(i);
  }
}

/**
 * Assign a function to a command number. Standard commands have default functions, 
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
  // Unpack all of the modbus registers as 8-bit values
  byte    cmd   =  mb.Hreg(0) >> 8;     // 'Top' 8 bits of Hreg 0 indicate the command number
  byte    argc  =  mb.Hreg(0) & 0x00ff; // 'Bottom' 8 bits of Hreg 0 indicate the number of arguments for a command
  
  size_t n_hregs = ceil(argc / 2);      // Number of holding registers occupied by the command args
  uint16_t hreg_array[ n_hregs ] = {};  // Temp. array to hold argument values
  for (int i{0}; i < n_hregs; i++) {
    hreg_array[i] = mb.Hreg(i+1);       // Copy over values from Hregs to temp array 
  }

  uint8_t * argv = (uint8_t *)calloc(argc, sizeof(byte)); // Argument array for the command
  memcpy(argv, &hreg_array, argc);                        // Copy over values into persisting array

  struct registers result = (callbackFunctions[cmd])(argc, argv); // Execute fn, collect result
  n_hregs = ceil(result.count / 2);       // Get number of holding registers needed to store/return the result
                                          // This is not the same as the number of arguments used in a command!
  
  uint16_t return_array[ n_hregs ] = {};  // Temp. array to store the collected values
  memcpy(return_array, result.value, result.count); // Copy over values
  for (int i{0}; i < n_hregs; i++) {
    mb.Hreg(i+1, return_array[i]);        // Update Hregs with result
  }
  
  free(argv);
  free(result.value);

  // Save the number of result values, return to idle command
  mb.Hreg(0, result.count);   // Sets the 'bottom' 8 bits of the 16-bit holding register (result.count is of type <uint8_t>) 
                              // to # return vals, & 'top' 8 bits to 0's
}


/**
 * Update modbus registers and check if a command has been received
 * @remark Will return false unless there is a Command function code besides IDLE in Hreg 0
 * _and_ there was data received since the last time mb.task() was called 
 * @return True or false
*/
bool ModmataClass::available() {
  return mb.task() && (mb.Hreg(0) >> 8);
}

