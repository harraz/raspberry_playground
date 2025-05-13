#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <secrets.h>

// ——— Forward declarations —————————————————————————————
inline void debugPrint(const String &msg);
void connectWiFi();
void ackUDP(const String &msg);
void handleUDP();
void handlePIR();
void handleSerial();

// ——— Configuration ———————————————————————————————————
const char* DEVICE_NAME          = "GHAFEER1";
const unsigned int LOCAL_PORT    = 12345;
const unsigned int TARGET_PORT   = 8080;
const unsigned long PIR_INTERVAL = 100;   // ms

#define DEBUG 0  // Set to 1 to enable debug prints

// ——— Pins & State ————————————————————————————————————
const int PIR_PIN    = 2;  // D4
const int RELAY_PIN  = 0;  // D3

bool motionTriggered      = false;
unsigned long lastMillis  = 0;
WiFiUDP    udp;
IPAddress  lastClientIP;
bool       hasClientIP    = false;
String     serialBuffer;            // for non-blocking Serial read

// ——— Helpers ———————————————————————————————————————
inline void debugPrint(const String &msg) {
  #if DEBUG
    Serial.println(msg);
  #endif
}

void connectWiFi() {
  Serial.begin(115200);
  debugPrint("Connecting to Wi-Fi…");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 30000) {
    delay(500);
    debugPrint("…still connecting");
  }

  if (WiFi.status() == WL_CONNECTED) {
    debugPrint("Wi-Fi OK: " + WiFi.localIP().toString());
  } else {
    debugPrint("Wi-Fi FAIL, status " + String(WiFi.status()));
  }
}

void ackUDP(const String &msg) {
  if (!hasClientIP) return;
  udp.beginPacket(lastClientIP, TARGET_PORT);
  udp.printf("ACK:%s MSG:%s IP:%s\n",
             DEVICE_NAME,
             msg.c_str(),
             WiFi.localIP().toString().c_str());
  udp.endPacket();
}

void handleUDP() {
  int pkt = udp.parsePacket();
  if (pkt == 0) return;

  char buf[256];
  int len = udp.read(buf, sizeof(buf) - 1);
  if (len < 0) return;
  buf[len] = '\0';

  lastClientIP = udp.remoteIP();
  hasClientIP  = true;

  String msg(buf);
  msg.trim();
  debugPrint("UDP from " + lastClientIP.toString() + ": " + msg);
  ackUDP(msg);

  // If this is a command, forward the payload to Serial
  if (msg.startsWith("CMD:")) {
    String cmd = msg.substring(4);  // text after "CMD:"
    cmd.trim();
    debugPrint("Serial " + cmd);
    Serial.println(cmd);
  }
}


void handlePIR() {
  bool motion = (digitalRead(PIR_PIN) == HIGH);

  if (motion && !motionTriggered) {
    motionTriggered = true;
    digitalWrite(RELAY_PIN, HIGH);
    debugPrint("Motion ON");
    if (hasClientIP) {
      udp.beginPacket(lastClientIP, TARGET_PORT);
      udp.printf("MD:%s IP:%s Time:%lu\n",
                 DEVICE_NAME,
                 WiFi.localIP().toString().c_str(),
                 millis());
      udp.endPacket();
    }
    Serial.println("ON");
  }
  else if (!motion && motionTriggered) {
    motionTriggered = false;
    digitalWrite(RELAY_PIN, LOW);
    debugPrint("Motion OFF");
    Serial.println("OFF");
  }
}

// Non-blocking Serial read, handle commands from Serial
// This is a simple command parser, it will not handle complex commands
// or arguments. It will only look for "ON", "OFF", and "STATUS" commands.
// It will also handle "SER:<command>" to send a command to the last client IP address
// and "IP:<ip>" to set the last client IP address.
// It will also handle "ACK:<message>" to send an ACK message to the last client IP address
// and "ACK" to send an ACK message to the last client IP address.
// It will also handle "IP" to send the last client IP address to the last client IP address
// and "IP:" to set the last client IP address.
// It will also handle "SER" to send the last command to the last client IP address
// and "SER:" to set the last command to the last client IP address.  
void handleSerial() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\r' || c == '\n') {
      if (serialBuffer.length() > 0) {
        String cmd = serialBuffer;
        serialBuffer = "";
        cmd.trim();

        if (hasClientIP) {
          udp.beginPacket(lastClientIP, TARGET_PORT);
          udp.printf("SER:%s IP:%s Time:%lu MSG:%s\n",
          DEVICE_NAME,
          WiFi.localIP().toString().c_str(),
          millis(), cmd.c_str());
          udp.endPacket();
          debugPrint("Serial command: " + cmd);
        }

        // if (cmd == "ON") {
        //   digitalWrite(RELAY_PIN, HIGH);
        //   Serial.println("Relay ON");
        // } else if (cmd == "OFF") {
        //   digitalWrite(RELAY_PIN, LOW);
        //   Serial.println("Relay OFF");
        // } else if (cmd == "STATUS") {
        //   Serial.println("Status: " + String(motionTriggered ? "ON" : "OFF"));
        // } else {
        //   Serial.println("Unknown command: " + cmd);
        // }
      }
    } else {
      serialBuffer += c;
    }
  }
}

// ——— Arduino entry points —————————————————————————————
void setup() {
  pinMode(PIR_PIN,   INPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  connectWiFi();
  udp.begin(LOCAL_PORT);
  debugPrint("UDP listening on port " + String(LOCAL_PORT));
}

void loop() {
  unsigned long now = millis();

  handleSerial();  // always check for serial commands

  if (now - lastMillis >= PIR_INTERVAL) {
    lastMillis = now;
    handleUDP();
    handlePIR();
  }
}
