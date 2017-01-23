/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HCPA-5V-U3
        This code is designed to work with the HCPA-5V-U3_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=HCPA-5V-U3_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "HCPA_5V_U3.h"
    
/**************************************************************************/
/*
        Constructor for class HCPA_5V_U3
*/
/**************************************************************************/
HCPA_5V_U3::HCPA_5V_U3()
{
}

/**************************************************************************/
/*
        Writes 16-bits to the destination register
*/
/**************************************************************************/
static void writeRegister(uint8_t i2cAddress, uint8_t value)
{
    Wire.beginTransmission(i2cAddress);
    Wire.write((uint8_t)value);
    Wire.endTransmission();
}

/**************************************************************************/
/*
        Instantiates a new HCPA_5V_U3 class with appropriate properties
*/
/**************************************************************************/
void HCPA_5V_U3::getAddr_HCPA_5V_U3(uint8_t i2cAddress)
{
    hcpa_i2cAddress = i2cAddress;
    hcpa_conversionDelay = HCPA_5V_U3_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void HCPA_5V_U3::begin()
{
    Wire.begin();
    // Ends Command Mode and transitions to Normal Operation Mode
    writeRegister(hcpa_i2cAddress, HCPA_5V_U3_COMMAND_NORMAL);
    // Start Command Mode: used to enter the command interpreting mode
    // writeRegister(hcpa_i2cAddress, HCPA_5V_U3_COMMAND_START);
    delay(hcpa_conversionDelay);
}

/**************************************************************************/
/*
        Gets the converted physical value of relative humidity
*/
/**************************************************************************/
float HCPA_5V_U3::getHumidity(void)
{
    if (! Measure_HumidityAndTemperature())
    {
        return NAN;
    }
    
    return humidity;
}

/**************************************************************************/
/*
        Gets the converted physical value of temperature
*/
/**************************************************************************/
float HCPA_5V_U3::getTemperature(void)
{
    if (! Measure_HumidityAndTemperature())
    {
        return NAN;
    }
    return temperature;
}

/**************************************************************************/
/*
        Measures the 14-bit (unsigned integer) values of Temperature and Humidity
*/
/**************************************************************************/
bool HCPA_5V_U3::Measure_HumidityAndTemperature()
{
    uint8_t buffer[4];
    
    Wire.requestFrom(hcpa_i2cAddress, (uint8_t)4);
    if (Wire.available() != 4)
    {
        return false;
    }
    for (uint8_t i=0; i<4; i++)
    {
        buffer[i] = Wire.read();
    }
    
    uint16_t rawRH, rawTemp;
    // Temperature and Humidity values that are linearized and compensated for temperature and supply voltage effects
    rawRH = buffer[0] & 0x3F;
    rawRH <<= 8;
    rawRH |= buffer[1];
    
    rawTemp = buffer[2];
    rawTemp <<= 8;
    rawTemp |= buffer[3] & 0xFC;
    
    //  Conversion of raw values into a physical scale
    float hum = rawRH;
    hum = (rawRH * 100.0) / 16384.0;
    humidity = hum;
    
    float temp = rawTemp >> 2;
    temp = (temp  * 165.0) / 16384.0 - 40.0;
    temperature = temp;
    
    return true;
}
