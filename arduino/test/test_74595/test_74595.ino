
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
}; Output output(4, 3, 2, 2);

 
void setup() {
  Serial.begin(9600);
}

void loop() {
  for (int i=0; i<16; i++) {
    output.write(i, 1);
    output.update();
    output.write(i, 0);
    delay(100);
  }
  for (int i=16; i>=0; i--) {
    output.write(i, 1);
    output.update();
    output.write(i, 0);
    delay(100);
  }
}
