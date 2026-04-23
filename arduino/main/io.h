
class Output {
  private:
    uint8_t PIN_SER;
    uint8_t PIN_LK;
    uint8_t PIN_CLK;
    uint8_t registerCount;

    unsigned int bin = 0;
    uint8_t len = sizeof(bin) * 8 - 1;

  public:
    Output(uint8_t ser, uint8_t lk, uint8_t clk, uint8_t regCount) {
      PIN_SER = ser;
      PIN_LK  = lk;
      PIN_CLK = clk;
      registerCount = regCount;

      pinMode(PIN_SER, OUTPUT);
      pinMode(PIN_LK , OUTPUT);
      pinMode(PIN_CLK, OUTPUT);
    }

    void write(int pin, int signal_) {
      bitWrite(bin, len - pin, signal_);
    }

    uint8_t read(int pin) {
      return bitRead(bin, len - pin);
    }

    void update() {
      digitalWrite(PIN_LK, 0);
      for (int i=0; i<registerCount; i++) {
        shiftOut(PIN_SER, PIN_CLK, LSBFIRST, (bin >> (8 * i)) & 0xFF);
      }
      digitalWrite(PIN_LK, 1);
    }
}; Output output(8, 9, 10, 2);

class Input {
  private:
    uint8_t PIN_DATA;
    uint8_t PIN_LATCH;
    uint8_t PIN_CLK;
    uint8_t registerCount;
    
    uint8_t* bin;
    uint8_t* last;
    uint8_t* lastDelay;

    const uint16_t debounceDelay = 20;

  public:
    Input(uint8_t data, uint8_t latch, uint8_t clk, uint8_t regCount) {
      PIN_DATA = data;
      PIN_LATCH  = latch;
      PIN_CLK = clk;
      registerCount = regCount;
      bin  = new uint8_t[registerCount];
      last = new uint8_t[registerCount];
      lastDelay = new uint8_t[registerCount * 8];

      for (int i = 0; i < registerCount * 8; i++) lastDelay[i] = 0;
      for (int i = 0; i < registerCount; i++) {
        bin[i] = 0;
        last[i] = 0;
      }

      pinMode(PIN_DATA, INPUT_PULLUP);
      pinMode(PIN_LATCH , OUTPUT);
      pinMode(PIN_CLK, OUTPUT);
    }

    uint8_t read(int pin, int from=1) {
      return bitRead(
        from ? bin[pin / 8] : last[pin / 8]
        , (pin % 8));
    }

    uint8_t pressed(int pin) {
      if (read(pin, 1) && !read(pin, 0)) {
        if (millis() - lastDelay[pin] > debounceDelay) {
          return true;
        }
      }
      return false;
    }

    void update() {
      digitalWrite(PIN_LATCH, 0);
      delayMicroseconds(5);
      digitalWrite(PIN_LATCH, 1);

      for (int i = 0; i < registerCount; i++) {
        last[i] = bin[i];
        uint8_t value = 0;

        for (int b = 0; b < 8; b++) {
          value <<= 1;
          value |= digitalRead(PIN_DATA);

          digitalWrite(PIN_CLK, 1);
          digitalWrite(PIN_CLK, 0);

          if (bitRead(bin[i], b) != bitRead(last[i], b)) {
            lastDelay[i * 8 + b] = millis();
          }
        }

        bin[i] = value;
      }
    }
}; Input input(11, 12, 13, 2);
