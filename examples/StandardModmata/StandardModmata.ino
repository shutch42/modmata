#include <Modmata.h>

void setup() {
  // put your setup code here, to run once:
  Modmata.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Modmata.commandRecieved()) {
    switch(Modmata.getCommand()) {
      case PINMODE:
        pinMode(Modmata.getPin(), Modmata.getValue());
        break;
      case DIGITALWRITE:
        digitalWrite(Modmata.getPin(), Modmata.getValue());
        break;
      case DIGITALREAD:
        Modmata.setValue(digitalRead(Modmata.getPin()));
        break;
      case ANALOGREFERENCE:
        analogReference(Modmata.getValue());
        break;
      case ANALOGWRITE:
        analogWrite(Modmata.getPin(), Modmata.getValue());
        break;
      case ANALOGREAD:
        Modmata.setValue(analogRead(Modmata.getPin()));
        break;
    }
    Modmata.complete();
  }
}
