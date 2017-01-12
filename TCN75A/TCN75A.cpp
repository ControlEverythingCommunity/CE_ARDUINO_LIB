/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TCN75A
        This code is designed to work with the TCN75A_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=TCN75A_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "TCN75A.h"

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
    i2cwrite((uint8_t)(value));
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
        Instantiates a new TCN75A class with appropriate properties
*/
/**************************************************************************/
void TCN75A::getAddr_TCN75A(uint8_t i2cAddress)
{
    tcn_i2cAddress = i2cAddress;
    tcn_conversionDelay = TCN75A_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void TCN75A::begin()
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
void TCN75A::setOneShot(tcnOneShot_t oneshot)
{
    tcn_oneshot = oneshot;
}

/**************************************************************************/
/*
        Gets the One-Shot Mode
*/
/**************************************************************************/
tcnOneShot_t TCN75A::getOneShot()
{
    return tcn_oneshot;
}


/**************************************************************************/
/*
        Sets the ADC Resolution
        The user can gain better insight into the trends and characteristics
        of the ambient temperature by using a finer resolution.
*/
/**************************************************************************/
void TCN75A::setResolution(tcnResolution_t resolution)
{
    tcn_resolution = resolution;
}

/**************************************************************************/
/*
        Gets the ADC Resolution
*/
/**************************************************************************/
tcnResolution_t TCN75A::getResolution()
{
    return tcn_resolution;
}

/**************************************************************************/
/*
        Sets the Fault Queue
        The fault queue feature can be used as a filter to lessen
        the probability of spurious activation of the ALERT pin
*/
/**************************************************************************/
void TCN75A::setFaultQue(tcnFaultQue_t faultqueue)
{
    tcn_faultqueue = faultqueue;
}

/**************************************************************************/
/*
        Gets the Fault Queue
*/
/**************************************************************************/
tcnFaultQue_t TCN75A::getFaultQue()
{
    return tcn_faultqueue;
}

/**************************************************************************/
/*
        Sets the Alert Polarity
        The ALERT output can be configured as either a
        comparator output or as Interrupt Output mode
*/
/**************************************************************************/
void TCN75A::setAlertPol(tcnAlertPol_t alertpol)
{
    tcn_alertpol = alertpol;
}

/**************************************************************************/
/*
        Gets the Alert Polarity
*/
/**************************************************************************/
tcnAlertPol_t TCN75A::getAlertPol()
{
    return tcn_alertpol;
}

/**************************************************************************/
/*
        Sets the Comparator/Interrupt Mode
*/
/**************************************************************************/
void TCN75A::setMod(tcnMod_t mod)
{
    tcn_mod = mod;
}

/**************************************************************************/
/*
        Gets the the Comparator/Interrupt Mode
*/
/**************************************************************************/
tcnMod_t TCN75A::getMod()
{
    return tcn_mod;
}

/**************************************************************************/
/*
        Sets the Shutdown Mode
*/
/**************************************************************************/
void TCN75A::setShutdown(tcnShutdown_t shutdown)
{
    tcn_shutdown = shutdown;
}

/**************************************************************************/
/*
        Gets the Shutdown Mode
*/
/**************************************************************************/
tcnShutdown_t TCN75A::getShutdown()
{
    return tcn_shutdown;
}

/**************************************************************************/
/*
        Reads the results, measuring the 16-bit temperature register
*/
/**************************************************************************/
int16_t TCN75A::Measure_Temp()
{
  
    // Start with default values
    uint16_t config =   TCN75A_REG_CONFIG_FAULTQUE_1    |      // 1 (Power-up default)
                        TCN75A_REG_CONFIG_ALERTPOL_LOW  |      // Active-low (Power-up default)
                        TCN75A_REG_CONFIG_COMPINT_COMP;        // Comparator output (Power-up default)
    
    // Set One-Shot Mode
    config |= tcn_oneshot;
    
    // Set ADC Resolution
    config |= tcn_resolution;
    
    // Set Shutdown Mode
    config |= tcn_shutdown;

    // Write config register to the tcn
    writeRegister(tcn_i2cAddress, TCN75A_REG_POINTER_CONFIG, config);

    // Wait for the conversion to complete
    delay(tcn_conversionDelay);

    // Read the conversion results
    // 16-bit unsigned results for the TCN75A
    uint16_t raw_temp = readRegister(tcn_i2cAddress, TCN75A_REG_POINTER_TEMP);
    return (int16_t)raw_temp;
}
