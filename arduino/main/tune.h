
class Tune {
  private:
    uint8_t INOFF;
    uint8_t OUTOFF;
    bool isQR = true;

  public:
    Tune(uint8_t outputOffset, uint8_t inputOffset) {
      OUTOFF = outputOffset;
      INOFF  = inputOffset;
    }

    void init() {
      output.write(OUTOFF, 0, 0, isQR);
      output.write(OUTOFF, 0, 1, Y);
      output.write(OUTOFF, 0, 2, B);
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
