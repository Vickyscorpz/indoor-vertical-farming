// Define required Blynk template info
#define BAUD_RATE 115200
#define BLYNK_TEMPLATE_ID "TMPL3ws8zLPaa"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

// Blynk credentials
char auth[] = "YMas-bvvh2Juu6THs48RfTrnulRAiHJv";
char ssid[] = "CSE_LAB - II";
char pass[] = "cse@#747";

// Pin Assignments
#define SOIL_MOISTURE_PIN 33   // Soil moisture sensor (analog)
#define WATER_LEVEL_PIN 32     // Water level sensor (digital)
#define RELAY_PIN 4            // Relay for water pump
#define DHT_PIN 27             // DHT sensor data pin
#define DHTTYPE DHT11          // DHT sensor type (change to DHT22 if needed)

DHT dht(DHT_PIN, DHTTYPE);
BlynkTimer timer;

bool autoMode = true; // Default: Auto Mode is ON

// Moving average filter parameters for soil moisture
const int numReadings = 10;  // Number of samples for the moving average
int readings[numReadings];   // Array to store individual soil moisture readings
int readIndex = 0;           // Current index in the readings array
int total = 0;               // Running total of the readings
int avgSoil = 0;             // Averaged soil moisture value

void setup() {
  Serial.begin(BAUD_RATE);

  // Initialize Blynk
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

  // Initialize sensors
  dht.begin();
  
  // Configure pins
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(WATER_LEVEL_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Ensure relay (pump) is OFF initially

  // Initialize the readings array
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
  
  // Set a timer to call sendSensorData() every 2 seconds
  timer.setInterval(2000L, sendSensorData);
}

void sendSensorData() {
  // ----- Soil Moisture with Moving Average Filter -----
  int currentSoil = analogRead(SOIL_MOISTURE_PIN);
  // Map the raw sensor value (0-4095) to percentage (0-100)
  currentSoil = map(currentSoil, 0, 4095, 0, 100);
  // Adjust if sensor output is inverted
  currentSoil = (currentSoil - 100) * -1;
  
  // Update moving average filter
  total = total - readings[readIndex];        // Remove oldest reading
  readings[readIndex] = currentSoil;            // Store new reading
  total = total + readings[readIndex];          // Add new reading to total
  readIndex = (readIndex + 1) % numReadings;      // Cycle through array
  avgSoil = total / numReadings;                // Calculate average

  // ----- Read Water Level -----
  int waterLevel = digitalRead(WATER_LEVEL_PIN); // 1 = Water Present, 0 = Empty

  // ----- Read Temperature and Humidity -----
  float temperature = dht.readTemperature(); // °C
  float humidity = dht.readHumidity();         // %
  
  // Check if DHT sensor returns valid data
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error: Failed to read from DHT sensor!");
    return;  // Skip sending data if invalid
  }

  // ----- Send Data to Blynk -----
  Blynk.virtualWrite(V0, avgSoil);       // V0: Filtered Soil Moisture (%)
  Blynk.virtualWrite(V1, waterLevel);      // V1: Water Level (1 = Full, 0 = Empty)
  Blynk.virtualWrite(V2, temperature);     // V2: Temperature (°C)
  Blynk.virtualWrite(V3, humidity);        // V3: Humidity (%)

  // ----- Print Data to Serial Monitor -----
  Serial.print("Soil Moisture (Avg): ");
  Serial.print(avgSoil);
  Serial.println(" %");
  Serial.print("Water Level: ");
  Serial.println(waterLevel ? "Full" : "Empty");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // ----- Automatic Watering Logic (Auto Mode) -----
  if (autoMode) {
    if (avgSoil < 30 && waterLevel == 1) { 
      Serial.println("Auto Mode: Soil dry. Pump ON.");
      digitalWrite(RELAY_PIN, LOW); // Turn pump ON
      Blynk.virtualWrite(V4, "Pump ON (Auto)");
    } else {
      Serial.println("Auto Mode: Soil is moist or water low. Pump OFF.");
      digitalWrite(RELAY_PIN, HIGH); // Turn pump OFF
      Blynk.virtualWrite(V4, "Pump OFF (Auto)");
    }
  }
}

// Blynk switch to toggle Auto/Manual Mode (V5)
BLYNK_WRITE(V5) {
  autoMode = param.asInt(); // 1 = Auto Mode, 0 = Manual Mode
  
  if (autoMode) {
    Serial.println("Switched to Auto Mode");
    Blynk.virtualWrite(V4, "Auto Mode Enabled");
  } else {
    Serial.println("Switched to Manual Mode");
    Blynk.virtualWrite(V4, "Manual Mode Enabled");
  }
}

// Blynk button to manually control pump (V6)
BLYNK_WRITE(V6) {
  if (!autoMode) { // Only allow manual control when in Manual Mode
    int relayState = param.asInt();
    if (relayState == 1) {
      digitalWrite(RELAY_PIN, LOW); // Turn pump ON
      Serial.println("Manual Mode: Pump ON");
      Blynk.virtualWrite(V4, "Manual: Pump ON");
    } else {
      digitalWrite(RELAY_PIN, HIGH); // Turn pump OFF
      Serial.println("Manual Mode: Pump OFF");
      Blynk.virtualWrite(V4, "Manual: Pump OFF");
    }
  }
}

void loop() {
  Blynk.run();
  timer.run();
}
