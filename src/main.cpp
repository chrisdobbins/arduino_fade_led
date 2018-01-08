#include <Wire.h>
#include <PCA9685.h>
#include <Arduino.h>
#include <math.h>
// PCA9685 with default settings
PCA9685 driver = PCA9685(0x40, PCA9685_MODE_N_DRIVER, PCA9685_MAX_FREQUENCY);
int DELAY = 20000;
/*
const byte RED_PIN = 9;
const byte GRN_PIN = 10;
const byte BLU_PIN = 11;
*/
unsigned long time;

void fade();
void blend(byte pin1, byte pin2);
float calculateLightValueOne(unsigned long time);
float calculateLightValueTwo(unsigned long time);

void setup()
{
  // initialize serial for debug output
  Serial.begin(9600);

  // initialize TwoWire communication
  Wire.begin();

  // setup the PCA9685 
  driver.setup();
}

void loop()
{
  // setting up pin to drive common + and GND rails for 
  // transistors, 100 ohm resistor and capacitor
  driver.getPin(PCA9685_LED15).fullOnAndWrite();

  fade();
/*
  // go through all possible 512 of 4096 possible pwm steps in the first LED
  Serial.println("FADE IN LED0");
  int i;
  for (i = 0; i < PCA9685_MAX_VALUE; i = i + 8)
  {
    // set the pwm value of the first led
    driver.getPin(PCA9685_LED0).setValueAndWrite(i);
    delay(5);
  }
  Serial.println("FADE OUT LED0");
  for (i = PCA9685_MAX_VALUE; i >= 0; i = i - 8)
  {
    // set the pwm value of the first led
    driver.getPin(PCA9685_LED0).setValueAndWrite(i);
    delay(5);
  }*/
}

void fade() {
    const int RED_PIN = PCA9685_LED0;
    const int GRN_PIN = PCA9685_LED1;
    const int BLU_PIN = PCA9685_LED2;

      // start with red --> green
    blend(RED_PIN, GRN_PIN);
    // add blue, take away green
    blend(BLU_PIN, GRN_PIN);
    // add red, take away blue
    blend(BLU_PIN, RED_PIN);
    // add green, take away red
    blend(GRN_PIN, RED_PIN);
    // add blue, take away green
    blend(GRN_PIN, BLU_PIN);
    // add red, take away blue
    blend(RED_PIN, BLU_PIN);
}

void blend(byte pin1, byte pin2) {
     unsigned long start = millis();
     unsigned long time = start;

     while (time < start + DELAY) {
          int pinSignal1 = static_cast<int>(4095 * calculateLightValueOne(time));
          driver.getPin(pin1).setValueAndWrite(pinSignal1);
          int pinSignal2 = static_cast<int>(4095 * calculateLightValueTwo(time));
          driver.getPin(pin2).setValueAndWrite(pinSignal2);
          time = millis();
     }
}

float calculateLightValueOne(unsigned long time) {
      return (-.5 * sin((time/(DELAY * 1.0) * PI) - (PI * .5))) + .5;
}

float calculateLightValueTwo(unsigned long time) {
      return (.5 * sin((time/(DELAY * 1.0) * PI) - (PI * .5))) + .5;
}

