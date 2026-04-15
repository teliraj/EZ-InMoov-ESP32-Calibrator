# EZ-InMoov ESP32 Calibration Tool

The Problem: The original EZ-InMoov robot head controller is a closed, proprietary system that locks users into its own software ecosystem. It does not allow the use of standard communication protocols like ROS2, MQTT, HTTP, WebSocket, or Serial communication without an expensive paid subscription. Even with a subscription, you are restricted to their proprietary software and cannot integrate the robot head with external systems, sensors, or custom applications. This makes it impossible to use the EZ-InMoov head in advanced robotics projects, research, or educational environments that require ROS2 integration or custom communication protocols.

The Solution: This project completely replaces the proprietary controller with an ESP32 and PCA9685, creating an open, flexible platform. The ESP32 supports all standard communication protocols including:

ROS2 (via micro-ROS or ROS2 serial)

HTTP/WebSocket (web-based control from any browser)

MQTT (IoT and home automation integration)

Serial/UART (direct communication with other microcontrollers)

WiFi/TCP/IP (network-based control)

Since the ESP32 is fully programmable with Arduino, MicroPython, or ESP-IDF, you can integrate the robot head with any system, use any communication protocol, and build custom applications without any subscription fees or software restrictions. The entire project is open source under the MIT license, giving you complete freedom to modify, extend, and integrate the EZ-InMoov head into ROS2-based robots, research projects, or educational platforms.
---

## Web Interface

<p align="center">
  <img src="docs/screenshots/web_interface.png" width="1000">
</p>

## What it does

- Control servos from browser
- Adjust min/max limits
- Save calibration (stored in EEPROM)
- Works on phone/laptop
- Real-time response

---

## Hardware

- ESP32  
- PCA9685  
- 3 servo motors (SG90 works fine)  
- External 5V supply (don’t skip this)  
- Jumper wires  

---

## Wiring

### ESP32 → PCA9685

- 3.3V → VCC  
- GND → GND  
- GPIO22 → SCL  
- GPIO21 → SDA  

### Power

- 5V supply → V+ (PCA9685)  
- GND → GND (PCA9685)  

**Important:**  
Connect ESP32 GND with power supply GND (common ground).  
If you miss this, things will behave weird.

---

### Servos

- Channel 0 → Eyes  
- Channel 1 → Mouth  
- Channel 2 → Neck

  <a href="docs/screenshots/demo.gif">
  <img src="docs/screenshots/demo.gif" width="600">
</a>


Each servo:
- Red → V+  
- Brown/Black → GND  
- Yellow/Orange → Signal  

---

## Setup

Install ESP32 board in Arduino IDE:

Add this in Preferences:

https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json


Then install ESP32 from Boards Manager.

Install library:
- Adafruit PWM Servo Driver

---

## WiFi

Before uploading, update this:

```cpp
const char* ssid = "YOUR_WIFI";
const char* password = "YOUR_PASSWORD";
Upload
Select: ESP32 Dev Module
Select correct port
Upload
Usage
Open Serial Monitor (115200)
You’ll see an IP address
Open it in browser
Move sliders → servos move
Click save when done
Calibration
Move servo
Extend limit
Confirm
Save

That’s it.

Issues

Servo shaking
→ Power supply is weak

No movement
→ Check SDA/SCL

Page not opening
→ Same WiFi?

Random movement
→ Missing common ground

Notes
Don’t power servos from ESP32
Use proper 5V supply
You can extend this for more servos easily

