#include "Functions.h"
#include "ModbusSerial.h"

#define MAX_REG_COUNT 100

namespace modmata {

  /**
   * @brief Base class for a host computer to control this (LattePanda's Arduino Leonardo) device
   */
  class ModmataClass {

    private:
      /**
       * @brief An array of references to callback functions indexed by their function code number. 
       * @n Use 'Modmata.attach( function_code, &function )' to add your own callback functions
       */
      registers (*callbackFunctions[20])(uint8_t argc, uint8_t *argv);
      ModbusSerial mb;    

    public:
      void begin(int baud);
      void attach(uint8_t command, struct registers (*fn)(uint8_t argc, uint8_t *argv));
      void processInput();
      bool available();

  };
}

extern modmata::ModmataClass Modmata;
