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

Keypad keypad = Keypad(makeKeymap(KEYS), ROW_PINS, COL_PINS, ROWS, COLS);

const char safePassword[] = { '1', '2', '3', '4', '\0'};

const uint8_t MAX_PIN_LENGTH = 5;
char enteredPins[MAX_PIN_LENGTH] = {'\0'};
uint8_t pinIndex = 0;

enum SAFE_STATE : uint8_t {
  LOCKED,
  UNLOCKED
};

SAFE_STATE current_state = LOCKED;

void displayPins();
void deleteLastPin();
void clearEnteredPins();

void setup() {
  Serial.begin(9600);
  Serial.println("Initiating Program...");
}

void loop() {
  char key = keypad.getKey();
  if (key != NO_KEY) {
    if (current_state == LOCKED) {
      if (key >= '0' && key <= '9') {
        if (pinIndex < MAX_PIN_LENGTH - 1) {
          enteredPins[pinIndex] = key;
          enteredPins[pinIndex + 1] = '\0';
          pinIndex++;
          displayPins();
        }
      } else if (key == 'B') {
        clearEnteredPins();
      } else if (key == 'A') {
        if (pinIndex > 0) {
          deleteLastPin();
          displayPins();
        }
      } else if (key == 'D') {
        Serial.println("----- CHECKING PASSWORD -----");
        if (strcmp(enteredPins, safePassword) == 0) {
          Serial.println("ACCESS GRANTED!");
          current_state = UNLOCKED;
        } else {
          Serial.println("ACCESS DENIED!");
        }
        clearEnteredPins();
      }
    } else if (current_state == UNLOCKED) {
      if (key == 'C')  {
        Serial.println("CLOSING SAFE!");
        current_state = LOCKED;
      }
    }
  }

  delay(30);
}

void displayPins() {
  Serial.print("Entered pin: ");
  Serial.println(enteredPins);
}

void deleteLastPin() {
  enteredPins[pinIndex - 1] = '\0';
  pinIndex--;
}

void clearEnteredPins() {
  pinIndex = 0;
  enteredPins[0] = '\0';
}
