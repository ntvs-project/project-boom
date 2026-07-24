
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
    }

    int8_t check() {
    }

    void fini() {
    }

    void miss() {
    }

    void loop() {
    }
};
