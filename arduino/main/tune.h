
class Tune {
  private:
    uint8_t INOFF;
    uint8_t OUTOFF;
    bool done = 0;
    bool isQR = 0;
    bool ackMiss = 0;
    bool sigMiss;

  public:
    bool finished = false;

    Tune(uint8_t outputOffset, uint8_t inputOffset) {
      OUTOFF = outputOffset;
      INOFF  = inputOffset;
    }

    void init() {
      Serial.println("tune | init");

      output.write(OUTOFF, 0, 0, isQR);
      output.write(OUTOFF, 0, 1, Y);
      output.write(OUTOFF, 0, 2, B);
      output.write(OUTOFF, 0, 3, ackMiss);
      done    = input.readRaw(INOFF, 0, 0);
      sigMiss = input.readRaw(INOFF, 0, 0);
    }

    int8_t check() {
    }

    void fini() {
    }

    void miss() {
    }

    void loop() {
      // isQR = input.readRaw(OUTOFF, 0, 0);
      // if (isQR) {
      //   Serial.println(isQR);
      //   output.write(OUTOFF, 0, 0, isQR);
      //   output.simpleUpdate();
      //   while (1) {}
      // }
    }
};
