#include <Modmata.h>
#include <LiquidCrystal.h>
#include <Wire.h>

const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
char str[16];

// | cmd | # of arguments | arguments

struct registers wireWrite(int argc, uint16_t *argv) {
  lcd.clear();
  lcd.print("Write: ");
  lcd.print(argc);
  if(argc >= 2) {
    // | addr | reg | bytes
    uint8_t addr = argv[0];
    uint8_t reg = argv[1];
    lcd.setCursor(0, 1);
    sprintf(str, "%02x %02x ", addr, reg);
    lcd.print(str);
    for(int i = 2; i < argc; i++) {
      sprintf(str, "%02x ", argv[i]);
      lcd.print(str);
    }

    Wire.beginTransmission(addr);
    Wire.write(reg);
    for(int i = 2; i < argc; i++) {
      Wire.write(argv[i]);
    }
    Wire.endTransmission();
  }

  struct registers result;
  result.count = 0;
  return result;
}

struct registers wireRead(int argc, uint16_t *argv) {
  struct registers result;
  lcd.clear();
  lcd.print("Read: ");
  lcd.print(argc);
  lcd.setCursor(0, 1);
  if(argc == 3) {
    // addr | reg | num_bytes
    uint8_t addr = argv[0];
    uint8_t reg = argv[1];
    uint8_t num_bytes = argv[2];
    sprintf(str, "%02x %02x %i        ", addr, reg, num_bytes);
    lcd.print(str);
    result.value = malloc(sizeof(uint16_t) * num_bytes);

    Wire.beginTransmission(addr);
    Wire.write(reg);
    Wire.endTransmission();

    Wire.requestFrom(addr, num_bytes);
    if(Wire.available() == num_bytes) {
        result.count = num_bytes;
        for(int i = 0; i < num_bytes; i++) {
          result.value[i] = Wire.read();
        }
    }
    else {
      result.count = 0;
    }
  }
  else {
    result.count = 0;
  }
  return result;
}

void setup() {
  // put your setup code here, to run once:
  Modmata.begin();
  Wire.begin();
  lcd.begin(16, 2);
  Modmata.attach(100, wireWrite);
  Modmata.attach(101, wireRead);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Modmata.available()) {
    Modmata.processInput();
  }
}
