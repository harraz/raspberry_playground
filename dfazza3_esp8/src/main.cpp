#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <secrets.h>

#define DEBUG 0 // Set to 1 to enable debug prints, 0 to disable them

const char* ghafeerName = "ROVER1";  // Device name

// WiFi credentials are now defined in secrets.h
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;


// UDP setup
WiFiUDP udp;
const unsigned int localPort = 12345; // Port for listening for incoming UDP packets
const unsigned int targetPort = 8080; // Port to send outgoing UDP messages

// Store the IP and port of the last UDP sender
IPAddress lastClientIP;
bool lastClientKnown = false; // Flag to check if we have a client IP

void setup() {
  // put your setup code here, to run once:

    Serial.begin(9600);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("WiFi Failed");
        while(1) {
            delay(1000);
        }
    }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to Wi-Fi");
    Serial.println(WiFi.localIP());
  } else {
      Serial.println("Failed to connect to Wi-Fi.");
      Serial.print("Error code: ");
      Serial.println(WiFi.status());
  }

  udp.begin(localPort);


}

void loop() {
  // put your main code here, to run repeatedly:

    // Check for incoming UDP messages
  int packetSize = udp.parsePacket();
  if (packetSize) {
    char incomingPacket[255];
    int len = udp.read(incomingPacket, sizeof(incomingPacket) - 1);
    if (len > 0) {
      incomingPacket[len] = '\0'; // Null-terminate the string
    }

    lastClientIP = udp.remoteIP();
    lastClientKnown = true; // Mark that we have a valid client IP

    #if DEBUG
    Serial.printf("Received message: <%s> from %s:%d\n", incomingPacket, lastClientIP.toString().c_str(), udp.remotePort());
    #endif

    udp.beginPacket(lastClientIP, targetPort);
    udp.printf("ack:%s\n", ghafeerName);
    udp.endPacket();

    Serial.print(incomingPacket);
  }
}

