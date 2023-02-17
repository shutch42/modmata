#include <Servo.h>
#include "Constants.h"
#include "ModbusSerial.h"

#define MAX_REG_COUNT 100

struct registers {
	uint16_t count;
	uint16_t* value;
};

namespace modmata {

class ModmataClass
{
  public:
    void begin();
    void attach(uint16_t command, struct registers (*fn)(uint16_t argc, uint16_t *argv));
    void processInput();
    bool available();
  
  private:
    struct registers (*callbackFunctions[20])(uint16_t argc, uint16_t *argv);
    ModbusSerial mb;
    
};
}

extern modmata::ModmataClass Modmata;

