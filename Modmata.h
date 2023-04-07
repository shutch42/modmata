/*
Modmata
Copyright © 2023 char* teamName <shutche@siue.edu>
Licensed under LGPL-2.1
*/

/**
 * @file Modmata.h
 * @author Sam Hutcherson, Chase Wallendorff, Iris Astrid
 * @brief Header file for 'Modmata.cpp'
 * @date 2023-04-04
 */

#include "Functions.h"
#include "ModbusSerial.h"

#ifndef MODMATA_H
#define MODMATA_H

#define MAX_REG_COUNT 100

/** @brief Modmata namespace */
namespace modmata {

  /** @brief Base class for a host computer to control this (LattePanda's Arduino Leonardo) device */
  class ModmataClass {
    public:
      void begin(int baud);
      void attach(uint8_t command, struct registers (*fn)(uint8_t argc, uint8_t *argv));
      void processInput();
      bool available();
    
    private:

      /** @brief An array of references to callback functions indexed by their function code number. 
       * Use 'Modmata.attach( function_code, &function )' to add your own callback functions */
      struct registers (*callbackFunctions[100])(uint8_t argc, uint8_t *argv);

      /** @brief Object representing an interactive Modbus connection over Serial */
      ModbusSerial mb;
      
  };
}

extern modmata::ModmataClass Modmata;
#endif
