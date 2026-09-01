
#define PIN_MODE 2
#define PIN_DONE 3
#define PIN_SIG_Y 4
#define PIN_SIG_B 5
#define PIN_SIG_MISS 6
#define PIN_ACK_MISS 7

#define PIN_SCL 13
#define PIN_SDA 11
#define PIN_RST 9
#define PIN_DC  8
#define PIN_CS  10

#define PIN_RANDOM A0
#define PIN_DATA  A1
#define PIN_LATCH A2
#define PIN_CLOCK A3
#define KNOB_F A4
#define KNOB_L A5
#define KNOB_C A6
#define KNOB_R A7

#include "output.h"
#define OUTPUT_AMOUNT 2
Output output(PIN_DATA, PIN_LATCH, PIN_CLOCK, OUTPUT_AMOUNT);

uint8_t YB, Y, B;

int clamp(int min, int val, int max) {
  if (val < min) return min;
  if (val > max) return max;
  return val;
}

int getKnobValue(int knob) {
  return (clamp(12, analogRead(knob), 1011) - 12) / 10;
}

String getUnit(int knob, int power) {
  String result = "";
  switch (power) {
    case (3):  result += "k"; break;
    case (-3): result += "m"; break;
    // case (-6): result += "µ"; break;
    case (-6): result += "u"; break;
    case (-9): result += "n"; break;
    default: break;
  }
  switch (knob) {
    case (KNOB_F): result += "Hz"; break;
    case (KNOB_L): result += "H"; break;
    case (KNOB_C): result += "F"; break;
    // case (KNOB_R): result += "Ω"; break;
    case (KNOB_R): result += "R"; break;
  }

  return result;
}

#include "data.h"
#include "screen.h"
#include "resonance.h"

Resonance resonance(0, 0);

void printDebug() {
  if (Serial.availableForWrite() < 63) return;

  Serial.print("YB: ");
  Serial.print(Y);
  Serial.print(B);

  Serial.print(" | F");
  Serial.print(getKnobValue(KNOB_F) / 10);
  Serial.print(" L");
  Serial.print(getKnobValue(KNOB_L));
  Serial.print(" C");
  Serial.print(getKnobValue(KNOB_C));
  Serial.print(" R");
  Serial.print(getKnobValue(KNOB_R));

  Serial.print(" | F:");
  Serial.print( pgm_read_float(&value_F[getKnobValue(KNOB_F) / 10]) );
  Serial.print( getUnit(KNOB_F, pgm_read_word(&power_F[getKnobValue(KNOB_F) / 10])) );
  Serial.print(" L:");
  Serial.print( pgm_read_float(&value_L[getKnobValue(KNOB_L)]) );
  Serial.print( getUnit(KNOB_L, pgm_read_word(&power_L[getKnobValue(KNOB_L)])) );
  Serial.print(" C:");
  Serial.print( pgm_read_float(&value_C[getKnobValue(KNOB_C)]) );
  Serial.print( getUnit(KNOB_C, pgm_read_word(&power_C[getKnobValue(KNOB_C)])) );
  Serial.print(" R:");
  Serial.print( pgm_read_float(&value_R[getKnobValue(KNOB_R)]) );
  Serial.print( getUnit(KNOB_R, pgm_read_word(&power_R[getKnobValue(KNOB_R)])) );
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(PIN_RANDOM));

  pinMode(PIN_MODE, INPUT_PULLUP);
  pinMode(PIN_DONE, OUTPUT);
  pinMode(PIN_SIG_Y, INPUT_PULLUP);
  pinMode(PIN_SIG_B, INPUT_PULLUP);
  pinMode(PIN_SIG_MISS, OUTPUT);
  // pinMode(PIN_ACK_MISS, INPUT_PULLUP);
  pinMode(PIN_ACK_MISS, OUTPUT);

  digitalWrite(PIN_DONE, 0);
  Y = digitalRead(PIN_SIG_Y);
  B = digitalRead(PIN_SIG_B);
  YB = Y << 1 + B;

  initScreen();
  resonance.init();
  debugScreen();
}

void loop() {
  output.simpleUpdate();

  // printDebug();
  // debugScreen();

  if (digitalRead(PIN_MODE) == 0) {
    switch (resonance.check()) {
      case  1: resonance.fini(); break;
      case  0: resonance.miss(); break;
      case -1: break;
    }

    resonance.loop();
  }
}
