// DisplayInfo.h
#ifndef DISPLAYINFO_H
#define DISPLAYINFO_H

#include <Arduino.h>
#include <TinyGPS++.h>
#include <LiquidCrystal_I2C.h>

extern TinyGPSPlus gps;
extern LiquidCrystal_I2C lcd;

enum DisplayType
{
    LOCATION,
    DATE_TIME,
    SATELLITE_INFO
};

void displayInfo(DisplayType displayType, bool toLCD, bool toSerial);

#endif
