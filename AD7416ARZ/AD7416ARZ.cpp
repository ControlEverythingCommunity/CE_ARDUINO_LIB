/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        AD7416ARZ
        This code is designed to work with the AD7416ARZ_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=AD7416ARZ_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "AD7416ARZ.h"

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
        Instantiates a new AD7416ARZ class with appropriate properties
*/
/**************************************************************************/
void AD7416ARZ::getAddr_AD7416ARZ(uint8_t i2cAddress)
{
    ad7416arz_i2cAddress = i2cAddress;
    ad7416arz_conversionDelay = AD7416ARZ_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void AD7416ARZ::begin()
{
    Wire.begin();
}

/**************************************************************************/
/*
        Sets the Channel Selection
*/
/**************************************************************************/
void AD7416ARZ::setChannel(ad7416arzChannel_t channel)
{
    ad7416arz_channel = channel;
}

/**************************************************************************/
/*
        Gets the Channel Selection
*/
/**************************************************************************/
ad7416arzChannel_t AD7416ARZ::getChannel()
{
    return ad7416arz_channel;
}

/**************************************************************************/
/*
        Sets the Fault Queue
        The number of faults that occur
*/
/**************************************************************************/
void AD7416ARZ::setFaultQue(ad7416arzFaultQue_t faultqueue)
{
    ad7416arz_faultqueue = faultqueue;
}

/**************************************************************************/
/*
        Gets the Fault Queue
*/
/**************************************************************************/
ad7416arzFaultQue_t AD7416ARZ::getFaultQue()
{
    return ad7416arz_faultqueue;
}

/**************************************************************************/
/*
        Sets the OTI Output
*/
/**************************************************************************/
void AD7416ARZ::setOTI(ad7416arzOTI_t oti)
{
    ad7416arz_OTI = oti;
}

/**************************************************************************/
/*
        Gets the OTI Output
*/
/**************************************************************************/
ad7416arzOTI_t AD7416ARZ::getOTI()
{
    return ad7416arz_OTI;
}

/**************************************************************************/
/*
        Sets the Mode of Operation
*/
/**************************************************************************/
void AD7416ARZ::setMod(ad7416arzMod_t mod)
{
    ad7416arz_mod = mod;
}

/**************************************************************************/
/*
        Gets the Mode of Operation
*/
/**************************************************************************/
ad7416arzMod_t AD7416ARZ::getMod()
{
    return ad7416arz_mod;
}

/**************************************************************************/
/*
        Sets the Shutdown Mode
*/
/**************************************************************************/
void AD7416ARZ::setShutdown(ad7416arzShutdown_t shutdown)
{
    ad7416arz_shutdown = shutdown;
}

/**************************************************************************/
/*
        Gets the Shutdown Mode
*/
/**************************************************************************/
ad7416arzShutdown_t AD7416ARZ::getShutdown()
{
    return ad7416arz_shutdown;
}

/**************************************************************************/
/*
        Reads the results, measuring the 16-bit temperature register
*/
/**************************************************************************/
int16_t AD7416ARZ::Measure_Temp()
{
  
    // Start with default values
    uint16_t config =   AD7416ARZ_REG_CONFIG_FAULTQUE_1     |   //  1 Fault Queue
                        AD7416ARZ_REG_CONFIG_OTI_LOW        |   //  Active-Low
                        AD7416ARZ_REG_CONFIG_MOD_COMP;          //  Comparator Output
    
    // Set Channel Selection
    config |= ad7416arz_channel;
    
    // Set Shutdown Mode
    config |= ad7416arz_shutdown;

    // Write config register to the ad
    writeRegister(ad7416arz_i2cAddress, AD7416ARZ_REG_POINTER_CONFIG, config);

    // Wait for the conversion to complete
    delay(ad7416arz_conversionDelay);

    // Read the conversion results
    // 16-bit unsigned results for the AD7416ARZ
    uint16_t raw_temp = readRegister(ad7416arz_i2cAddress, AD7416ARZ_REG_POINTER_TEMP);
    return (int16_t)raw_temp;
}
