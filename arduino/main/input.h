#include <assert.h>

class Input {
  private:
    uint8_t PIN_DATA;
    uint8_t PIN_LATCH;
    uint8_t PIN_CLK;
    uint8_t registerCount;

    uint8_t *bin;
    uint8_t *binHold;
    uint8_t *curr;
    uint8_t *prev;
    unsigned long **debounceStart, **holdStart;

    unsigned long holdT = 500;

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
    Input(uint8_t data, uint8_t latch, uint8_t clock, uint8_t regCount) {
      PIN_DATA  = data;
      PIN_LATCH = latch;
      PIN_CLK   = clock;
      registerCount = regCount;

      bin      = new uint8_t[registerCount]();
      binHold  = new uint8_t[registerCount]();
      curr     = new uint8_t[registerCount]();
      prev     = new uint8_t[registerCount]();
      debounceStart = new unsigned long*[registerCount]();
      holdStart     = new unsigned long*[registerCount]();
      for (int i = 0; i < registerCount; i++) {
        debounceStart[i] = new unsigned long[8]();
        holdStart[i]     = new unsigned long[8]();
      }

      pinMode(PIN_DATA , INPUT);
      pinMode(PIN_LATCH, OUTPUT);
      pinMode(PIN_CLK  , OUTPUT);
    }

    // update
    void update() {
      digitalWrite(PIN_LATCH, 0);
      delayMicroseconds(5);
      digitalWrite(PIN_LATCH, 1);

      for (int i=0; i < registerCount; i++) {
        prev[i] = curr[i];
        curr[i] = 0;

        for (int b=0; b < 8; b++) {
          bitWrite(curr[i], b, digitalRead(PIN_DATA));
          bool currBit = bitRead(curr[i], b);
          bool prevBit = bitRead(prev[i], b);

          if (currBit != prevBit && !debounceStart[i][b]) {
            debounceStart[i][b] = millis();
          }
          if (holdStart[i][b] && millis() - holdStart[i][b] > holdT) {
            bitWrite(binHold[i], b, 1);
          }

          if (debounceStart[i][b] && millis() - debounceStart[i][b] > 20) {
            debounceStart[i][b] = 0;

            if (currBit) {
              bitWrite(bin[i], b, 1);
              if (!holdStart[i][b]) {
                holdStart[i][b] = millis();
              }
            } else {
              holdStart[i][b] = 0;  
              bitWrite(bin[i], b, 0);
              bitWrite(binHold[i], b, 0);
            }
          }

          digitalWrite(PIN_CLK, 1);
          digitalWrite(PIN_CLK, 0);
        }
      }
    }

    // read
    bool readPressed(int8_t set, uint8_t pin) {
      getSetPin(set, pin);
      bool val = bitRead(bin[set], 7 - pin);
      bitClear(bin[set], 7 - pin);
      return val;
    }

    bool readHold(int8_t set, uint8_t pin) {
      getSetPin(set, pin);
      return bitRead(binHold[set], 7 - pin);
    }

}; Input input(11, 12, 13, 1);
