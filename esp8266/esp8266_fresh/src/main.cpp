#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <secrets.h>

// WiFi credentials are now defined in secrets.h
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

int PIRSensorOutputPin = 2; // GPIO2 (D4 on most ESP8266 modules)
bool isTriggered = false;  // To track PIR state

// UDP setup
WiFiUDP udp;
const unsigned int localPort = 12345; // Port for listening for incoming UDP packets
const unsigned int targetPort = 8080; // Port to send outgoing UDP messages

// Store the IP and port of the last UDP sender
IPAddress lastClientIP;
bool lastClientKnown = false; // Flag to check if we have a client IP

void setup() {
  pinMode(PIRSensorOutputPin, INPUT); // PIR sensor input
  Serial.begin(115200);              // Serial communication for debugging
  WiFi.begin(ssid, password);

  WiFi.disconnect();   // Disconnect from the network
  delay(1000);         // Wait for 1 second
  WiFi.reconnect();    // Reconnect to the Wi-Fi network


  // Connect to WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println(WiFi.status());
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to Wi-Fi");
  Serial.println(WiFi.localIP());

  // Start UDP server
  udp.begin(localPort);
  Serial.printf("Listening for UDP packets on IP %s, port %d\n", WiFi.localIP().toString().c_str(), localPort);

  // Set Serial timeout
  Serial.setTimeout(100);
}

void loop() {
  // Check for incoming UDP messages
  int packetSize = udp.parsePacket();
  if (packetSize) {
    char incomingPacket[255];
    int len = udp.read(incomingPacket, sizeof(incomingPacket) - 1);
    if (len > 0) {
      incomingPacket[len] = '\0'; // Null-terminate the string
    }

    // Store the sender's IP
    lastClientIP = udp.remoteIP();
    lastClientKnown = true; // Mark that we have a valid client IP
    Serial.printf("Received message: <%s> from %s:%d\n", incomingPacket, lastClientIP.toString().c_str(), udp.remotePort());

    // Send acknowledgment back to the sender
    udp.beginPacket(lastClientIP, targetPort);
    udp.print("ack");
    udp.endPacket();
    Serial.println("Acknowledgment sent.");
  }

  // Check the PIR sensor state
  if (digitalRead(PIRSensorOutputPin) == HIGH) { // Motion detected
    if (!isTriggered) { // Avoid repeated triggers
      isTriggered = true;
      Serial.println("Motion detected!");

      // Send a motion-detected message via UDP
      if (lastClientKnown) {
        udp.beginPacket(lastClientIP, targetPort);
        udp.print("Motion detected!");
        udp.endPacket();
        Serial.println("Motion detected message sent.");
      } else {
        Serial.println("No client IP available to send motion message.");
      }
    }
  } else { // No motion
    if (isTriggered) { // Reset the trigger state
      isTriggered = false;
      Serial.println("Motion stopped.");
    }
  }

  delay(100); // Small delay for stability
}
