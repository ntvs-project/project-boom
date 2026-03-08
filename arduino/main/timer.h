
class Timer {
  public:
    int minute;
    int second;

    void displayNumber(int digit_, int number_);
    void init();
    bool adjustTime();
    void displayTime();
};

void Timer::displayNumber(int digit_, int number_) {
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
  minute = 0;
  second = 5;
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
    displayNumber(i, number[i]);
    delay(1);
  }
}
