/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        AT30TS74
        This code is designed to work with the AT30TS74_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "AT30TS74.h"

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
        Writes 16-bits to the specified destination register
*/
/**************************************************************************/
static void writeRegister(uint8_t i2cAddress, uint8_t reg, uint16_t value)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    i2cwrite((uint8_t)(value>>8));
    i2cwrite((uint8_t)(value & 0xFF));
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
    return (int16_t)((i2cread()<< 8) | i2cread());
}

/**************************************************************************/
/*
        Instantiates a new AT30TS74 class with appropriate properties
*/
/**************************************************************************/
void AT30TS74::getAddr_AT30TS74(uint8_t i2cAddress)
{
    at_i2cAddress = i2cAddress;
    at_conversionDelay = AT30TS74_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void AT30TS74::begin()
{
    Wire.begin();
}

/**************************************************************************/
/*
        Sets the One-Shot Mode
        The One-shot mode performs a single temperature
        measurement and returns to Shutdown mode
*/
/**************************************************************************/
void AT30TS74::setOneShot(atOneShot_t oneshot)
{
    at_oneshot = oneshot;
}

/**************************************************************************/
/*
        Gets the One-Shot Mode
*/
/**************************************************************************/
atOneShot_t AT30TS74::getOneShot()
{
    return at_oneshot;
}


/**************************************************************************/
/*
        Sets the Conversion Resolution
        The user can select the conversion resolution
        of the internal sigma-delta ADC
*/
/**************************************************************************/
void AT30TS74::setResolution(atResolution_t resolution)
{
    at_resolution = resolution;
}

/**************************************************************************/
/*
        Gets the Conversion Resolution
*/
/**************************************************************************/
atResolution_t AT30TS74::getResolution()
{
    return at_resolution;
}

/**************************************************************************/
/*
        Sets the Fault Tolerance Queue
        It defines how many consecutive faults must occur before 
        the ALERT pin will be activated
*/
/**************************************************************************/
void AT30TS74::setFaultQue(atFaultQue_t faultqueue)
{
    at_faultqueue = faultqueue;
}

/**************************************************************************/
/*
    Gets the Fault Tolerance Queue
*/
/**************************************************************************/
atFaultQue_t AT30TS74::getFaultQue()
{
    return at_faultqueue;
}

/**************************************************************************/
/*
        Sets the ALERT Polarity
*/
/**************************************************************************/
void AT30TS74::setAlertPol(atAlertPol_t alertpol)
{
    at_alertpol = alertpol;
}

/**************************************************************************/
/*
        Gets the Alert Polarity
*/
/**************************************************************************/
atAlertPol_t AT30TS74::getAlertPol()
{
    return at_alertpol;
}

/**************************************************************************/
/*
        Sets the Comparator/Interrupt Mode
*/
/**************************************************************************/
void AT30TS74::setMod(atMod_t mod)
{
    at_mod = mod;
}

/**************************************************************************/
/*
        Gets the the Comparator/Interrupt Mode
*/
/**************************************************************************/
atMod_t AT30TS74::getMod()
{
    return at_mod;
}

/**************************************************************************/
/*
        Sets the Shutdown Mode
*/
/**************************************************************************/
void AT30TS74::setShutdown(atShutdown_t shutdown)
{
    at_shutdown = shutdown;
}

/**************************************************************************/
/*
        Gets the Shutdown Mode
*/
/**************************************************************************/
atShutdown_t AT30TS74::getShutdown()
{
    return at_shutdown;
}

/**************************************************************************/
/*
        Reads the results, measuring the 16-bit temperature register
*/
/**************************************************************************/
int16_t AT30TS74::Measure_Temp()
{
    
    // Start with default values
    uint16_t config =   AT30TS74_REG_CONFIG_FAULTQUE_1    |     // Alarm after 1 Fault (Default)
                        AT30TS74_REG_CONFIG_ALERTPOL_LOW  |     // ALERT pin is Active Low (Default)
                        AT30TS74_REG_CONFIG_CMPINT_COMP;        // Comparator Mode (Default)
    
    // Set One-Shot Mode
    config |= at_oneshot;
    
    // Set Conversion Resolution
    config |= at_resolution;
    
    // Set Shutdown Mode
    config |= at_shutdown;
    
    // Write config register to the at
    writeRegister(at_i2cAddress, AT30TS74_REG_POINTER_CONFIG, config);
    
    // Wait for the conversion to complete
    delay(at_conversionDelay);
    
    // Read the conversion results
    // 16-bit unsigned results for the AT30TS74
    uint16_t raw_temp = readRegister(at_i2cAddress, AT30TS74_REG_POINTER_TEMP);
    return (int16_t)raw_temp;
}
