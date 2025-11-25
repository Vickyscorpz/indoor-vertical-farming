🌱 Indoor Vertical Farming – Smart AI-Driven IoT System

A fully automated indoor vertical farming system built using ESP32, AI-driven climate prediction, and mobile-based monitoring.
This project optimizes water usage, lighting, humidity, and nutrient cycles using real-time sensors and intelligent automation.

🚀 Features
🔧 Hardware Integration

ESP32 (WiFi + BLE microcontroller)

DHT22 / SHT31 – Temperature & Humidity Sensor

Capacitive Soil Moisture Sensor (YL-69 / SEN0193)

Ultrasonic / VL53L0X Water Level Sensor

LED Grow Lights (Full Spectrum)

Water Pump / Solenoid Valves

🤖 AI & Automation

AI-based moisture prediction for smart irrigation

AI-driven climate control (temperature & humidity)

Automated lighting based on plant growth stage

Predictive water usage model

📱 Mobile App: Blynk IoT

Live sensor dashboard

Manual and automatic pump control

Real-time alerts (dry soil, low water, over-heat)

Data history graphs

🧠 System Workflow

Sensors collect temperature, humidity, moisture, and water level.

ESP32 sends data to Blynk Cloud & AI module.

AI analyzes patterns and decides:

When to water

How long to water

Whether to increase/decrease lighting

Pump and grow lights operate automatically.

User monitors everything from the mobile app.

🛠️ Tech Stack
Layer	Technologies
Microcontroller	ESP32
Sensors	DHT22, Moisture Sensor, VL53L0X
Communication	Wi-Fi, Blynk IoT
AI Models	Moisture prediction, climate forecasting
Programming	Arduino (C++), Python for model training
📂 Folder Structure
Indoor-Vertical-Farming/
│── src/
│   ├── main.ino
│   ├── ai_model.h
│   └── sensors/
│── data/
│   ├── moisture_dataset.csv
│   └── climate_readings.csv
│── mobile-app/
│   └── blynk-dashboard.json
│── hardware/
│   └── circuit-diagram.png
│── README.md

⚡ Installation & Setup
1. Clone the Repository
git clone https://github.com/your-username/indoor-vertical-farming.git

2. Install Arduino Libraries

Blynk

DHT sensor library

Adafruit Unified Sensor

WiFi.h

3. Upload Code to ESP32

Select ESP32 Dev Module → Upload.

4. Configure Blynk

Add widgets

Copy the authentication token

Paste token into .ino file

🧪 AI Model Overview

The AI system is trained on:

Moisture vs. Temperature

Moisture vs. Humidity

Pump running patterns

Previous irrigation history

Model outputs:

0 → No irrigation

1 → Irrigation required

Duration → Smart pump timing

💡 Future Enhancements

Nutrient dispenser automation

CO₂ monitoring

Solar power integration

Real-time disease detection using cameras

🙌 Contributing

Pull requests are welcome. Please follow code-style guidelines.
