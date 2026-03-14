#pragma once
#include <EasyButton.h>
// #include <LiquidCrystal_I2C.h>

typedef unsigned long ul;
typedef unsigned int  ui;
typedef EasyButton eb;

#define length(arr) (sizeof(arr) / sizeof(arr[0]))

#if defined(__AVR_ATmega328P__) // UNO

#elif defined(__AVR_ATmega2560__) // MEGA
const ui PIN_SEG[] = { 3, 4, 5, 6, 7, 8, 9 };
const ui PIN_DIG[] = { 10, 11, 12, 13 };
const ui PIN_GREEN[] = { 22, 23, 24, 25, 26 };
const ui PIN_RED[]   = { 27, 28, 29 };
const ui OUT[] = {
  3, 4, 5, 6, 7, 8, 9, // seg a~g
  10, 11, 12, 13,      // D1~4
  22, 23, 24, 25, 26,  // green
  27, 28, 29,          // red
};

const eb  PIN_FN[]     = { eb(30) }; // start reset
const eb  PIN_MORSE[]  = { eb(31), eb(32) };
const eb  PIN_TEST[]   = { eb(33), eb(34), eb(35), eb(36), eb(37),
                           eb(38), eb(39), eb(40) };
const eb* PIN_EB[]     = { PIN_FN, PIN_MORSE, PIN_TEST };
const ui  PIN_EB_LEN[] = { length(PIN_FN), length(PIN_MORSE), length(PIN_TEST) };
const ui  IN[]  = {
  30,                 // start
  31, 32,             // morse
  33, 34, 35, 36, 37, 38, 39, 40, // test1~5
};
#endif

ui TIME[] = { 1, 0 };
ui MODULE_COUNT = 5;

bool next = true;
ui index = 0;
ui miss  = 0;

template <typename T>
void swap(T& a, T& b) {
  T tmp = a;
  a = b;
  b = tmp;
}

class Bomb {
  public:
    void (*reset)(void) = 0;
    void missed() {
      miss++;
      for (int i=0; i<miss; i++) digitalWrite(PIN_RED[i], 1);
      if (miss == 3) {
        Serial.println("\nKid, you Kaboom!");
        restart();
      }
    }
    void fini() {
      index++;
      next = true;
      for (int i=0; i<index; i++) digitalWrite(PIN_GREEN[i], 1);
    }
    void restart() {
      while (!Serial.available() && !PIN_FN[0].wasReleased()) PIN_FN[0].read();
      Serial.read();
      reset();
    }

}; Bomb B;
