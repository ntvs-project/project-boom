#include "bomb.h"
#include "timer.h"
#include "pitches.h"

Bomb B;
Timer T;
int prev, duration;
bool ended = false;

int OUT[] = {3, 4, 5, 6, 8, 9, 10, 11, 12};

void setup() {
  Serial.begin(9600);
  Serial.println("version: " + B.version);
  for (int pin : OUT) pinMode(pin, OUTPUT);

  T.init();
  prev = millis();
}

void loop() {
  duration = millis() - prev;
  if (duration >= 1000) {
    ended = T.adjustTime();
    if (ended) {
      Serial.println("END");

      while (true) T.displayTime(); 
    } else {
      prev = millis();
    }
  }
  T.displayTime();
}
