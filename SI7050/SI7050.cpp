/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        SI7050
        This code is designed to work with the SI7050_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=SI7050_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "SI7050.h"

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
        Instantiates a new SI7050 class with appropriate properties
*/
/**************************************************************************/
void SI7050::getAddr_SI7050(uint8_t i2cAddress)
{
    si_i2cAddress = i2cAddress;
    si_conversionDelay = SI7050_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void SI7050::begin()
{
    Wire.begin();
}

/**************************************************************************/
/*
        Sets the Temperature Measurement Mode
 */
/**************************************************************************/
void SI7050::setTempMode(sitempMode_t tempmod)
{
    si_tempmode = tempmod;
}

/**************************************************************************/
/*
        Gets the Temperature Measurement Mode
 */
/**************************************************************************/
sitempMode_t SI7050::getTempMode()
{
    return si_tempmode;
}

/**************************************************************************/
/*
            Resets up the Hardware
 */
/**************************************************************************/
void SI7050::Reset(void)
{
    // Resets the Si7050 with the RESET command
    writeRegister(si_i2cAddress, SI7050_CMD_RESET);
}

/**************************************************************************/
/*
        Reads the results by sending temperature command
        Measuring the 16-bit temperature register
*/
/**************************************************************************/
int16_t SI7050::Measure_Temp()
{
    // Set Temperature Measurement Mode
    uint8_t config = si_tempmode;

    // Sending command to the si7050
    writeRegister(si_i2cAddress, config);

    // Wait for the conversion to complete
    delay(si_conversionDelay);

    // Read the conversion results
    // 16-bit unsigned results for the SI7050
    uint16_t raw_temp = readRegister(si_i2cAddress);
    return (int16_t)raw_temp;
}
