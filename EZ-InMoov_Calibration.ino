#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <EEPROM.h>

const char* ssid = "IQOO Neo 10R";
const char* password = "11111111";

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
#define SERVO_FREQ 50

// EEPROM addresses
#define EEPROM_SIZE 64
#define ADDR_EYES_MIN 0
#define ADDR_EYES_MAX 4
#define ADDR_MOUTH_MIN 8
#define ADDR_MOUTH_MAX 12
#define ADDR_NECK_MIN 16
#define ADDR_NECK_MAX 20

// Default calibration values
int eyesMin = 250;
int eyesMax = 500;
int mouthMin = 250;
int mouthMax = 500;
int neckMin = 250;
int neckMax = 500;

WebServer server(80);

// Save to EEPROM
void saveCalibration() {
  EEPROM.put(ADDR_EYES_MIN, eyesMin);
  EEPROM.put(ADDR_EYES_MAX, eyesMax);
  EEPROM.put(ADDR_MOUTH_MIN, mouthMin);
  EEPROM.put(ADDR_MOUTH_MAX, mouthMax);
  EEPROM.put(ADDR_NECK_MIN, neckMin);
  EEPROM.put(ADDR_NECK_MAX, neckMax);
  EEPROM.commit();
  Serial.println("Calibration saved");
}

// Load from EEPROM
void loadCalibration() {
  EEPROM.get(ADDR_EYES_MIN, eyesMin);
  EEPROM.get(ADDR_EYES_MAX, eyesMax);
  EEPROM.get(ADDR_MOUTH_MIN, mouthMin);
  EEPROM.get(ADDR_MOUTH_MAX, mouthMax);
  EEPROM.get(ADDR_NECK_MIN, neckMin);
  EEPROM.get(ADDR_NECK_MAX, neckMax);
  
  // If EEPROM is empty, use defaults
  if (eyesMin == 255 || eyesMin == 0) {
    eyesMin = 250;
    eyesMax = 500;
    mouthMin = 250;
    mouthMax = 500;
    neckMin = 250;
    neckMax = 500;
    Serial.println("Using default values");
  } else {
    Serial.println("Loaded saved calibration");
  }
}

// Set servo with limits
void setServo(int channel, int pulse) {
  // Apply limits based on servo
  if (channel == 0) {
    pulse = constrain(pulse, eyesMin, eyesMax);
  } else if (channel == 1) {
    pulse = constrain(pulse, mouthMin, mouthMax);
  } else if (channel == 2) {
    pulse = constrain(pulse, neckMin, neckMax);
  }
  
  pwm.setPWM(channel, 0, pulse);
  Serial.printf("Servo %d -> %d\n", channel, pulse);
}

// Move servo with angle (0-100% of range)
void setServoPercent(int channel, int percent) {
  int pulse;
  if (channel == 0) {
    pulse = map(percent, 0, 100, eyesMin, eyesMax);
  } else if (channel == 1) {
    pulse = map(percent, 0, 100, mouthMin, mouthMax);
  } else {
    pulse = map(percent, 0, 100, neckMin, neckMax);
  }
  setServo(channel, pulse);
}

