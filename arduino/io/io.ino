#include "output.h"
#include "input.h"

uint8_t patterns[4][4];
int i = 0;
unsigned long prev;

void setup() {
  output.pwmAll(true);
  randomSeed(analogRead(A0));
  Serial.begin(9600);
  prev = millis();

  for (int j=0; j<4; j++) {
    for (int i=0; i<4; i++) {
      patterns[j][i] = random(1, 7);
    }
  }
}

void loop() {
  output.update();

  if (millis() - prev >= 500) {
    i = (i + 1) % 4;

    char bin[17] = "0000000000000000";
    for (int j=0; j<4; j++) {
      bin[3 * j + 0] = ((patterns[j][i] >> 0) & 1) + '0';
      bin[3 * j + 1] = ((patterns[j][i] >> 1) & 1) + '0';
      bin[3 * j + 2] = ((patterns[j][i] >> 2) & 1) + '0';
    }

    output.writeRange(0, 0, 0, 7, bin);
    output.writeRange(1, 0, 1, 7, bin + 8);

    prev = millis();
  }
}
