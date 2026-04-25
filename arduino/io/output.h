#include <assert.h>

class Output {
  private:
    uint8_t PIN_DATA;
    uint8_t PIN_LATCH;
    uint8_t PIN_CLK;
    uint8_t registerCount;

    uint8_t *bin;
    uint8_t *pwm;

    int dutyCount = 0;
    int dutyPositive = 1;
    uint32_t prev;

    void getSetPin(int8_t &set, uint8_t &pin) {
      if (set == -1) {
        set = pin / 8;
        pin = pin % 8;
      }
    }

    uint8_t getBinIdx(int8_t set, uint8_t pin) {
      return (set == -1) ? pin : (set * 8 + pin);
    }

  public:
    Output(uint8_t data, uint8_t latch, uint8_t clock, uint8_t regCount) {
      PIN_DATA  = data;
      PIN_LATCH = latch;
      PIN_CLK   = clock;
      registerCount = regCount;

      bin = new uint8_t[registerCount]();
      pwm = new uint8_t[registerCount]();
      prev = micros();

      pinMode(PIN_DATA , OUTPUT);
      pinMode(PIN_LATCH, OUTPUT);
      pinMode(PIN_CLK  , OUTPUT);
    }

    // update
    void simple_update() {
      //
    }

    void update() {
      //
    }

    // write
    void write(int8_t set, uint8_t pin, bool sig) {
      getSetPin(set, pin);
      bitWrite(bin[set], pin, sig);
    }

    void writeRange(int8_t set1, uint8_t pin1, int8_t set2, uint8_t pin2, const char* sig) {
      getSetPin(set1, pin1);
      getSetPin(set2, pin2);
      uint8_t offset = getBinIdx(set1, pin1);

      uint8_t bitCount = getBinIdx(set2, pin2) - getBinIdx(set1, pin1) + 1;
      assert(strlen(sig) == bitCount); // assert if false

      for (int8_t s = set1; s <= set2; s++) {
        uint8_t pinStart = (s == set1) ? pin1 : 0;
        uint8_t pinEnd   = (s == set2) ? pin2 : 7;

        for (uint8_t p = pinStart; p <= pinEnd; p++) {
          uint8_t sigIdx = bitCount - (getBinIdx(s, p) - offset) - 1;
          write(s, p, sig[sigIdx] == '1');
        }
      }
    }

    void writeAll(const char* sig) {
      writeRange(0, 0, registerCount - 1, 7, sig);
    }

    void writeAll(bool sig) {
      for (uint8_t s = 0; s < registerCount; s++) {
        bin[s] = sig ? 0xFF : 0x00;
      }
    }

    // read
    bool read(int8_t set, uint8_t pin) {
      getSetPin(set, pin);
      return bitRead(bin[set], pin);
    }

    // pwm
    void pwmSet(int8_t set, uint8_t pin, bool sig) {
      getSetPin(set, pin);
      bitWrite(pwm[set], pin, sig);
    }

    void pwmRange(int8_t set1, uint8_t pin1, int8_t set2, uint8_t pin2, const char* sig) {
      getSetPin(set1, pin1);
      getSetPin(set2, pin2);
      uint8_t offset = getBinIdx(set1, pin1);

      uint8_t bitCount = getBinIdx(set2, pin2) - getBinIdx(set1, pin1) + 1;
      assert(strlen(sig) == bitCount); // assert if false

      for (int8_t s = set1; s <= set2; s++) {
        uint8_t pinStart = (s == set1) ? pin1 : 0;
        uint8_t pinEnd   = (s == set2) ? pin2 : 7;

        for (uint8_t p = pinStart; p <= pinEnd; p++) {
          uint8_t sigIdx = bitCount - (getBinIdx(s, p) - offset) - 1;
          pwmSet(s, p, sig[sigIdx] == '1');
        }
      }
    }

    void pwmAll(const char* sig) {
      pwmRange(0, 0, registerCount - 1, 7, sig);
    }

    void pwmAll(bool sig) {
      for (uint8_t s = 0; s < registerCount; s++) {
        pwm[s] = sig ? 0xFF : 0x00;
      }
    }
};
