
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
  "1111000", // DJ
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

const int memTrislatorTrigPatternsLength = 10;
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
};

const int memTrislatorVocabPatternsLength = 4;
const int memTrislatorVocabPatterns[][2][5] PROGMEM = {
  { { DP, DL, D_, DN, DT }, { DA } },
  { { D_, DL, DI, DN, DD }, { DB } },
  { { DS, DO, DN, DI, D_ }, { DC } },
  { { DF, DO, DU, DN, D_ }, { DD } },
};

class Trislator {
  private:
    uint8_t INOFF;
    uint8_t OUTOFF;

    const int DIGIT_BTN_LIST[10] = { BTN0, BTN1, BTN2, BTN3, BTN4, BTN5, BTN6, BTN7, BTN8, BTN9 };
    const int DIGIT_LIST[10] = { D0, D1, D2, D3, D4, D5, D6, D7, D8, D9 };
    const int CHAR_BTN_LIST[4] = { BTNA, BTNB, BTNC, BTND };
    const int CHAR_LIST[4] = { DA, DB, DC, DD };

    int phase = 1;
    int patternTrig[5];
    int patternVocab[5];
    int answer[5] = { D0, D1, D4, D1, DA };
    int user[5] = { D_, D_, D_, D_, D_ };
    int inputIdx = 0;
    int prevButtonState[16];
    int buttonState[16];
    int trigIdx, vocabIdx;

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
        if (user[0] == answer[0] &&
            user[1] == answer[1] &&
            user[2] == answer[2] &&
            user[3] == answer[3]) {
          // output.write(OUTOFF, 2, 7, true);
          phase = 2;
        }
      }

      if (checkButton(BTNK)) {
        if (inputIdx <= 0) return;
        inputIdx--;
        user[inputIdx] = D_;
      }

      for (int i=0; i<4; i++) {
        if (checkButton(CHAR_BTN_LIST[i])) {
          user[inputIdx] = CHAR_LIST[i];
          inputIdx++;
        }
      }

      if (inputIdx >= 4) return;
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

    void show(int S1, int S2, int S3, int Q1, int Q2, int DEG, int A1, int A2, int A3, int A4) {
      setSegment(S1); setDigit(0, DIGIT_ON); output.simpleUpdate(); delayMicroseconds(200); setDigit(0, DIGIT_OFF);
      setSegment(S2); setDigit(1, DIGIT_ON); output.simpleUpdate(); delayMicroseconds(200); setDigit(1, DIGIT_OFF);
      setSegment(S3); setDigit(2, DIGIT_ON); output.simpleUpdate(); delayMicroseconds(200); setDigit(2, DIGIT_OFF);
      setSegment(Q1); setDigit(3, DIGIT_ON); output.simpleUpdate(); delayMicroseconds(200); setDigit(3, DIGIT_OFF);
      setSegment(Q2); setDigit(4, DIGIT_ON); output.simpleUpdate(); delayMicroseconds(200); setDigit(4, DIGIT_OFF);
      setSegment(DEG); setDigit(5, DIGIT_ON); output.simpleUpdate(); delayMicroseconds(200); setDigit(5, DIGIT_OFF);

      setSegment(DEQ); setDigit(6, DIGIT_ON); output.simpleUpdate(); delayMicroseconds(200); setDigit(6, DIGIT_OFF);
      setSegment(A1, true); setDigit(7, DIGIT_ON); output.simpleUpdate(); delayMicroseconds(200); setDigit(7, DIGIT_OFF);
      setSegment(A2); setDigit(8, DIGIT_ON); output.simpleUpdate(); delayMicroseconds(200); setDigit(8, DIGIT_OFF);
      setSegment(A3); setDigit(9, DIGIT_ON); output.simpleUpdate(); delayMicroseconds(200); setDigit(9, DIGIT_OFF);
      setSegment(A4); setDigit(10, DIGIT_ON); output.simpleUpdate(); delayMicroseconds(200); setDigit(10, DIGIT_OFF);
    }

  public:
    Trislator(uint8_t outputOffset, uint8_t inputOffset) {
      OUTOFF = outputOffset;
      INOFF  = inputOffset;
    }

    void init() {
      output.writeRange(OUTOFF, -1, 8, -1, 19, "11111111111");
      output.writeRange(OUTOFF, 2, 3, 2, 6, "1111");
      output.write(OUTOFF, 2, 3 + 0, false);

      // output.write(OUTOFF, 2, 7, true);

      trigIdx  = random(0, memTrislatorTrigPatternsLength);
      vocabIdx = random(0, memTrislatorVocabPatternsLength);
      memcpy_P(patternTrig,  memTrislatorTrigPatterns[trigIdx][0],   5 * sizeof(int));
      memcpy_P(answer,       memTrislatorTrigPatterns[trigIdx][1],   4 * sizeof(int));
      memcpy_P(patternVocab, memTrislatorVocabPatterns[vocabIdx][0], 5 * sizeof(int));
      memcpy_P(answer + 4,   memTrislatorVocabPatterns[vocabIdx][1], 1 * sizeof(int));

      for (int a : answer) {
        Serial.print(a-1);
        Serial.print(" ");
      }
      Serial.println();
    }

    int8_t check() {
    }

    void fini() {
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
          D,
          user[0],
          user[1],
          user[2],
          user[3]
        );
      }
    }
};
