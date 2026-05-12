
const uint8_t memMatrixPatterns[4][16] PROGMEM = {
  {
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 1,
    1, 0, 1, 1
  },
  {
    0, 0, 0, 0,
    0, 0, 1, 1,
    1, 1, 0, 0,
    0, 0, 0, 0
  },
  {
    1, 0, 0, 0,
    1, 1, 0, 0,
    0, 0, 0, 0,
    0, 1, 0, 0
  },
  {
    0, 1, 1, 1,
    0, 0, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 0
  }
};

const uint8_t memMatrixOrders[4][2] PROGMEM = {
  {2, 3},
  {3, 2},
  {2, 1},
  {1, 2}
};

class Matrix {
  private:
    uint8_t INOFF;
    uint8_t OUTOFF;

    uint8_t patterns[16];
    uint8_t order[2];
    uint8_t answer[16];

    uint8_t idx = 0;
    uint8_t n = 1;
    unsigned long prev;

  public:
    Matrix(uint8_t outputOffset, uint8_t inputOffset) {
      INOFF  = inputOffset;
      OUTOFF = outputOffset;
    }

    void init() {
      prev = millis();
      pinMode(4, INPUT_PULLUP);

      memcpy_P(patterns, memMatrixPatterns[2], sizeof(patterns));
      memcpy_P(order   , memMatrixOrders[2]  , sizeof(order));
    }

    int8_t check() {
      if (idx == 1) {
        if (memcmp(patterns, answer, sizeof(patterns)) == 0) {
          return true;
        }
      }
      return -1;
    }

    void fini() {
      // output.writeRange(OUTOFF, 1, 4, 1, 5, "01");
    }

    void miss() {
      // output.writeRange(OUTOFF, 1, 4, 1, 5, "10");
    }

    void loop() {
      output.write(OUTOFF, 0, 0, 1);
      output.write(OUTOFF, 0, 1, bitRead(n, 1));
      output.write(OUTOFF, 0, 2, bitRead(n, 0));
      output.write(OUTOFF, 0, 3, !0);

      if (digitalRead(4) == 0) {
        delay(20);
        while (digitalRead(4) == 0) {}
        n = (n + 1) % 4;
      }
    }
};
