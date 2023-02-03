#include <Servo.h>
#include "Constants.h"
#include "ModbusSerial.h"

struct debug {
	uint16_t cmd;
	uint16_t arg1;
	uint16_t arg2;
	uint16_t result;
};

namespace modmata {

class ModmataClass
{
  public:
    void begin();
    void attach(uint16_t command, int (*fn)(uint8_t *arg1, uint8_t *arg2));
    debug processInput();
    bool available();
  
  private:
    int (*callbackFunctions[20])(uint8_t *arg1, uint8_t *arg2);
    ModbusSerial mb;
    
};
}

extern modmata::ModmataClass Modmata;

