// SINGLE, FULL
#define SINGLE
// DEBUG, NODEBUG
#define DEBUG

#include <ezBuzzer.h>
#include <TM1637Display.h>

#include "output.h"
#include "input.h" // CHANGE

#define BUZZER 5
#define YELLOW 7
#define BLUE   6

#define OUTPUT_AMOUNT 3 // CHANGE
#define INPUT_AMOUNT  1 // CHANGE
#define BOARD_AMOUNT  1 // CHANGE

uint8_t YB, Y, B, mis;
uint8_t moduleFinished = 0;

Output output(8, 9, 10, OUTPUT_AMOUNT);
Input input(11, 12, 13, INPUT_AMOUNT);

TM1637Display timer(3, 4, 10); // CLK DIO
ezBuzzer buzzer(BUZZER, BUZZER_TYPE_PASSIVE);

#include "MB.h"
#include "folour.h"
#include "grey.h"
#include "caesar.h"
#include "tune.h"
#include "trislator.h"
#include "test.h"

#ifdef FULL
MB mb(3, 3);
Folour folour(0, 0); // 1, 0 (bar)
Grey   grey(2, 1);
Caesar caesar(2, 1);
#endif

#ifdef SINGLE; // CHANGE
Trislator trislator(0, 0);
#endif

void setup() {
  Serial.begin(9600);

  randomSeed(analogRead(A0) + micros());

  pinMode(YELLOW, OUTPUT);
  pinMode(BLUE  , OUTPUT);

  #ifdef FULL
  mb.init();
  folour.init();
  if (Y != B) {
    grey.init();
  } else {
    caesar.init();
  }
  #endif

  #ifdef SINGLE; // CHANGE
  trislator.init();
  #endif
}

void loop() {
  #ifdef DEBUG
  mis = 0;
  // mb.resetTimer();
  #endif

  output.simpleUpdate();
  input.update();

  #ifdef FULL
  switch (folour.check()) {
    case  1: folour.fini(); break;
    case  0: folour.miss(); break;
    case -1: break;
  }
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

  #ifdef SINGLE; // CHANGE
  switch (trislator.check()) {
    case  1: trislator.fini(); break;
    case  0: trislator.miss(); break;
    case -1: break;
  }
  #endif

  #ifdef FULL
  mb.loop();
  if (!folour.finished) folour.loop();

  if (moduleFinished == BOARD_AMOUNT - 1) {
    buzzer.loop();

    if (Y != B) {
      grey.loop();
    } else {
      caesar.loop();
    }
  }
  #endif

  #ifdef SINGLE; // CHANGE
  trislator.loop();
  #endif
}
