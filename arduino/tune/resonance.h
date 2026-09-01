
const uint8_t memResonanceAnswers[][7] PROGMEM = {
  {0, 63, 74, 95, 59, 36, 13}, // 0  5.03kHz   1.1mH  910nF  6.8kΩ   220Ω   27Ω   3.3Ω
  {1, 48, 69, 91, 50, 19, 42}, // 1    11kHz   374μH  560nF  4.7kΩ   100Ω  5.6Ω    47Ω
  {2, 29, 78,  0, 87, 71, 30}, // 2    15kHz  86.6μH  1.3μF     1Ω  3.3kΩ  680Ω    15Ω
  {3, 14, 87, 67,  5, 46, 89}, // 3    16kHz    30μH  3.3μF   470Ω   1.5Ω   68Ω  3.9kΩ
  {4, 92, 21, 83, 44, 65, 21}, // 4  19.3kHz    10mH  6.8nF  2.2kΩ    56Ω  390Ω   6.8Ω
  {5, 53, 50, 22, 25, 63, 99}, // 5    22kHz   523μH  100nF   7.5Ω    10Ω  330Ω   10kΩ
  {6, 11, 70, 17, 34, 55, 79}, // 6    43kHz  22.1μH  620nF   4.7Ω    22Ω  150Ω  1.5kΩ
  {7, 91,  2,  9, 93, 28, 61}, // 7  50.3kHz   9.1mH  1.1nF   2.2Ω  5.6kΩ   12Ω   270Ω
  {8, 62, 24, 75, 53, 38, 23}, // 8    51kHz  1.07mH  9.1nF    1kΩ   120Ω   33Ω   8.2Ω
  {9, 14, 51, 32, 77, 72, 24}, // 9    91kHz    30μH  102nF    18Ω  1.2kΩ  820Ω   9.1Ω
};

// 5.03kHz   1.1mH  910nF  8.2Ω   5.1Ω    3Ω   22Ω
//   11kHz   374μH  560nF  1.5Ω   1.6Ω    2Ω  3.6Ω
//   15kHz  86.6μH  1.3μF  2.2Ω   1.2Ω  2.4Ω   27Ω
//   16kHz    30μH  3.3μF  3.9Ω   4.7Ω 1.02Ω  1.8Ω
// 19.3kHz    10mH  6.8nF   10Ω   9.1Ω   15Ω  2.7Ω
//   22kHz   523μH  100nF  5.6Ω   4.3Ω   36Ω   20Ω
//   43kHz  22.1μH  620nF  6.2Ω    16Ω   30Ω   13Ω
// 50.3kHz   9.1mH  1.1nF  1.3Ω    18Ω  6.8Ω   12Ω
//   51kHz  1.07mH  9.1nF   24Ω  10.2Ω  3.3Ω  7.5Ω
//   91kHz    30μH  102nF  1.1Ω     1Ω   33Ω   11Ω

class Resonance {
  private:
    uint8_t INOFF;
    uint8_t OUTOFF;

    uint8_t answerIndex;
    uint8_t user[4];
    uint8_t answer[4];
    unsigned long prevCheck;

    unsigned long prevL;
    unsigned long prevR;
    int gapL, gapR;
    bool blinkL = false;
    bool blinkR = false;

    char ledBar[11] = "0000000000";

    void blink() {
      if (gapL <= 0) {
        blinkL = false;
      } else if (millis() - prevL > gapL * 100) {
        blinkL = !blinkL;
        prevL = millis();
      }

      if (gapR <= 0) {
        blinkR = false;
      } else if (millis() - prevR > gapR * 100) {
        blinkR = !blinkR;
        prevR = millis();
      }

      // Serial.print(blinkL);
      // Serial.print(" ");
      // Serial.print(blinkR);
      // Serial.println();

      output.write(0, 0, 0, blinkL);
      output.write(0, 0, 1, blinkR);
    }

  public:
    Resonance(uint8_t outputOffset, uint8_t inputOffset) {
      OUTOFF = outputOffset;
      INOFF  = inputOffset;
    }

    void init() {
      prevCheck = millis();
      prevL = millis();
      prevR = millis();

      answerIndex = random(0, 8);
      memcpy_P(answer,memResonanceAnswers[answerIndex], sizeof(answer) - 1);
      answer[3] = pgm_read_byte(&memResonanceAnswers[answerIndex][YB + 3]);
      for (int a : answer) {
        Serial.print(a);
        Serial.print(" ");
      }
    }

    int8_t check() {
      if (memcmp(answer, user, sizeof(answer)) != 0) {
        prevCheck = millis();
        return -1;
      }

      if (millis() - prevCheck > 1000) {
        return 1;
      }

      return -1;
    }

    void fini() {
      digitalWrite(PIN_DONE, 1);
      while (true) {
        // printDebug();
        debugScreen();
        output.update();
      }
    }

    void miss() {
      // output.writeRange(OUTOFF, 0, 0, 0, 7, "10101010");
    }

    void loop() {
      ledBar[user[0]] = '0';
      if (user[0] != getKnobValue(KNOB_F) / 10) requestF();
      if (user[1] != getKnobValue(KNOB_L)) requestL();
      if (user[2] != getKnobValue(KNOB_C)) requestC();
      if (user[3] != getKnobValue(KNOB_R)) requestR();
      user[0] = getKnobValue(KNOB_F) / 10;
      user[1] = getKnobValue(KNOB_L);
      user[2] = getKnobValue(KNOB_C);
      user[3] = getKnobValue(KNOB_R);
      ledBar[user[0]] = '1';

      gapL = 5 + user[0] - answer[0];
      gapR = 10 - gapL;
      if (gapL == gapR) blinkL = blinkR;
      output.writeRange(OUTOFF, -1, 2, -1, 11, ledBar);

      blink();
    }
};
