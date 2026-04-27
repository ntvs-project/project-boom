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

char* b2s(uint8_t value) {
    static char s[9];  // 8 bits + null terminator
    for (int i = 7; i >= 0; i--) {
        s[7 - i] = (value & (1 << i)) ? '1' : '0';
    }
    s[8] = '\0';  // null-terminate
    return s;
}

void loop() {
  output.update();

  if (millis() - prev >= 500) {
    i = (i + 1) % 4;

    uint16_t bin = 0;
    for (int j=0; j<4; j++) {
      bin = (bin << 3) + patterns[j][i];
    }
    output.writeRange(0, 0, 0, 7, b2s(bin >> 8));
    output.writeRange(1, 0, 1, 7, b2s(bin & 0xFF));

    prev = millis();
  }
}
