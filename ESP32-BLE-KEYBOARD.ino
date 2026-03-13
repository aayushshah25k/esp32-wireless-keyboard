#include <BleKeyboard.h>

// BLE Keyboard: Name = "ESP32 Keyboard", Manufacturer = "wircuitry"
BleKeyboard bleKeyboard("ESP32 Keyboard", "wircuitry", 100);

// Button pins
const int buttonPins[6] = {4, 5, 18, 19, 21, 22};
bool lastState[6];

void setup() {
  Serial.begin(115200);

  // Initialize buttons
  for (int i = 0; i < 6; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    lastState[i] = HIGH;
  }

  bleKeyboard.begin();
  delay(5000); // wait before advertising for stable connection
}

void loop() {
  if (bleKeyboard.isConnected()) {
    for (int i = 0; i < 6; i++) {
      bool currentState = digitalRead(buttonPins[i]);

      if (currentState == LOW && lastState[i] == HIGH) {
        switch (i) {
          case 0: bleKeyboard.print("A"); break;
          case 1: bleKeyboard.print("B"); break;
          case 2: bleKeyboard.print("C"); break;

          // Task-oriented buttons
          case 3: shutdownLaptop(); break;         // Button 4
          case 4: openChrome(); break;            // Button 5
          case 5: openYouTubeSearch(); break;     // Button 6
        }
        delay(300); // debounce
      }
      lastState[i] = currentState;
    }
  }
}

// ------------------- Task Functions -------------------

void shutdownLaptop() {
  // Win + X → U → U (shutdown)
  bleKeyboard.press(KEY_LEFT_GUI); // Win
  bleKeyboard.press('x');
  bleKeyboard.releaseAll();
  delay(500);

  bleKeyboard.press('u'); 
  bleKeyboard.releaseAll();
  delay(500);

  bleKeyboard.press('u'); 
  bleKeyboard.releaseAll();
  delay(500); // extra wait
}

void openChrome() {
  bleKeyboard.press(KEY_LEFT_GUI);
  bleKeyboard.press('r');
  bleKeyboard.releaseAll();
  delay(500);

  bleKeyboard.print("chrome");
  bleKeyboard.write(KEY_RETURN);
}

void openYouTubeSearch() {
  // Step 1: Open Chrome
  openChrome();

  // Step 2: Wait 3 seconds for Chrome to fully load
  delay(3000);

  // Step 3: Make sure no keys are pressed
  bleKeyboard.releaseAll();
  delay(100);

  // Step 4: Focus address bar (Ctrl + L)
  bleKeyboard.press(KEY_LEFT_CTRL);
  bleKeyboard.press('l');
  bleKeyboard.releaseAll();
  delay(100);

  // Step 5: Type YouTube search URL and press Enter automatically
  bleKeyboard.print("https://www.youtube.com/results?search_query=OpenAI");
  bleKeyboard.write(KEY_RETURN);
}