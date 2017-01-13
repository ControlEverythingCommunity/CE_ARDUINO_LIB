/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ISL29001
        This code is designed to work with the ISL29001_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "ISL29001.h"

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
    return (uint16_t)(i2cread() | (i2cread() << 8));
}

/**************************************************************************/
/*
        Instantiates a new ISL29001 class with appropriate properties
*/
/**************************************************************************/
void ISL29001::getAddr_ISL29001(uint8_t i2cAddress)
{
    isl_i2cAddress = i2cAddress;
    isl_conversionDelay = ISL29001_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void ISL29001::begin(isl_command_t command)
{
    Wire.begin();
    
    // Sending command to the ISL29001
    writeRegister(isl_i2cAddress, command);
}

/**************************************************************************/
/*
        Reads the results by sending read command
        Measuring the 16-bit resolution register
*/
/**************************************************************************/
uint16_t ISL29001::Measure_Light()
{
    // Read the conversion results
    // Reading 16 bit unsigned Integrated Light data
    uint16_t raw_light = readRegister(isl_i2cAddress);
    return (uint16_t)raw_light;
}
