#include <Wire.h>
#include <time.h>
#include <DS3231.h>

DS3231 myRTC;
bool h12;
bool hPM;
bool CenturyBit;

#define A0_Light A0
#define D7_RTC 7
#define A1_Audio A1
//#define D1 1
bool status = false;
bool preferredStatus = false;

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

  //int audio = analogRead(A1_Audio);

  int audio = analogRead(A1_Audio);
  if (light < 500) {
    preferredStatus = true;
  }
  else if (audio > 260) {
    preferredStatus = true;
  }
  //else if (theHour > 16 || theHour < 7) {
    //preferredStatus = true;
  //} 
  else {
    preferredStatus = false;
  }

  if (status != preferredStatus) {
    if (status == false) {
      Serial.println("Blinds Down");
      status = true;
    } else {
      Serial.println("Blinds Up");
      status = false;
    }
  }
  
  
  // Tests
  Serial.println(light);
  Serial.println(theHour);
  //Serial.println("Minutes",theMinute);
  //Serial.println("Seconds",theSecond);
  Serial.println(audio);
  delay(1000);
}
