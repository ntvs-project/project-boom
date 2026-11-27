
class Test {
  private:
    uint8_t INOFF;
    uint8_t OUTOFF;

  public:
    bool finished = false;

    Test(uint8_t outputOffset, uint8_t inputOffset) {
      OUTOFF = outputOffset;
      INOFF  = inputOffset;
    }

    void init() {
      output.write(OUTOFF, 0, 0, true);
      output.writeRange(OUTOFF, 0, 1, 0, 7, "1111111");
    }

    int8_t check() {
    }

    void fini() {
    }

    void miss() {
    }

    void loop() {
      if (input.readReleased(OUTOFF, 1, 4)) {
        Serial.println("test | pressed");
      }
    }
};
