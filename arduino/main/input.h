#include <assert.h>

class Input {
  private:
    uint8_t PIN_DATA;
    uint8_t PIN_LATCH;
    uint8_t PIN_CLK;
    uint8_t registerCount;

    uint8_t *binReleased;   // latched until consumed by readReleased()
    uint8_t *binHold;       // unused for now, kept for future per-bit hold caching if needed
    uint8_t *curr;
    uint8_t *prev;
    uint8_t *holdFired;     // bitmask: has hold already fired for this press?
    unsigned long **debounceStart;

    unsigned long holdT = 500;

    void getSetPin(uint8_t INOFF, int8_t &set, uint8_t &pin) {
      if (set == -1) {
        set = pin / 8;
        pin = pin % 8;
      }

      set += INOFF;
      assert(set >= 0 && set < registerCount);
    }

  public:
    Input(uint8_t data, uint8_t latch, uint8_t clock, uint8_t regCount) {
      PIN_DATA  = data;
      PIN_LATCH = latch;
      PIN_CLK   = clock;
      registerCount = regCount;

      binReleased = new uint8_t[registerCount]();
      binHold     = new uint8_t[registerCount]();
      curr        = new uint8_t[registerCount]();
      prev        = new uint8_t[registerCount]();
      holdFired   = new uint8_t[registerCount]();
      debounceStart = new unsigned long*[registerCount]();
      for (int i = 0; i < registerCount; i++) {
        debounceStart[i] = new unsigned long[8]();
      }

      pinMode(PIN_DATA , INPUT);
      pinMode(PIN_LATCH, OUTPUT);
      pinMode(PIN_CLK  , OUTPUT);
    }

    void update() {
      digitalWrite(PIN_LATCH, 0);
      delayMicroseconds(5);
      digitalWrite(PIN_LATCH, 1);

      for (int i = 0; i < registerCount; i++) {
        curr[i] = 0;

        for (int b = 0; b < 8; b++) {
          bitWrite(curr[i], b, digitalRead(PIN_DATA));
          bool currBit = bitRead(curr[i], b);
          bool prevBit = bitRead(prev[i], b);

          if (currBit != prevBit) {
            if (currBit == 0 && debounceStart[i][b]) {
              // released: only count as a "release event" if it wasn't a hold
              unsigned long heldFor = millis() - debounceStart[i][b];
              if (heldFor >= 20 && heldFor < holdT) {
                bitWrite(binReleased[i], b, 1);
              }
              debounceStart[i][b] = 0;
              bitWrite(holdFired[i], b, 0);
            }
            if (currBit == 1 && !debounceStart[i][b]) {
              debounceStart[i][b] = millis();
              bitWrite(holdFired[i], b, 0);
            }
            bitWrite(prev[i], b, currBit);
          }

          digitalWrite(PIN_CLK, 1);
          digitalWrite(PIN_CLK, 0);
        }
      }
    }

    // read

    bool readReleased(uint8_t INOFF, int8_t set, uint8_t pin) {
      getSetPin(INOFF, set, pin);
      pin = 7 - pin;
      bool result = bitRead(binReleased[set], pin);
      bitWrite(binReleased[set], pin, 0); // consume: only reported once
      return result;
    }

    // Edge-triggered: returns true once when the hold threshold is first crossed.
    // Button stays "pressed" (debounceStart untouched) so release still fires normally.
    bool readHold(uint8_t INOFF, int8_t set, uint8_t pin) {
      getSetPin(INOFF, set, pin);
      pin = 7 - pin;

      if (debounceStart[set][pin] &&
          !bitRead(holdFired[set], pin) &&
          millis() - debounceStart[set][pin] >= holdT) {
        bitWrite(holdFired[set], pin, 1);
        return true;
      }
      return false;
    }

    uint8_t raw(uint8_t i) { return curr[i]; }
};