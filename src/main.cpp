#include <Arduino.h>
#include <math.h>

int DELAY = 2000;

const byte RED_PIN = 9;
const byte GRN_PIN = 10;
const byte BLU_PIN = 11;

const byte INTERRUPT_PIN = 2;

unsigned long time;

volatile int counter = 0;
volatile bool RESET_PINS = false;

void blend(byte pin1, byte pin2);
float calculateLightValueOne(unsigned long time);
float calculateLightValueTwo(unsigned long time);

void flash();
void fade();
void resetLEDPins();
void interruptHandler();

void setup() {
    Serial.begin(9600);
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), interruptHandler, RISING);
}
void loop() {
   if (RESET_PINS){
       resetLEDPins();
       RESET_PINS = false;
   }

   if (counter%2 == 0) {
       fade();
   } else {
       flash();
   }
}

void resetLEDPins() {
  analogWrite(BLU_PIN, 0);
  analogWrite(RED_PIN, 0);
  analogWrite(GRN_PIN, 0);
}

void interruptHandler() {
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();

    if (interrupt_time - last_interrupt_time > DELAY) {
        counter++;
        RESET_PINS = true;
    }
    last_interrupt_time = interrupt_time;
}


void fade() {
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

void flash() {
    Serial.println("inside flash: ");
    Serial.println(DELAY);
    float colors[8][3] = {{1, 0, 0}, // RED
                         {1, .25, 0}, // ORANGE
                         {1, 1, 0}, // YELLOW
                         {0, 1, 0}, // GREEN
                         {0, 0, 1}, // BLUE
                         {.25, 0, 1}, // VIOLET
                         {0, 1, 1}, // CYAN
                         {1, 0, 1} // PINK
                         };
    unsigned long previousTime = 0;
    for (int i=0; i<8; i++) {
        unsigned long start = millis();
        unsigned long currentTime = start;
       do {
            analogWrite(RED_PIN, static_cast<byte>(255*colors[i][0]));
            analogWrite(GRN_PIN, static_cast<byte>(255*colors[i][1]));
            analogWrite(BLU_PIN, static_cast<byte>(255*colors[i][2]));
            currentTime = millis();

        } while((currentTime - start <= DELAY/2));
    }

}

float calculateLightValueOne(unsigned long time) {
  return (-.5 * sin((time/(DELAY * 1.0) * PI) - (PI * .5))) + .5;
}

float calculateLightValueTwo(unsigned long time) {
  return (.5 * sin((time/(DELAY * 1.0) * PI) - (PI * .5))) + .5;
}

  // blend works by using reciprocal sine functions
  // the colors are blended as follows:
  // one is increased from 0, the other starts at 1 * 255 and decreases
  // when it is time to change colors, to avoid the appearance of a
  // muddy white color, the value that decreased from 255 to 0 is
  // replaced with the color to be added in.

  // changes are done with a series of 6 steps to allow
  // the colors to 'wrap around'. there are 3 combinations
  // but every run of blend() alternates which values are
  // high (~255) or low (~0). to ensure that the alternating
  // end states of the params don't interfere with the color
  // transitions, 6 transitions are done to get back to
  // the starting color (red)

void blend(byte pin1, byte pin2) {
    Serial.println("inside blend: ");
    Serial.println(DELAY);
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
