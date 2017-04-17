/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HIH7130
        This code is designed to work with the HIH7130_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/humidity-and-temperature-sensor-3-rh-1-0-c
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "HIH7130.h"

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
        Instantiates a new HIH7130 class with appropriate properties
*/
/**************************************************************************/
void HIH7130::getAddr_HIH7130(uint8_t i2cAddress)
{
    hih_i2cAddress = i2cAddress;
    hih_conversionDelay = HIH7130_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void HIH7130::begin()
{
    Wire.begin();
    temperature = 0.0;
    humidity = 0.0;
}

/**************************************************************************/
/*
        Reads 32-bits from the destination register
        Reads the results for Humidity, Measures the 14-bit humidity data
        Reads the results for temperature, Measures the 14-bit temperature data
*/
/**************************************************************************/
void HIH7130::readRegister(uint8_t i2cAddress)
{
    uint8_t Hum_Hi, Hum_Lo, Temp_Hi, Temp_Lo;
    uint16_t raw_humidity, raw_temperature;
    
    Wire.beginTransmission(i2cAddress);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)4);
    Hum_Hi  = i2cread();
    Hum_Lo  = i2cread();
    Temp_Hi = i2cread();
    Temp_Lo = i2cread();
    
    // Convert the data to 14-bits
    raw_humidity = ((Hum_Hi & 0x3F) << 8) | Hum_Lo;
    humidity = (raw_humidity * 100.0) / 16382.0;
    raw_temperature = ((Temp_Hi << 8) | (Temp_Lo & 0xFC)) >> 2;
    temperature = (raw_temperature / 16382.0) * 165.0 - 40.0;
    
}

/**************************************************************************/
/*
        Gets the results for Humidity
*/
/**************************************************************************/
float HIH7130::Measure_Humidity()
{
    return humidity;
}

/**************************************************************************/
/*
        Gets the results for Temperature
*/
/**************************************************************************/
float HIH7130::Measure_Temperature()
{
    return temperature;
}
