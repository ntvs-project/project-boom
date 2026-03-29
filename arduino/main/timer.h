#pragma once

TM1637Display clock(PIN_7SEG[0], PIN_7SEG[1]);

class Timer {
  public:
    ui MINUTE;
    ui SECOND;

    void initTime();
    void adjustTime();
    String getTime();
    void displayTime();
    void displayDigit();
    void displaySegment();
    void fini();
}; Timer T;

void Timer::initTime() {
  MINUTE = TIME[0];
  SECOND = TIME[1];
  displayTime();
}

void Timer::adjustTime() {
  if (SECOND > 0) {
    SECOND--;
  } else if (MINUTE > 0) {
    SECOND = 59;
    MINUTE--;
  } else {
    fini();
  }
}

void Timer::fini() {
  Serial.println("\nTIMEOUT");

  TONE.boom();

  B.restart();
}

String Timer::getTime() {
  return \
    String(MINUTE / 10) + String(MINUTE % 10) + ":" + \
    String(SECOND / 10) + String(SECOND % 10) + " ";
}

void Timer::displayTime() {
  clock.showNumberDecEx(MINUTE * 100 + SECOND, 0b01000000, true);
}
