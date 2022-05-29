#include <SevSeg.h>
SevSeg sevseg;

int i = 0;
void setup() {
  // put your setup code here, to run once:
  byte numDigits = 4;
  byte digitPins[] = {10, 11, 12, 13};
  byte segmentPins[] = {2,3,4,5,6,7,8,9};
  bool resistorsOnSegment = 0;
  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins, resistorsOnSegment);
  sevseg.setBrightness(50);
  sevseg.setNumber(0000, -1);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long timer = millis();

  sevseg.setNumber(timer/1000, -1);
  sevseg.refreshDisplay();
}
