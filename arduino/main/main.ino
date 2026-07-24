#include <ezBuzzer.h>
#include <TM1637Display.h>

#include "output.h"
#include "input.h"

#define BUZZER 5

#define OUTPUT_AMOUNT 3
#define INPUT_AMOUNT  1

uint8_t YB, Y, B, mis;

Output output(8, 9, 10, OUTPUT_AMOUNT);
Input input(11, 12, 13, INPUT_AMOUNT);

TM1637Display timer(3, 4, 10); // CLK DIO
ezBuzzer buzzer(BUZZER, BUZZER_TYPE_PASSIVE);

#include "MB.h"
#include "folour.h"
#include "grey.h"
#include "test.h"

// Test test(0, 0);
MB mb(0, 0);
Folour folour(1, 0);
// Grey   grey  (2, 1);

void setup() {
  Serial.begin(9600);

  output.pwmAll(false);
  randomSeed(analogRead(A0));
  
  mb.init();
  folour.init();
  // grey.init();
  // test.init();
}

void loop() {
  buzzer.loop();
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

  mb.loop();
  folour.loop();
  // grey.loop();
  // test.loop();
}
