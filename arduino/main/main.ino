#include "output.h"
#include "input.h"

uint8_t YB;

#include "folour.h"

Folour folour(0, 0);

void setup() {
  Serial.begin(9600);

  output.pwmAll(true);
  randomSeed(analogRead(A0));

  YB = random(0, 4);
  Serial.println();
  Serial.println(YB);
  
  folour.init();
}

void loop() {
  output.update();
  input.update();

  switch (folour.check()) {
    case  1: folour.fini(); break;
    case  0: folour.miss(); break;
    case -1: break;
  }

  folour.loop();
}
