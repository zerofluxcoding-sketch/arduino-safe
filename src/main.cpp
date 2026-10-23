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

SAFE_STATE currentState = LOCKED;

const char KEY_CLEAR = 'A';
const char KEY_DELETE = 'B';
const char KEY_CLOSE = 'C';
const char KEY_ENTER = 'D';

void handleLockedInput(char key);
void handlePasswordChecking();
void handleUnlockedInput(char key);
void printPinBuffer();
void storeDigit(char value);
void backspacePin();
void resetPinBuffer();

void setup() {
  Serial.begin(9600);
  Serial.println("Initiating Program...");
}

void loop() {
  char key = keypad.getKey();
  if (key != NO_KEY) {
    if (currentState == LOCKED) {
      handleLockedInput(key);
    } else if (currentState == UNLOCKED) {
      handleUnlockedInput(key);
    }
    printPinBuffer();
  }

  delay(30);
}

void handleLockedInput(char key) {
  if (key >= '0' && key <= '9') {
    if (pinIndex < MAX_PIN_LENGTH - 1) storeDigit(key);
  } else {
    switch (key) {
    case KEY_CLEAR:
      resetPinBuffer();
      break;

    case KEY_DELETE:
      if (pinIndex > 0) backspacePin();
      break;

    case KEY_ENTER:
      handlePasswordChecking();
      break;

    default:
      break;
    }
  }
}

void handlePasswordChecking() {
  Serial.println("----- CHECKING PASSWORD -----");
  if (strcmp(enteredPins, safePassword) == 0) {
    Serial.println("ACCESS GRANTED!");
    currentState = UNLOCKED;
  } else {
    Serial.println("ACCESS DENIED!");
  }
  resetPinBuffer();
}

void handleUnlockedInput(char key) {
  if (key == KEY_CLOSE)  {
    Serial.println("CLOSING SAFE!");
    currentState = LOCKED;
  }
}

void printPinBuffer() {
  Serial.print("Entered pin: ");
  Serial.println(enteredPins);
}

void storeDigit(char value) {
  enteredPins[pinIndex] = value;
  enteredPins[pinIndex + 1] = '\0';
  pinIndex++;
}

void backspacePin() {
  enteredPins[pinIndex - 1] = '\0';
  pinIndex--;
}

void resetPinBuffer() {
  pinIndex = 0;
  enteredPins[0] = '\0';
}
