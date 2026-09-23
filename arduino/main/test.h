
class Test {
  private:
    uint8_t INOFF;
    uint8_t OUTOFF;

  public:
    Test(uint8_t outputOffset, uint8_t inputOffset) {
      OUTOFF = outputOffset;
      INOFF  = inputOffset;
    }

    void init() {
      output.writeAll(true);
    }

    int8_t check() {
    }

    void fini() {
    }

    void miss() {
    }

    void loop() {
      for (int i=0; i<16; i++) {
        if (input.readReleased(INOFF, -1, i)) {
          delay(20);
          while (input.readReleased(INOFF, -1, i));
          Serial.println(i);
        }
      }
    }
};
