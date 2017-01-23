/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        BH1715
        This code is designed to work with the BH1715_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Light?sku=BH1715_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "BH1715.h"

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
        Reads 16-bits from the destination register
*/
/**************************************************************************/
static uint16_t readRegister(uint8_t i2cAddress)
{
    Wire.beginTransmission(i2cAddress);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)2);
    return (uint16_t)((i2cread()<< 8) | i2cread());
}

/**************************************************************************/
/*
        Instantiates a new BH1715 class with appropriate properties
*/
/**************************************************************************/
void BH1715::getAddr_BH1715(uint8_t i2cAddress)
{
    bh_i2cAddress = i2cAddress;
    bh_conversionDelay = BH1715_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void BH1715::begin()
{
    Wire.begin();
}

/**************************************************************************/
/*
        Sets the Measurement Mode
 */
/**************************************************************************/
void BH1715::setResMode(bhresMode_t resmod)
{
    bh_resmode = resmod;
}

/**************************************************************************/
/*
        Gets the Measurement Mode
 */
/**************************************************************************/
bhresMode_t BH1715::getResMode()
{
    return bh_resmode;
}

/**************************************************************************/
/*
        Sets the Resolution Value
*/
/**************************************************************************/
void BH1715::setResolution(bhresolution_t resolution)
{
    bh_resolution = resolution;
}

/**************************************************************************/
/*
        Gets the Resolution Value
*/
/**************************************************************************/
bhresolution_t BH1715::getResolution()
{
    return bh_resolution;
}

/**************************************************************************/
/*
            Resets up the Hardware
 */
/**************************************************************************/
void BH1715::Reset(void)
{
    // Resets the BH1715 with the RESET Instruction
    writeRegister(bh_i2cAddress, BH1715_INTR_RESET);
}

/**************************************************************************/
/*
        Reads the results by sending measurement command
        Measuring the 16-bit result register
*/
/**************************************************************************/
uint16_t BH1715::Measure_Lux()
{
    // Set Measurement Mode
    uint8_t mode = bh_resmode;

    // Sending instruction to the BH1715
    writeRegister(bh_i2cAddress, mode);
    
    // Set Resolution Value
    uint8_t resolution = bh_resolution;
    
    // Sending instruction to the BH1715
    writeRegister(bh_i2cAddress, resolution);

    // Wait for the command to complete
    delay(bh_conversionDelay);

    // Read the conversion results
    // 16-bit unsigned results for the BH1715
    uint16_t raw_lux = readRegister(bh_i2cAddress);
    return (uint16_t)raw_lux;
}
