#include <Wire.h>
#include <Keypad.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// =====================================================
// OLED
// =====================================================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

// =====================================================
// BUZZER
// =====================================================
#define BUZZER_PIN 27

// =====================================================
// KEYPAD
// =====================================================
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {
  13, 14, 16, 17
};

byte colPins[COLS] = {
  18, 19, 25, 26
};

Keypad keypad = Keypad(
  makeKeymap(keys),
  rowPins,
  colPins,
  ROWS,
  COLS
);

// =====================================================
// PASSWORD VARIABLES
// =====================================================

// Default password when ESP32 starts
String savedCode = "1234";

// Stores what the user is currently typing
String enteredCode = "";

// New password while changing code
String newCode = "";

// =====================================================
// SYSTEM STATES
// =====================================================
enum SystemState {
  ENTER_CODE,
  VERIFY_OLD_CODE,
  ENTER_NEW_CODE,
  CONFIRM_NEW_CODE
};

SystemState currentState = ENTER_CODE;

// =====================================================
// BUZZER FUNCTIONS
// =====================================================

void beepShort() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(80);
  digitalWrite(BUZZER_PIN, LOW);
}

void beepSuccess() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(120);

  digitalWrite(BUZZER_PIN, LOW);
  delay(80);

  digitalWrite(BUZZER_PIN, HIGH);
  delay(120);

  digitalWrite(BUZZER_PIN, LOW);
}

void beepError() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(500);
  digitalWrite(BUZZER_PIN, LOW);
}

// =====================================================
// DISPLAY FUNCTIONS
// =====================================================

void clearOLED() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
}

// Display stars instead of the actual password
void showStars(String title, int amount) {

  clearOLED();

  display.setCursor(0, 5);
  display.println(title);

  display.setTextSize(2);
  display.setCursor(0, 23);

  for (int i = 0; i < amount; i++) {
    display.print("*");
  }

  display.display();
}

// =====================================================
// MAIN ENTER-CODE SCREEN
// =====================================================

void showEnterScreen() {

  clearOLED();

  display.setTextSize(1);

  display.setCursor(0, 5);
  display.println("ENTER CODE:");

  display.setCursor(0, 25);

  for (int i = 0; i < enteredCode.length(); i++) {
    display.print("*");
  }

  display.setCursor(0, 45);
  display.println("#=ENTER  A=CHANGE");

  display.display();
}

// =====================================================
// ACCESS GRANTED
// =====================================================

void accessGranted() {

  clearOLED();

  display.setTextSize(2);
  display.setCursor(20, 15);
  display.println("ACCESS");

  display.setCursor(18, 38);
  display.println("GRANTED");

  display.display();

  beepSuccess();

  delay(2000);

  enteredCode = "";

  showEnterScreen();
}

// =====================================================
// ACCESS DENIED
// =====================================================

void accessDenied() {

  clearOLED();

  display.setTextSize(2);
  display.setCursor(20, 15);
  display.println("ACCESS");

  display.setCursor(25, 38);
  display.println("DENIED");

  display.display();

  beepError();

  delay(2000);

  enteredCode = "";

  showEnterScreen();
}

// =====================================================
// ASK FOR CURRENT PASSWORD
// =====================================================

void showOldPasswordScreen() {

  clearOLED();

  display.setTextSize(1);

  display.setCursor(0, 5);
  display.println("CHANGE CODE");

  display.setCursor(0, 20);
  display.println("CURRENT CODE:");

  display.setCursor(0, 35);

  for (int i = 0; i < enteredCode.length(); i++) {
    display.print("*");
  }

  display.setCursor(0, 53);
  display.println("#=ENTER  *=CANCEL");

  display.display();
}

// =====================================================
// ASK FOR NEW PASSWORD
// =====================================================

void showNewCodeScreen() {

  clearOLED();

  display.setTextSize(1);

  display.setCursor(0, 5);
  display.println("CREATE NEW CODE:");

  display.setTextSize(2);
  display.setCursor(0, 25);

  for (int i = 0; i < enteredCode.length(); i++) {
    display.print("*");
  }

  display.setTextSize(1);
  display.setCursor(0, 53);
  display.println("#=SAVE  *=CANCEL");

  display.display();
}

// =====================================================
// CONFIRM NEW PASSWORD
// =====================================================

void showConfirmCodeScreen() {

  clearOLED();

  display.setTextSize(1);

  display.setCursor(0, 5);
  display.println("CONFIRM NEW CODE:");

  display.setTextSize(2);
  display.setCursor(0, 25);

  for (int i = 0; i < enteredCode.length(); i++) {
    display.print("*");
  }

  display.setTextSize(1);
  display.setCursor(0, 53);
  display.println("#=ENTER  *=CANCEL");

  display.display();
}

// =====================================================
// PASSWORD CHANGED MESSAGE
// =====================================================

void codeChangedMessage() {

  clearOLED();

  display.setTextSize(1);
  display.setCursor(15, 15);
  display.println("NEW CODE");

  display.setTextSize(2);
  display.setCursor(15, 32);
  display.println("SAVED!");

  display.display();

  beepSuccess();

  delay(2000);

  enteredCode = "";

  currentState = ENTER_CODE;

  showEnterScreen();
}

// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  // Buzzer
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  // OLED
  Wire.begin(21, 22);

  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        SCREEN_ADDRESS
      )) {

    Serial.println(
      "OLED initialization failed"
    );

    while (true);
  }

  display.clearDisplay();
  display.display();

  // Startup message
  clearOLED();

  display.setTextSize(2);
  display.setCursor(15, 15);
  display.println("SECURITY");

  display.setTextSize(1);
  display.setCursor(25, 45);
  display.println("SYSTEM READY");

  display.display();

  beepSuccess();

  delay(1500);

  showEnterScreen();
}

// =====================================================
// LOOP
// =====================================================

void loop() {

  char key = keypad.getKey();

  // Nothing pressed
  if (!key) {
    return;
  }

  Serial.print("Key pressed: ");
  Serial.println(key);

  // Small beep every time a key is pressed
  beepShort();

  // ===================================================
  // NORMAL PASSWORD ENTRY
  // ===================================================

  if (currentState == ENTER_CODE) {

    // A = change password
    if (key == 'A') {

      enteredCode = "";

      currentState = VERIFY_OLD_CODE;

      showOldPasswordScreen();

      return;
    }

    // * = clear current entry
    if (key == '*') {

      enteredCode = "";

      showEnterScreen();

      return;
    }

    // # = submit password
    if (key == '#') {

      if (enteredCode.length() == 0) {
        return;
      }

      if (enteredCode == savedCode) {

        accessGranted();

      } else {

        accessDenied();
      }

      return;
    }

    // Only accept numbers
    if (key >= '0' && key <= '9') {

      // Maximum password length = 8
      if (enteredCode.length() < 8) {

        enteredCode += key;

        showEnterScreen();
      }
    }
  }

  // ===================================================
  // VERIFY CURRENT CODE BEFORE CHANGING
  // ===================================================

  else if (currentState == VERIFY_OLD_CODE) {

    // Cancel
    if (key == '*') {

      enteredCode = "";

      currentState = ENTER_CODE;

      showEnterScreen();

      return;
    }

    // Submit old password
    if (key == '#') {

      if (enteredCode == savedCode) {

        enteredCode = "";

        currentState = ENTER_NEW_CODE;

        showNewCodeScreen();

      } else {

        clearOLED();

        display.setTextSize(1);
        display.setCursor(10, 20);
        display.println("WRONG CURRENT CODE");

        display.display();

        beepError();

        delay(1500);

        enteredCode = "";

        showOldPasswordScreen();
      }

      return;
    }

    // Numbers only
    if (key >= '0' && key <= '9') {

      if (enteredCode.length() < 8) {

        enteredCode += key;

        showOldPasswordScreen();
      }
    }
  }

  // ===================================================
  // ENTER NEW CODE
  // ===================================================

  else if (currentState == ENTER_NEW_CODE) {

    // Cancel
    if (key == '*') {

      enteredCode = "";
      newCode = "";

      currentState = ENTER_CODE;

      showEnterScreen();

      return;
    }

    // Save temporary new code
    if (key == '#') {

      // Require at least 4 digits
      if (enteredCode.length() < 4) {

        clearOLED();

        display.setTextSize(1);
        display.setCursor(5, 20);
        display.println("USE AT LEAST");

        display.setCursor(20, 35);
        display.println("4 DIGITS");

        display.display();

        beepError();

        delay(1500);

        enteredCode = "";

        showNewCodeScreen();

        return;
      }

      newCode = enteredCode;

      enteredCode = "";

      currentState = CONFIRM_NEW_CODE;

      showConfirmCodeScreen();

      return;
    }

    // Numbers only
    if (key >= '0' && key <= '9') {

      if (enteredCode.length() < 8) {

        enteredCode += key;

        showNewCodeScreen();
      }
    }
  }

  // ===================================================
  // CONFIRM NEW CODE
  // ===================================================

  else if (currentState == CONFIRM_NEW_CODE) {

    // Cancel
    if (key == '*') {

      enteredCode = "";
      newCode = "";

      currentState = ENTER_CODE;

      showEnterScreen();

      return;
    }

    // Confirm
    if (key == '#') {

      if (enteredCode == newCode) {

        savedCode = newCode;

        enteredCode = "";
        newCode = "";

        codeChangedMessage();

      } else {

        clearOLED();

        display.setTextSize(1);
        display.setCursor(10, 15);
        display.println("CODES DO NOT");

        display.setCursor(30, 30);
        display.println("MATCH");

        display.display();

        beepError();

        delay(1500);

        enteredCode = "";
        newCode = "";

        currentState = ENTER_NEW_CODE;

        showNewCodeScreen();
      }

      return;
    }

    // Numbers only
    if (key >= '0' && key <= '9') {

      if (enteredCode.length() < 8) {

        enteredCode += key;

        showConfirmCodeScreen();
      }
    }
  }
}
