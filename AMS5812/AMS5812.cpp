/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        AMS5812
        This code is designed to work with the AMS5812_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "AMS5812.h"

/**************************************************************************/
/*
        Abstract away platform differences in Arduino wire library
*/
/**************************************************************************/
static uint8_t i2cread(void)
{
    #if ARDUINO >= 100
        return Wire.read();
    #else
        return Wire.receive();
    #endif
}

/**************************************************************************/
/*
        Abstract away platform differences in Arduino wire library
*/
/**************************************************************************/
static void i2cwrite(uint8_t x)
{
    #if ARDUINO >= 100
        Wire.write((uint8_t)x);
    #else
        Wire.send(x);
    #endif
}

/**************************************************************************/
/*
        Instantiates a new AMS5812 class with appropriate properties
*/
/**************************************************************************/
void AMS5812::getAddr_AMS5812(uint8_t i2cAddress)
{
    ams_i2cAddress = i2cAddress;
    ams_conversionDelay = AMS5812_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void AMS5812::begin()
{
    Wire.begin();
    temperature = 0.0;
    pressure = 0.0;
}

/**************************************************************************/
/*
        Reads 32-bits (4 bytes) from the device
        This contains two pressure bytes and two temperature bytes.
        Reads the results for pressure, Measures the 14-bit pressure and
        Reads the results for temperature, Measures the 11-bit temperature
*/
/**************************************************************************/
void AMS5812::Measure_PressureAndTemperature(float pMin, float pMax, float tMin, float tMax)
{
    uint8_t Pres_Hi, Pres_Low, Temp_Hi, Temp_Low;
    // pressure and temperature digital output in counts
    uint16_t pressureCounts, temperatureCounts;
    uint16_t DigoutPmin = 3277.0;   // digital output at minimum specified pressure in counts
    uint16_t DigoutPmax = 29491.0;  // digital output at maximum specified pressure in counts
    uint16_t DigoutTmin = 3277.0;   // digital output at minimum specified temperature (T = -25°C ) in counts
    uint16_t DigoutTmax = 29491.0;  // digital output at maximum specified temperature (T = 85°C) in counts
    
    Wire.beginTransmission(ams_i2cAddress);
    Wire.endTransmission();
    Wire.requestFrom(ams_i2cAddress, (uint8_t)4);
    Pres_Hi = i2cread();
    Pres_Low = i2cread();
    Temp_Hi = i2cread();
    Temp_Low = i2cread();
    
    // Convert the digital pressure and temperature values into 15-bits words (without units) and then in physical units Pressure: (PSI), Temperature: (C Scale)
    // Each pressure and temperature value is transmitted as a 15-bit word; the actual resolution of the internal A/D converter, however, is only 14 bits
    // Pressure Data
    pressureCounts = (Pres_Hi << 8) | Pres_Low;
    pressure = (((float)pressureCounts - DigoutPmin)/((DigoutPmax - DigoutPmin)/(pMax - pMin)) + pMin);
    // Temperature Data
    temperatureCounts = (Temp_Hi << 8) | Temp_Low;
    temperature = (((float)temperatureCounts - DigoutTmin)/((DigoutTmax - DigoutTmin)/(tMax - tMin)) + tMin);
}

/**************************************************************************/
/*
        Gets the results for Pressure
*/
/**************************************************************************/
float AMS5812::getPressure()
{
    return pressure;
}

/**************************************************************************/
/*
        Gets the results for Temperature
*/
/**************************************************************************/
float AMS5812::getTemperature()
{
    return temperature;
}
