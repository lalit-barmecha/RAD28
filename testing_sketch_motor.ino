#include <Adafruit_MotorShield.h> // Motor shield library

const int brightness_lightSensorPin = A0;
const int open_detectLightSensorPin = A1;
const int close_detectLightSensorPin = A2;

const int light_threshold = 100; // Adjust after testing
const int noise_threshold = 50;  // Not yet used
const int close_detection_val = 300; // Sensor value when blinds fully closed
const int open_detection_val = 400;  // Sensor value when blinds fully opened

bool blind_status = false;  // false = blinds open, true = blinds closed

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Select motor port M1 (can also be M2, M3, or M4)
Adafruit_DCMotor *motor = AFMS.getMotor(1); //pointer to motor object

void stop() 
{
  while(1)
  {
    Serial.print("The Code is Stopped!"); //the infinite loop stops the code from executing!
    delay(1000);
  }
}
void setup() {
  Serial.begin(9600);
  AFMS.begin();
  motor->setSpeed(25);  
  motor->run(RELEASE);
  // pinMode() for analog pins isn't necessary
}

void loop() {
  int brightness = analogRead(brightness_lightSensorPin);
  int close_check_brightness = analogRead(close_detectLightSensorPin);
  int open_check_brightness = analogRead(open_detectLightSensorPin);

  Serial.print("Brightness:");
  Serial.print(brightness);
  Serial.print(" | Close Sensor: ");
  Serial.print(close_check_brightness);
  Serial.print(" | Open Sensor: ");
  Serial.println(open_check_brightness);
  bool testing = false;
  delay(5000);
if (!blind_status && brightness < light_threshold) {
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
    delay(10);
  }
}
if (blind_status && brightness >= light_threshold) {
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
      delay(10);
    }
  }
}


