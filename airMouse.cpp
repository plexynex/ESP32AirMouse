#include <BleMouse.h>
#include <Wire.h>
#include <MPU6050_6Axis_MotionApps20.h>

// Pin konfigurasi
const int leftButtonPin = 25;    // GPIO25 - Klik kiri
const int rightButtonPin = 26;   // GPIO26 - Klik kanan
const int middleButtonPin = 27;  // GPIO27 - Ubah sensitivitas
const int calibrateButtonPin = 23; // GPIO23 - Kalibrasi
const int scrollUpPin = 32;
const int scrollDownPin = 33;
const int ledPin = 2;           // GPIO2 - LED internal ESP32

// Sensitivitas mode
float sensitivityModes[] = {5.0, 20.0, 50.0, 100.0};
int currentSensitivityMode = 1;  

// BLE Mouse
BleMouse bleMouse(std::string("AirMouse"), std::string("Your Name"));

// MPU6050
MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;

// Variabel gerakan
float gyroCalY = 0, gyroCalZ = 0;
float smoothCursorX = 0, smoothCursorY = 0;
float smoothingFactor = 0.1;
float threshold = 0.01;

// Double-click
unsigned long lastClickTime = 0;
unsigned long doubleClickThreshold = 500;  
bool isDoubleClick = false;
bool leftButtonPressed = false;

// Status koneksi
bool deviceConnected = false;

void blinkLedNonBlocking(long interval) {
    static unsigned long previousMillis = 0;
    static bool ledState = LOW;
    unsigned long currentMillis = millis();
    
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        ledState = !ledState;
        digitalWrite(ledPin, ledState);
    }
}

void calibrateGyro(int sampleCount = 100) {
    float sumY = 0, sumZ = 0;
    
    for (int i = 0; i < sampleCount; i++) {
        mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        sumY += gy;
        sumZ += gz;
        delay(10);
        blinkLedNonBlocking(100);
    }
    
    gyroCalY = sumY / sampleCount;
    gyroCalZ = sumZ / sampleCount;
}

void setup() {
    Serial.begin(115200);
    Wire.begin();
    
    // Inisialisasi pin
    pinMode(ledPin, OUTPUT);
    pinMode(leftButtonPin, INPUT_PULLUP);
    pinMode(rightButtonPin, INPUT_PULLUP);
    pinMode(middleButtonPin, INPUT_PULLUP);
    pinMode(calibrateButtonPin, INPUT_PULLUP);
    pinMode(scrollUpPin, INPUT_PULLUP);
    pinMode(scrollDownPin, INPUT_PULLUP);
    
    // Inisialisasi BLE Mouse
    bleMouse.begin();
    
    // Inisialisasi MPU6050
    Serial.println("Initializing MPU6050...");
    mpu.initialize();
    
    if (!mpu.testConnection()) {
        Serial.println("Error: MPU6050 tidak terhubung!");
        while (1) {
            blinkLedNonBlocking(200);  // LED berkedip cepat untuk indikasi error
        }
    }
    
    Serial.println("MPU6050 connection successful");
    calibrateGyro();
    digitalWrite(ledPin, LOW);
}

void loop() {
    // Baca status tombol
    bool leftButtonState = digitalRead(leftButtonPin) == LOW;
    bool rightButtonState = digitalRead(rightButtonPin) == LOW;
    bool middleButtonState = digitalRead(middleButtonPin) == LOW;
    bool calibrateButtonState = digitalRead(calibrateButtonPin) == LOW;
    
    // Cek koneksi BLE
    if (!bleMouse.isConnected()) {
        blinkLedNonBlocking(500);
        return;  // Keluar dari loop jika tidak terhubung
    }
    
    // LED mati jika terhubung
    digitalWrite(ledPin, LOW);
    
    // Ganti sensitivitas
    if (middleButtonState) {
        currentSensitivityMode = (currentSensitivityMode + 1) % 4;
        delay(200);
    }
    
    // Kalibrasi
    if (calibrateButtonState) {
        calibrateGyro();
        return;
    }
    
    // Baca data sensor
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    float gyroY = (gy - gyroCalY) / 131.0;  // Konversi ke derajat per detik
    float gyroZ = (gz - gyroCalZ) / 131.0;
    
    // Terapkan threshold
    if (abs(gyroY) < threshold) gyroY = 0;
    if (abs(gyroZ) < threshold) gyroZ = 0;
    
    // Hitung pergerakan kursor
    float cursorX = gyroZ * sensitivityModes[currentSensitivityMode];
    float cursorY = gyroY * sensitivityModes[currentSensitivityMode];
    
    // Smoothing
    smoothCursorX = (smoothingFactor * cursorX) + ((1 - smoothingFactor) * smoothCursorX);
    smoothCursorY = (smoothingFactor * cursorY) + ((1 - smoothingFactor) * smoothCursorY);
    
    // Gerakkan kursor
    if (abs(smoothCursorX) > 0 || abs(smoothCursorY) > 0) {
        bleMouse.move(round(smoothCursorX), round(-smoothCursorY), 0);
    }
    
    // Scroll
    if (digitalRead(scrollUpPin) == LOW) {
        bleMouse.move(0, 0, 1);
        delay(50);
    }
    if (digitalRead(scrollDownPin) == LOW) {
        bleMouse.move(0, 0, -1);
        delay(50);
    }
    
    // Handle klik kiri
    if (leftButtonState && !leftButtonPressed) {
        leftButtonPressed = true;
        unsigned long currentMillis = millis();
        if (currentMillis - lastClickTime < doubleClickThreshold) {
            isDoubleClick = true;
        } else {
            isDoubleClick = false;
        }
        lastClickTime = currentMillis;
        bleMouse.press(MOUSE_LEFT);
    }
    else if (!leftButtonState && leftButtonPressed) {
        leftButtonPressed = false;
        bleMouse.release(MOUSE_LEFT);
        if (isDoubleClick) {
            bleMouse.press(MOUSE_LEFT);
            bleMouse.release(MOUSE_LEFT);
            isDoubleClick = false;
        }
    }
    
    // Handle klik kanan
    if (rightButtonState) {
        bleMouse.press(MOUSE_RIGHT);
    } else {
        bleMouse.release(MOUSE_RIGHT);
    }
    
    delay(10);  // Sedikit delay untuk stabilitas
}