
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
      output.writeRange(OUTOFF, 0, 0, 0, 7, "11111111");
      output.writeRange(OUTOFF, 1, 0, 2, 2, "00000000000");
      // output.write(OUTOFF, 2, 7, true);
      output.writeRange(OUTOFF, 2, 3, 2, 6, "1111");
      output.write(OUTOFF, 2, 3 + 0, false);
    }

    int8_t check() {
    }

    void fini() {
    }

    void miss() {
    }

    void loop() {
      for (int i=0; i < 4; i++) {
        output.write(OUTOFF, 2, 3 + i, false);
        output.simpleUpdate();
        input.update();
        
        for (int j=0; j < 4; j++) {
          Serial.print( input.readRaw(INOFF, 0, j) );
        }
        Serial.print(" ");
        
        output.write(OUTOFF, 2, 3 + i, true);
        output.simpleUpdate();
      }
      Serial.println();
    }
};
