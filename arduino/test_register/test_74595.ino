
int PIN_LED[] = {12, 11, 13}; // latch data clock
int PIN_BTN[] = {10, 8, 9};
// DS    STCP SHCP
// latch data clock
// blue grey purple

byte result;

void setup() {
  Serial.begin(9600);
  DDRB = 0b00111110;
         //FEDCBA98

  digitalWrite(PIN_LED[0], 0);
  shiftOut(PIN_LED[1], PIN_LED[2], LSBFIRST, 0b11111111);
  digitalWrite(PIN_LED[0], 1);
}

void loop() {
  digitalWrite(PIN_BTN[0], LOW);   // latch inputs
  delayMicroseconds(5);
  digitalWrite(PIN_BTN[0], HIGH);
  result = shiftIn(PIN_BTN[1], PIN_BTN[2], MSBFIRST);
  
  if (result & (1 << 1)) Serial.println("A pressed");
  if (result & (1 << 2)) Serial.println("B pressed");
  if (result & (1 << 3)) Serial.println("C pressed");
  if (result & (1 << 4)) Serial.println("D pressed");
  if (result & (1 << 5)) Serial.println("E pressed");
  if (result & (1 << 6)) Serial.println("F pressed");
  if (result & (1 << 7)) Serial.println("G pressed");

  delay(100);
}
