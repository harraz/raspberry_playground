#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <secrets.h>

const char* ghafeerName = "ELRASHAASH";  // Device name

// WiFi credentials are now defined in secrets.h
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

int PIRSensorOutputPin = 2;  // GPIO2 (D4 on most ESP8266 modules)
int relayPin = 0;  // GPIO0 (D3 on most ESP8266 modules), change if needed

bool isTriggered = false;  // To track PIR state

// UDP setup
WiFiUDP udp;
const unsigned int localPort = 12345; // Port for listening for incoming UDP packets
const unsigned int targetPort = 8080; // Port to send outgoing UDP messages

// Store the IP and port of the last UDP sender
IPAddress lastClientIP;  // No longer const, as it changes
bool lastClientKnown = false; // Flag to check if we have a client IP

#define DEBUG 1 // Set to 1 to enable debug prints, 0 to disable them

unsigned long previousMillis = 0;  
const long interval = 100;  // Interval to check motion

void setup() {
  pinMode(PIRSensorOutputPin, INPUT); // PIR sensor input

  pinMode(relayPin, OUTPUT);          // Relay output
  digitalWrite(relayPin, LOW);        // Turn off the relay

  Serial.begin(115200);              // Serial communication for debugging

  #if DEBUG
  Serial.println("Starting up...");
  #endif

  WiFi.begin(ssid, password);

  // Connect to WiFi with a timeout to prevent indefinite blocking
  unsigned long startAttemptTime = millis();
  unsigned long timeout = 30000;  // 30 seconds timeout

  #if DEBUG
  Serial.println("Attempting to connect to WiFi...");
  #endif

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout) {
    delay(1000);
    #if DEBUG
    Serial.println("Connecting to WiFi...");
    #endif
  }

  if (WiFi.status() == WL_CONNECTED) {
    #if DEBUG
    Serial.println("Connected to Wi-Fi");
    Serial.println(WiFi.localIP());
    #endif
  } else {
    #if DEBUG
      Serial.println("Failed to connect to Wi-Fi.");
      Serial.print("Error code: ");
      Serial.println(WiFi.status());
    #endif
  }

  // Start UDP server
  udp.begin(localPort);
  #if DEBUG
  Serial.printf("Listening for UDP packets on IP %s, port %d\n", WiFi.localIP().toString().c_str(), localPort);
  #endif
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Check if it's time to check the PIR sensor
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Save the last time we checked

    // Check for incoming UDP messages
    int packetSize = udp.parsePacket();
    if (packetSize) {
      char incomingPacket[255];
      int len = udp.read(incomingPacket, sizeof(incomingPacket) - 1);
      if (len > 0) {
        incomingPacket[len] = '\0'; // Null-terminate the string
      }

      lastClientIP = udp.remoteIP();   // Store the client IP
      lastClientKnown = true; // Mark that we have a valid client IP

      #if DEBUG
      Serial.printf("Received message: <%s> from %s:%d\n", incomingPacket, lastClientIP.toString().c_str(), udp.remotePort());
      #endif

      udp.beginPacket(lastClientIP, targetPort);
      udp.printf("ACK:%s, IP:%s\n", ghafeerName, WiFi.localIP().toString().c_str());
      udp.endPacket();

      #if DEBUG
      Serial.printf("Acknowledgment sent from: %s\n", ghafeerName);
      #endif
    }

    // PIR sensor logic
    if (digitalRead(PIRSensorOutputPin) == HIGH) { // Motion detected
      if (!isTriggered) { // Avoid repeated triggers
        isTriggered = true;
        #if DEBUG
        Serial.printf("Motion detected from %s\n", ghafeerName);
        #endif

        // Activate the relay when motion is detected
        digitalWrite(relayPin, HIGH); // Turn on the relay
        
        // Send motion-detected message via UDP
        if (lastClientKnown) {
          udp.beginPacket(lastClientIP, targetPort);
          udp.printf("MD:%s, IP:%s, Time:%lu\n", ghafeerName, WiFi.localIP().toString().c_str(), millis());
          udp.endPacket();
          #if DEBUG
          Serial.printf("Motion detected message sent by: %s\n", ghafeerName);
          #endif
        }
      }
    } else { // No motion
      if (isTriggered) { // Reset the trigger state
        isTriggered = false;
  
        // Deactivate the relay when motion stops
        digitalWrite(relayPin, LOW); // Turn off the relay
        
        #if DEBUG
        Serial.println("Motion stopped.");
        #endif
      }
    }
  }
}
