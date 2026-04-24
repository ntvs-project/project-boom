
typedef struct {
    unsigned int bits;  // holds the 0/1 pattern
} Morse;

const char *morse[256];

void init_morse() {
  morse['A'] = ".-";
  morse['B'] = "-...";
  morse['C'] = "-.-.";
  morse['D'] = "-..";
  morse['E'] = ".";
  morse['F'] = "..-.";
  morse['G'] = "--.";
  morse['H'] = "....";
  morse['I'] = "..";
  morse['J'] = ".---";
  morse['K'] = "-.-";
  morse['L'] = ".-..";
  morse['M'] = "--";
  morse['N'] = "-.";
  morse['O'] = "---";
  morse['P'] = ".--.";
  morse['Q'] = "--.-";
  morse['R'] = ".-.";
  morse['S'] = "...";
  morse['T'] = "-";
  morse['U'] = "..-";
  morse['V'] = "...-";
  morse['W'] = ".--";
  morse['X'] = "-..-";
  morse['Y'] = "-.--";
  morse['Z'] = "--..";
}

// dit = 1200 / wpm
unsigned int freq = 700;
unsigned int dit  = 60; // 20 wpm
unsigned int dah  = 3 * dit;

unsigned int send_morse(char code) {
  const char *sound = morse[(unsigned char)code];
  for (int i=0; sound[i] != '\0'; i++) {
    tone(6, freq);
    delay(sound[i] == '.' ? dit : dah);
    noTone(6);
    delay(dit);
  }

  return sound;
}

void setup() {
  pinMode(6, OUTPUT); // BUZ
  pinMode(2, INPUT_PULLUP); // BTN
  pinMode(10, OUTPUT); // LED
  init_morse();
  randomSeed(analogRead(A0));
  Serial.begin(9600);
  // for (int i=65; i<65+26; i++) {
  //   delay( send_morse(i) );
  // }
}

char letter, *output;
bool state = 0;

bool raw = 1, curr = 1, prev = 1, hold = 0;
unsigned long debounceStart = 0, holdStart = 0, holdT = 0;
char input[5] = "";
int inputLen = 0;

void loop() {
  if (state == 0) {
    letter = 'A' + random(26);
    output = send_morse(letter);
    state = 1;
  } else {
    prev = curr;
    curr = digitalRead(2);

    if (curr != prev && !debounceStart) debounceStart = millis();
    if (debounceStart && millis() - debounceStart > 20) {
      debounceStart = 0;
      if (curr) {
        input[inputLen++] = hold ? '-' : '.';
        input[inputLen] = '\0';
        if (strcmp(input, output) == 0) {
          digitalWrite(10, 1);
        }
        Serial.println(input);
        holdStart = 0;
        hold = false;
      }
      if (!curr && !holdStart) holdStart = millis();
    }
    if (!curr && holdStart && millis() - holdStart > dah) hold = true;
  }
}
