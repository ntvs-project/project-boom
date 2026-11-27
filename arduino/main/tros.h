
// enum segmentEnum { D, D0, D1, D2, D3, D4, D5, D6, D7, D8, D9, DA, DB, DC, DD, DE, DF, DG, DH, DI, DJ, DL, DN, DO, DP, DQ, DS, DT, DU, DY, DZ, DDEG, DEQ, D_ };
// enum buttonEnum  { BTN1, BTN2, BTN3, BTNA, BTN4, BTN5, BTN6, BTNB, BTN7, BTN8, BTN9, BTNC, BTNK, BTN0, BTNH, BTND };
// enum digitStateEnum { DIGIT_ON, DIGIT_OFF };
// const char segment[][8] = {
//   "0000000", // D
//   "1111110", // D0
//   "0110000", // D1
//   "1101101", // D2
//   "1111001", // D3
//   "0110011", // D4
//   "1011011", // D5
//   "1011111", // D6
//   "1110000", // D7
//   "1111111", // D8
//   "1111011", // D9
// };

class Tros {
  private:
    uint8_t INOFF;
    uint8_t OUTOFF;

    void setSegment() {
      output.writeRange(OUTOFF, 0, 1, 0, 7, "0110101");
    }

  public:
    bool finished = false;

    Tros(uint8_t outputOffset, uint8_t inputOffset) {
      OUTOFF = outputOffset;
      INOFF  = inputOffset;
    }

    void init() {
      output.write(OUTOFF, 0, 0, false);
      setSegment();
    }

    int8_t check() {
    }

    void fini() {
    }

    void miss() {
    }

    void loop() {
      if (input.readReleased(INOFF, 1, 4)) {
        Serial.println("tros | pressed");

        Serial.println("tros | ins: ");
        Serial.print(input.readRaw(INOFF, 1, 0));
        Serial.print(input.readRaw(INOFF, 1, 1));
        Serial.print(input.readRaw(INOFF, 1, 2));
        Serial.print(input.readRaw(INOFF, 1, 3));
        Serial.println();

        Serial.print("tros | slot0: ");
        Serial.println( 3 - input.readRaw(INOFF, 0, 0) * 2 - input.readRaw(INOFF, 0, 1) );
        Serial.print("tros | slot1: ");
        Serial.println( 3 - input.readRaw(INOFF, 0, 2) * 2 - input.readRaw(INOFF, 0, 3) );
        Serial.print("tros | slot2: ");
        Serial.println( 3 - input.readRaw(INOFF, 0, 4) * 2 - input.readRaw(INOFF, 0, 5) );
        Serial.print("tros | slot3: ");
        Serial.println( 3 - input.readRaw(INOFF, 0, 6) * 2 - input.readRaw(INOFF, 0, 7) );
      }

      if (input.readRaw(INOFF, 1, 0) == 0 &&
          input.readRaw(INOFF, 1, 1) == 0 &&
          input.readRaw(INOFF, 1, 2) == 0 &&
          input.readRaw(INOFF, 1, 3) == 0 ) {
        output.write(OUTOFF, 0, 0, false);
      }
      else {
        output.write(OUTOFF, 0, 0, true);
      }
    }
};
