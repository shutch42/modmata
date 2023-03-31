#include "Functions.h"
#include "ModbusSerial.h"

#define MAX_REG_COUNT 100

namespace modmata {
class ModmataClass
{
  public:
    void begin(int baud);
    void attach(uint8_t command, struct registers (*fn)(uint8_t argc, uint8_t *argv));
    void processInput();
    bool available();
  
  private:
    struct registers (*callbackFunctions[100])(uint8_t argc, uint8_t *argv);
    ModbusSerial mb;
    
};
}

extern modmata::ModmataClass Modmata;

