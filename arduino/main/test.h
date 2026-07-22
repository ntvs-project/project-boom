
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
      output.writeAll(false);
    }

    int8_t check() {
    }

    void fini() {
    }

    void miss() {
    }

    void loop() {
      for (int btnIdx=0; btnIdx < 4; btnIdx++) {
        if (input.readReleased(INOFF, -1, btnIdx)) {
          Serial.println(btnIdx);
        }
      }
    }
};
