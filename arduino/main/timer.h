#pragma once

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
  B.fini();
}

String Timer::getTime() {
  return \
    String(MINUTE / 10) + String(MINUTE % 10) + ":" + \
    String(SECOND / 10) + String(SECOND % 10) + " ";
}

void Timer::displayTime() {
  Serial.print(String(MINUTE) + ":" + String(SECOND) + " ");
}

