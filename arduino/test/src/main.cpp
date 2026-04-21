#include <Arduino.h>

const uint8_t PIN_RCLK  = 3;
const uint8_t PIN_SER   = 2;
const uint8_t PIN_SRCLK = 4;

uint16_t led_byte = 0b0101010101010101;

void led_set(uint16_t value) {
  digitalWrite(PIN_RCLK, LOW);                              // hold latch low while shifting
  shiftOut(PIN_SER, PIN_SRCLK, MSBFIRST, (value >> 8) & 0xFF); // high byte → chip #2
  shiftOut(PIN_SER, PIN_SRCLK, MSBFIRST,  value       & 0xFF); // low byte  → chip #1
  digitalWrite(PIN_RCLK, HIGH);                             // rising edge latches to outputs
}

void setup() {
  pinMode(PIN_RCLK,  OUTPUT);
  pinMode(PIN_SER,   OUTPUT);
  pinMode(PIN_SRCLK, OUTPUT);

  led_set(led_byte);
}

void loop() {}
