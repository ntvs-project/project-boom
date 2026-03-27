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
  B.restart();
}

String Timer::getTime() {
  return \
    String(MINUTE / 10) + String(MINUTE % 10) + ":" + \
    String(SECOND / 10) + String(SECOND % 10) + " ";
}

#if defined(__AVR_ATmega328P__) // UNO
void Timer::displayTime() {}
#elif defined(__AVR_ATmega2560__) // MEGA
void Timer::displayTime() {
  ui time = {
    (MINUTE / 10), (MINUTE % 10),
    (SECOND / 10), (SECOND % 10),
  };

  
}
#endif
