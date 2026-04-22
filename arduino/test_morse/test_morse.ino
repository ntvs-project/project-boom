
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

void send_morse(char code) {
  unsigned int short_ = 150;
  unsigned int long_  = 3 * short_;

  const char *sound = morse[(unsigned char)code];
  for (int i=0; sound[i] != '\0'; i++) {
    tone(2, 248, sound[i] == '-' ? long_ : short_);
    delay(sound[i] == '-' ? long_ : short_);
    delay(short_);
  }
}

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, INPUT_PULLUP);
  init_morse();
//  for (int i=65; i<65+26; i++) {
//    send_morse(i);
//    delay(300);
//  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
