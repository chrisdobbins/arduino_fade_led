#include <Arduino.h>
#include <math.h>

const byte RED_PIN = 3;
const int DELAY = 10000;
const byte GRN_PIN = 6;
const byte BLU_PIN = 5;
unsigned long time;

void blend(byte pin1, byte pin2);
float calculateLightValueOne(unsigned long time);
float calculateLightValueTwo(unsigned long time);

void setup() {
  Serial.begin(9600);
}

void loop() {
  // blend works by using reciprocal sine functions
  // the colors are blended as follows:
  // one is increased from 0, the other starts at 1 * 255 and decreases
  // when it is time to change colors, to avoid the appearance of a
  // muddy white color, the value that decreased from 255 to 0 is
  // replaced with the color to be added in.
  // the periodicity(?) is 10 s, so the delay should be multiples of 10s
  // start with red --> green

  blend(RED_PIN, GRN_PIN);
  // take away green, add blue
  blend(BLU_PIN, GRN_PIN);
  blend(BLU_PIN, RED_PIN);
  blend(GRN_PIN, RED_PIN);
  blend(GRN_PIN, BLU_PIN);
  blend(RED_PIN, BLU_PIN);
}

float calculateLightValueOne(unsigned long time) {
  float lightVal = (-.5 * sin((time/10000.0 * PI) - (PI * .5))) + .5;
  return lightVal;
}

float calculateLightValueTwo(unsigned long time) {
  float lightVal = (.5 * sin((time/10000.0 * PI) - (PI * .5))) + .5;
  return lightVal;
}

void blend(byte pin1, byte pin2) {
  unsigned long start;
  start = millis();
  unsigned long time = start;
  int pinSignal1;
  int pinSignal2;
  while (time < start + DELAY) {
    pinSignal1 = static_cast<int>(255 * calculateLightValueOne(time));
    analogWrite(pin1, pinSignal1);
    pinSignal2 = static_cast<int>(255 * calculateLightValueTwo(time));
    analogWrite(pin2, pinSignal2);
    time = millis();
  }
}
