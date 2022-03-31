/*
  State change detection (edge detection)

  Often, you don't need to know the state of a digital input all the time, but
  you just need to know when the input changes from one state to another.
  For example, you want to know when a button goes from OFF to ON. This is called
  state change detection, or edge detection.

  This example shows how to detect when a button or button changes from off to on
  and on to off.

  The circuit:
  - pushbutton attached to pin 2 from +5V
  - 10 kilohm resistor attached to pin 2 from ground
  - LED attached from pin 13 to ground (or use the built-in LED on most
    Arduino boards)

  created  27 Sep 2005
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/ButtonStateChange
*/
#include <Adafruit_CircuitPlayground.h>
#include <Wire.h>
#include "Adafruit_DRV2605.h"

Adafruit_DRV2605 drv;

#define COLOR         0xFF00ff    // change this to your favorite color
#define RATE          150         // lower is faster

// this constant won't change:
bool leftButton;
bool rightButton;
bool slideSwitch;

// Variables will change:
int BWbuttonPushCounter = 0;   // counter for the number of button presses
int BWbuttonState = 0;         // current state of the button
int BWlastButtonState = 0;     // previous state of the button

int EMDRbuttonPushCounter = 0;   // counter for the number of button presses
int EMDRbuttonState = 0;         // current state of the button
int EMDRlastButtonState = 0;     // previous state of the button

int led = 13;           // the PWM pin the LED is attached to
int brightness = 0;    // how bright the LED is
int fadeAmount = 3;    // how many points to fade the LED by

void setup() {
  // initialize serial communication:
  CircuitPlayground.begin();
  CircuitPlayground.setBrightness(100);
  pinMode(led, OUTPUT);

  drv.begin();

  // I2C trigger by sending 'go' command
  drv.setMode(DRV2605_MODE_INTTRIG); // default, internal trigger when sending GO command

  drv.selectLibrary(1);

}

void loop() {
  // read the pushbutton input pin:
  slideSwitch = CircuitPlayground.slideSwitch();
  BWbuttonState =  CircuitPlayground.leftButton();
  EMDRbuttonState =  CircuitPlayground.rightButton();

  // turns on the LED every four button pushes by checking the modulo of the
  // button push counter. the modulo function gives you the remainder of the
  // division of two numbers:
  if (!slideSwitch) {
    //Everything off
    CircuitPlayground.clearPixels();
  } else {
    drv.setWaveform(0, 82);  // Transition Ramp Up Long Smooth 1 – 0 to 100%
    drv.setWaveform(1, 70);  // Transition Ramp Down Long Smooth 1 – 100 to 0%
    drv.setWaveform(2, 0);  // end of waveforms
    drv.go();
    delay(1000);
    // Turn ON all the NeoPixels
    for (int p = 0; p < 10; p++) {
      CircuitPlayground.setPixelColor(p, 0, 0, 255);
    }
    // Compute DT
    unsigned long DT = 5000 / 10;
    // Turn OFF the NeoPixels one at a time, waiting DT each time
    for (int p = 0; p < 10; p++) {
      delay(DT);
      CircuitPlayground.setPixelColor(p, 0, 0, 0);
    }
  }


  // compare the buttonState to its previous state
  if (EMDRbuttonState != EMDRlastButtonState) {
    // if the state has changed, increment the counter
    if (EMDRbuttonState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      EMDRbuttonPushCounter++;
      Serial.println("on");
      Serial.print("number of button pushes: ");
      Serial.println(EMDRbuttonPushCounter);
    } else {
      // if the current state is LOW then the button went from on to off:
      Serial.println("off");
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  EMDRlastButtonState = EMDRbuttonState;


  if (EMDRbuttonPushCounter % 2 == 0) {
    //Everything off
    CircuitPlayground.clearPixels();
  } else {
    // The animation loop
    for (int pixel = 0; pixel < 10; pixel++) {
      // Turn OFF all pixels
      CircuitPlayground.clearPixels();

      // Turn ON the current pixel to COLOR
      CircuitPlayground.setPixelColor(pixel, COLOR);

      // Don't spin too fast
      delay(RATE);
    }
    drv.setWaveform(0, 85);  // ramp up medium 1, see datasheet part 11.2
    drv.setWaveform(1, 1);  // strong click 100%, see datasheet part 11.2
    drv.setWaveform(2, 0);  // end of waveforms
    drv.go();
    delay(RATE);
  }

  // compare the buttonState to its previous state
  if (BWbuttonState != BWlastButtonState) {
    // if the state has changed, increment the counter
    if (BWbuttonState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      BWbuttonPushCounter++;
      Serial.println("on");
      Serial.print("number of button pushes: ");
      Serial.println(BWbuttonPushCounter);
    } else {
      // if the current state is LOW then the button went from on to off:
      Serial.println("off");
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state, for next time through the loop
  BWlastButtonState = BWbuttonState;

  if (BWbuttonPushCounter % 2 == 0) {
    pinMode(led, LOW);
  } else {
    // set the brightness of pin 13:
    analogWrite(led, brightness);
    // change the brightness for next time through the loop:
    brightness = brightness + fadeAmount;

    // reverse the direction of the fading at the ends of the fade:
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;
    }
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }
}
