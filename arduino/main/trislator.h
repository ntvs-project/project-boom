
enum segmentEnum { D, D0, D1, D2, D3, D4, D5, D6, D7, D8, D9, DA, DC, DI, DN, DO, DS, DT, DDEG, DEQ, D_ };
enum buttonEnum  { BTN1, BTN2, BTN3, BTNA, BTN4, BTN5, BTN6, BTNB, BTN7, BTN8, BTN9, BTNC, BTNK, BTN0, BTNH, BTND };
enum stateEnum { ON, OFF };
const char segment[][7] = {
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
  "1001110", // DC
  "0110000", // DI
  "0010101", // DN
  "0011101", // DO
  "1011011", // DS
  "0001111", // DT
  "1100011", // DDEG
  "0001001", // DEQ
  "0001000", // D_
};

const int memTrislatorMathPatterns[][2][5] PROGMEM = {
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

const int memTrislatorVocabPatterns[][2][5] PROGMEM = {
  { { DS, DO, DN, DI, D_ }, { DC } },
};

class Trislator {
  private:
    uint8_t INOFF;
    uint8_t OUTOFF;

    const int BTN_LIST[10] = { BTN0, BTN1, BTN2, BTN3, BTN4, BTN5, BTN6, BTN7, BTN8, BTN9 };
    const int DIGIT_LIST[10] = { D0, D1, D2, D3, D4, D5, D6, D7, D8, D9 };

    int pattern[5];
    int answer[4] = { D0, D1, D4, D1 };
    int user[4];
    int inputIdx = 0;
    int prevButtonState[16];
    int buttonState[16];
    int rdNumber;

    void muxButtons() {
      for (int i=0; i < 4; i++) {
        output.write(OUTOFF, 2, 3 + i, false);
        output.simpleUpdate();
        input.update();
        
        for (int j=0; j < 4; j++) {
          prevButtonState[4 * i + j] = buttonState[4 * i + j];
          buttonState[4 * i + j] = input.readRaw(INOFF, 0, j);
        }
        
        output.write(OUTOFF, 2, 3 + i, true);
        output.simpleUpdate();
      }
    }

    bool checkButton(int btn) {
      return (!buttonState[btn] && prevButtonState[btn]);
    }

    void updateInput() {
      if (checkButton(BTNH)) {
        if (user[0] == answer[0] &&
            user[1] == answer[1] &&
            user[2] == answer[2] &&
            user[3] == answer[3]) {
          output.write(OUTOFF, 2, 7, true);
        }
      }

      if (checkButton(BTNK)) {
        if (inputIdx <= 0) return;
        inputIdx--;
        user[inputIdx] = D;
      }

      if (inputIdx >= 4) return;
      for (int i=0; i<10; i++) {
        if (checkButton(BTN_LIST[i])) {
          user[inputIdx] = DIGIT_LIST[i];
          inputIdx++;
        }
      }
    }

    void setSegment(segmentEnum digit, bool dp=false) {
      output.writeRange(OUTOFF, 0, 0, 0, 6, segment[digit]);
      output.write(OUTOFF, 0, 7, dp);
    }

    void setDigit(int digit, bool state) {
      output.write(OUTOFF, -1, digit + 8, state);
    }

    void show(segmentEnum S1, segmentEnum S2, segmentEnum S3, segmentEnum Q1, segmentEnum Q2, segmentEnum A1, segmentEnum A2, segmentEnum A3, segmentEnum A4) {
      setSegment(S1);
      setDigit(0, ON);
      output.simpleUpdate();
      delayMicroseconds(200);
      setDigit(0, OFF);

      setSegment(S2);
      setDigit(1, ON);
      output.simpleUpdate();
      delayMicroseconds(200);
      setDigit(1, OFF);

      setSegment(S3);
      setDigit(2, ON);
      output.simpleUpdate();
      delayMicroseconds(200);
      setDigit(2, OFF);

      setSegment(Q1);
      setDigit(3, ON);
      output.simpleUpdate();
      delayMicroseconds(200);
      setDigit(3, OFF);

      setSegment(Q2);
      setDigit(4, ON);
      output.simpleUpdate();
      delayMicroseconds(200);
      setDigit(4, OFF);

      setSegment(DDEG);
      setDigit(5, ON);
      output.simpleUpdate();
      delayMicroseconds(200);
      setDigit(5, OFF);

      setSegment(DEQ);
      setDigit(6, ON);
      output.simpleUpdate();
      delayMicroseconds(200);
      setDigit(6, OFF);

      setSegment(A1, true);
      setDigit(7, ON);
      output.simpleUpdate();
      delayMicroseconds(200);
      setDigit(7, OFF);

      setSegment(A2);
      setDigit(8, ON);
      output.simpleUpdate();
      delayMicroseconds(200);
      setDigit(8, OFF);

      setSegment(A3);
      setDigit(9, ON);
      output.simpleUpdate();
      delayMicroseconds(200);
      setDigit(9, OFF);

      setSegment(A4);
      setDigit(10, ON);
      output.simpleUpdate();
      delayMicroseconds(200);
      setDigit(10, OFF);
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

      rdNumber = random(0, 10);
      memcpy_P(pattern, memTrislatorMathPatterns[rdNumber][0], sizeof(pattern));
      memcpy_P(answer,  memTrislatorMathPatterns[rdNumber][1], sizeof(answer));
    }

    int8_t check() {
    }

    void fini() {
    }

    void miss() {
    }

    void loop() {
      muxButtons();
      updateInput();
      show(pattern[0], pattern[1], pattern[2], pattern[3], pattern[4],
           user[0], user[1], user[2], user[3]);
      // 
    }
};
