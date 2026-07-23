#include "output.h"
#include "input.h"

Output output(8, 9, 10, 2);
Input input(11, 12, 13, 1);

#define BUZZER 5
#define YELLOW 7
#define BLUE   6

bool buzzing = false;
uint8_t YB, Y, B;

#include "test.h"
#include "folour.h"
#include "grey.h"

// Test test(0, 0);
Folour folour(0, 0);
// Grey   grey  (2, 1);

void setup() {
  Serial.begin(9600);

  output.pwmAll(true);
  randomSeed(analogRead(A0));

  pinMode(BUZZER, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(BLUE, OUTPUT);

  // YB = 2;
  YB = random(0, 4);
  Y  = (YB >> 1);
  B  = (YB % 2);

  digitalWrite(YELLOW, Y);
  digitalWrite(BLUE, B);

  Serial.println("\n-----");
  Serial.print(Y);
  Serial.print(B);
  Serial.println("\n-----");
  
  // test.init();
  folour.init();
  // grey.init();
}

void loop() {
  output.update();
  input.update();

  switch (folour.check()) {
    case  1: folour.fini(); break;
    case  0: folour.miss(); break;
    case -1: break;
  }
  // switch (grey.check()) {
  //   case  1: grey.fini(); break;
  //   case  0: grey.miss(); break;
  //   case -1: break;
  // }

  // test.loop();
  folour.loop();
  // grey.loop();
}
