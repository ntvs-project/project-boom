
enum trosAnswerEnum { AA, AB, AC, AD };

const char trosAnswerLength = 12;
const char trosSegment[][8] = {
  "0100111", // mu
  "1100011", // o on top
  "1000111", // F
  "1001110", // bpm"f"
  "1100111", // P
  "1000110", // J reversed
  "0100101", // lightning
  "1000001", // equals
  "0000111", // T 90deg
  "1100100", // 2 - equals
  "1100001", // k hangul
  "0101010", // face
};
const char trosAnswer[][4] = {
  { AC, AA, AD, AB }, // mu
  { AC, AA, AB, AD }, // o on top
  { AC, AD, AA, AB }, // F
  { AA, AD, AB, AC }, // bpm"f"
  { AB, AA, AC, AD }, // P
  { AC, AB, AD, AA }, // J reversed
  { AB, AC, AD, AA }, // lightning
  { AA, AC, AB, AD }, // equals
  { AB, AC, AA, AD }, // T 90deg
  { AD, AC, AB, AA }, // 2 - equals
  { AB, AD, AC, AA }, // k hangul
  { AC, AB, AA, AD }, // face
};

class Tros {
  private:
    uint8_t INOFF;
    uint8_t OUTOFF;

    const int MAX_ITERATION = 1;
    int iteration = 0;

    int trosIdx;
    int answer[4], user[4], ins[4];
    bool allInserted;

    void setSegment(int trosIdx) {
      output.writeRange(OUTOFF, 0, 1, 0, 7, trosSegment[trosIdx]);
    }

  public:
    bool finished = false;

    Tros(uint8_t outputOffset, uint8_t inputOffset) {
      OUTOFF = outputOffset;
      INOFF  = inputOffset;
    }

    void init() {
      output.write(OUTOFF, 0, 0, false); // amber
      output.write(OUTOFF, 0, 2, false); // green

      trosIdx = random(0, trosAnswerLength);
      for (int i=0; i<4; i++) answer[i] = trosAnswer[trosIdx][i];
      setSegment(trosIdx);

      Serial.print("tros | answer: ");
      for (int i=0; i<4; i++) Serial.write(65 + answer[i]);
      Serial.println();
    }

    int8_t check() {
      if (iteration == MAX_ITERATION) {
        return 1;
      }
      return -1;
    }

    void fini() {
      Serial.println("tros | fini");
      iteration++;

      // output.write(OUTOFF, 0, 2, true);
      output.writeRange(OUTOFF, 0, 1, 0, 7, "0010000");
      finished = true;
      moduleFinished++;
    }

    void miss() {
      mis++;
    }

    void loop() {
      for (int i=0; i<4; i++) ins[i]  = input.readRaw(INOFF, 1, i);
      for (int i=0; i<4; i++) user[i] = 3 - input.readRaw(INOFF, 0, 2*i) * 2 - input.readRaw(INOFF, 0, 2*i+1);
      allInserted = (ins[0] == 0 && ins[1] == 0 && ins[2] == 0 && ins[3] == 0);

      if (input.readReleased(INOFF, 1, 4)) {
        Serial.println();
        Serial.println("tros | pressed");

        Serial.print("tros | ins:  ");
        for (int i=0; i<4; i++) Serial.print(ins[i] ? "?" : "+");
        Serial.println();

        Serial.print("tros | user: ");
        for (int i=0; i<4; i++) Serial.write(65 + user[i]);
        Serial.println();

        if (allInserted &&
            user[0] == answer[0] &&
            user[1] == answer[1] &&
            user[2] == answer[2] &&
            user[3] == answer[3]) {
          iteration++;
        } else {
          miss();
        }
      }

      if (allInserted) {
        output.write(OUTOFF, 0, 0, false);
      }
      else {
        output.write(OUTOFF, 0, 0, true);
      }
    }
};
