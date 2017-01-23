/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        AMS5915
        This code is designed to work with the AMS5915_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "AMS5915.h"

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
        Instantiates a new AMS5915 class with appropriate properties
*/
/**************************************************************************/
void AMS5915::getAddr_AMS5915(uint8_t i2cAddress)
{
    ams_i2cAddress = i2cAddress;
    ams_conversionDelay = AMS5915_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void AMS5915::begin()
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
void AMS5915::Measure_PressureAndTemperature(float pMin, float pMax)
{
    uint8_t Pres_Hi, Pres_Low, Temp_Hi, Temp_Low;
    // pressure and temperature digital output in counts
    uint16_t pressureCounts, temperatureCounts;
    uint16_t DigoutPmin = 1638.0;   // digital output at minimum specified pressure in counts
    uint16_t DigoutPmax = 14745.0;  // digital output at maximum specified pressure in counts
    
    Wire.beginTransmission(ams_i2cAddress);
    Wire.endTransmission();
    Wire.requestFrom(ams_i2cAddress, (uint8_t)4);
    Pres_Hi = i2cread();
    Pres_Low = i2cread();
    Temp_Hi = i2cread();
    Temp_Low = i2cread();
    
    // Convert the pressure data to 14-bits and then in physical units (mbar)
    pressureCounts = ((Pres_Hi & 0x3F) << 8) | Pres_Low;
    pressure = (((float)pressureCounts - DigoutPmin)/((DigoutPmax - DigoutPmin)/(pMax - pMin)) + pMin);
    
    // Convert the temperature data to 11-bits and then in physical units (C scale)
    temperatureCounts = ((Temp_Hi << 8) | (Temp_Low & 0xE0));
    temperatureCounts >>= 5;
    temperature = ((float)temperatureCounts * 200.0) / 2048.0 - 50.0;
}

/**************************************************************************/
/*
        Gets the results for Pressure
*/
/**************************************************************************/
float AMS5915::getPressure()
{
    return pressure;
}

/**************************************************************************/
/*
        Gets the results for Temperature
*/
/**************************************************************************/
float AMS5915::getTemperature()
{
    return temperature;
}
