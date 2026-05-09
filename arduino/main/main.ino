#include "output.h"
#include "input.h"

#include "folour.h"

Folour folour(0, 0);

void setup() {
  output.pwmAll(true);
  randomSeed(analogRead(A0));
  Serial.begin(9600);
  
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
