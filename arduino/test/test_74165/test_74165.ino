
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
}; Input input(10, 8, 9, 2);

void setup() {
  Serial.begin(9600);
}

void loop() {
  input.update();
  for (int i=0; i<16; i++) {
    if (input.pressed(i)) Serial.println("button " + String(i) + " pressed");
  }
}
