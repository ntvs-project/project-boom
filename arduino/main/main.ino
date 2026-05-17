#include "output.h"
#include "input.h"

Output output(8, 9, 10, 3);
Input input(11, 12, 13, 2);

#define BUZZER 2
uint8_t YB;

#include "folour.h"
#include "grey.h"

Folour folour(0, 0);
Grey   grey  (2, 1);

void setup() {
  Serial.begin(9600);

  output.pwmAll(true);
  randomSeed(analogRead(A0));

  pinMode(BUZZER, OUTPUT);

  // YB = random(0, 4);
  YB = 2;
  Serial.println();
  Serial.println(YB);
  
  folour.init();
  grey.init();
}

void loop() {
  output.update();
  input.update();

  switch (folour.check()) {
    case  1: folour.fini(); break;
    case  0: folour.miss(); break;
    case -1: break;
  }
  switch (grey.check()) {
    case  1: grey.fini(); break;
    case  0: grey.miss(); break;
    case -1: break;
  }

  folour.loop();
  grey.loop();
}
