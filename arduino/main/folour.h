
#define R 4
#define G 2
#define B 1

const uint8_t memPatterns[][4][6] PROGMEM = {
  {
    {G, R, B, B},
    {R, B, R, B, G},
    {R, R, B, G, G},
    {B, B, R, G},
  },
  {
    {G, R, G, B},
    {R, B, G},
    {G, R, B, R, G},
    {G, B, R},
  },
  {
    {R, G, B, G, R},
    {B, G, R, G, G},
    {B, R, G, B},
    {G, R, R, B},
  },
  {
    {R, G, B, B},
    {R, G, B, G, B},
    {B, R, G, R},
    {G, G, R, G, R},
  }
};

const uint8_t memLength[][4] PROGMEM = {
  {4, 5, 5, 4},
  {4, 3, 5, 3},
  {5, 5, 4, 4},
  {4, 5, 4, 5}
};

const uint8_t memAnswers[][4] PROGMEM = {
  {R, R, B, G},
  {G, R, B, G},
  {R, B, R, B},
  {R, G, G, R}
};

class Folour {
  private:
    uint8_t INOFF;
    uint8_t OUTOFF;

    uint8_t patterns[4][6];
    uint8_t patternLen[4];
    uint8_t prevPattern[4];
    uint8_t answer[4];
    uint8_t fixed = 0b0000;
    int colourIdx = 0;

    unsigned long duration = 250;
    unsigned long now;
    unsigned long prev;
    unsigned long blinkPrev;

    void swap(uint8_t &a, uint8_t &b) {
      const int temp = a;
      a = b;
      b = temp;
    }

  public:
    Folour(uint8_t outputOffset, uint8_t inputOffset) {
      INOFF  = inputOffset;
      OUTOFF = outputOffset;
    }

    void init() {
      prev = millis();
      blinkPrev = millis();

      memcpy_P(patterns,   memPatterns[YB], sizeof(patterns));
      memcpy_P(answer,     memAnswers[YB] , sizeof(answer));
      memcpy_P(patternLen, memLength[YB]  , sizeof(patternLen));

      for (int i=0; i<4; i++) {
        const int length = patternLen[i];
        const int idx = random(0, length);
        swap(patterns[i][idx], patterns[i][length]);
        patternLen[i]++;
      }

      output.writeRange(OUTOFF, 1, 4, 1, 5, "11");
    }

    int8_t check() {
      if (fixed == 0b1111) {
        return ( memcmp(answer, prevPattern, sizeof(answer)) == 0 );
      }
      return -1;
    }

    void fini() {
      output.writeRange(OUTOFF, 1, 4, 1, 5, "01");
    }

    void miss() {
      output.writeRange(OUTOFF, 1, 4, 1, 5, "10");
    }

    void loop() {
      now = millis();
      
      if (now - prev >= duration) {
        char bin[17] = "000000000000";
        for (int ledIdx=0; ledIdx<4; ledIdx++) {
          for (int digitIdx=0; digitIdx<3; digitIdx++) {
            int idx = 3 * ledIdx + digitIdx;
            if (!bitRead(fixed, ledIdx))
              bin[idx] = (
                (patterns[ledIdx][ colourIdx % patternLen[ledIdx] ] >> digitIdx)
                & 1) + '0';
            else
              bin[idx] = ((prevPattern[ledIdx] >> digitIdx) & 1) + '0';
          }
          if (!bitRead(fixed, ledIdx))
            prevPattern[ledIdx] = patterns[ledIdx][ colourIdx % patternLen[ledIdx] ];
        }

        output.writeRange(OUTOFF, 0, 0, 1, 3, bin);

        colourIdx = (colourIdx + 1) % 60;
        prev += duration;
      }

      for (int btnIdx=0; btnIdx < 4; btnIdx++) {
        if (input.readPressed(0, -1, btnIdx)) {
          bitWrite(fixed, btnIdx, !bitRead(fixed, btnIdx));
        }
      }
    }
};
