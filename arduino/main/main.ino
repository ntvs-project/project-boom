#include "output.h"
#include "input.h"

// 4 red
// 2 green
// 1 blue
const uint8_t memPatterns[][4][6] PROGMEM = {
  {
    {2, 4, 1, 1, 0},
    {4, 1, 4, 1, 2, 0},
    {4, 4, 1, 2, 2, 0},
    {1, 1, 4, 2, 0},
  }
};

const uint8_t memLength[][4] PROGMEM = {
  {5, 6, 6, 5}
};

const uint8_t memAnswers[][4] PROGMEM = {
  {4, 4, 1, 2}
};

uint8_t patterns[4][6];
uint8_t patternLen[4];
uint8_t prevPattern[4];
uint8_t answer[4];
int colourIdx = 0;
unsigned long now;
unsigned long prev;
unsigned long blinkPrev;
int fixed = 0b0000;

void setup() {
  output.pwmAll(true);
  randomSeed(analogRead(A0));
  Serial.begin(9600);
  prev = millis();
  blinkPrev = millis();

  memcpy_P(patterns,   memPatterns[0], sizeof(patterns));
  memcpy_P(answer,     memAnswers[0] , sizeof(answer));
  memcpy_P(patternLen, memLength[0]  , sizeof(patternLen));

  output.writeRange(1, 4, 1, 5, "11");
}

void loop() {
  output.update();
  input.update();

  if (fixed == 0b1111) {
    if (memcmp(answer, prevPattern, sizeof(answer)) == 0) {
      output.writeRange(1, 4, 1, 5, "01");
    else {
      output.writeRange(1, 4, 1, 5, "10");
    }
    return;
  }

  now = millis();
  
  if (now - prev >= 250) {
    char bin[17] = "000000000000";
    for (int ledIdx=0; ledIdx<4; ledIdx++) {
      for (int digitIdx=0; digitIdx<3; digitIdx++) {
        int idx = 3 * ledIdx + digitIdx;
        if (!bitRead(fixed, ledIdx))
          bin[idx] = ((patterns[ledIdx][colourIdx % patternLen[ledIdx]] >> digitIdx) & 1) + '0';
        else
          bin[idx] = ((prevPattern[ledIdx] >> digitIdx) & 1) + '0';
      }
      if (!bitRead(fixed, ledIdx))
        prevPattern[ledIdx] = patterns[ledIdx][colourIdx % patternLen[ledIdx]];
    }

    output.writeRange(0, 0, 1, 3, bin);

    colourIdx = (colourIdx + 1) % 60;
    prev += 250;
  }

  for (int btnIdx=0; btnIdx < 4; btnIdx++) {
    if (input.readPressed(-1, btnIdx)) {
      bitWrite(fixed, btnIdx, !bitRead(fixed, btnIdx));
    }
  }
}
