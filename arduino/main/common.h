#pragma once

#include <LiquidCrystal_I2C.h>
#include <TM1637Display.h>
#include "pitches.h"

typedef unsigned long ul;
typedef unsigned int  ui;

#define length(arr) (sizeof(arr) / sizeof(arr[0]))

const ui PIN_TONE[]  = { 15 };
const ui PIN_7SEG[]  = { 2, 3 }; // CLK DIO
const ui PIN_LED[] = { 12, 11, 13 }; // GGGGGRRR
const ui PIN_BTN[] = { 10, 8, 9 };   // 12345 Start Reset

const ui OUT[] = {
  9, 10, 11, 12, 13
};
const ui  IN[]  = {
  8
};
const ui VCC[] = { 4 };
const ui GND[] = { 5 };

const ui IDX_START = 0;
const ui IDX_RESET = 1;
const ui IDX_TEST[]  = { 5, 4, 3, 2, 1 };

ui TIME[] = { 1, 0 };
ui MODULE_COUNT = 5;
ui MISS_COUNT   = 3;
byte led_byte = 0b00000000;

// LiquidCrystal_I2C lcd(0x27, 16, 2);

bool next = true;
ui index = 0;
ui miss  = 0;

template <typename T>
void swap(T& a, T& b) {
  T tmp = a;
  a = b;
  b = tmp;
}

class Button {
  public:
    byte raw;
    bool prev[8] = {};
    bool curr[8] = {};

    void update() {
      digitalWrite(PIN_BTN[0], LOW);
      delayMicroseconds(5);
      digitalWrite(PIN_BTN[0], HIGH);

      digitalWrite(PIN_BTN[2], HIGH);
      delayMicroseconds(1);
      digitalWrite(PIN_BTN[2], LOW);

      raw = shiftIn(PIN_BTN[1], PIN_BTN[2], LSBFIRST);
      
      // for (int i = 0; i <= 7; i++) Serial.print(bitRead(raw, i));
      // Serial.println();

      for (int i = 0; i <= 7; i++) {
        prev[i] = curr[i];
        curr[i] = bitRead(raw, i);
      }
    }

    bool wasPressed(ui idx) {
      return !prev[idx] && curr[idx];
    }

    bool wasReleased(ui idx) {
      return prev[idx] && !curr[idx];
    }
}; Button BTN;

class Tone {
  public:
    void begin() {
      tone(PIN_TONE[0], NOTE_C5, 500); delay(500);
    }

    void pass() {
      tone(PIN_TONE[0], NOTE_C4, 100); delay(100);
      tone(PIN_TONE[0], NOTE_E4, 100); delay(100);
      tone(PIN_TONE[0], NOTE_G4, 100); delay(100);
    }

    void miss() {
      tone(PIN_TONE[0], NOTE_G4, 100); delay(100);
      tone(PIN_TONE[0], NOTE_C4, 100); delay(100);
    }

    void fini() {
      tone(PIN_TONE[0], NOTE_C4, 100); delay(100);
      tone(PIN_TONE[0], NOTE_D4, 100); delay(100);
      tone(PIN_TONE[0], NOTE_E4, 100); delay(100);
      tone(PIN_TONE[0], NOTE_F4, 100); delay(100);
      tone(PIN_TONE[0], NOTE_G4, 100); delay(100);
      tone(PIN_TONE[0], NOTE_A4, 100); delay(100);
      tone(PIN_TONE[0], NOTE_B4, 100); delay(100);
      tone(PIN_TONE[0], NOTE_C5, 100); delay(100);
    }

    void boom() {
      tone(PIN_TONE[0], NOTE_C5, 100); delay(100);
      tone(PIN_TONE[0], NOTE_B4, 100); delay(100);
      tone(PIN_TONE[0], NOTE_A4, 100); delay(100);
      tone(PIN_TONE[0], NOTE_G4, 100); delay(100);
      tone(PIN_TONE[0], NOTE_F4, 100); delay(100);
      tone(PIN_TONE[0], NOTE_E4, 100); delay(100);
      tone(PIN_TONE[0], NOTE_D4, 100); delay(100);
      tone(PIN_TONE[0], NOTE_C4, 100); delay(100);
    }
}; Tone TONE;

class Bomb {
  public:
    
    void reset() {
      asm volatile ("jmp 0");
    }
    
    void led_set() {
      digitalWrite(PIN_LED[0], 0);
      shiftOut(PIN_LED[1], PIN_LED[2], MSBFIRST, led_byte);
      digitalWrite(PIN_LED[0], 1);
    }
    void missed() {
      TONE.miss();

      miss++;
      for (int i=0; i<miss; i++) bitWrite(led_byte, i + 5, 1);
      led_set();
      if (miss == MISS_COUNT) {
        Serial.println("\nKid, you Kaboom!");

        delay(100);
        
        TONE.boom();

        restart();
      }
    }
    void fini() {
      TONE.pass();

      index++;
      next = true;
      for (int i=0; i<index; i++) bitWrite(led_byte, i, 1);
      led_set();
    }
    void restart() {
      while (!Serial.available() && !BTN.wasReleased(IDX_START)) BTN.update();
      Serial.read();

      reset();
    }

}; Bomb B;
