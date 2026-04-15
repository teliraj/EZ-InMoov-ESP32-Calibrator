# EZ-InMoov ESP32 Calibration Tool

## The Problem: Proprietary Lock-in

The original EZ-InMoov controller is a closed, proprietary system that restricts what you can do with your robot head.

### Communication Protocol Restrictions

| Protocol | Original Controller |
|----------|--------------------|
| ROS2 | ❌ Not supported (requires paid subscription) |
| MQTT | ❌ Not supported |
| HTTP/WebSocket | ❌ Subscription required |
| Serial/UART | ❌ Restricted |
| Custom Protocols | ❌ Impossible |

### Software Restrictions

- ❌ Forced to use their proprietary software
- ❌ Cannot write custom firmware
- ❌ No API access without subscription
- ❌ Locked into their ecosystem
- ❌ No integration with external systems

### Cost Issues

- 💰 Monthly subscription for basic features
- 💰 Extra fees for communication protocols
- 💰 Pay to integrate with ROS2 or other systems
- 💰 Expensive long-term ownership

### What You Cannot Do with Original Controller

- 🔒 Use ROS2 for robotics research
- 🔒 Integrate with home automation (Home Assistant, Alexa)
- 🔒 Build custom web dashboards
- 🔒 Create multi-robot systems
- 🔒 Add custom sensors or peripherals
- 🔒 Write your own firmware

---

## The Solution: Open ESP32 Platform

This project replaces the proprietary controller with an ESP32 and PCA9685, giving you complete freedom.

### Supported Protocols (All Free, No Subscription)

| Protocol | ESP32 Solution | Use Case |
|----------|----------------|----------|
| **ROS2** | ✅ via micro-ROS | Robotics research, navigation, manipulation |
| **MQTT** | ✅ Built-in support | Home Assistant, IoT, Node-RED |
| **HTTP/WebSocket** | ✅ Built-in web server | Custom dashboards, remote control |
| **Serial/UART** | ✅ Full support | Arduino, Raspberry Pi, PC communication |
| **TCP/IP** | ✅ Native support | Network-based control from anywhere |
| **Custom Protocols** | ✅ Any protocol | Implement whatever you need |

### What You Can Do Now

| Capability | With This Project |
|------------|-------------------|
| ROS2 Integration | ✅ Full support via micro-ROS |
| Home Assistant | ✅ MQTT ready |
| Custom Web Dashboard | ✅ HTTP/WebSocket API |
| Multi-Robot Systems | ✅ Network ready |
| Add Sensors | ✅ Any I2C/GPIO device |
| Custom Firmware | ✅ Full source code (MIT) |
| Monthly Fees | ✅ $0 |

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

