#include "api.h"
#include "bomb.h"
#include "timer.h"

Bomb B;
Timer T;
int prev, duration;
bool ended = false;

void setup() {
  Serial.begin(9600);
  Serial.println("version: " + version);
  for (int pin : OUT) pinMode(pin, OUTPUT);
  for (int pin : IN) pinMode(pin, INPUT_PULLUP);

  T.init();
  B.init();
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
  // %%%%%%%%%% %%%%%%%%%% %%%%%%%%%% %%%%%%%%%%

  B.loop();
}
