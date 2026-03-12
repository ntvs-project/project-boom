#include "common.h"
#include "timer.h"
#include "module.h"

Module* MODULES[] = {
  &test1,
  &test2,
  &test3,
  &test4,
  &test5,
};

ul prev, duration;

void setup() {
  Serial.begin(9600);
  Serial.println("!! start");

  for (ui pin : OUT) pinMode(pin, OUTPUT);
  for (ui pin : IN)  pinMode(pin, INPUT);

  randomSeed(micros());
  for (int i = length(MODULES) - 1; i > 0; i--)
    swap(MODULES[i], MODULES[random(i + 1)]);
  Serial.println("# slice modules");

  Serial.println("!! waiting for signal to start");
  while (!Serial.available());
  Serial.read();

  index = 0;
  T.initTime();
  prev = millis();
}

void loop() {
  if (index >= length(MODULES)) {
    Serial.println("\nFINISH");
    B.fini();
  }
  MODULES[index]->loop();
  
  duration = millis() - prev;
  if (duration >= 1000) {
    T.adjustTime();

    prev = millis();
  }

  Serial.println(T.getTime());
}
