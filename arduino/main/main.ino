// TEST, SINGLE, FULL
#define SINGLE
// DEBUG, NODEBUG
#define DEBUG

#include <ezBuzzer.h>
#include <TM1637Display.h>

#include "output.h"
#include "input.h"

#define BUZZER 5
#define YELLOW 7
#define BLUE   6

#define OUTPUT_AMOUNT 2
#define INPUT_AMOUNT  1

uint8_t YB, Y, B, mis;

Output output(8, 9, 10, OUTPUT_AMOUNT);
Input input(11, 12, 13, INPUT_AMOUNT);

TM1637Display timer(3, 4, 10); // CLK DIO
ezBuzzer buzzer(BUZZER, BUZZER_TYPE_PASSIVE);

#include "MB.h"
#include "folour.h"
#include "grey.h"
#include "caesar.h"
#include "test.h"

#ifdef FULL
MB mb(0, 0);
Folour folour(1, 0);
// Grey   grey  (2, 1);
#endif

#ifdef SINGLE;
MB mb(2, 1);
Grey   grey(0, 0);
Caesar caesar(0, 0);
#endif

#ifdef TEST;
Test test(0, 0);
#endif

void setup() {
  Serial.begin(9600);

  output.pwmAll(false);
  randomSeed(analogRead(A0) + micros());

  pinMode(YELLOW, OUTPUT);
  pinMode(BLUE  , OUTPUT);

  #ifdef FULL
  mb.init();
  folour.init();
  // grey.init();
  #endif

  #ifdef SINGLE;
  mb.init();
  if (Y != B) {
    grey.init();
  } else {
    caesar.init();
  }
  #endif

  #ifdef TEST;
  test.init();
  #endif
}

void loop() {
  #ifdef DEBUG
  mis = 0;
  mb.resetTimer();
  #endif

  buzzer.loop();
  output.update();
  input.update();

  #ifdef FULL
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
  #endif

  #ifdef SINGLE;
  if (Y != B) {
    switch (grey.check()) {
      case  1: grey.fini(); break;
      case  0: grey.miss(); break;
      case -1: break;
    }
  } else {
    switch (caesar.check()) {
      case  1: caesar.fini(); break;
      case  0: caesar.miss(); break;
      case -1: break;
    }
  }
  #endif

  #ifdef FULL
  mb.loop();
  folour.loop();
  // grey.loop();
  #endif

  #ifdef SINGLE;
  mb.loop();
  if (Y != B) {
    grey.loop();
  } else {
    caesar.loop();
  }
  #endif

  #ifdef TEST
  test.loop();
  #endif
}
