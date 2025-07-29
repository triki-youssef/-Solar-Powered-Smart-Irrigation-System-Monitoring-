// Blynk template configuration
#define BLYNK_TEMPLATE_ID "TMPL2qo32o41H"
#define BLYNK_TEMPLATE_NAME "smart irrigation system monitoring"
#define BLYNK_AUTH_TOKEN "EtYVA4g5mMZf7qV_QhFmO5DfAFebW-gY"  // Your unique Blynk auth token

// Include necessary libraries
#include <WiFi.h>                  // ESP32 WiFi support
#include <WiFiClient.h>           // Allows ESP32 to connect as a client
#include <BlynkSimpleEsp32.h>     // Blynk library for ESP32
#include "DHT.h"                  // DHT sensor library

// WiFi credentials
char ssid[] = "OPPO Reno7";       // WiFi network name
char pass[] = "12345123456";      // WiFi password

// Pin definitions
#define DHTPIN 4                  // DHT sensor data pin
#define DHTTYPE DHT11             // Using the DHT11 sensor
#define WATER_SENSOR_PIN 34       // Analog pin connected to water level sensor
#define SOIL_SENSOR_PIN 35        // Analog pin connected to soil moisture sensor
#define PUMP_PIN 5                // Digital pin connected to the pump relay

// Create DHT sensor object
DHT dht(DHTPIN, DHTTYPE);

// Variables to manage irrigation timing
unsigned long lastIrrigationTime = 0;     // Last time the irrigation was triggered
bool coolingDown = false;                 // Flag to prevent continuous irrigation
bool manualPumpOn = false;                // Flag for manual pump control via Blynk
const unsigned long irrigationCooldown = 60000;  // 1 minute cooldown time

void setup() {
  Serial.begin(115200);                     // Start Serial monitor for debugging
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass); // Connect to Blynk and WiFi
  dht.begin();                              // Initialize DHT sensor

  pinMode(PUMP_PIN, OUTPUT);               // Set pump pin as output
  digitalWrite(PUMP_PIN, LOW);             // Ensure pump is OFF initially
  delay(1000);                              // Small delay for stability
}

// Blynk virtual pin V4 - Manual pump control
BLYNK_WRITE(V4) {
  int state = param.asInt();                // Read value from Blynk app (1 = ON, 0 = OFF)
  manualPumpOn = state;                     // Set manual control flag
  digitalWrite(PUMP_PIN, manualPumpOn ? HIGH : LOW); // Control pump accordingly

  Serial.print("Manual pump control: ");
  Serial.println(manualPumpOn ? "ON" : "OFF");
}

void loop() {
  Blynk.run();                             // Run Blynk background tasks

  // Read data from sensors
  float temp = dht.readTemperature();       // Temperature in °C
  float humidity = dht.readHumidity();      // Humidity in %

  int soilRaw = analogRead(SOIL_SENSOR_PIN);       // Raw soil sensor value (0-4095)
  int waterLevelRaw = analogRead(WATER_SENSOR_PIN); // Raw water level value (0-4095)

  // Convert raw soil moisture to percentage
  float soilPercent = map(soilRaw, 4095, 0, 0, 100); // Invert and scale to 0-100%
  soilPercent = constrain(soilPercent, 0, 100);       // Limit values within 0-100%
  bool isSoilDry = soilPercent < 30;                  // Check if soil is dry

  // Convert raw water level to percentage
  int waterPercent = map(waterLevelRaw, 0, 4095, 0, 100);
  waterPercent = constrain(waterPercent, 0, 100);     // Limit within 0-100%

  // Determine water tank level based on percentage
  String tankLevel = "LOW";
  if (waterPercent >= 60 && waterPercent < 85) tankLevel = "MEDIUM";
  else if (waterPercent >= 85) tankLevel = "FULL";

  // Debug info on Serial Monitor
  Serial.print("Temp: "); Serial.print(temp); Serial.print(" °C | ");
  Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %");
  Serial.print("Soil Moisture: "); Serial.print(soilPercent); Serial.print(" % - ");
  Serial.println(isSoilDry ? "DRY" : "OK");
  Serial.print("Water Tank: "); Serial.print(waterPercent); Serial.print("% - ");
  Serial.println(tankLevel);

  // Send data to Blynk app
  Blynk.virtualWrite(V1, (int)temp);           // Send temperature
  Blynk.virtualWrite(V2, (int)humidity);       // Send humidity
  Blynk.virtualWrite(V0, (int)soilPercent);    // Send soil moisture
  Blynk.virtualWrite(V3, waterPercent);        // Send water tank percentage
  Blynk.virtualWrite(V4, tankLevel);           // Send tank level label

  unsigned long currentMillis = millis();      // Get current time

  // Skip auto irrigation if manual mode is ON
  if (!manualPumpOn) {
    // Check if cooldown period has ended
    if (coolingDown && (currentMillis - lastIrrigationTime >= irrigationCooldown)) {
      coolingDown = false;
      Serial.println("Cooldown finished - Ready to irrigate again.");
    }

    // Automatic irrigation logic
    if (!coolingDown && isSoilDry) {
      if (tankLevel == "MEDIUM" || tankLevel == "FULL") {
        // Determine irrigation duration based on temperature
        int duration = (temp > 35) ? 12000 : 6000;
        Serial.print("Irrigating for ");
        Serial.print(duration / 1000);
        Serial.println(" seconds...");

        digitalWrite(PUMP_PIN, HIGH);     // Start pump
        delay(duration);                  // Keep pump ON for set duration
        digitalWrite(PUMP_PIN, LOW);      // Stop pump

        lastIrrigationTime = millis();    // Update irrigation time
        coolingDown = true;               // Start cooldown period
      } else {
        Serial.println("Tank is LOW - Cannot irrigate, waiting...");
      }
    } else if (coolingDown && isSoilDry) {
      Serial.println("Soil is DRY but in cooldown - Skipping irrigation.");
    } else {
      Serial.println("Soil is moist - No irrigation needed.");
    }
  }

  Serial.println("------------------------");
  delay(5000);  // Wait 5 seconds before next loop
}
