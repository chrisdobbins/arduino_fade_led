#include <Arduino.h>
#include <math.h>

const int DELAY = 1000;
const byte RED_PIN = 9;
const byte GRN_PIN = 6;
const byte BLU_PIN = 5;
unsigned long time;

void blend(byte pin1, byte pin2);
float calculateLightValueOne(unsigned long time);
float calculateLightValueTwo(unsigned long time);

void setup() {
  // Serial.begin(9600);
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

  // NB: this is done with a series of 6 steps to allow
  // the colors to 'wrap around'. there are 3 combinations
  // but every run of blend() alternates which values are
  // high (~255) or low (~0). to ensure that the alternating
  // end states of the params don't interfere with the color
  // transitions, 6 transitions are done to get back to
  // the starting color (red)
}

float calculateLightValueOne(unsigned long time) {
  return (-.5 * sin((time/(DELAY * 1.0) * PI) - (PI * .5))) + .5;
}

float calculateLightValueTwo(unsigned long time) {
  return (.5 * sin((time/(DELAY * 1.0) * PI) - (PI * .5))) + .5;
}

void blend(byte pin1, byte pin2) {
  unsigned long start = millis();
  unsigned long time = start;

  while (time < start + DELAY) {
    byte pinSignal1 = static_cast<byte>(255 * calculateLightValueOne(time));
    analogWrite(pin1, pinSignal1);
    byte pinSignal2 = static_cast<byte>(255 * calculateLightValueTwo(time));
    analogWrite(pin2, pinSignal2);
    time = millis();
  }
}
