#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Arduino.h>
#include "DisplayInfo.h"

// Create an instance of the TinyGPS++ object
TinyGPSPlus gps;

// Create a SoftwareSerial instance for communication with the GPS module
// RX pin 12, TX pin 13
SoftwareSerial ss(12, 13);

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Update with your actual I2C address

// Forward declaration of displayInfo()
void displayInfo();

void setup() {
  // Start the serial communication for debugging
  Serial.begin(9600);
  ss.begin(9600);  // Start GPS communication

  lcd.init();        // Initialize the LCD
  lcd.backlight();   // Turn on the LCD backlight
  lcd.print("Initializing"); // Print a test message

  Serial.println("GPS Starting");
}


void loop()
{
    while (ss.available() > 0)
    {
        if (gps.encode(ss.read()))
        {
            displayInfo(LOCATION, true, true);
            delay(2000);

            displayInfo(DATE_TIME, true, true);
            delay(2000);

            displayInfo(SATELLITE_INFO, true, true);
            delay(2000);
        }
    }

    if (millis() > 5000 && gps.charsProcessed() < 10)
    {
        Serial.println(F("No GPS detected: check wiring."));
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("No GPS detected");
        while (true);
    }
}