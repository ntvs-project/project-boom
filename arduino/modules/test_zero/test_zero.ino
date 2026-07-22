#include "output.h"

Output output(8, 9, 10, 1);

// #define BUZZER 2
#define BTN_START 2
uint8_t YB;
uint8_t RED;
char indicators[6];

void setup() {
  Serial.begin(9600);

  output.pwmAll(true);
  randomSeed(analogRead(A0));

  pinMode(BTN_START, INPUT_PULLUP);

  // YB = random(0, 4);
  YB  = 0b11;
  RED = 0b000;

  while (digitalRead(BTN_START) == 1);
  delay(20);
  while (digitalRead(BTN_START) == 0);
  Serial.println("start");

  for (int i=0; i<5; i++) {
    indicators[i] = bitRead(YB << 3 | RED, i) ? '1' : '0';
  }
  indicators[5] = '\0';

  Serial.println(indicators);
  output.writeRange(0, 0, 0, 0, 4, indicators, false);
}

void loop() {
  output.update();
}
