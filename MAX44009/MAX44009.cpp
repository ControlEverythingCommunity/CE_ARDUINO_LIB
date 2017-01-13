/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MAX44009
        This code is designed to work with the MAX44009_I2CS I2C Mini Module available from CONFIGEverything.com.
        https://www.CONFIGeverything.com/products
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "MAX44009.h"

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
        Reads 8-bits from the specified destination register
*/
/**************************************************************************/
static uint8_t readRegister(uint8_t i2cAddress, uint8_t reg)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)1);
    return (uint8_t)(i2cread());
}

/**************************************************************************/
/*
        Instantiates a new MAX44009 class with appropriate properties
*/
/**************************************************************************/
void MAX44009::getAddr_MAX44009(uint8_t i2cAddress)
{
    max_i2cAddress = i2cAddress;
    max_conversionDelay = MAX44009_CONVERSIONDELAY;
    max_intrenable = ENABLE_NO_ASSRT;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void MAX44009::begin()
{
    Wire.begin();
}

/**************************************************************************/
/*
        Gets the Interrupt Status
*/
/**************************************************************************/
int8_t MAX44009::getIntr_Status(void)
{
    return readRegister(max_i2cAddress, MAX44009_REG_INTR_STATUS);
}

/**************************************************************************/
/*
        Sets the Interrupt Enable
*/
/**************************************************************************/
void MAX44009::setIntrEnable(maxIntrEnable_t intrenable)
{
    max_intrenable = intrenable;
}

/**************************************************************************/
/*
        Gets the Interrupt Enable
*/
/**************************************************************************/
maxIntrEnable_t MAX44009::getIntrEnable()
{
    return max_intrenable;
}

/**************************************************************************/
/*
        Sets the Continuous Mode
*/
/**************************************************************************/
void MAX44009::setContMode(maxContMode_t contmode)
{
    max_contmode = contmode;
}

/**************************************************************************/
/*
        Gets the Continuous Mode
*/
/**************************************************************************/
maxContMode_t MAX44009::getContMode()
{
    return max_contmode;
}

/**************************************************************************/
/*
        Sets the Manual Configuration Mode
*/
/**************************************************************************/
void MAX44009::setManualMode(maxManualMode_t manualmode)
{
    max_manualmode = manualmode;
}

/**************************************************************************/
/*
        Gets the Manual Configuration Mode
*/
/**************************************************************************/
maxManualMode_t MAX44009::getManualMode()
{
    return max_manualmode;
}

/**************************************************************************/
/*
        Sets the Current Division Ratio (CDR)
*/
/**************************************************************************/
void MAX44009::setCDR(maxCDR_t cdr)
{
    max_cdr = cdr;
}

/**************************************************************************/
/*
        Gets the Current Division Ratio (CDR)
*/
/**************************************************************************/
maxCDR_t MAX44009::getCDR()
{
    return max_cdr;
}

/**************************************************************************/
/*
        Sets the Integration Timer Bits
*/
/**************************************************************************/
void MAX44009::setIntrTimer(maxIntrTimer_t intrtimer)
{
    max_intrtimer = intrtimer;
}

/**************************************************************************/
/*
        Gets the Integration Timer Bits
*/
/**************************************************************************/
maxIntrTimer_t MAX44009::getIntrTimer()
{
    return max_intrtimer;
}

/**************************************************************************/
/*
        Reads the results, measuring the 16-bit Light register
*/
/**************************************************************************/
float MAX44009::Measure_Light()
{
    // Start with default values
    uint8_t intrenable = max_intrenable;    // Interrupt Enable, the INT pin and the INTS bit are not asserted even if an interrupt event has occurred
    
    // Write command register to the device
    writeRegister(max_i2cAddress, MAX44009_REG_INTR_ENABLE, intrenable);

    // Set the Continous Mode
    uint8_t config = max_contmode;
    
    // Set the Manual Configuration Mode
    config |= max_manualmode;
    
    // Set the Current Division Ratio (CDR)
    config |= max_cdr;
    
    // Set the Integration Timer Bits
    config |= max_intrtimer;
    
    // Write configuration register to the device
    writeRegister(max_i2cAddress, MAX44009_REG_CONFIG, config);

    // Wait for the conversion to complete
    delay(max_conversionDelay);

    // Read the conversion results
    // 16-bit signed results for the MAX44009
    uint8_t raw_high = readRegister(max_i2cAddress, MAX44009_REG_LUX_HIGH);
    uint8_t raw_low = readRegister(max_i2cAddress, MAX44009_REG_LUX_LOW);
    uint8_t exponent = ((uint8_t)raw_high & 0xF0) >> 4;
    uint8_t mantissa = (((uint8_t)raw_high & 0x0F) << 4) | ((uint8_t)raw_low & 0x0F);
    float raw_light = pow(2, exponent) * mantissa * 0.045;
    return (float)raw_light;
}
