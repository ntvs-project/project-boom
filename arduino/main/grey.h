
class Grey {
  private:
    uint8_t INOFF;
    uint8_t OUTOFF;

    short original;
    short answer;
    short user;

    uint8_t idx = 0;
    char*   led = "11111111";
    unsigned long prev;
    short freq = 700;
    short dit = 100;
    short dah = 3 * dit;
    int melody[8] = {freq, 0, freq, 0, freq, 0, freq, 0};
    int duration[8];
    int melodyIdx = 0;

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
      Serial.print("orig: ");
      Serial.println(original, BIN);

      if (YB == 2) answer = binaryToGrey(original);
      if (YB == 1) answer = greyToBinary(original);
      Serial.print("ans : ");
      Serial.println(answer, BIN);

      output.write(OUTOFF, 1, 0, 0);
      output.writeRange(OUTOFF, 0, 0, 0, 7, led);

      for (int i=3; i>=0; i--) {
        int j = (3 - i) * 2;
        duration[j] = bitRead(original, i) ? dah : dit;
        duration[j + 1] = dit;
      }
    }

    int8_t check() {
      if (idx == 4) {
        idx = 0;
        // Serial.print("user: ");
        // Serial.println(user, BIN);
        return (user == answer);
      } else {
        return -1;
      }
    }

    void fini() {
      output.write(OUTOFF, 1, 0, 1);
      output.writeRange(OUTOFF, 0, 0, 0, 7, "00000000");
      while (true) {
        output.update();
        buzzer.turnOFF();
        buzzer.loop();
      }
    }

    void miss() {
      output.writeRange(OUTOFF, 0, 0, 0, 7, "10101010");
    }

    void loop() {
      if (buzzer.getState() == BUZZER_IDLE) {
        buzzer.beep(duration[melodyIdx], (melodyIdx == 0) ? 2 * dah : 0, melody[melodyIdx]);
        melodyIdx = (melodyIdx + 1) % 8;
      }

      bool pressed = input.readReleased(INOFF, 0, 0);
      bool hold    = input.readHold(INOFF, 0, 0);

      if (pressed) {
        // Serial.println("pressed");
        bitWrite(user, 3 - idx, 0);
      }
      if (hold) {
        // Serial.println("hold");
        bitWrite(user, 3 - idx, 1);
      }
      if (pressed || hold) {
        led[2 * idx] = '0';
        led[2 * idx + 1] = '1';

        idx++;
        output.writeRange(OUTOFF, 0, 0, 0, 7, led);
      }
    }
};
