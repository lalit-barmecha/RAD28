#include <Wire.h>
#include <time.h>
#include <DS3231.h> //RTC
#include <U8g2lib.h> //LCD Display

DS3231 myRTC;
bool h12;
bool hPM;
bool CenturyBit;

#define A0_Light A0
#define A1_Audio A1

#define A2_Light A2
#define A3_Light A3

#define D1 1

bool status = false;
bool preferredStatus = false;

U8G2_SSD1306_64X32_1F_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  Wire.begin();  
  Serial.begin(9600); 
  u8g2.begin();

  //Get the current status of the blinds and display on the LCD display

  // Setup RTC
  // bool mode12 = false;
  // myRTC.setClockMode(mode12);
  //myRTC.setHour(8);
  //myRTC.setMinute(44);
  //myRTC.setSecond(30);
  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_t0_16_tf);
}

void loop() {
  //Getting all inputs
  int light = analogRead(A0_Light);
  int light2 = analogRead(A2_Light);
  int light3 = analogRead(A3_Light);
  
  byte theHour = myRTC.getHour(h12, hPM);
  byte theMinute = myRTC.getMinute();
  byte theSecond = myRTC.getSecond();

  int audio = analogRead(A1_Audio);

  // Compare sensor values
  if (light < 300) {
    preferredStatus = true;
  }
  else if (audio > 300) {
    preferredStatus = true;
  }
  //else if (theHour > 16 || theHour < 7) {
    //preferredStatus = true;
  //} 
  else {
    preferredStatus = false;
  }

  if (status != preferredStatus) {
    u8g2.clearBuffer();					
    u8g2.setFont(u8g2_font_t0_16_tf); 
    if (status == false) {
      u8g2.drawStr(0, 10, "EG1004"); 
      u8g2.sendBuffer(); 
      // Send signal to motor
      status = true;
    } else {
      u8g2.drawStr(0, 10, "EG1004");
      u8g2.sendBuffer(); 
      // Send signal to motor
      status = false;
    }
  }
  
  
  // Tests
  // Serial.print("light ");
  // Serial.println(light);
  // Serial.print("light2 ");
  // Serial.println(light2);
  // Serial.print("light3 ");
  // Serial.println(light3);
  // Serial.print("Hour");
  // Serial.println(theHour);
  // Serial.println(theMinute);
  // Serial.println(theSecond);
  // Serial.print("audio");
  // Serial.println(audio);
  // Serial.println("");
  delay(1000);
}
