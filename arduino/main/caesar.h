
const char* morseCode[] = {
  "01--", "1000", "1010", "100-", "0---", "0010", "110-",
  "0000", "00--", "0111", "101-", "0100", "11--", "10--",
  "111-", "0110", "1101", "010-", "000-", "1---", "001-",
  "0001", "011-", "1001", "1011", "1100"
};

const uint8_t morseLength[] = {
  2, 4, 4, 3, 1, 4, 3,
  4, 2, 4, 3, 4, 2, 2,
  3, 4, 4, 3, 3, 1, 3,
  4, 3, 4, 4, 4
};

class Caesar {
  private:
    uint8_t INOFF;
    uint8_t OUTOFF;

    short original;
    short shiftAmount;
    short answer;
    char user[5] = "----";

    uint8_t idx = 0;
    char    led[8];
    unsigned int ansLength;
    unsigned int origLength;
  
    short highFreq = 1000;
    short lowFreq  = 500;
    short dit = 100;
    short dah = 3 * dit;
  
    int melody[20];
    int duration[20];
    int melodyIdx = 0;

    short leftShift(int original, int shiftAmount) {
      return (original > shiftAmount) ? (original - shiftAmount) : (original - shiftAmount + 26);
    }

    short rightShift(int original, int shiftAmount) {
      return (original + shiftAmount < 26) ? (original + shiftAmount) : (original + shiftAmount - 26);
    }

  public:
    Caesar(uint8_t outputOffset, uint8_t inputOffset) {
      OUTOFF = outputOffset;
      INOFF  = inputOffset;
    }

    void init() {
      original = random(0, 26);
      origLength = morseLength[original];
      Serial.print("orig: ");
      Serial.write('A' + original);
      Serial.print(" ");
      Serial.println(morseCode[original]);

      shiftAmount = random(1, 8);
      if (YB == 0) Serial.print("left ");
      if (YB == 3) Serial.print("right ");
      Serial.print("shift: ");
      Serial.println(shiftAmount);

      if (YB == 0) answer = leftShift (original, shiftAmount);
      if (YB == 3) answer = rightShift(original, shiftAmount);
      ansLength = morseLength[answer];
      Serial.print("ans : ");
      Serial.write('A' + answer);
      Serial.print(" ");
      Serial.println(morseCode[answer]);

      for (int i = 0; i < ansLength; i++) {
        led[i * 2] = '1';
        led[i * 2 + 1] = '1';
      }
      for (int i = ansLength; i < 4; i++) {
        led[i * 2] = '0';
        led[i * 2 + 1] = '0';
      }
      led[4 * 2] = '\0';

      output.write(OUTOFF, 1, 0, 0);
      output.writeRange(OUTOFF, 0, 0, 0, 7, led);

      for (int i=0; i<origLength; i++) {
        int j = i * 2;

        melody[j] = highFreq;
        melody[j + 1] = 0;
        duration[j] = morseCode[original][i] == '1' ? dah : dit;
        duration[j + 1] = dit;
      }

      for (int i=2; i>=0; i--) {
        int j = (2 - i) * 2 + origLength * 2;

        melody[j] = lowFreq;
        melody[j + 1] = 0;
        duration[j] = bitRead(shiftAmount, i) == 1 ? dah : dit;
        duration[j + 1] = dit;
      }
    }

    int8_t check() {
      if (idx == morseLength[answer]) {
        idx = 0;
        // Serial.print("user: ");
        // Serial.println(user);
        return (strcmp(user, morseCode[answer]) == 0);
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
        melodyIdx = (melodyIdx + 1) % 20;
      }

      bool pressed = input.readReleased(INOFF, 0, 2);
      bool hold    = input.readHold(INOFF, 0, 2);

      if (pressed) {
        Serial.println("pressed");
        user[idx] = '0';
        // bitWrite(user, 3 - idx, 0);
      }
      if (hold) {
        Serial.println("hold");
        user[idx] = '1';
        // bitWrite(user, 3 - idx, 1);
      }
      if (pressed || hold) {
        led[2 * idx] = '0';
        led[2 * idx + 1] = '1';

        idx++;
        output.writeRange(OUTOFF, 0, 0, 0, 7, led);
      }
    }
};

