/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        STS21
        This code is designed to work with the STS21_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=STS21_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "STS21.h"

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
    return (int16_t)((i2cread()<< 8) | i2cread());
}

/**************************************************************************/
/*
        Instantiates a new STS21 class with appropriate properties
*/
/**************************************************************************/
void STS21::getAddr_STS21(uint8_t i2cAddress)
{
    sts_i2cAddress = i2cAddress;
    sts_conversionDelay = STS21_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void STS21::begin()
{
    Wire.begin();
}

/**************************************************************************/
/*
        Sets the Temperature Measurement Mode
 */
/**************************************************************************/
void STS21::setTempMode(ststempMode_t tempmod)
{
    sts_tempmode = tempmod;
}

/**************************************************************************/
/*
        Gets the Temperature Measurement Mode
 */
/**************************************************************************/
ststempMode_t STS21::getTempMode()
{
    return sts_tempmode;
}

/**************************************************************************/
/*
            Resets up the Hardware
 */
/**************************************************************************/
void STS21::Reset(void)
{
    // Resets the STS21 with the RESET command
    writeRegister(sts_i2cAddress, STS21_CMD_RESET);
}

/**************************************************************************/
/*
        Reads the results by sending temperature command
        Measuring the 16-bit temperature register
*/
/**************************************************************************/
int16_t STS21::Measure_Temp()
{
    // Set Temperature Measurement Mode
    uint8_t config = sts_tempmode;

    // Sending command to the STS21
    writeRegister(sts_i2cAddress, config);

    // Wait for the conversion to complete
    delay(sts_conversionDelay);

    // Read the conversion results
    // 16-bit unsigned results for the STS21
    uint16_t raw_temp = readRegister(sts_i2cAddress);
    return (int16_t)raw_temp;
}
