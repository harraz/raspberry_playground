#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// Create an instance of the TinyGPS++ object
TinyGPSPlus gps;

// Create a SoftwareSerial instance for communication with the GPS module
// RX pin 12, TX pin 13
SoftwareSerial ss(12, 13);

// Forward declaration of displayInfo()
void displayInfo();

void setup() {
  // Start the serial communication for debugging
  Serial.begin(9600);
  ss.begin(9600);  // Start GPS communication

  Serial.println("GPS Test Started");
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (ss.available() > 0)
    if (gps.encode(ss.read())) {
      displayInfo();
      delay(3000);
  }

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(", "));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();

  // Add Satellite Information
  Serial.print(F("Satellites: "));
  if (gps.satellites.isValid())
  {
    Serial.println(gps.satellites.value());
  }
  else
  {
    Serial.println(F("INVALID"));
  }

  // Add HDOP (Horizontal Dilution of Precision)
  Serial.print(F("HDOP: "));
  if (gps.hdop.isValid())
  {
    Serial.println(gps.hdop.value()); // Lower values indicate better precision
  }
  else
  {
    Serial.println(F("INVALID"));
  }

  Serial.println(F("--------------------------------"));
}
