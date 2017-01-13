/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TMP101NA
        This code is designed to work with the TMP101NA_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=TMP101NA_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "TMP101NA.h"

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
        Instantiates a new TMP101NA class with appropriate properties
*/
/**************************************************************************/
void TMP101NA::getAddr_TMP101NA(uint8_t i2cAddress)
{
    tmp_i2cAddress = i2cAddress;
    tmp_conversionDelay = TMP101NA_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void TMP101NA::begin()
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
void TMP101NA::setOneShot(tmpOneShot_t oneshot)
{
    tmp_oneshot = oneshot;
}

/**************************************************************************/
/*
        Gets the One-Shot Mode
*/
/**************************************************************************/
tmpOneShot_t TMP101NA::getOneShot()
{
    return tmp_oneshot;
}


/**************************************************************************/
/*
        Sets the Converter Resolution
        The user can gain better insight into the trends and characteristics
        of the ambient temperature by using a finer resolution
*/
/**************************************************************************/
void TMP101NA::setResolution(tmpResolution_t resolution)
{
    tmp_resolution = resolution;
}

/**************************************************************************/
/*
        Gets the Converter Resolution
*/
/**************************************************************************/
tmpResolution_t TMP101NA::getResolution()
{
    return tmp_resolution;
}

/**************************************************************************/
/*
        Sets the Fault Queue
        The fault queue feature can be used as a filter to lessen
        the probability of spurious activation of the ALERT pin
*/
/**************************************************************************/
void TMP101NA::setFaultQue(tmpFaultQue_t faultqueue)
{
    tmp_faultqueue = faultqueue;
}

/**************************************************************************/
/*
        Gets the Fault Queue
*/
/**************************************************************************/
tmpFaultQue_t TMP101NA::getFaultQue()
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
void TMP101NA::setAlertPol(tmpAlertPol_t alertpol)
{
    tmp_alertpol = alertpol;
}

/**************************************************************************/
/*
        Gets the Alert Polarity
*/
/**************************************************************************/
tmpAlertPol_t TMP101NA::getAlertPol()
{
    return tmp_alertpol;
}

/**************************************************************************/
/*
        Sets the Comparator/Interrupt Mode
*/
/**************************************************************************/
void TMP101NA::setMod(tmpMod_t mod)
{
    tmp_mod = mod;
}

/**************************************************************************/
/*
        Gets the Comparator/Interrupt Mode
*/
/**************************************************************************/
tmpMod_t TMP101NA::getMod()
{
    return tmp_mod;
}

/**************************************************************************/
/*
        Sets the Shutdown Mode
*/
/**************************************************************************/
void TMP101NA::setShutdown(tmpShutdown_t shutdown)
{
    tmp_shutdown = shutdown;
}

/**************************************************************************/
/*
        Gets the Shutdown Mode
*/
/**************************************************************************/
tmpShutdown_t TMP101NA::getShutdown()
{
    return tmp_shutdown;
}

/**************************************************************************/
/*
        Reads the results, measuring the 16-bit temperature register
*/
/**************************************************************************/
int16_t TMP101NA::Measure_Temp()
{
  
    // Start with default values
    uint16_t config =   TMP101NA_REG_CONFIG_FAULTQUE_1    |       // 1 Consecutive Fault
                        TMP101NA_REG_CONFIG_ALERTPOL_LOW  |       // Active-Low
                        TMP101NA_REG_CONFIG_TM_COMP;              // Comparator Mode
    
    // Set One-Shot Mode
    config |= tmp_oneshot;
    
    // Set Converter Resolution
    config |= tmp_resolution;
    
    // Set Shutdown Mode
    config |= tmp_shutdown;

    // Write config register to the tmp
    writeRegister(tmp_i2cAddress, TMP101NA_REG_POINTER_CONFIG, config);

    // Wait for the conversion to complete
    delay(tmp_conversionDelay);

    // Read the conversion results
    // 16-bit unsigned results for the TMP101NA
    uint16_t raw_temp = readRegister(tmp_i2cAddress, TMP101NA_REG_POINTER_TEMP);
    return (int16_t)raw_temp;
}