void setup() {
  Serial.begin(115200);
  Serial.println("\nEZ-InMoov Calibration Tool\n");
  
  // Initialize EEPROM
  EEPROM.begin(EEPROM_SIZE);
  loadCalibration();
  
  // Initialize PCA9685
  Wire.begin(21, 22);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);
  
  // Center all servos on boot
  setServo(0, 375);
  setServo(1, 375);
  setServo(2, 375);
  
  // WiFi connection
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Web server routes
  server.on("/", HTTP_GET, []() {
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>EZ-InMoov Calibration</title>";
    html += "<style>";
    html += "body{font-family:Arial;background:#1a1a2e;color:white;padding:20px;margin:0}";
    html += "h1{text-align:center;color:#00d4ff}";
    html += ".container{display:flex;flex-wrap:wrap;justify-content:center;gap:20px;max-width:1200px;margin:0 auto}";
    html += ".card{background:#2a2a3e;border-radius:20px;padding:20px;width:300px}";
    html += ".card h2{margin:0 0 10px 0;color:#00d4ff}";
    html += ".value{font-size:24px;font-weight:bold;text-align:center;margin:10px 0}";
    html += "input{width:100%;margin:10px 0;cursor:pointer}";
    html += "button{padding:8px 15px;margin:5px;background:#3a3a5e;color:white;border:none;border-radius:8px;cursor:pointer}";
    html += "button:hover{background:#00d4ff;color:#1a1a2e}";
    html += ".limit-btn{background:#ff6600}";
    html += ".limit-btn:hover{background:#ffaa00}";
    html += ".save-btn{background:#28a745;width:100%;padding:15px;font-size:18px;margin-top:10px}";
    html += ".home-btn{background:#00d4ff;color:#1a1a2e;width:100%;padding:15px;font-size:18px;margin-top:10px}";
    html += ".reset-btn{background:#dc3545;width:100%;padding:15px;font-size:18px;margin-top:10px}";
    html += ".status{background:#0a0a1e;padding:10px;border-radius:10px;text-align:center;margin-top:20px}";
    html += ".limit-info{font-size:12px;color:#888;text-align:center;margin-top:10px}";
    html += "</style></head><body>";
    
    html += "<h1>EZ-InMoov Calibration Tool</h1>";
    html += "<div class='container'>";
    
    // Eyes Card
    html += "<div class='card'><h2>Eyes</h2>";
    html += "<div class='value'><span id='v0'>375</span> pulse</div>";
    html += "<input type='range' id='s0' min='200' max='600' value='375' oninput='move(0, this.value)'>";
    html += "<div><button onclick='move(0,250)'>Left</button><button onclick='move(0,375)'>Center</button><button onclick='move(0,500)'>Right</button></div>";
    html += "<div><button class='limit-btn' onclick='extend(0, \"min\")'>Extend Left Limit</button>";
    html += "<button class='limit-btn' onclick='extend(0, \"max\")'>Extend Right Limit</button></div>";
    html += "<div class='limit-info'>Min: <span id='min0'>" + String(eyesMin) + "</span> | Max: <span id='max0'>" + String(eyesMax) + "</span></div></div>";
    
    // Mouth Card
    html += "<div class='card'><h2>Mouth</h2>";
    html += "<div class='value'><span id='v1'>375</span> pulse</div>";
    html += "<input type='range' id='s1' min='200' max='600' value='375' oninput='move(1, this.value)'>";
    html += "<div><button onclick='move(1,250)'>Open</button><button onclick='move(1,375)'>Half</button><button onclick='move(1,500)'>Closed</button></div>";
    html += "<div><button class='limit-btn' onclick='extend(1, \"min\")'>Extend Open Limit</button>";
    html += "<button class='limit-btn' onclick='extend(1, \"max\")'>Extend Close Limit</button></div>";
    html += "<div class='limit-info'>Min: <span id='min1'>" + String(mouthMin) + "</span> | Max: <span id='max1'>" + String(mouthMax) + "</span></div></div>";
    
    // Neck Card
    html += "<div class='card'><h2>Neck</h2>";
    html += "<div class='value'><span id='v2'>375</span> pulse</div>";
    html += "<input type='range' id='s2' min='200' max='600' value='375' oninput='move(2, this.value)'>";
    html += "<div><button onclick='move(2,250)'>Left</button><button onclick='move(2,375)'>Center</button><button onclick='move(2,500)'>Right</button></div>";
    html += "<div><button class='limit-btn' onclick='extend(2, \"min\")'>Extend Left Limit</button>";
    html += "<button class='limit-btn' onclick='extend(2, \"max\")'>Extend Right Limit</button></div>";
    html += "<div class='limit-info'>Min: <span id='min2'>" + String(neckMin) + "</span> | Max: <span id='max2'>" + String(neckMax) + "</span></div></div>";
    
    html += "</div>";
    
    // Control buttons
    html += "<div style='max-width:400px;margin:0 auto'>";
    html += "<button class='home-btn' onclick='homeAll()'>Home All Servos (Center)</button>";
    html += "<button class='save-btn' onclick='saveCal()'>Save Calibration to Memory</button>";
    html += "<button class='reset-btn' onclick='resetCal()'>Reset to Default</button>";
    html += "</div>";
    
    html += "<div class='status' id='status'>Ready - Move sliders to find limits, then extend and save</div>";
    
    html += "<script>";
    html += "function move(servo, pulse) {";
    html += "  fetch('/set?servo='+servo+'&pulse='+pulse);";
    html += "  document.getElementById('v'+servo).innerText = pulse;";
    html += "  document.getElementById('s'+servo).value = pulse;";
    html += "}";
    
    html += "function homeAll() {";
    html += "  move(0,375); move(1,375); move(2,375);";
    html += "  document.getElementById('status').innerHTML = 'All servos moved to center';";
    html += "}";
    
    html += "let pendingExtend = [null, null, null];";
    
    html += "function extend(servo, type) {";
    html += "  pendingExtend[servo] = type;";
    html += "  let msg = 'Move slider to desired position, then click Confirm on the popup';";
    html += "  let confirmMsg = confirm(msg + '\\n\\nClick OK to set new limit, Cancel to abort.');";
    html += "  if(confirmMsg) {";
    html += "    let currentPulse = parseInt(document.getElementById('s'+servo).value);";
    html += "    fetch('/extend?servo='+servo+'&type='+type+'&value='+currentPulse);";
    html += "    if(type === 'min') {";
    html += "      document.getElementById('min'+servo).innerText = currentPulse;";
    html += "      document.getElementById('status').innerHTML = 'Servo '+(servo+1)+' min limit set to '+currentPulse;";
    html += "    } else {";
    html += "      document.getElementById('max'+servo).innerText = currentPulse;";
    html += "      document.getElementById('status').innerHTML = 'Servo '+(servo+1)+' max limit set to '+currentPulse;";
    html += "    }";
    html += "    document.getElementById('s'+servo).min = document.getElementById('min'+servo).innerText;";
    html += "    document.getElementById('s'+servo).max = document.getElementById('max'+servo).innerText;";
    html += "  }";
    html += "  pendingExtend[servo] = null;";
    html += "}";
    
    html += "function saveCal() {";
    html += "  fetch('/save');";
    html += "  document.getElementById('status').innerHTML = 'Calibration saved to memory!';";
    html += "}";
    
    html += "function resetCal() {";
    html += "  if(confirm('Reset all limits to default?')) {";
    html += "    fetch('/reset');";
    html += "    document.getElementById('status').innerHTML = 'Reset to default. Refreshing...';";
    html += "    setTimeout(() => location.reload(), 2000);";
    html += "  }";
    html += "}";
    html += "</script></body></html>";
    
    server.send(200, "text/html", html);
  });
  
  server.on("/set", []() {
    if (server.hasArg("servo") && server.hasArg("pulse")) {
      int servo = server.arg("servo").toInt();
      int pulse = server.arg("pulse").toInt();
      setServo(servo, pulse);
      server.send(200, "text/plain", "OK");
    }
  });
  
  server.on("/extend", []() {
    if (server.hasArg("servo") && server.hasArg("type") && server.hasArg("value")) {
      int servo = server.arg("servo").toInt();
      String type = server.arg("type");
      int value = server.arg("value").toInt();
      
      if (type == "min") {
        if (servo == 0) eyesMin = value;
        else if (servo == 1) mouthMin = value;
        else if (servo == 2) neckMin = value;
      } else {
        if (servo == 0) eyesMax = value;
        else if (servo == 1) mouthMax = value;
        else if (servo == 2) neckMax = value;
      }
      server.send(200, "text/plain", "OK");
    }
  });
  
  server.on("/save", []() {
    saveCalibration();
    server.send(200, "text/plain", "OK");
  });
  
  server.on("/reset", []() {
    eyesMin = 250; eyesMax = 500;
    mouthMin = 250; mouthMax = 500;
    neckMin = 250; neckMax = 500;
    saveCalibration();
    server.send(200, "text/plain", "OK");
  });
  
  server.begin();
  Serial.println("Web server started");
  Serial.println("Open browser to: " + WiFi.localIP().toString());
}

void loop() {
  server.handleClient();
  delay(1);
}
