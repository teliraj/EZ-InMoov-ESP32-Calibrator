# EZ-InMoov ESP32 Calibration Tool

A simple web-based tool to calibrate servos on an EZ-InMoov robot head using **ESP32 + PCA9685**.

Control your robot from a browser → adjust servo limits → save → done.

---

## 🚀 Quick Start (5 Minutes)

1. Upload code to ESP32  
2. Connect ESP32 + PCA9685 + servos  
3. Open Serial Monitor → copy IP address  
4. Open browser → enter IP  
5. Move sliders → click **Save Calibration**

---

## ✨ Features

- Web-based control (phone, laptop, tablet)
- Real-time servo movement
- Adjustable servo limits (250–500 pulse)
- Save calibration to EEPROM
- Reset to default option
- Mobile-friendly interface

---

## 🔧 Hardware Required

- ESP32 development board  
- PCA9685 PWM driver  
- 3× Servo motors (SG90 or similar)  
- 5V power supply (≥ 2A recommended)  
- Jumper wires  

---

## 🔌 Wiring

### ESP32 ↔ PCA9685

- VCC → 3.3V  
- GND → GND  
- SCL → GPIO22  
- SDA → GPIO21  

### ⚡ External Power (Important)

- 5V → PCA9685 V+  
- GND → PCA9685 GND  
- ALSO connect this GND to ESP32 GND  

**Note:** Without common ground, servos behave unpredictably.

---

## ⚙️ Setup

### Install ESP32 Board

1. Open Arduino IDE  
2. Go to File → Preferences  
3. Add this URL:

https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

4. Install ESP32 from Boards Manager  

---

### Install Library

- Adafruit PWM Servo Driver Library  

---

## 📶 Configure WiFi

Edit before uploading:

```cpp
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";
⬆️ Upload Code
Select Board: ESP32 Dev Module
Select correct port
Click Upload
🌐 Use the Web Interface
Open Serial Monitor (115200 baud)
Note the IP address
Open browser → enter IP
Control servos using sliders
Click Save Calibration
🎯 Calibration Steps
Move servo using slider
Click Extend Left/Right
Confirm popup
Click Save Calibration
⚠️ Common Issues

Servo jittering
→ Use proper 5V supply (≥2A)

Servos not moving
→ Check SDA/SCL wiring

Web page not loading
→ Ensure same WiFi network

Random behavior
→ Check common ground
