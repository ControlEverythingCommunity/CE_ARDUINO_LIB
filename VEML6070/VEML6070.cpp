/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        VEML6070
        This code is designed to work with the VEML6070_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "VEML6070.h"

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
static void writeRegister(uint8_t i2cAddress, uint8_t reg)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    Wire.endTransmission();
}

/**************************************************************************/
/*
        Reads 8-bits from the destination register
*/
/**************************************************************************/
static uint8_t readRegister(uint8_t i2cAddress)
{
    Wire.beginTransmission(i2cAddress);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)1);
    return (uint8_t)(i2cread());
}

/**************************************************************************/
/*
        Instantiates a new VEML6070 class with appropriate properties
*/
/**************************************************************************/
void VEML6070::getAddr_VEML6070(uint8_t i2cAddress)
{
    veml_i2cAddress = i2cAddress;
    veml_conversionDelay = VEML6070_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void VEML6070::begin(veml_integrationtime_t itime)
{
    Wire.begin();
    
    // Sending command to the VEML6070
    writeRegister(veml_i2cAddress, itime | 0x02);
}

/**************************************************************************/
/*
        Reads the results by sending read command
        Measuring the 16-bit resolution register
*/
/**************************************************************************/
uint16_t VEML6070::ReadUV()
{
    // Read the conversion results
    // Reading UV data MSB
    uint8_t raw_MSB = readRegister(VEML6070_ADDRESS_HIGH);
    // Reading UV data LSB
    uint8_t raw_LSB = readRegister(VEML6070_ADDRESS_LOW);
    // 16-bit unsigned results for the VEML6070
    uint16_t raw_UV = raw_MSB << 8 | raw_LSB;
    return (uint16_t)raw_UV;
}
