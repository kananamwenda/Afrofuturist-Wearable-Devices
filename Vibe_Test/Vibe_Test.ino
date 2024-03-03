#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define MOTOR_PIN 26
#define BUTTON_A 33
#define BUTTON_B 32
#define BUTTON_C 25

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {
  if (digitalRead(BUTTON_A) == LOW) {
    vibratePattern(1);
    displayMessage("Button A pressed");
  }

  if (digitalRead(BUTTON_B) == LOW) {
    vibratePattern(2);
    displayMessage("Button B pressed");
  }

  if (digitalRead(BUTTON_C) == LOW) {
    vibratePattern(3);
    displayMessage("Button C pressed");
  }
}

void vibratePattern(int pattern) {
  switch(pattern) {
    case 1:
      // Implement your first vibration pattern
      break;
    case 2:
      // Implement your second vibration pattern
      break;
    case 3:
      // Implement your third vibration pattern
      break;
    default:
      // Default pattern or error handling
      break;
  }
}

void displayMessage(String message) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(message);
  display.display();
  delay(2000); // Display the message for 2 seconds
}
