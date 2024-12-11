#include <ESP8266WiFi.h>
#include <TinyGPS++.h>
#include <secrets.h>
#include <WiFiUdp.h>

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char* udpAddress = "192.168.1.206";  // Replace with your PC's IP address
const int udpPort = 8080;                  // Port for UDP communication

TinyGPSPlus gps;
WiFiUDP udp;

void setup() {
  Serial.begin(115200);   // Start serial communication at 9600 baud
  Serial.swap();        // Use GPIO13 (D7) as RX and GPIO15 (D8) as TX for GPS communication

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");

  // Start the UDP service
  udp.begin(udpPort);
  Serial.println("UDP service started");

  // Send initial packet to confirm connection
  udp.beginPacket(udpAddress, udpPort);
  udp.print("UDP service started\n");
  udp.print(WiFi.localIP());
  udp.print("\n");
  udp.endPacket();
}

void loop() {
  while (Serial.available() > 0) {  
    char c = Serial.read();
    if (gps.encode(c)) {  // If GPS data is successfully encoded
      if (gps.location.isUpdated()) {  // If GPS location is updated
        String gpsData = String("Latitude: ") + gps.location.lat() + ", Longitude: " + gps.location.lng();
        Serial.println(gpsData);  // Print GPS data to Serial Monitor

        // Send GPS data over UDP
        udp.beginPacket(udpAddress, udpPort);
        udp.print(gpsData);
        udp.endPacket();
      }
    } else {
      // If no valid GPS data is received
      Serial.println("No data received from GPS");
      delay(1000);  // Add a small delay before checking again
    }
  }
}
