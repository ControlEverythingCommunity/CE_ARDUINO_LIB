/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TMP112
        This code is designed to work with the TMP112_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=TMP112_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "TMP112.h"

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
        Instantiates a new TMP112 class with appropriate properties
*/
/**************************************************************************/
void TMP112::getAddr_TMP112(uint8_t i2cAddress)
{
    tmp_i2cAddress = i2cAddress;
    tmp_conversionDelay = TMP112_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void TMP112::begin()
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
void TMP112::setOneShot(tmpOneShot_t oneshot)
{
    tmp_oneshot = oneshot;
}

/**************************************************************************/
/*
        Gets the One-Shot Mode
*/
/**************************************************************************/
tmpOneShot_t TMP112::getOneShot()
{
    return tmp_oneshot;
}

/**************************************************************************/
/*
        Sets the Fault Queue
        The fault queue feature can be used as a filter to lessen
        the probability of spurious activation of the ALERT pin
*/
/**************************************************************************/
void TMP112::setFaultQue(tmpFaultQue_t faultqueue)
{
    tmp_faultqueue = faultqueue;
}

/**************************************************************************/
/*
        Gets the Fault Queue
*/
/**************************************************************************/
tmpFaultQue_t TMP112::getFaultQue()
{
    return tmp_faultqueue;
}

/**************************************************************************/
/*
        Sets the Polarity
        The ALERT output can be configured as either a
        comparator output or as Interrupt Output mode
*/
/**************************************************************************/
void TMP112::setAlertPol(tmpAlertPol_t alertpol)
{
    tmp_alertpol = alertpol;
}

/**************************************************************************/
/*
        Gets the Alert Polarity
*/
/**************************************************************************/
tmpAlertPol_t TMP112::getAlertPol()
{
    return tmp_alertpol;
}

/**************************************************************************/
/*
        Sets the Comparator/Interrupt Mode
*/
/**************************************************************************/
void TMP112::setTherMod(tmpTherMod_t thermod)
{
    tmp_thermod = thermod;
}

/**************************************************************************/
/*
        Gets the Comparator/Interrupt Mode
*/
/**************************************************************************/
tmpTherMod_t TMP112::getTherMod()
{
    return tmp_thermod;
}

/**************************************************************************/
/*
        Sets the Shutdown Mode
*/
/**************************************************************************/
void TMP112::setShutdown(tmpShutdown_t shutdown)
{
    tmp_shutdown = shutdown;
}

/**************************************************************************/
/*
        Gets the Shutdown Mode
*/
/**************************************************************************/
tmpShutdown_t TMP112::getShutdown()
{
    return tmp_shutdown;
}

/**************************************************************************/
/*
        Sets the Continuos-Conversion Mode
*/
/**************************************************************************/
void TMP112::setConvRate(tmpConvRate_t convrate)
{
    tmp_convrate = convrate;
}

/**************************************************************************/
/*
        Gets the Continuos-Conversion Mode
*/
/**************************************************************************/
tmpConvRate_t TMP112::getConvRate()
{
    return tmp_convrate;
}

/**************************************************************************/
/*
        Sets the Extended Mode
*/
/**************************************************************************/
void TMP112::setExMod(tmpExMod_t exmod)
{
    tmp_exmod = exmod;
}

/**************************************************************************/
/*
        Gets the Extended Mode
*/
/**************************************************************************/
tmpExMod_t TMP112::getExMod()
{
    return tmp_exmod;
}

/**************************************************************************/
/*
        Reads the results, measuring the 16-bit temperature register
*/
/**************************************************************************/
int16_t TMP112::Measure_Temp()
{
  
    // Start with default values
    uint16_t config =   TMP112_REG_CONFIG_RES_12BIT     |       // 12 bit (0.0625°C) Resolution
                        TMP112_REG_CONFIG_FAULTQUE_1    |       // 1 Consecutive Fault
                        TMP112_REG_CONFIG_ALERTPOL_LOW  |       // Active-Low
                        TMP112_REG_CONFIG_TM_COMP       |       // Comparator Mode
                        TMP112_REG_CONFIG_CR_4          |       // 4 Hz Conversion Rate
                        TMP112_REG_CONFIG_ALERT_MASK    |       // Converter Resolution
                        TMP112_REG_CONFIG_EM_NORMAL;            // Normal Mode
    
    // Set One-Shot Mode
    config |= tmp_oneshot;
    
    // Set Shutdown Mode
    config |= tmp_shutdown;

    // Write config register to the tmp
    writeRegister(tmp_i2cAddress, TMP112_REG_POINTER_CONFIG, config);

    // Wait for the conversion to complete
    delay(tmp_conversionDelay);

    // Read the conversion results
    // 16-bit unsigned results for the TMP112
    uint16_t raw_temp = readRegister(tmp_i2cAddress, TMP112_REG_POINTER_TEMP);
    return (int16_t)raw_temp;
}
