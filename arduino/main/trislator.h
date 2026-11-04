
enum segmentEnum { D, D0, D1, D2, D3, D4, D5, D6, D7, D8, D9, DA, DB, DC, DD, DE, DF, DG, DH, DI, DJ, DL, DN, DO, DP, DQ, DS, DT, DU, DY, DZ, DDEG, DEQ, D_ };
enum buttonEnum  { BTN1, BTN2, BTN3, BTNA, BTN4, BTN5, BTN6, BTNB, BTN7, BTN8, BTN9, BTNC, BTNK, BTN0, BTNH, BTND };
enum digitStateEnum { DIGIT_ON, DIGIT_OFF };
const char segment[][8] = {
  "0000000", // D
  "1111110", // D0
  "0110000", // D1
  "1101101", // D2
  "1111001", // D3
  "0110011", // D4
  "1011011", // D5
  "1011111", // D6
  "1110000", // D7
  "1111111", // D8
  "1111011", // D9

  "1110111", // DA
  "0011111", // DB
  "1001110", // DC
  "0111101", // DD
  "1001111", // DE
  "1000111", // DF
  "1111011", // DG
  "0110111", // DH
  "0110000", // DI
  "0111000", // DJ
  "0001110", // DL
  "0010101", // DN
  "0011101", // DO
  "1100111", // DP
  "1110011", // DQ
  "1011011", // DS
  "0001111", // DT
  "0111110", // DU
  "0111011", // DY
  "1101101", // DZ

  "1100011", // DDEG
  "0001001", // DEQ
  "0001000", // D_
};

const int memTrislatorTrigPatternsLength = 30;
const int memTrislatorTrigPatterns[][2][5] PROGMEM = {
  { { DS, DI, DN, D0, D3 }, { D0, D0, D5, D2 } },
  { { DS, DI, DN, D1, D1 }, { D0, D1, D9, D1 } },
  { { DS, DI, DN, D1, D8 }, { D0, D3, D0, D9 } },
  { { DS, DI, DN, D2, D7 }, { D0, D4, D5, D4 } },
  { { DS, DI, DN, D3, D5 }, { D0, D5, D7, D4 } },
  { { DS, DI, DN, D4, D2 }, { D0, D6, D6, D9 } },
  { { DS, DI, DN, D5, D1 }, { D0, D7, D7, D7 } },
  { { DS, DI, DN, D5, D9 }, { D0, D8, D5, D7 } },
  { { DS, DI, DN, D6, D6 }, { D0, D9, D1, D4 } },
  { { DS, DI, DN, D7, D3 }, { D0, D9, D5, D6 } },

  { { DT, DA, DN, D0, D8 }, { D0, D1, D4, D1 } },
  { { DT, DA, DN, D1, D5 }, { D0, D2, D6, D8 } },
  { { DT, DA, DN, D2, D3 }, { D0, D4, D2, D4 } },
  { { DT, DA, DN, D3, D1 }, { D0, D6, D0, D1 } },
  { { DT, DA, DN, D3, D9 }, { D0, D8, D1, D0 } },
  { { DT, DA, DN, D4, D6 }, { D1, D0, D3, D6 } },
  { { DT, DA, DN, D5, D4 }, { D1, D3, D7, D6 } },
  { { DT, DA, DN, D6, D1 }, { D1, D8, D0, D4 } },
  { { DT, DA, DN, D6, D9 }, { D2, D6, D0, D5 } },
  { { DT, DA, DN, D7, D6 }, { D4, D0, D1, D1 } },

  { { DS, DE, DC, D2, D9 }, { D1, D1, D4, D3 } },
  { { DS, DE, DC, D3, D0 }, { D1, D1, D5, D5 } },
  { { DS, DE, DC, D3, D4 }, { D1, D2, D0, D6 } },
  { { DS, DE, DC, D4, D0 }, { D1, D3, D0, D5 } },
  { { DS, DE, DC, D4, D3 }, { D1, D3, D6, D7 } },
  { { DS, DE, DC, D5, D5 }, { D1, D7, D4, D3 } },
  { { DS, DE, DC, D6, D5 }, { D2, D3, D6, D6 } },
  { { DS, DE, DC, D6, D7 }, { D2, D5, D5, D9 } },
  { { DS, DE, DC, D7, D0 }, { D2, D9, D2, D4 } },
  { { DS, DE, DC, D7, D7 }, { D4, D4, D4, D5 } },
};

