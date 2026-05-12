#include "output.h"
#include "input.h"

uint8_t YB;

#include "folour.h"
#include "matrix.h"

Folour folour(0, 0);
Matrix matrix(0, 0);

void setup() {
  Serial.begin(9600);

  output.pwmAll(true);
  randomSeed(analogRead(A0));

  YB = random(0, 4);
  YB = 0;
  Serial.println();
  Serial.println(YB);
  
  matrix.init();
}

void loop() {
  output.update();
  input.update();

  switch (matrix.check()) {
    case  1: matrix.fini(); break;
    case  0: matrix.miss(); break;
    case -1: break;
  }

  matrix.loop();
}
