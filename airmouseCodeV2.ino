#include <BleMouse.h>
#include <Adafruit_MPU6050.h>

#define SPEED_LOW 50
#define SPEED_MEDIUM 80
#define SPEED_HIGH 100
#define NOISE_THRESHOLD 0.01
#define SMOOTHING_FACTOR 0.1

const int leftButtonPin = 25;
const int rightButtonPin = 18;
const int sensitivityModePin = 27;
const int calibrateButtonPin = 33;
const int scrollUpPin = 32;
const int scrollDownPin = 19;
const int ledPin = 2;

Adafruit_MPU6050 mpu;
BleMouse bleMouse("test mouse", "msxki", 100);

long previousMillis = 0;
long lastClickTime = 0;
long lastScrollTime = 0;
long lastSensitivityChange = 0;
long lastCalibrationTime = 0;

int sensitivity = SPEED_MEDIUM;
bool ledState = false;
bool sleepMPU = true;

float gyroXOffset = 0, gyroZOffset = 0;
const int clickDelay = 500;
const int scrollDelay = 50;
const int sensitivityDelay = 500;
const int calibrationDelay = 500;

void setup() {
  Serial.begin(115200);

  pinMode(leftButtonPin, INPUT_PULLUP);
  pinMode(rightButtonPin, INPUT_PULLUP);
  pinMode(sensitivityModePin, INPUT_PULLUP);
  pinMode(calibrateButtonPin, INPUT_PULLUP);
  pinMode(scrollUpPin, INPUT_PULLUP);
  pinMode(scrollDownPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);

  digitalWrite(ledPin, LOW);
  bleMouse.begin();

  delay(1000);
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  mpu.enableSleep(sleepMPU);
}

void loop() {
  unsigned long currentMillis = millis();

  if (bleMouse.isConnected()) {
    if (sleepMPU) {
      if (currentMillis - previousMillis >= 3000) {
        Serial.println("MPU6050 awakened!");
        sleepMPU = false;
        mpu.enableSleep(sleepMPU);
        digitalWrite(ledPin, LOW);
        previousMillis = currentMillis;
      }
    }
  } else {
    if (currentMillis - previousMillis >= 500) {
      previousMillis = currentMillis;
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
    }
  }

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float deltaX = g.gyro.z - gyroZOffset;
  float deltaY = g.gyro.x - gyroXOffset;

  if (abs(deltaX) > NOISE_THRESHOLD) deltaX *= SMOOTHING_FACTOR;
  if (abs(deltaY) > NOISE_THRESHOLD) deltaY *= SMOOTHING_FACTOR;

  bleMouse.move(deltaX * -sensitivity, deltaY * -sensitivity);

  if (!digitalRead(leftButtonPin) && (currentMillis - lastClickTime >= clickDelay)) {
    Serial.println("Left click");
    bleMouse.click(MOUSE_LEFT);
    lastClickTime = currentMillis;
  }

  if (!digitalRead(rightButtonPin) && (currentMillis - lastClickTime >= clickDelay)) {
    Serial.println("Right click");
    bleMouse.click(MOUSE_RIGHT);
    lastClickTime = currentMillis;
  }

  if (!digitalRead(sensitivityModePin) && (currentMillis - lastSensitivityChange >= sensitivityDelay)) {
    Serial.println("Changing sensitivity");
    if (sensitivity == SPEED_LOW) {
      sensitivity = SPEED_MEDIUM;
    } else if (sensitivity == SPEED_MEDIUM) {
      sensitivity = SPEED_HIGH;
    } else {
      sensitivity = SPEED_LOW;
    }
    Serial.print("New sensitivity: ");
    Serial.println(sensitivity);
    lastSensitivityChange = currentMillis;
  }

  if (!digitalRead(calibrateButtonPin) && (currentMillis - lastCalibrationTime >= calibrationDelay)) {
    Serial.println("Calibrating MPU6050");
    mpu.begin();
    lastCalibrationTime = currentMillis;
  }

  if (!digitalRead(scrollUpPin) && (currentMillis - lastScrollTime >= scrollDelay)) {
    Serial.println("Scrolling up");
    bleMouse.move(0, 0, 1);
    lastScrollTime = currentMillis;
  }

  if (!digitalRead(scrollDownPin) && (currentMillis - lastScrollTime >= scrollDelay)) {
    Serial.println("Scrolling down");
    bleMouse.move(0, 0, -1);
    lastScrollTime = currentMillis;
  }
}
