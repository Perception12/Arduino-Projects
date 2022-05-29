int ledR1 = 6;
int ledR2 = 7;
int ledY1 = 4;
int ledY2 = 5;
int ledB1 = 2;
int ledB2 = 3;

void setup() {
  // put your setup code here, to run once:
  for (int i = 2; i<=7; i++) {
    pinMode(i, OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  ledOn(ledR1, ledB2);
  delay(2000);
  ledOn(ledY1,ledB2);
  delay(2000);
  ledOn(ledB1, ledR2);
  delay(2000);
  ledOn(ledY2, ledB1);
  delay(2000);
}

void ledOn(int led1, int led2){
  for (int i = 2; i <= 7; i++) {
    if (i == led1 ) {
      digitalWrite(i, HIGH);
    } else if (i == led2){
      digitalWrite(i, HIGH);  
    } else {
      digitalWrite(i, LOW);
    }
  }
}
