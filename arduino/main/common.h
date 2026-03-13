#pragma once
#include <EasyButton.h>
#include <LiquidCrystal_I2C.h>


typedef unsigned long ul;
typedef unsigned int  ui;
typedef EasyButton eb;

#define length(arr) (sizeof(arr) / sizeof(arr[0]))

#if defined(__AVR_ATmega328P__) // UNO

#elif defined(__AVR_ATmega2560__) // MEGA
ui PIN_SEG[] = { 3, 4, 5, 6, 7, 8, 9 };
ui PIN_DIG[] = { 10, 11, 12, 13 };
ui PIN_GREEN[] = { 22, 23, 24, 25, 26 };
ui PIN_RED[]   = { 27, 28, 29 };
ui OUT[] = {
  3, 4, 5, 6, 7, 8, 9, // seg a~g
  10, 11, 12, 13,      // D1~4
  22, 23, 24, 25, 26,  // green
  27, 28, 29,          // red
};

eb  PIN_FN[]     = { eb(30), eb(38) }; // start reset
eb  PIN_MORSE[]  = { eb(31), eb(32) };
eb  PIN_TEST[]   = { eb(33), eb(34), eb(35), eb(36), eb(37) };
eb* PIN_EB[]     = { PIN_FN, PIN_MORSE, PIN_TEST };
ui  PIN_EB_LEN[] = { length(PIN_FN), length(PIN_MORSE), length(PIN_TEST) };
ui  IN[]  = {
  30, 38,             // start
  31, 32,             // morse
  33, 34, 35, 36, 37, // test1~5
};
#endif

ui TIME[] = { 1, 0 };
ui index;

template <typename T>
void swap(T& a, T& b) {
  T tmp = a;
  a = b;
  b = tmp;
}

class Bomb {
  public:
    void (*reset)(void) = 0;
    void fini() {
      while (!Serial.available() && !PIN_FN[0].wasReleased()) PIN_FN[0].read();
      Serial.read();
      reset();
    }

}; Bomb B;
