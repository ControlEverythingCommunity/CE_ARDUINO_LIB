/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        DS620
        This code is designed to work with the DS620_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=DS620_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "DS620.h"

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
        Writes 8-bits to the destination register
*/
/**************************************************************************/
static void writeRegister8(uint8_t i2cAddress, uint8_t value)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)value);
    Wire.endTransmission();
}

/**************************************************************************/
/*
        Writes 8-bits to the specified destination register
*/
/**************************************************************************/
static void writeRegister(uint8_t i2cAddress, uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    i2cwrite((uint8_t)value);
    Wire.endTransmission();
}

/**************************************************************************/
/*
        Reads 16-bits from the specified destination register
*/
/**************************************************************************/
static uint16_t readRegister(uint8_t i2cAddress, uint8_t reg)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)2);
    return (int16_t)((i2cread() << 8) | (i2cread() & 0xF8));
}

/**************************************************************************/
/*
        Instantiates a new DS620 class with appropriate properties
*/
/**************************************************************************/
void DS620::getAddr_DS620(uint8_t i2cAddress)
{
    ds_i2cAddress = i2cAddress;
    ds_conversionDelay = DS620_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void DS620::begin()
{
    Wire.begin();
}

/**************************************************************************/
/*
        Sets the Conversion Resolution
*/
/**************************************************************************/
void DS620::setResolution(dsResolution_t resolution)
{
    ds_resolution = resolution;
}

/**************************************************************************/
/*
        Gets the Conversion Resolution
*/
/**************************************************************************/
dsResolution_t DS620::getResolution()
{
    return ds_resolution;
}

/**************************************************************************/
/*
        Reads the results, measuring the 13-bit temperature register
*/
/**************************************************************************/
int16_t DS620::Measure_Temp()
{
    // Set the Start Command
    uint8_t command = 0x51;  // START CONVERT
    
    // Write the Configuration for the Command Set
    writeRegister8(ds_i2cAddress, command);
    
    // Start with default values for Configuration Register MSB
    uint8_t config_msb =   DS620_REG_CONFIG_DONE_0     |   // Temperature Conversion is in Progress
                            DS620_REG_CONFIG_NVB_0      |   // Write to an E2 Memory Cell is in Progress
                            DS620_REG_CONFIG_THF_0      |   // The Measured Temperature has Reached or Exceeded the Value Stored in the TH Register at Anytime Since Power-Up or Since the Bit was Last Cleared
                            DS620_REG_CONFIG_TLF_0      |   // The Measured Temperature has Met or Fallen the Value Stored in the TL Register at Anytime Since Power-Up or Since the Bit was Last Cleared
                            DS620_REG_CONFIG_AUTOC_1    |   // DS620 Powers-up Converting Temperature
                            DS620_REG_CONFIG_1SHOT_0;       // Continuous Conversion Mode
    
    // Set Conversion Resolution
    config_msb |= ds_resolution;

    // Write data to the Configuration Register MSB
    writeRegister(ds_i2cAddress, DS620_REG_CONFIG_MSB, config_msb);

    // Wait for the conversion to complete
    delay(ds_conversionDelay);
    
    // Start with default values for Configuration Register LSB
    uint8_t config_lsb = DS620_REG_CONFIG_RPO_00;       // Force PO Low
    
    // Write data to the Configuration Register LSB
    writeRegister(ds_i2cAddress, DS620_REG_CONFIG_LSB, config_lsb);
    
    // Wait for the conversion to complete
    delay(ds_conversionDelay);

    // Read the results
    // 16-bit signed results for the DS620
    int16_t raw_temp = readRegister(ds_i2cAddress, DS620_REG_TEMP_MSB);
    // Conversion of the result
    // 13-bit signed result for Temperature Data
    raw_temp = raw_temp / 8;
    return raw_temp;
}
