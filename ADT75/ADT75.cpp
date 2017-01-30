/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ADT75
        This code is designed to work with the ADT75_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=ADT75_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "ADT75.h"

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
        Instantiates a new ADT75 class with appropriate properties
*/
/**************************************************************************/
void ADT75::getAddr_ADT75(uint8_t i2cAddress)
{
    adt_i2cAddress = i2cAddress;
    adt_conversionDelay = ADT75_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void ADT75::begin()
{
    Wire.begin();
}

/**************************************************************************/
/*
        Sets the Interrupt Mode Only. Enable SMBus alert function mode
*/
/**************************************************************************/
void ADT75::setSM(adtSM_t sm)
{
    adt_sm = sm;
}

/**************************************************************************/
/*
        Gets the Interrupt Mode Only. Enable SMBus alert function mode
*/
/**************************************************************************/
adtSM_t ADT75::getSM()
{
    return adt_sm;
}

/**************************************************************************/
/*
        Sets the One-Shot Mode
        The One-shot mode performs a single temperature
        measurement and returns to Shutdown mode
*/
/**************************************************************************/
void ADT75::setOneShot(adtOneShot_t oneshot)
{
    adt_oneshot = oneshot;
}

/**************************************************************************/
/*
        Gets the One-Shot Mode
*/
/**************************************************************************/
adtOneShot_t ADT75::getOneShot()
{
    return adt_oneshot;
}

/**************************************************************************/
/*
        Sets the Overtemperature Fault Queue
        The number of overtemperature faults that occur before setting the OS/ALERT pin
*/
/**************************************************************************/
void ADT75::setFaultQue(adtFaultQue_t faultqueue)
{
    adt_faultqueue = faultqueue;
}

/**************************************************************************/
/*
        Gets the Overtemperature Fault Queue
*/
/**************************************************************************/
adtFaultQue_t ADT75::getFaultQue()
{
    return adt_faultqueue;
}

/**************************************************************************/
/*
        Sets the OS/Alert Polarity
        Selects the output polarity of the OS/ALERT pin
*/
/**************************************************************************/
void ADT75::setAlertPol(adtAlertPol_t alertpol)
{
    adt_alertpol = alertpol;
}

/**************************************************************************/
/*
        Gets the OS/Alert Polarity
*/
/**************************************************************************/
adtAlertPol_t ADT75::getAlertPol()
{
    return adt_alertpol;
}

/**************************************************************************/
/*
        Sets the Over Temperature Interrupt Modes
*/
/**************************************************************************/
void ADT75::setMod(adtMod_t mod)
{
    adt_mod = mod;
}

/**************************************************************************/
/*
        Gets the Over Temperature Interrupt Modes
*/
/**************************************************************************/
adtMod_t ADT75::getMod()
{
    return adt_mod;
}

/**************************************************************************/
/*
        Sets the Shutdown Mode
*/
/**************************************************************************/
void ADT75::setShutdown(adtShutdown_t shutdown)
{
    adt_shutdown = shutdown;
}

/**************************************************************************/
/*
        Gets the Shutdown Mode
*/
/**************************************************************************/
adtShutdown_t ADT75::getShutdown()
{
    return adt_shutdown;
}

/**************************************************************************/
/*
        Reads the results, measuring the 16-bit temperature register
*/
/**************************************************************************/
int16_t ADT75::Measure_Temp()
{
  
    // Start with default values
    uint16_t config =   ADT75_REG_CONFIG_FAULTQUE_1    |    // 1 Overtemperature Fault
                        ADT75_REG_CONFIG_ALERTPOL_LOW  |    // Active-Low
                        ADT75_REG_CONFIG_TMPMOD_COMP;       // Comparator Output
    
    // Set OS/SMBus Alert Mode
    config |= adt_sm;
    
    // Set One-Shot Mode
    config |= adt_oneshot;
    
    // Set Shutdown Mode
    config |= adt_shutdown;

    // Write config register to the adt
    writeRegister(adt_i2cAddress, ADT75_REG_POINTER_CONFIG, config);

    // Wait for the conversion to complete
    delay(adt_conversionDelay);

    // Read the conversion results
    // 16-bit unsigned results for the ADT75
    uint16_t raw_temp = readRegister(adt_i2cAddress, ADT75_REG_POINTER_TEMP);
    return (int16_t)raw_temp;
}
