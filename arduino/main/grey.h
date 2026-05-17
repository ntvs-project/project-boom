
class Grey {
  private:
    uint8_t INOFF;
    uint8_t OUTOFF;

    short original;
    short answer;
    short user;

    uint8_t idx = 0;
    char*   led = "0000";
    unsigned long prev;
    short freq = 700;
    short dit = 60;
    short dah = 3 * dit;

    short binaryToGrey(int binary) {
      return binary ^ (binary >> 1);
    }

    short greyToBinary(int grey) {
      unsigned int binary = grey;
      while (grey >>= 1) {
          binary ^= grey;
      }
      return binary;
    }

  public:
    Grey(uint8_t outputOffset, uint8_t inputOffset) {
      OUTOFF = outputOffset;
      INOFF  = inputOffset;
    }

    void init() {
      prev = millis();

      original = random(1, 15);
      Serial.println(original);

      if (YB == 2) answer = binaryToGrey(original);
      if (YB == 1) answer = greyToBinary(original);
      Serial.println(answer, BIN);

      output.writeRange(OUTOFF, 0, 0, 0, 1, "11");
    }

    int8_t check() {
      if (idx == 4) {
        idx = 0;
        return (user == answer);
      } else {
        return -1;
      }
    }

    void fini() {
      output.writeRange(OUTOFF, 0, 0, 0, 1, "01");
      output.writeRange(OUTOFF, 0, 2, 0, 5, led);
    }

    void miss() {
      output.writeRange(OUTOFF, 0, 0, 0, 1, "10");
    }

    void loop() {
      if (input.readReleased(INOFF, 0, 1)) {
        for (int i=3; i>=0; i--) {
          tone(BUZZER, freq);
          delay(bitRead(original, i) ? dah : dit);
          noTone(BUZZER);
          delay(dit);
        }
      }

      bool pressed = input.readReleased(INOFF, 0, 0);
      bool hold    = input.readHold(INOFF, 0, 0);

      if (pressed) {
        bitWrite(user, 3 - idx, 0);
      }
      if (hold) {
        bitWrite(user, 3 - idx, 1);
      }
      if (pressed || hold) {
        led[idx] = '1';
        idx = (idx + 1) % 5;
        output.writeRange(OUTOFF, 0, 2, 0, 5, led);
      }
    }
};
