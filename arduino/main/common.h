#pragma once

// #include <LiquidCrystal_I2C.h>
#include <EasyButton.h>
#include <TM1637Display.h>
#include "pitches.h"


typedef unsigned long ul;
typedef unsigned int  ui;
typedef EasyButton eb;

#define length(arr) (sizeof(arr) / sizeof(arr[0]))

#if defined(__AVR_ATmega328P__) // UNO
const ui PIN_TONE[]  = { 5 };
const ui PIN_7SEG[]  = { 8, 9 }; // DIO CLK
const ui PIN_GREEN[] = { 13, 12 };
const ui PIN_RED[]   = { 11, 10 };
const ui OUT[] = {
  8, 9,
  12, 13,
  10, 11
};

const eb  PIN_FN[]     = { eb(2), eb(3) };
const eb* PIN_EB[]     = { PIN_FN };
const ui  PIN_EB_LEN[] = { length(PIN_FN) };
const ui  IN[]  = {
  2
};
#elif defined(__AVR_ATmega2560__) // MEGA

#endif

ui TIME[] = { 1, 0 };
ui MODULE_COUNT = 2;
ui MISS_COUNT   = 2;

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
      tone(PIN_TONE[0], NOTE_G4, 100); delay(100);
      tone(PIN_TONE[0], NOTE_C4, 100); delay(100);

      miss++;
      for (int i=0; i<miss; i++) digitalWrite(PIN_RED[i], 1);
      if (miss == MISS_COUNT) {
        Serial.println("\nKid, you Kaboom!");

        delay(100);
        tone(PIN_TONE[0], NOTE_C5, 100); delay(100);
        tone(PIN_TONE[0], NOTE_B4, 100); delay(100);
        tone(PIN_TONE[0], NOTE_A4, 100); delay(100);
        tone(PIN_TONE[0], NOTE_G4, 100); delay(100);
        tone(PIN_TONE[0], NOTE_F4, 100); delay(100);
        tone(PIN_TONE[0], NOTE_E4, 100); delay(100);
        tone(PIN_TONE[0], NOTE_D4, 100); delay(100);
        tone(PIN_TONE[0], NOTE_C4, 100); delay(100);

        restart();
      }
    }
    void fini() {
      tone(PIN_TONE[0], NOTE_C4, 100); delay(100);
      tone(PIN_TONE[0], NOTE_E4, 100); delay(100);
      tone(PIN_TONE[0], NOTE_G4, 100); delay(100);

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
