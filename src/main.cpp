#include <Arduino.h>
#include <Keypad.h>

const uint8_t ROWS = 4;
const uint8_t COLS = 4;
uint8_t ROW_PINS[ROWS] = { 11, 10, 9, 8 };
uint8_t COL_PINS[COLS] = { 7, 6, 5, 4 };

const char KEYS[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

const char safePassword[] = "1234";

const uint8_t MAX_PIN_LENGTH = 5;
char enteredPin[MAX_PIN_LENGTH] = {'\0'};
uint8_t pinIndex = 0;

Keypad keypad = Keypad(makeKeymap(KEYS), ROW_PINS, COL_PINS, ROWS, COLS);

void displayPins();

void setup() {
  Serial.begin(9600);
  Serial.println("Initiating Program...");
}

void loop() {
  char key = keypad.getKey();

  if (key != NO_KEY) {
    Serial.print("Key entered: ");
    Serial.println(key);

    if (key >= '0' && key <= '9') {
      if (pinIndex < MAX_PIN_LENGTH - 1) {
        enteredPin[pinIndex] = key;
        enteredPin[pinIndex + 1] = '\0';
        pinIndex++;
      }

      displayPins();
    }
  }

  delay(30);
}

void displayPins() {
  Serial.print("Entered pin: ");
  Serial.println(enteredPin);
}
