
/*  Getting_BPM_to_Monitor prints the BPM to the Serial Monitor, using the least lines of code and PulseSensor Library.
 *  Tutorial Webpage: https://pulsesensor.com/pages/getting-advanced
 *
--------Use This Sketch To------------------------------------------
1) Displays user's live and changing BPM, Beats Per Minute, in Arduino's native Serial Monitor.
2) Print: "♥  A HeartBeat Happened !" when a beat is detected, live.
2) Learn about using a PulseSensor Library "Object".
4) Blinks LED on PIN 13 with user's Heartbeat.
--------------------------------------------------------------------*/

#define USE_ARDUINO_INTERRUPTS false    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.  
 
#include <Adafruit_CircuitPlayground.h>
#define COLOR  0x4286f4
int Sensor;

//  Variables
const int PulseWire = A1;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = 13;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
                               // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                               // Otherwise leave the default "550" value.
/*
   samplesUntilReport = the number of samples remaining to read
   until we want to report a sample over the serial connection.

   We want to report a sample value over the serial port
   only once every 20 milliseconds (10 samples) to avoid
   doing Serial output faster than the Arduino can send.
*/
byte samplesUntilReport;
const byte SAMPLES_PER_SERIAL_SAMPLE = 10;                               

PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"


void setup() {   

  Serial.begin(115200);          // For Serial Monitor
  CircuitPlayground.begin(); 
  CircuitPlayground.setBrightness(255);
   
  // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);   
  
  
  // Skip the first SAMPLES_PER_SERIAL_SAMPLE in the loop().
  samplesUntilReport = SAMPLES_PER_SERIAL_SAMPLE;

  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }
}



void loop() {


      
 int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
                                               // "myBPM" hold this BPM value now. 
  Sensor = abs(CircuitPlayground.motionX())+abs(CircuitPlayground.motionY())+abs(CircuitPlayground.motionZ());
  if(Sensor > 19){
  // Turn ON the current pixel to COLOR
  for(int pixel=0; pixel<10; pixel++) {
    CircuitPlayground.setPixelColor(pixel, COLOR);
  }
   } 
  else {
    CircuitPlayground.clearPixels();
   }

 /*
     See if a sample is ready from the PulseSensor.

     If USE_INTERRUPTS is true, the PulseSensor Playground
     will automatically read and process samples from
     the PulseSensor.

     If USE_INTERRUPTS is false, this call to sawNewSample()
     will, if enough time has passed, read and process a
     sample (analog voltage) from the PulseSensor.
  */
  if (pulseSensor.sawNewSample()) {
    /*
       Every so often, send the latest Sample.
       We don't print every sample, because our baud rate
       won't support that much I/O.
    */
    if (--samplesUntilReport == (byte) 0) {
      samplesUntilReport = SAMPLES_PER_SERIAL_SAMPLE;

      pulseSensor.outputSample();
   
      
if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened". 
 Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
 Serial.print("BPM: ");                        // Print phrase "BPM: " 
 Serial.println(myBPM);                        // Print the value inside of myBPM. 
 CircuitPlayground.setPixelColor(0, 255,   0,   0);  // red
 CircuitPlayground.setPixelColor(2, 255,   0,   0);  // red
 CircuitPlayground.setPixelColor(4, 255,   0,   0);  // red
 CircuitPlayground.setPixelColor(5, 255,   0,   0);  // red
 CircuitPlayground.setPixelColor(7, 255,   0,   0);  // red
 CircuitPlayground.setPixelColor(9, 255,   0,   0);  // red 
}


  //delay(20);                    // considered best practice in a simple sketch.

}
  }
}

  
