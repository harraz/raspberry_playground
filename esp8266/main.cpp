
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "XXX";
const char* password = "XXX";
const IPAddress raspberryPiIP(192, 168, 1, 206); // Replace with your Raspberry Pi IP
const unsigned int rpiPort = 8080; // Port to send to on Raspberry Pi

WiFiUDP udp;

const unsigned int localPort = 12345; // Port to listen on for incoming UDP packets

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to Wi-Fi");
  Serial.println(WiFi.localIP());

  udp.begin(localPort);
  Serial.printf("Listening for UDP packets on IP %s, port %d\n", WiFi.localIP().toString().c_str(), localPort);
}

void loop() {
  int packetSize = udp.parsePacket();
  if (packetSize) {
    char incomingPacket[255];
    int len = udp.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = '\0';
    }
//    Serial.printf("Received message: %s\n", incomingPacket);
    Serial.println(incomingPacket);  // Send the message to Arduino via Serial
    }

  // Check for Serial input from Arduino
  if (Serial.available()) {
    String message = Serial.readStringUntil('\n');
    udp.beginPacket(raspberryPiIP, rpiPort);
    udp.print(message);
    udp.endPacket();
  }
}