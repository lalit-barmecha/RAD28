/*

*/
#include <Wire.h>
#include <time.h>
#include <DS3231.h>

DS3231 myRTC;
bool h12;
bool hPM;
bool CenturyBit;

#define A0_Light A0
#define D7_RTC 7


void setup() {
  Wire.begin();  
  Serial.begin(9600); 
  bool mode12 = false;
  myRTC.setClockMode(mode12);
}

void loop() {
  //Getting all inputs
  int light = analogRead(A0_Light);
  byte theHour = myRTC.getHour(h12, hPM);
  byte theMinute = myRTC.getMinute();
  byte theSecond = myRTC.getSecond();









  
  // Tests
  Serial.println("Hour",theHour);
  Serial.println("Minutes",theMinute);
  Serial.println("Seconds",theSecond);
  delay(1000);
}
