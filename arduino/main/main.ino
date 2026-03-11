#include "common.h"
#include "timer.h"
#include "module.h"

Module* MODULES[] = {
  &test1,
  &test2,
};

ul prev, duration;

void setup() {
  Serial.begin(9600);
  Serial.println("start");

  Serial.println("init pins");
  Serial.println("pick modules");

  Serial.println("waiting for signal to start");
  while (!Serial.available()) {}
  Serial.read();

  index = 0;
  T.initTime();
  prev = millis();
}

void loop() {
  if (index >= length(MODULES)) {
    Serial.println("\nFINISH");
    while (true) {}
  }
  MODULES[index]->loop();
  
  duration = millis() - prev;
  if (duration >= 1000) {
    T.adjustTime();

    prev = millis();
  }

  T.displayTime();
  Serial.println();
}
