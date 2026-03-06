#include "bomb.h"

Bomb B;

int _OUT_[] = {3, 4, 5, 6, 8, 9, 10, 11, 12};

void displayNumber(int digit_, int number_) {
  int digits[]  = { 9, 10, 11, 12 };
  int numbers[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09 };
  int n     = numbers[number_];
  int digit = digits[digit_];

  for (int d : digits) digitalWrite(d, d == digit ? 1 : 0);

  digitalWrite(6, bitRead(n, 3)); // D
  digitalWrite(5, bitRead(n, 2)); // C
  digitalWrite(4, bitRead(n, 1)); // B
  digitalWrite(3, bitRead(n, 0)); // A
}

void setup() {
  Serial.begin(9600);
  Serial.println(B.version);
  for (int pin : _OUT_) pinMode(pin, OUTPUT);

  displayNumber(0, 0);
}

void loop() {
  int number[] = {5, 6, 7, 8};
  for (int i=0; i < 4; i++) {
    displayNumber(i, number[i]);
    delay(1);
  }
}
