/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LM84
        This code is designed to work with the LM84_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "LM84.h"

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
    return (int8_t)(i2cread());
}


/**************************************************************************/
/*
        Instantiates a new LM84 class with appropriate properties
*/
/**************************************************************************/
void LM84::getAddr_LM84(uint8_t i2cAddress)
{
    lm_i2cAddress = i2cAddress;
    lm_conversionDelay = LM84_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
boolean LM84::begin()
{
    Wire.begin();
    
    if (readRegister(lm_i2cAddress, LM84_REG_MANUFID) != 0x00) return false;
    return true;
}

/**************************************************************************/
/*
        Sets the T_CRIT_A Interrupts Configuration
*/
/**************************************************************************/
void LM84::setTCritA(lmTCritA_t tcrita)
{
    lm_tcrita = tcrita;
}

/**************************************************************************/
/*
        Gets the T_CRIT_A Interrupts Configuration
*/
/**************************************************************************/
lmTCritA_t LM84::getTCritA()
{
    return lm_tcrita;
}

/**************************************************************************/
/*
        Sets up the Diode Input Digital Temperature Sensor
*/
/**************************************************************************/
void LM84::setUpConfig()
{
    // Set Up the Configuration
    // Set the T_CRIT_A Interrupts Mask
    uint8_t config =  lm_tcrita;

    // Write config register to the device
    writeRegister(lm_i2cAddress, LM84_REG_COMMAND_WC, config);
    
    // Wait for the conversion to complete
    delay(lm_conversionDelay);
    
}

/**************************************************************************/
/*
        Reads the results, measuring the 8-bit temperature registers
*/
/**************************************************************************/
void LM84::Measure_Temp()
{
    // Read the Local Temperature conversion results
    // 8-bit signed result for the LM84
    uint8_t local_temp = readRegister(lm_i2cAddress, LM84_REG_COMMAND_RLT);
    lm_tempData.LT = (int8_t)local_temp;
    
    // Read the Remote Temperature conversion results
    // 8-bit signed result for the LM84
    uint8_t remote_temp = readRegister(lm_i2cAddress, LM84_REG_COMMAND_RRT);
    lm_tempData.RT = (int8_t)remote_temp;
    
}
