/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HYT939
        This code is designed to work with the HYT939_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=HYT939_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "HYT939.h"

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
        Instantiates a new HYT939 class with appropriate properties
*/
/**************************************************************************/
void HYT939::getAddr_HYT939(uint8_t i2cAddress)
{
    hih_i2cAddress = i2cAddress;
    hih_conversionDelay = HYT939_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void HYT939::begin()
{
    Wire.begin();
    temperature = 0.0;
    humidity = 0.0;
}

/**************************************************************************/
/*
        Reads 32-bits from the destination register
        Reads the results for Humidity, Measures the 14-bit right-adjusted humidity data
        Reads the results for temperature, Measures the 14-bit left-adjusted temperature data
*/
/**************************************************************************/
void HYT939::readRegister(uint8_t i2cAddress)
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
    humidity = (100.0 / 16383.0) * raw_humidity;
    raw_temperature = ((Temp_Hi << 8) | (Temp_Lo & 0xFC)) >> 2;
    temperature = (165.0 / 16383.0) * raw_temperature - 40.0;
}

/**************************************************************************/
/*
        Gets the results for Humidity
*/
/**************************************************************************/
float HYT939::Measure_Humidity()
{
    return humidity;
}

/**************************************************************************/
/*
        Gets the results for Temperature
*/
/**************************************************************************/
float HYT939::Measure_Temperature()
{
    return temperature;
}
