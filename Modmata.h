#include "ModbusSerial.h"
#include "Constants.h"

namespace modmata {

class ModmataClass
{
  public:
    void begin();
    bool commandRecieved();
    int getCommand();
    int getPin();
    int getValue();
    void setValue(uint16_t value);
    void complete();

  private:
    ModbusSerial mb;
};
}

extern modmata::ModmataClass Modmata;
