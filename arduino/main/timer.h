
class Timer {
  public:
    ui MINUTE;
    ui SECOND;

    void initTime();
    void adjustTime();
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
    T.fini();
  }
}

void Timer::fini() {
  Serial.println("\nfinish");
  while (true) {}
}

void Timer::displayTime() {
  Serial.print(String(T.MINUTE) + ":" + String(T.SECOND) + " ");
}

