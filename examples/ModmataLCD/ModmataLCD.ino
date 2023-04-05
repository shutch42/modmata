#include <Modmata.h>
#include <LiquidCrystal.h>

// Define codes to associate with each function (20-100)
#define LCD_SETCURSOR 20
#define LCD_PRINT 21
#define LCD_CLEAR 22

// Initialize LCD
const int rs = A4, en = A5, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Modmata function to move the cursor on the LCD
// Accepts row and column for argv[0] and argv[1]
// Returns no registers
struct registers lcdSetCursor(uint8_t argc, uint8_t* argv) {
  lcd.setCursor(argv[0], argv[1]);

  struct registers result;
  result.count = 0;
  return result;
}

// Modmata function to print text to the LCD
// Accepts an array of characters as argv
// Returns no registers
struct registers lcdPrint(uint8_t argc, uint8_t* argv) {
  for(int i = 0; i < argc; i++) {
      lcd.print((char)argv[i]);    
  }

  struct registers result;
  result.count = 0;
  return result;
}

// Modmata function to clear the lcd
// Accepts nothing in argv
// Returns no registers
struct registers lcdClear(uint8_t argc, uint8_t* argv) {
  lcd.clear();

  struct registers result;
  result.count = 0;
  return result;
}

void setup() {

  // Set LCD size
  lcd.begin(16, 2);

  // Start modmata at 9600 baud
  Modmata.begin(115200);

  // Include custom functions in modmata communication
  Modmata.attach(LCD_CLEAR, lcdClear);
  Modmata.attach(LCD_SETCURSOR, lcdSetCursor);
  Modmata.attach(LCD_PRINT, lcdPrint);
  

}

void loop() {
  // Check if a command was received
  // If one is available, process it
  if(Modmata.available()) {
    Modmata.processInput();
  }
}
