#include <Arduino.h>
#include <math.h>

const byte RED_PIN = 3;
const int DELAY = 10000;
const byte GRN_PIN = 6;
const byte BLU_PIN = 5;
unsigned long time;

void blend(byte pin1, byte pin2);

void setup() {
  Serial.begin(9600);
  // blend(RED_PIN, GRN_PIN);
  // // take away green, add blue
  // blend(BLU_PIN, GRN_PIN);
  // // take away blue, add red
  // blend(BLU_PIN, RED_PIN);
  // delay(10000);
  // blend(RED_PIN, GRN_PIN);
}

float calculateDecreasingLightValue(unsigned long time) {
  float lightVal = (.5 * sin((time/10000.0 * PI) - (PI * .5))) + .5;
  return lightVal;
}

float calculateIncreasingLightValue(unsigned long time) {
  float lightVal = (-.5 * sin((time/10000.0 * PI) - (PI * .5))) + .5;
  return lightVal;
}


int blueVal = 0;
int redVal = 0;
int greenVal = 0;

void blend(byte pin1, byte pin2) {
  unsigned long start;
  start = millis();
  unsigned long time = start;
  int pinSignal1;
  int pinSignal2;
  while (time < start + DELAY) {
    // delay(20);
    pinSignal1 = static_cast<int>(255 * calculateIncreasingLightValue(time));
    analogWrite(pin1, pinSignal1);
    pinSignal2 = static_cast<int>(255 * calculateDecreasingLightValue(time));
    analogWrite(pin2, pinSignal2);
    time = millis();
  }
  if (time % 10000 == 0) {
  Serial.println(time);
  Serial.println(pinSignal1);
  Serial.println(pinSignal2);
}
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

  // // take away blue, add red
  // blend(BLU_PIN, RED_PIN);
  // // // take away blue, add red
  // blend(BLU_PIN, RED_PIN);
  // take away red, add green



  // blend(BLU_PIN, RED_PIN);
    // unsigned long start;
    // start = millis();
    // int time = start;

    // blend(GRN_PIN, BLU_PIN);
    // delay(5000);

      // delay(1000);
    //   while (time < start + 30000) {
    //     delay(20);
    //   pinSignal = static_cast<int>(255 * calculateIncreasingLightValue(millis()));
    //   analogWrite(RED_PIN, pinSignal);
    //   redVal = pinSignal;
    //   int pinSignalTwo = static_cast<int>(255 * calculateDecreasingLightValue(millis()));
    //     analogWrite(BLU_PIN, pinSignalTwo);
    //     blueVal = pinSignalTwo;
    //   if (pinSignal > 125 && pinSignal < 129) {
    //     Serial.println(time);
    //     Serial.println(pinSignal);
    //   }
    //   time = millis();
    // }
}
