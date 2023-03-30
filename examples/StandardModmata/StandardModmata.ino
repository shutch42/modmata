#include <Modmata.h>

void setup() {
  // put your setup code here, to run once:
  Modmata.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Modmata.available()) {
    Modmata.processInput();
  }
}
