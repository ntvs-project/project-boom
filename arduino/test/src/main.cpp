#include <Arduino.h>

// put function declarations here:
void noise(int);

void setup() {
  pinMode(4, OUTPUT);
  pinMode(8, OUTPUT);

  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  digitalWrite(9,  1);
  digitalWrite(10, 0);

  randomSeed(analogRead(0));

  digitalWrite(8, 1);
  noise(5000);
  digitalWrite(8, 0);
}

void loop() {}

// put function definitions here:
void noise(int duration) {
  for (int i=0; i < duration; i++) {
    tone(4, random(20, 300)); delay(1);
  }
  noTone(4);
}