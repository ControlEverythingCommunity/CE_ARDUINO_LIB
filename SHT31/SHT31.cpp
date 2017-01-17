/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        SHT31
        This code is designed to work with the SHT31_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=SHT31_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "SHT31.h"
    
/**************************************************************************/
/*
        Constructor for class SHT31
*/
/**************************************************************************/
SHT31::SHT31()
{
}

/**************************************************************************/
/*
        Writes 16-bits to the destination register
*/
/**************************************************************************/
static void writeRegister(uint8_t i2cAddress, uint16_t value)
{
    Wire.beginTransmission(i2cAddress);
    Wire.write((uint8_t)(value>>8));
    Wire.write((uint8_t)(value & 0xFF));
    Wire.endTransmission();
}

/**************************************************************************/
/*
        Instantiates a new SHT31 class with appropriate properties
*/
/**************************************************************************/
void SHT31::getAddr_SHT31(uint8_t i2cAddress)
{
    sht_i2cAddress = i2cAddress;
    sht_conversionDelay = SHT31_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void SHT31::begin()
{
    Wire.begin();
    Reset();
}

/**************************************************************************/
/*
        Reads the Status register
*/
/**************************************************************************/
uint16_t SHT31::readStatus(void)
{
    // Command to read out the status register
    writeRegister(sht_i2cAddress, SHT31_CMD_READSTATUS);
    delay(sht_conversionDelay);
    Wire.requestFrom(sht_i2cAddress, (uint8_t)3);
    uint16_t status = Wire.read();
    status <<= 8;
    status |= Wire.read();
    return status;
}

/**************************************************************************/
/*
        Resets the system
*/
/**************************************************************************/
void SHT31::Reset(void)
{
    // Resets the SHT31 with the RESET command
    writeRegister(sht_i2cAddress, SHT31_CMD_SOFTRESET);
    delay(sht_conversionDelay);
}

/**************************************************************************/
/*
        Sets the Heater Command Status
*/
/**************************************************************************/
void SHT31::setHeaterStatus(uint16_t heaterstatus)
{
    // Write the command to set the heater enable or disable
    writeRegister(sht_i2cAddress, heaterstatus);
    delay(sht_conversionDelay);
}

/**************************************************************************/
/*
        Gets the converted physical value of temperature
*/
/**************************************************************************/
float SHT31::getTemperature(void)
{
    if (! Measure_TemperatureAndHumidity())
    {
        return NAN;
    }
    return temperature;
}

/**************************************************************************/
/*
        Gets the converted physical value of relative humidity
*/
/**************************************************************************/
float SHT31::getHumidity(void)
{
    if (! Measure_TemperatureAndHumidity())
    {
        return NAN;
    }
    
    return humidity;
}

/**************************************************************************/
/*
        Measures the 16-bit (unsigned integer), the linearized and compensated
        values of Temperature and Humidity for temperature and supply voltage effects.
*/
/**************************************************************************/
bool SHT31::Measure_TemperatureAndHumidity(void)
{
    uint8_t buffer[6];
    
    // Command to set high repeatability measurement with clock stretching enabled
    writeRegister(sht_i2cAddress, SHT31_MEAS_HIGHREP_STRETCH_EN);
    delay(sht_conversionDelay);
    
    Wire.requestFrom(sht_i2cAddress, (uint8_t)6);
    if (Wire.available() != 6)
    {
        return false;
    }
    for (uint8_t i=0; i<6; i++)
    {
        buffer[i] = Wire.read();
    }
    
    uint16_t rawTemp, rawRH;
    // Temperature and Humidity values that are linearized and compensated for temperature and supply voltage effects
    rawTemp = buffer[0];
    rawTemp <<= 8;
    rawTemp |= buffer[1];
    
    if (buffer[2] != CRC8(buffer, 2))
    {
        return false;
    }
    rawRH = buffer[3];
    rawRH <<= 8;
    rawRH |= buffer[4];
    
    if (buffer[5] != CRC8(buffer+3, 2))
    {
        return false;
    }
    
    //  Conversion of raw values into a physical scale
    float temp = rawTemp;
    temp = -45.0 + (175.0 * rawTemp / 65535.0);
    temperature = temp;
    
    float hum = rawRH;
    hum = 100.0 * (rawRH / 65535.0);
    humidity = hum;
    
    return true;
}

/**************************************************************************/
/*
        Checksum Calculation
        A CRC algorithm generates the 8-bit CRC checksum transmitted after each data word
*/
/**************************************************************************/
uint8_t SHT31::CRC8(const uint8_t *idata, int len)
{
    /*
        Polynomial: 0x31 (x8 + x5 + x4 + 1)
        Initialization: 0xFF
        Final XOR: 0x00
        Example: CRC (0xBEEF) = 0x92
    */
    const uint8_t POLYNOMIAL(0x31);
    uint8_t CRC(0xFF);
    
    for ( int j = len; j; --j )
    {
        CRC ^= *idata++;
        for ( int i = 8; i; --i )
        {
            CRC = ( CRC & 0x80 )
            ? (CRC << 1) ^ POLYNOMIAL
            : (CRC << 1);
        }
    }
    return CRC;
}
