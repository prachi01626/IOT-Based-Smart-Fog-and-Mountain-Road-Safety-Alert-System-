#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MPU6050.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// ----------------- LCD -----------------
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ----------------- Ultrasonic -----------------
const int trigPin = D8;
const int echoPin = D7;

// ----------------- IR sensor ------------------
const int irPin = D5;
const int irLED = D6;

// ----------------- Buzzer ----------------------
const int buzzer = D3;

// ----------------- MPU6050 ---------------------
MPU6050 mpu;
int16_t ax, ay, az;
float pitch;

// ----------------- ThingSpeak ------------------
const char* WIFI_SSID = "YOUR_WIFI_SSID";
const char* WIFI_PASS = "YOUR_WIFI_PASS";

unsigned long CHANNEL_ID = YOUR_CHANNEL_ID;
const char* WRITE_API_KEY = "YOUR_API_KEY";

WiFiClient client;

unsigned long lastLCD = 0;
unsigned long lastUpload = 0;
unsigned long lastBuzz = 0;
bool buzzState = false;

// Thresholds
float cliffThreshold = -10.0;
float steepThreshold = 10.0;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  lcd.init();
  lcd.backlight();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(irPin, INPUT);
  pinMode(irLED, OUTPUT);

  pinMode(buzzer, OUTPUT);

  digitalWrite(buzzer, LOW);
  digitalWrite(irLED, LOW);

  // MPU Init
  mpu.initialize();
  Serial.println(mpu.testConnection() ? "MPU Connected" : "MPU ERROR");

  // Splash screen
  lcd.setCursor(0, 0);
  lcd.print("Smart Fog System");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(1500);
  lcd.clear();

  // WiFi
  lcd.print("WiFi Connecting");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
  }
  lcd.clear();

  ThingSpeak.begin(client);
}

// ----------- ULTRASONIC FUNCTION ---------------
float readUltrasonic() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 25000);
  if (duration == 0) return -1;

  return duration * 0.034 / 2.0;
}

void loop() {
  unsigned long now = millis();

  // ----------- SENSOR READINGS -----------
  float distance = readUltrasonic();
  bool irDetected = (digitalRead(irPin) == LOW);

  mpu.getAcceleration(&ax, &ay, &az);
  pitch = atan2(ax, sqrt(ay * ay + az * az)) * (180.0 / 3.14159);

  // Print MPU on Serial
  Serial.print("Pitch: "); Serial.print(pitch);
  Serial.print(" | AX: "); Serial.print(ax);
  Serial.print(" AY: "); Serial.print(ay);
  Serial.print(" AZ: "); Serial.println(az);

  // ----------- ALERT CONDITIONS ----------
  bool cliffAlert = (pitch < cliffThreshold);
  bool steepAlert = (pitch > steepThreshold);
  bool ultrasonicAlert = (distance > 0 && distance <= 10);

  // ----------- BUZZER LOGIC --------------
  if (cliffAlert || steepAlert) {
    // MPU alert = continuous buzzer
    digitalWrite(buzzer, HIGH);
  }
  else if (ultrasonicAlert) {
    // Ultrasonic alert = beep every 1 sec
    if (millis() - lastBuzz >= 1000) {
      lastBuzz = millis();
      buzzState = !buzzState;
      digitalWrite(buzzer, buzzState);
    }
  }
  else {
    digitalWrite(buzzer, LOW);
    buzzState = false;
  }

  // ----------- IR LED BLINK --------------
  if (irDetected) {
    digitalWrite(irLED, HIGH);
    delay(40);
    digitalWrite(irLED, LOW);
  }

  // ----------- LCD UPDATE ---------------
  if (now - lastLCD > 300) {
    lastLCD = now;

    lcd.setCursor(0, 0);
    // lcd.print("Dist:");
    if (distance > 0 && distance <=10) {
      lcd.print(distance);
       lcd.print("cm ");
      lcd.print("TOO CLOSE      ");
    } else {
      lcd.print("SAFE DISTANCE    ");
    }

    lcd.setCursor(0, 1);
    lcd.print("IR:");
    lcd.print(irDetected ? "Object   " : "No Obj   ");
  }

  // ----------- THINGSPEAK UPLOAD --------
  if (now - lastUpload > 20000) {
    lastUpload = now;

    ThingSpeak.setField(1, distance);
    ThingSpeak.setField(2, irDetected ? 1 : 0);
    ThingSpeak.setField(3, pitch);
    ThingSpeak.setField(4, ax);
    ThingSpeak.setField(5, ay);
    ThingSpeak.setField(6, az);

    int status = ThingSpeak.writeFields(CHANNEL_ID, WRITE_API_KEY);
    Serial.print("ThingSpeak status: ");
    Serial.println(status);
  }

  delay(20);
}

