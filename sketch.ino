#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Pin konfigurasi
const int pirPin = A0;
const int trigPin = A1;
const int echoPin = A2;
const int leds[] = {13, 12, 11};  // Merah, Kuning, Hijau LED
const String ledStatus[] = {"red", "yellow", "green"};

void setup() {
  Serial.begin(9600);

  // Menyusun pin mode untuk sensor dan LED
  setupPins();
  
  // Inisialisasi OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("OLED failed!"));
    while(1);
  }
  display.clearDisplay();
  display.display();
  delay(1000);
}
// Fungsi untuk setup pin
void setupPins() {
  pinMode(pirPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Set pin LED sebagai output
  for (int i = 0; i < 3; i++) {
    pinMode(leds[i], OUTPUT);
  }
}
void loop() {
  // Membaca status PIR dan jarak objek
  String pirText = readPIR();
  int distance = getDistance();
  String condition = determineCondition(distance);

  // Menampilkan data pada OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  
  display.setCursor(0, 0);
  display.print("PIR: ");
  display.println(pirText);
  
  display.setCursor(0, 20);
  display.print("Distance: ");
  display.print(distance);
  display.println(" cm");
  
  display.setCursor(0, 40);
  display.print("Status: ");
  display.println(condition);

  display.display();

  // Menunda pembaruan agar tidak terlalu cepat
  delay(500);
}

// Fungsi untuk membaca sensor PIR
String readPIR() {
  int pirStatus = digitalRead(pirPin);
  return (pirStatus == HIGH) ? "Detected" : "Not Detected";
}

// Fungsi untuk membaca jarak dengan sensor ultrasonik
int getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2;
}

// Fungsi untuk menentukan kondisi berdasarkan jarak
String determineCondition(int distance) {
  String condition;
  // Matikan semua LED terlebih dahulu
  for (int i = 0; i < 3; i++) {
    digitalWrite(leds[i], LOW);
  }

  if (distance < 50) {
    condition = "Near";
    digitalWrite(leds[0], HIGH);  // LED Merah
  } else if (distance >= 50 && distance <= 125) {
    condition = "Medium";
    digitalWrite(leds[1], HIGH);  // LED Kuning
  } else {
    condition = "Far";
    digitalWrite(leds[2], HIGH);  // LED Hijau
  }

  return condition;
}