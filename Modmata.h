#ifndef MODMATA_H
#define MODMATA_H
#include "ModbusSerial.h"
#include "Constants.h"
#include <inttypes.h>

using namespace std;

class ModmataClass {
  public:
    /**
    Initialize the Modbus/Firmata connection over serial
    @return void
    */
    void begin() {
      this->mb.config(&Serial, 9600, SERIAL_8N1);
      this->mb.setSlaveId(1);
      this->mb.addHreg(0,0);
      this->mb.addHreg(1,0);
      this->mb.addHreg(2,0);
      return;
    }


    /**
    Close the Modbus/Firmata serial connection
    @return void
    */
    void close() {
      // TODO: be able to free registers, close connection, etc...
      return;
    }


    /**
    Read data in holding registers and execute commands accordingly
    @return void
    */
    void parseCmd() {
      uint16_t CMD = this->mb.Hreg(0);
      uint16_t PIN = this->mb.Hreg(1);
      uint16_t VAL = this->mb.Hreg(2);

      switch (CMD) {
        case IDLE:                                          break;
        case PINMODE:         pinMode(PIN, VAL);            break;    // Make sure 'VAL' here... 
        case DIGITALWRITE:    digitalWrite(PIN, VAL);       break;
        case DIGITALREAD:     mb.Hreg(2, digitalRead(PIN)); break;
        case ANALOGREFERENCE: analogReference(VAL);         break;    // ...and here are consistent with acceptable values/named constants
        case ANALOGWRITE:     analogWrite(PIN, VAL);        break;
        case ANALOGREAD:      mb.Hreg(2, analogRead(PIN));  break;
        default: break;
      }

      mb.Hreg(0,0);   // Set cmd to 'IDLE' before exiting
      return;
    }


  private:
    ModbusSerial mb;

};

#endif