int r1 = 13;
int y1 = 12;
int b1 = 11;

int r4 = 10;
int y4 = 9;
int b4 = 8;

int r2 = 7;
int y2 = 6;
int b2 = 5;

int r3 = 4;
int y3 = 3;
int b3 = 2;
void setup() {
  // put your setup code here, to run once:
  for (int i = 2; i <=13 ; i++ ){
    pinMode(i, OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  regulate(b1, y2, r3, r4);
  delay(2000);
  regulate(b2, y3, r4, r1);
  delay(2000);
  regulate(b3, y4, r1, r2);
  delay(2000);
  regulate(b4, y1, r2, r3);
  delay(2000);
}

void regulate(int led1, int led2, int led3, int led4){
  for (int i = 2; i <=13; i++) {
   if (i == led1) {
    digitalWrite(i , HIGH);
   } else if (i == led2) {
    digitalWrite(i , HIGH);
   } else if (i == led3) {
    digitalWrite(i , HIGH);
   } else if (i == led4) {
    digitalWrite(i , HIGH);
   } else {
    digitalWrite(i, LOW);
   }
  }
}