const int memTrislatorVocabPatternsLength = 8;
const int memTrislatorVocabPatterns[][2][6] PROGMEM = {
  // { { DP, DL, D_, DN, DT }, { DA } },
  // { { D_, DL, DI, DN, DD }, { DB } },
  // { { DS, DO, DN, DI, D_ }, { DC } },
  // { { DF, DO, DU, DN, D_ }, { DD } },
  { { DP, DI, DZ, DZ, D_, D  }, { DA } },
  { { D_, DP, DP, DL, DE, D  }, { DA } },
  { { DP, D_, DU, DS, DE, D  }, { DA } },
  { { D_, DA, DS, DI, DC, D  }, { DB } },
  { { DT, DA, D_, DL, DE, D  }, { DB } },
  { { DJ, DU, DI, D_, DE, D  }, { DC } },
  { { DE, DF, DF, DE, D_, DT }, { DC } },
  { { D_, DO, DU, DP, DL, DE }, { DC } },
};

class Trislator {
  private:
    uint8_t INOFF;
    uint8_t OUTOFF;

    const int DIGIT_BTN_LIST[10] = { BTN0, BTN1, BTN2, BTN3, BTN4, BTN5, BTN6, BTN7, BTN8, BTN9 };
    const int DIGIT_LIST[10] = { D0, D1, D2, D3, D4, D5, D6, D7, D8, D9 };
    const int CHAR_BTN_LIST[4] = { BTNA, BTNB, BTNC, BTND };
    const int CHAR_LIST[4] = { DA, DB, DC, DD };

    const int MAX_ITERATION = 5;
    int iteration = 0;
    int phase = 1;
    int patternTrig[5];
    int patternVocab[6];
    int answer[5] = { D0, D1, D4, D1, DA };
    int user[5] = { D_, D_, D_, D_, D_ };
    int inputIdx = 0;
    int prevButtonState[16];
    int buttonState[16];
    int trigIdx[5], vocabIdx[5]; // MAX_ITERATION

    void updateInput() {
      for (int i=0; i < 4; i++) {
        output.write(OUTOFF, 2, 3 + i, false);
        output.simpleUpdate();
        delayMicroseconds(100);
        input.update();
        
        for (int j=0; j < 4; j++) {
          prevButtonState[4 * i + j] = buttonState[4 * i + j];
          buttonState[4 * i + j] = input.readRaw(INOFF, 0, j);
          // Serial.print(input.readRaw(INOFF, 0, j));
        }
        
        output.write(OUTOFF, 2, 3 + i, true);
        output.simpleUpdate();

        // Serial.print(" ");
      }
      // Serial.println();
    }

    bool checkButton(int btn) {
      return (!buttonState[btn] && prevButtonState[btn]);
    }

    void processInput() {
      if (checkButton(BTNH)) {
        if (phase == 1 &&
            user[0] == answer[0] &&
            user[1] == answer[1] &&
            user[2] == answer[2] &&
            user[3] == answer[3]) {
          phase = 2;
        }
        if (phase == 2 && user[4] == answer[4]) {
          phase = 1;
          iteration++;
          initPhase();
        }
      }

      if (checkButton(BTNK)) {
        if (inputIdx <= (phase == 1 ? 0 : 4)) return;
        inputIdx--;
        user[inputIdx] = D_;
      }

      if (inputIdx >= (phase == 1 ? 4 : 5)) return;

      for (int i=0; i<4; i++) {
        if (checkButton(CHAR_BTN_LIST[i])) {
          user[inputIdx] = CHAR_LIST[i];
          inputIdx++;
        }
      }

      for (int i=0; i<10; i++) {
        if (checkButton(DIGIT_BTN_LIST[i])) {
          user[inputIdx] = DIGIT_LIST[i];
          inputIdx++;
        }
      }
    }

    void setSegment(int digit, bool dp=false) {
      output.writeRange(OUTOFF, 0, 0, 0, 6, segment[digit]);
      output.write(OUTOFF, 0, 7, dp);
    }

    void setDigit(int digit, bool state) {
      output.write(OUTOFF, -1, digit + 8, state);
    }

