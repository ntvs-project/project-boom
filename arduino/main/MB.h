
class MB {
  private:
    uint8_t INOFF;
    uint8_t OUTOFF;

    uint8_t minute = 10;
    uint8_t second = 0;

    uint8_t prevMis = 0;
    bool blink = true;
    unsigned long blinkPrev;

  public:
    MB(uint8_t outputOffset, uint8_t inputOffset) {
      OUTOFF = outputOffset;
      INOFF  = inputOffset;
    }

    void init() {
      timer.setBrightness(2);
      timer.clear();
      timer.showNumberDecEx(8888, blink ? 64 : 0, true);

      setYB();

      output.write(OUTOFF, 0, 0, Y);
      output.write(OUTOFF, 0, 1, B);
      output.write(OUTOFF, 0, 2, mis != 0);
      output.write(OUTOFF, 0, 3, mis >= 2);
      output.write(OUTOFF, 0, 4, mis == 3);

      blinkPrev = millis();
    }

    void miss() {
      buzzer.beep(3000, 0, NOTE_C8);
      while (true) {
        buzzer.loop();
        output.update();
      }
    }

    void setYB() {
      YB = random(0, 4);
      Y  = (YB >> 1);
      B  = (YB % 2);

      Serial.println("\n-----");
      Serial.print(Y);
      Serial.print(B);
      Serial.println("\n-----");
    }

    void loop() {
      if (Serial.available()) {
        Serial.read();
        mis++;
      }

      if (millis() - blinkPrev >= 500) {
        blink = !blink;

        if (!blink) {
          buzzer.beep(random(100, 200), random(0, 100), NOTE_AS4);
          if (second != 0) {
            second--;
          } else {
            second = 59;
            if (minute != 0) {
              minute--;
            } else {
              minute = 0;
              second = 0;
              miss();
              while (true) output.update();
            }
          }
        }

        timer.showNumberDecEx(minute * 100 + second, blink ? 64 : 0, true);
        blinkPrev = millis();
      }

      if (mis != prevMis) {
        output.write(OUTOFF, 0, 2, mis != 0);
        output.write(OUTOFF, 0, 3, mis >= 2);
        output.write(OUTOFF, 0, 4, mis == 3);
        prevMis = mis;

        if (mis >= 3) miss();
      }
    }
};
