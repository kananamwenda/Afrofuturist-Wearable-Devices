#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NEOPIXEL_PIN 13
#define NUM_PIXELS 1
Adafruit_NeoPixel strip(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

#define VIBRATION_MOTOR_PIN 14

bool AButton;
bool BButton;
bool CButton;
bool slideSwitch;

int AButtonPushCounter = 0;
int AButtonState = 0;
int ALastButtonState = 0;

int BButtonPushCounter = 0;
int BButtonState = 0;
int BLastButtonState = 0;

int CButtonPushCounter = 0;
int CButtonState = 0;
int CLastButtonState = 0;

int ledBrightness = 0;
int fadeAmount = 3;

void setup() {
  pinMode(VIBRATION_MOTOR_PIN, OUTPUT);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {
  slideSwitch = digitalRead(15); // Assuming slide switch is connected to pin 15

  AButtonState = digitalRead(27); // Assuming button A is connected to pin 27
  BButtonState = digitalRead(33); // Assuming button B is connected to pin 33
  CButtonState = digitalRead(32); // Assuming button C is connected to pin 32

  if (slideSwitch == LOW) {
    // Everything off
    display.clearDisplay();
    display.display();
    digitalWrite(VIBRATION_MOTOR_PIN, LOW);
    strip.setBrightness(0);
    strip.show();
  } else {
    digitalWrite(VIBRATION_MOTOR_PIN, HIGH);

    // Display on the OLED
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0,0);
    display.println("Vibration Motor On");
    display.display();

    // NeoPixel LED animation
    for (int i = 0; i < 255; i += 5) {
      strip.setBrightness(i);
      strip.show();
      delay(20);
    }
    for (int i = 255; i >= 0; i -= 5) {
      strip.setBrightness(i);
      strip.show();
      delay(20);
    }
  }

  // A Button State Change Detection
  if (AButtonState != ALastButtonState) {
    if (AButtonState == HIGH) {
      AButtonPushCounter++;
      Serial.println("A button pressed");
      Serial.print("Number of button pushes: ");
      Serial.println(AButtonPushCounter);
    }
    delay(50);
  }
  ALastButtonState = AButtonState;

  // B Button State Change Detection
  if (BButtonState != BLastButtonState) {
    if (BButtonState == HIGH) {
      BButtonPushCounter++;
      Serial.println("B button pressed");
      Serial.print("Number of button pushes: ");
      Serial.println(BButtonPushCounter);
    }
    delay(50);
  }
  BLastButtonState = BButtonState;

  // C Button State Change Detection
  if (CButtonState != CLastButtonState) {
    if (CButtonState == HIGH) {
      CButtonPushCounter++;
      Serial.println("C button pressed");
      Serial.print("Number of button pushes: ");
      Serial.println(CButtonPushCounter);
    }
    delay(50);
  }
  CLastButtonState = CButtonState;

  // A Button LED Dimming
  if (AButtonPushCounter % 2 == 0) {
    ledBrightness = 0;
  } else {
    ledBrightness += fadeAmount;
    if (ledBrightness <= 0 || ledBrightness >= 255) {
      fadeAmount = -fadeAmount;
    }
    analogWrite(NEOPIXEL_PIN, ledBrightness);
    delay(30);
  }
}
