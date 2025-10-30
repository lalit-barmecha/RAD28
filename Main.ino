#include <Wire.h>
#include <Adafruit_MotorShield.h> // Motor shield library
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

const int brightness_lightSensorPin = A0;
const int open_detectLightSensorPin = A1;
const int close_detectLightSensorPin = A2;
const int audio_detect = A3;


const int light_threshold = 100; // Adjust after testing
const int noise_threshold = 700;  // Threshold for noise, adjust as needed.
const int close_detection_val = 300; // Sensor value when blinds fully closed
const int open_detection_val = 400;  // Sensor value when blinds fully opened

bool blind_status = false;  // false = blinds open, true = blinds closed

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Select motor port M1 (can also be M2, M3, or M4)
Adafruit_DCMotor *motor = AFMS.getMotor(1); //pointer to motor object (M1 port)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
   Serial.begin(9600);
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // 0x3C is default I2C address
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Loop forever if display doesn't initialize (that way user knows somethings up)
  }

  display.clearDisplay();             // Clear the buffer
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Set text color
  display.setCursor(0, 0);            // Start at top-left corner
  display.println("Hello, OLED!");
  display.display();                  // Show the message
  delay(2000);

  AFMS.begin();
  motor->setSpeed(25); //set speed 
  motor->run(RELEASE); //motor is stopped
}

void loop() {
  
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Blinds State: ");
  if(blind_status)
  {
    display.println("Closed");
  }
  else 
  {
    display.println("Open");
  }
  display.display();

  delay(1000);

  int brightness = analogRead(brightness_lightSensorPin);
  int close_check_brightness = analogRead(close_detectLightSensorPin);
  int open_check_brightness = analogRead(open_detectLightSensorPin);
  int noise_level = analogRead(audio_detect);
  Serial.print("Brightness:");
  Serial.print(brightness);
  Serial.print(" | Close Sensor: ");
  Serial.print(close_check_brightness);
  Serial.print(" | Open Sensor: ");
  Serial.println(open_check_brightness);
  Serial.println("| Noise: ");
  Serial.println(noise_level);

  bool testing = false;
  delay(1000);

if (!blind_status && (brightness < light_threshold || noise_level > noise_threshold)) {
  motor->run(BACKWARD);
  while (true) {
    close_check_brightness = analogRead(close_detectLightSensorPin);
    Serial.print("Close Sensor: ");
    Serial.println(close_check_brightness);

    if (close_check_brightness > close_detection_val) {
      Serial.println("STOPPING - FULLY CLOSED");
      motor->run(RELEASE);
      blind_status = true;
      break;
    }
    delay(5000);
  }
}
else if (blind_status && (brightness >= light_threshold && noise_level <= noise_threshold)) { //if noiselevel is greater than threshold, close. 

  motor->run(FORWARD);
  while (true) {
    open_check_brightness = analogRead(open_detectLightSensorPin);
    Serial.print("Open Sensor: ");
    Serial.println(open_check_brightness);

    if (open_check_brightness < open_detection_val) {
      Serial.println("STOPPING - FULLY OPEN");
      motor->run(RELEASE);
      blind_status = false;
      break;
    }
      delay(1000);
    }
  }
}



