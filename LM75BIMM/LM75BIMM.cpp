/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LM75BIMM
        This code is designed to work with the LM75BIMM_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=LM75BIMM_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "LM75BIMM.h"

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
        Reads 16-bits to the specified destination register
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
        Instantiates a new LM75BIMM class with appropriate properties
*/
/**************************************************************************/
void LM75BIMM::getAddr_LM75BIMM(uint8_t i2cAddress)
{
    lm_i2cAddress = i2cAddress;
    lm_conversionDelay = LM75BIMM_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void LM75BIMM::begin()
{
    Wire.begin();
}

/**************************************************************************/
/*
        Sets the Fault Queue
        The fault queue prevents OS false tripping in noisy environments
*/
/**************************************************************************/
void LM75BIMM::setFaultQue(lmFaultQue_t faultqueue)
{
    lm_faultqueue = faultqueue;
}

/**************************************************************************/
/*
        Gets the Fault Queue
*/
/**************************************************************************/
lmFaultQue_t LM75BIMM::getFaultQue()
{
    return lm_faultqueue;
}

/**************************************************************************/
/*
        Sets the OS Polarity
*/
/**************************************************************************/
void LM75BIMM::setOSPol(lmOSPol_t ospol)
{
    lm_ospol = ospol;
}

/**************************************************************************/
/*
        Gets the OS Polarity
*/
/**************************************************************************/
lmOSPol_t LM75BIMM::getOSPol()
{
    return lm_ospol;
}

/**************************************************************************/
/*
        Sets the Comparator/Interrupt Mode
*/
/**************************************************************************/
void LM75BIMM::setMod(lmMod_t mod)
{
    lm_mod = mod;
}

/**************************************************************************/
/*
        Gets the the Comparator/Interrupt Mode
*/
/**************************************************************************/
lmMod_t LM75BIMM::getMod()
{
    return lm_mod;
}

/**************************************************************************/
/*
        Sets the Shutdown Mode
*/
/**************************************************************************/
void LM75BIMM::setShutdown(lmShutdown_t shutdown)
{
    lm_shutdown = shutdown;
}

/**************************************************************************/
/*
        Gets the Shutdown Mode
*/
/**************************************************************************/
lmShutdown_t LM75BIMM::getShutdown()
{
    return lm_shutdown;
}

/**************************************************************************/
/*
        Reads the results, measuring the 16-bit temperature register
*/
/**************************************************************************/
int16_t LM75BIMM::Measure_Temp()
{
  
    // Start with default values
    uint8_t config =    LM75BIMM_REG_CONFIG_FAULTQUE_1      |       // 1 Fault Queue
                        LM75BIMM_REG_CONFIG_OSPOL_LOW       |       // Active-Low
                        LM75BIMM_REG_CONFIG_COMPINT_COMP;           // Comparator Output
    
    // Set Shutdown Mode
    config |= lm_shutdown;

    // Write config register to the lm
    writeRegister(lm_i2cAddress, LM75BIMM_REG_POINTER_CONFIG, config);

    // Wait for the conversion to complete
    delay(lm_conversionDelay);

    // Read the conversion results
    // 16-bit signed results for the LM75BIMM
    int16_t raw_temp = readRegister(lm_i2cAddress, LM75BIMM_REG_POINTER_TEMP);
    return (int16_t)raw_temp;
}
