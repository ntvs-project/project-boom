#include "common.h"
#include "timer.h"
#include "module.h"

// #include <Wire.h>
// #include <hd44780.h>
// #include <hd44780ioClass/hd44780_I2Cexp.h>

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
  Serial.println("!! init");

  for (ui pin : OUT) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, 0);
  }

  for (ui pin : IN) pinMode(pin, INPUT_PULLUP);

  for (ui pin : VCC) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, 1);
  }
  for (ui pin : GND) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, 0);
  }

  clock.setBrightness(0x0f);
  clock.clear();
  B.led_set();
  for (int i=0; i<4; i++) digitalWrite(PIN_7SEG[i], bitRead(15, i));

  // hd44780_I2Cexp lcd;
  // lcd.begin(16, 2);
  // lcd.backlight();
  // lcd.print("hello");

  clock.showNumberDecEx(0, 0b01000000, true);

  randomSeed(analogRead(A0));
  for (int i = length(MODULES) - 1; i > 0; i--)
    swap(MODULES[i], MODULES[random(i + 1)]);

  Serial.println("!! waiting for signal to start");

  while (!Serial.available() && !BTN.wasReleased(IDX_START)) BTN.update();
  Serial.read();

  index = 0;
  T.initTime();
  prev = millis();
  
  TONE.begin();
}

void loop() {
  BTN.update();

  if (index >= MODULE_COUNT) {
    Serial.print("\nFINISH ");
    Serial.println(T.getTime());

    delay(100);
    TONE.fini();

    B.restart();
  }
  // if (BTN.wasReleased(IDX_RESET)) B.reset();
  if (next) {
    MODULES[index]->init();
    next = false;
  }
  MODULES[index]->loop();

  duration = millis() - prev;
  if (duration >= 1000) {
    T.adjustTime();
    // Serial.print(T.getTime());
    T.displayTime();

    prev += 1000;
  }
}