    void show(int S1, int S2, int S3, int Q1, int Q2, int DEG, int A1, int A2, int A3, int A4, bool showSymbol=true) {
      setSegment(S1); setDigit(0, DIGIT_ON); output.simpleUpdate(); delayMicroseconds(200); setDigit(0, DIGIT_OFF);
      setSegment(S2); setDigit(1, DIGIT_ON); output.simpleUpdate(); delayMicroseconds(200); setDigit(1, DIGIT_OFF);
      setSegment(S3); setDigit(2, DIGIT_ON); output.simpleUpdate(); delayMicroseconds(200); setDigit(2, DIGIT_OFF);
      setSegment(Q1); setDigit(3, DIGIT_ON); output.simpleUpdate(); delayMicroseconds(200); setDigit(3, DIGIT_OFF);
      setSegment(Q2); setDigit(4, DIGIT_ON); output.simpleUpdate(); delayMicroseconds(200); setDigit(4, DIGIT_OFF);

      if (showSymbol) {
        setSegment(DEG); setDigit(5, DIGIT_ON); output.simpleUpdate(); delayMicroseconds(200); setDigit(5, DIGIT_OFF);
        setSegment(DEQ); setDigit(6, DIGIT_ON); output.simpleUpdate(); delayMicroseconds(200); setDigit(6, DIGIT_OFF);
      }

      setSegment(A1, showSymbol); setDigit(7, DIGIT_ON); output.simpleUpdate(); delayMicroseconds(200); setDigit(7, DIGIT_OFF);
      setSegment(A2); setDigit(8, DIGIT_ON); output.simpleUpdate(); delayMicroseconds(200); setDigit(8, DIGIT_OFF);
      setSegment(A3); setDigit(9, DIGIT_ON); output.simpleUpdate(); delayMicroseconds(200); setDigit(9, DIGIT_OFF);
      setSegment(A4); setDigit(10, DIGIT_ON); output.simpleUpdate(); delayMicroseconds(200); setDigit(10, DIGIT_OFF);
    }

    void initPhase() {
      for (int i=0; i < 5; i++) user[i] = D_;
      inputIdx = 0;

      memcpy_P(patternTrig,  memTrislatorTrigPatterns[trigIdx[iteration]][0],   5 * sizeof(int));
      memcpy_P(answer,       memTrislatorTrigPatterns[trigIdx[iteration]][1],   4 * sizeof(int));
      memcpy_P(patternVocab, memTrislatorVocabPatterns[vocabIdx[iteration]][0], 6 * sizeof(int));
      memcpy_P(answer + 4,   memTrislatorVocabPatterns[vocabIdx[iteration]][1], 1 * sizeof(int));

      for (int i=0; i < 4; i++) {
        Serial.print(answer[i] - 1);
        Serial.print(" ");
      }
      Serial.write(answer[4] - 1 + 55);
      Serial.println();
    }

  public:
    bool finished = false;

    Trislator(uint8_t outputOffset, uint8_t inputOffset) {
      OUTOFF = outputOffset;
      INOFF  = inputOffset;
    }

    void init() {
      output.writeRange(OUTOFF, -1, 8, -1, 19, "11111111111");
      output.writeRange(OUTOFF, 2, 3, 2, 6, "1111");
      output.write(OUTOFF, 2, 3 + 0, false);

      for (int i=0; i<MAX_ITERATION; i++) {
        bool duplicate;

        do {
          trigIdx[i] = random(0, memTrislatorTrigPatternsLength);

          duplicate = false;
          for (int j=0; j<i; j++) {
            if (trigIdx[i] == trigIdx[j]) {
              duplicate = true;
              break;
            }
          }
        } while (duplicate);
      }

      for (int i=0; i<MAX_ITERATION; i++) {
        bool duplicate;

        do {
          vocabIdx[i] = random(0, memTrislatorVocabPatternsLength);

          duplicate = false;
          for (int j=0; j<i; j++) {
            if (vocabIdx[i] == vocabIdx[j]) {
              duplicate = true;
              break;
            }
          }
        } while (duplicate);
      }

      initPhase();
    }

    int8_t check() {
      if (iteration == MAX_ITERATION) {
        return 1;
      }
      return -1;
    }

    void fini() {
      show(
        D, D, D, D, D, D,
        D, D, D, D, false
      );
      output.write(OUTOFF, 2, 7, true);
      finished = true;
    }

    void miss() {
    }

    void loop() {
      updateInput();
      processInput();
      if (phase == 1) {
        show(
          patternTrig[0],
          patternTrig[1],
          patternTrig[2],
          patternTrig[3],
          patternTrig[4],
          DDEG,
          user[0],
          user[1],
          user[2],
          user[3]
        );
      } else if (phase == 2) {
        show(
          (patternVocab[0] == D_) ? user[4] : patternVocab[0],
          (patternVocab[1] == D_) ? user[4] : patternVocab[1],
          (patternVocab[2] == D_) ? user[4] : patternVocab[2],
          (patternVocab[3] == D_) ? user[4] : patternVocab[3],
          (patternVocab[4] == D_) ? user[4] : patternVocab[4],
          (patternVocab[5] == D_) ? user[4] : patternVocab[5],
          user[0],
          user[1],
          user[2],
          user[3]
        );
      }
    }
};
