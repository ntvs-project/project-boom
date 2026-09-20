
#include "input_bar.h"

Input input(11, 12, 13, 2);

void setup() {
  Serial.begin(9600);
}

void loop() {
  input.update();

  for (int i=0; i<16; i++) {
    if (input.readReleased(0, -1, i)) {
      Serial.println(i);
    }
  }
}
