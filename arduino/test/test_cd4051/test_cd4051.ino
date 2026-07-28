
#define A 5
#define B 6
#define C 7

int number, o1, o2, o3;

void setup() {
  Serial.begin(9600);
  pinMode(A1, INPUT);
  pinMode(4, INPUT);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
}

void loop() {
  if (digitalRead(4) == 0) {
    delay(20);
    while (digitalRead(4) == 0);
    digitalWrite(C, 0); digitalWrite(B, 0); digitalWrite(A, 0);
    o1 = analogRead(A1) / 1023.0 * 50.0;

    digitalWrite(C, 0); digitalWrite(B, 0); digitalWrite(A, 1);
    o2 = analogRead(A1) / 1023.0 * 50.0;

    digitalWrite(C, 0); digitalWrite(B, 1); digitalWrite(A, 0);
    o3 = analogRead(A1) / 1023.0 * 50.0;
  }

  digitalWrite(C, 0); digitalWrite(B, 0); digitalWrite(A, 0);
  number = analogRead(A1) / 1023.0 * 50.0 - o1;
  Serial.print(number);
  Serial.print(" ");

  digitalWrite(C, 0); digitalWrite(B, 0); digitalWrite(A, 1);
  number = analogRead(A1) / 1023.0 * 50.0 - o2;
  Serial.print(number);
  Serial.print(" ");

  digitalWrite(C, 0); digitalWrite(B, 1); digitalWrite(A, 0);
  number = analogRead(A1) / 1023.0 * 50.0 - o3;
  Serial.println(number);
}
