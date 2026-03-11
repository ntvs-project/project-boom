#pragma once

class Timer {
  public:
    int minute;
    int second;

    void displayDigit(int digit_, int number_);
    void displaySegment(int digit_, int number_);
    void init();
    bool adjustTime();
    void displayTime();
};

void Timer::displaySegment(int digit_, int number_) {
  int digits[]  = { 10, 11, 12, 13 };
  int segments[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F };
  int n     = segments[number_];
  int digit = digits[digit_];

  for (int d : digits) digitalWrite(d, d == digit ? 1 : 0);

  digitalWrite(9,  bitRead(n, 6)); // g
  digitalWrite(8,  bitRead(n, 5)); // f
  digitalWrite(7,  bitRead(n, 4)); // e
  digitalWrite(6,  bitRead(n, 3)); // d
  digitalWrite(5,  bitRead(n, 2)); // c
  digitalWrite(4,  bitRead(n, 1)); // b
  digitalWrite(3,  bitRead(n, 0)); // a
}

void Timer::displayDigit(int digit_, int number_) {
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

void Timer::init() {
  minute = MINUTE;
  second = SECOND;
  displayTime();
}

bool Timer::adjustTime() {
  if (second != 0) {
    second --;
  } else if (minute != 0) {
    minute --;
    second = 59;
  } else {
    return true;
  }
  return false;
}

void Timer::displayTime() {
  int number[] = {
    minute / 10, minute % 10,
    second / 10, second % 10
  };
  for (int i=0; i < 4; i++) {
    displaySegment(i, number[i]);
    delay(1);
  }
}
