/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        CPS120
        This code is designed to work with the CPS120_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Barometer?sku=CPS120_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "CPS120.h"

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
        Instantiates a new CPS120 class with appropriate properties
*/
/**************************************************************************/
void CPS120::getAddr_CPS120(uint8_t i2cAddress)
{
    cps_i2cAddress = i2cAddress;
    cps_conversionDelay = CPS120_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void CPS120::begin()
{
    Wire.begin();
    temperature = 0.0;
    pressure = 0.0;
}

/**************************************************************************/
/*
        Reads 32-bits (4 bytes) from the device
        Total 5 bytes are received. The first byte contains the I2C address
        followed by two pressure bytes and two temperature bytes.
        Reads the results for pressure, Measures the 14-bit compensated pressure
        with a full scale range of 30 to 120 kPa
        Reads the results for temperature, Measures the 14-bit compensated temperature
*/
/**************************************************************************/
void CPS120::readRegister(uint8_t i2cAddress)
{
    uint8_t Pres_Hi, Pres_Lo, Temp_Hi, Temp_Lo;
    uint16_t raw_pressure, raw_temperature;
    
    Wire.beginTransmission(i2cAddress);
    delay(cps_conversionDelay);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)4);
    Pres_Hi = i2cread();
    Pres_Lo = i2cread();
    Temp_Hi = i2cread();
    Temp_Lo = i2cread();
    
    // Convert the data to 14-bits
    raw_pressure = ((Pres_Hi & 0x3F) << 8) | Pres_Lo;
    pressure = ((raw_pressure / 16384.0) * 90.0) + 30.0;
    raw_temperature = ((Temp_Hi << 8) | (Temp_Lo & 0xFC)) >> 2;
    temperature = ((raw_temperature/ 16384.0) * 165.0) - 40.0;
}

/**************************************************************************/
/*
        Gets the results for Pressure
*/
/**************************************************************************/
float CPS120::Measure_Pressure()
{
    return pressure;
}

/**************************************************************************/
/*
        Gets the results for Temperature
*/
/**************************************************************************/
float CPS120::Measure_Temperature()
{
    return temperature;
}
