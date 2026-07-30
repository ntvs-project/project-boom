
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
      if (input.readReleased(0, 0, 0)) {
        delay(20);
        while (input.readReleased(0, 0, 0));
        Serial.println("pressed");
      }
    }
};
