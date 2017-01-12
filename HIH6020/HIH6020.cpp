/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HIH6020
        This code is designed to work with the HIH6020_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=HIH6020_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "HIH6020.h"

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
        Instantiates a new HIH6020 class with appropriate properties
*/
/**************************************************************************/
void HIH6020::getAddr_HIH6020(uint8_t i2cAddress)
{
    hih_i2cAddress = i2cAddress;
    hih_conversionDelay = HIH6020_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void HIH6020::begin()
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
void HIH6020::readRegister(uint8_t i2cAddress)
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
float HIH6020::Measure_Humidity()
{
    return humidity;
}

/**************************************************************************/
/*
        Gets the results for Temperature
*/
/**************************************************************************/
float HIH6020::Measure_Temperature()
{
    return temperature;
}
