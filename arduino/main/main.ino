#include "common.h"
#include "timer.h"
#include "module.h"

Module* MODULES[] = {
  &test1,
  &test2,
  &test3,
  &test4,
  &test5,
  &test6,
  &test7,
  &test8,
};

ul prev, duration;

void setup() {
  Serial.begin(9600);
  Serial.println("!! init");

  clock.setBrightness(0x0f);
  clock.clear();

  for (ui pin : OUT) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, 0);
  }

  for (ui pin : IN) pinMode(pin, INPUT_PULLUP);
  for (int j = 0; j < length(PIN_EB); j++)
    for (int i = 0; i < PIN_EB_LEN[j]; i++)
      PIN_EB[j][i].begin();

  randomSeed(micros());
  for (int i = length(MODULES) - 1; i > 0; i--)
    swap(MODULES[i], MODULES[random(i + 1)]);

  Serial.println("!! waiting for signal to start");
  while (!Serial.available() && !PIN_FN[0].wasReleased()) PIN_FN[0].read();
  Serial.read();

  index = 0;
  T.initTime();
  prev = millis();
  
  tone(PIN_TONE[0], NOTE_C5, 500); delay(500);
}

void loop() {
  for (int j = 0; j < length(PIN_EB); j++)
    for (int i = 0; i < PIN_EB_LEN[j]; i++)
      PIN_EB[j][i].read();

  if (index >= MODULE_COUNT) {
    Serial.print("\nFINISH ");
    Serial.println(T.getTime());

    delay(100);
    tone(PIN_TONE[0], NOTE_C4, 100); delay(100);
    tone(PIN_TONE[0], NOTE_D4, 100); delay(100);
    tone(PIN_TONE[0], NOTE_E4, 100); delay(100);
    tone(PIN_TONE[0], NOTE_F4, 100); delay(100);
    tone(PIN_TONE[0], NOTE_G4, 100); delay(100);
    tone(PIN_TONE[0], NOTE_A4, 100); delay(100);
    tone(PIN_TONE[0], NOTE_B4, 100); delay(100);
    tone(PIN_TONE[0], NOTE_C5, 100); delay(100);

    B.restart();
  }
  // if (PIN_FN[1].wasReleased()) B.reset();
  if (next) {
    MODULES[index]->init();
    next = false;
  }
  MODULES[index]->loop();

  duration = millis() - prev;
  if (duration >= 1000) {
    T.adjustTime();
    Serial.print(T.getTime());
    T.displayTime();

    prev = millis();
  }
}
