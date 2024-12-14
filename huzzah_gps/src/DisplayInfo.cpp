#include "DisplayInfo.h"

// Assuming these are globally defined in the main program
extern TinyGPSPlus gps;
extern LiquidCrystal_I2C lcd;

void printToLCD(const String &line1, const String &line2)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
}

void printToSerial(const String &message)
{
  Serial.println(message);
}

// void displayLocation(bool toLCD, bool toSerial)
// {
//   String message = "Location: ";
//   String line2;

//   if (gps.location.isValid())
//   {
//     line2 = "Lat:" + String(gps.location.lat(), 6) + ", Lng:" + String(gps.location.lng(), 6);
//     message += "Lat: " + String(gps.location.lat(), 6) + ", Lng: " + String(gps.location.lng(), 6);
//   }
//   else
//   {
//     line2 = "INVALID";
//     message += "INVALID";
//   }

//   if (toLCD)
//     printToLCD("Location:", line2);
//   if (toSerial)
//     printToSerial(message);
// }

// void displayDateTime(bool toLCD, bool toSerial)
// {
//   String dateMessage = "Date: ";
//   String timeMessage = "Time: ";
//   String line1, line2;

//   if (gps.date.isValid())
//   {
//     line1 = String(gps.date.month()) + "/" + String(gps.date.day()) + "/" + String(gps.date.year());
//     dateMessage += line1;
//   }
//   else
//   {
//     line1 = "INVALID";
//     dateMessage += "INVALID";
//   }

//   if (gps.time.isValid())
//   {
//     line2 = (gps.time.hour() < 10 ? "0" : "") + String(gps.time.hour()) + ":" +
//             (gps.time.minute() < 10 ? "0" : "") + String(gps.time.minute()) + ":" +
//             (gps.time.second() < 10 ? "0" : "") + String(gps.time.second());
//     timeMessage += line2;
//   }
//   else
//   {
//     line2 = "INVALID";
//     timeMessage += "INVALID";
//   }

//   if (toLCD)
//     printToLCD("Date/Time:", line1 + " " + line2);
//   if (toSerial)
//   {
//     printToSerial(dateMessage);
//     printToSerial(timeMessage);
//   }
// }

// void displaySatelliteInfo(bool toLCD, bool toSerial)
// {
//   String satMessage = "Satellites: ";
//   String hdopMessage = "HDOP: ";
//   String line1, line2;

//   if (gps.satellites.isValid())
//   {
//     line1 = "Sat: " + String(gps.satellites.value());
//     satMessage += String(gps.satellites.value());
//   }
//   else
//   {
//     line1 = "Sat: INVALID";
//     satMessage += "INVALID";
//   }

//   if (gps.hdop.isValid())
//   {
//     line2 = "HDOP: " + String(gps.hdop.value());
//     hdopMessage += String(gps.hdop.value());
//   }
//   else
//   {
//     line2 = "HDOP: INVALID";
//     hdopMessage += "INVALID";
//   }

//   if (toLCD)
//     printToLCD(line1, line2);
//   if (toSerial)
//   {
//     printToSerial(satMessage);
//     printToSerial(hdopMessage);
//   }
// }

void displayInfo(DisplayType displayType, bool toLCD, bool toSerial)
{
  if (toLCD) lcd.clear(); // Clear LCD before each update

  switch (displayType)
  {
    case LOCATION:
      if (toSerial)
      {
        Serial.print(F("Location: "));
        if (gps.location.isValid())
        {
          Serial.print(gps.location.lat(), 6);
          Serial.print(F(", "));
          Serial.println(gps.location.lng(), 6);
        }
        else
        {
          Serial.println(F("INVALID"));
        }
      }

      if (toLCD)
      {
        lcd.setCursor(0, 0);
        if (gps.location.isValid())
        {
          lcd.print(F("Lat:"));
          lcd.print(gps.location.lat(), 2);
          lcd.setCursor(0, 1);
          lcd.print(F("Lon:"));
          lcd.print(gps.location.lng(), 2);
        }
        else
        {
          lcd.print(F("Location INVALID"));
        }
      }
      break;

    case DATE_TIME:
      if (toSerial)
      {
        Serial.print(F("Date/Time: "));
        if (gps.date.isValid() && gps.time.isValid())
        {
          Serial.print(gps.date.month());
          Serial.print(F("/"));
          Serial.print(gps.date.day());
          Serial.print(F("/"));
          Serial.print(gps.date.year());
          Serial.print(F(" "));
          Serial.print(gps.time.hour());
          Serial.print(F(":"));
          Serial.print(gps.time.minute());
          Serial.print(F(":"));
          Serial.println(gps.time.second());
        }
        else
        {
          Serial.println(F("INVALID"));
        }
      }

      if (toLCD)
      {
        lcd.setCursor(0, 0);
        if (gps.date.isValid() && gps.time.isValid())
        {
          lcd.print(F("Date:"));
          lcd.print(gps.date.month());
          lcd.print(F("/"));
          lcd.print(gps.date.day());
          lcd.setCursor(0, 1);
          lcd.print(F("Time:"));
          if (gps.time.hour() < 10) lcd.print(F("0"));
          lcd.print(gps.time.hour());
          lcd.print(F(":"));
          if (gps.time.minute() < 10) lcd.print(F("0"));
          lcd.print(gps.time.minute());
        }
        else
        {
          lcd.print(F("Date/Time INVALID"));
        }
      }
      break;

    case SATELLITE_INFO:
      if (toSerial)
      {
        Serial.print(F("Satellites: "));
        if (gps.satellites.isValid())
        {
          Serial.println(gps.satellites.value());
        }
        else
        {
          Serial.println(F("INVALID"));
        }

        Serial.print(F("HDOP: "));
        if (gps.hdop.isValid())
        {
          Serial.println(gps.hdop.value());
        }
        else
        {
          Serial.println(F("INVALID"));
        }
      }

      if (toLCD)
      {
        lcd.setCursor(0, 0);
        if (gps.satellites.isValid())
        {
          lcd.print(F("Sat:"));
          lcd.print(gps.satellites.value());
        }
        else
        {
          lcd.print(F("Sat INVALID"));
        }

        lcd.setCursor(0, 1);
        if (gps.hdop.isValid())
        {
          lcd.print(F("HDOP:"));
          lcd.print(gps.hdop.value());
        }
        else
        {
          lcd.print(F("HDOP INVALID"));
        }
      }
      break;
  }
}

