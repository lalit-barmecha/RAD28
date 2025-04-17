#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>  // RTC library

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

const int brightness_lightSensorPin = A0;
const int open_detectLightSensorPin = A1;
const int close_detectLightSensorPin = A3;
const int audio_detect = A2;

const int light_threshold = 100;
const int noise_threshold = 900;
const int close_detection_val = 450;
const int open_detection_val = 750;

bool blind_status = false;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *motor = AFMS.getMotor(1);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
RTC_DS3231 rtc;  // Create RTC object

void stop() {
  while (1) {
    Serial.println("The Code is Stopped!"); //used for testing purposes only to "soft" turn off the arduino
    delay(1000);
  }
}

void setup() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); //if it cant find the display.
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("POWERING ON!");
  display.display();
  delay(2000);

  AFMS.begin();
  motor->setSpeed(25);
  motor->run(RELEASE);

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // If the RTC is off, uncomment the below and upload the sketch, then comment out again and reuploadl.
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  DateTime now = rtc.now();
  int currentHour = now.hour();
  int currentMinute = now.minute();

  int brightness = analogRead(brightness_lightSensorPin);
  int close_check_brightness = analogRead(close_detectLightSensorPin);
  int open_check_brightness = analogRead(open_detectLightSensorPin);
  int noise_level = analogRead(audio_detect);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Blinds State: ");
  display.println(blind_status ? "Closed" : "Open");
  display.setCursor(0, 10);
  display.print("Time: ");
  display.print(currentHour);
  display.print(":");
  if (currentMinute < 10) display.print("0");
  display.println(currentMinute);
  display.display();

  Serial.print("Brightness: ");
  Serial.print(brightness);
  Serial.print(" | Noise: ");
  Serial.print(noise_level);
  Serial.print(" | Time: ");
  Serial.print(currentHour);
  Serial.print(":");
  Serial.print(currentMinute);
  Serial.print(" | Close Sensor: ");
  Serial.print(close_check_brightness);
  Serial.print(" | Open Sensor: ");
  Serial.println(open_check_brightness);

  delay(1000);

  // === Close Blinds ===
  if (!blind_status &&
      (brightness < light_threshold || 
       noise_level > noise_threshold || 
       currentHour >= 19)) { // 7 PM
    Serial.println("Closing blinds...");
    motor->run(BACKWARD);
    while (true) {
      close_check_brightness = analogRead(close_detectLightSensorPin);
      if (close_check_brightness > close_detection_val) {
        Serial.println("STOP - Fully Closed");
        motor->run(RELEASE);
        blind_status = true;

        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("Blinds State: ");
        display.println("Closed");
        display.display();

        delay(5000);
        break;
      }
      delay(10);
    }
  }

  // === Open Blinds ===
  else if (blind_status &&
           (brightness >= light_threshold &&
            currentHour >= 7 &&
            currentHour < 19 &&
            noise_level <= noise_threshold)) {
    Serial.println("Opening blinds...");
    motor->run(FORWARD);
    while (true) {
      open_check_brightness = analogRead(open_detectLightSensorPin);
      if (open_check_brightness < open_detection_val) {
        Serial.println("STOP - Fully Open");
        motor->run(RELEASE);
        blind_status = false;

        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("Blinds State: ");
        display.println("Open");
        display.display();

        delay(5000);
        break;
      }
      delay(10);
    }
  }
}

