#include "ModbusSerial.h"
#include "Constants.h"

namespace modmata {

class ModmataClass
{
  public:
    void begin();
    void attach(uint16_t command, int (*fn)(uint8_t *arg1, uint8_t *arg2));
    void processInput();
    bool available();
  
  private:
    int (*callbackFunctions[20])(uint8_t *arg1, uint8_t *arg2);
    ModbusSerial mb;
};
}

extern modmata::ModmataClass Modmata;
