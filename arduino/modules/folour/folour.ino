#include "io.h"

void setup() {
  Serial.begin(9600);
  output.all(0);

  output.writeAll(0b1110010101001111);
}

long prev;
int color;
long bin;
int color_idx = 0;
int colors[] = {
  0b001, 0b010, 0b011,
  0b100, 0b101, 0b110, 0b111
};

void loop() {
  output.pwm_update();

  if (millis() - prev >= 500) {
    color = colors[color_idx];

    for (int i=0; i<4; i++) {
      output.write(i * 3 + 0, (color >> 2) & 1);
      output.write(i * 3 + 1, (color >> 1) & 1);
      output.write(i * 3 + 2, (color >> 0) & 1);
    }

    color_idx = (color_idx + 1) % 7;

    prev = millis();
  }
}
