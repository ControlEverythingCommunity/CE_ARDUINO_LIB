/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TSL45315
        This code is designed to work with the TSL45315_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "TSL45315.h"

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
        Reads 8-bits to the specified destination register
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
        Instantiates a new TSL45315 class with appropriate properties
*/
/**************************************************************************/
void TSL45315::getAddr_TSL45315(uint8_t i2cAddress)
{
    tsl_i2cAddress = i2cAddress;
    tsl_conversionDelay = TSL45315_CONVERSIONDELAY;
    
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool TSL45315::begin()
{
    Wire.begin();
    
    // Set up for Digital Ambient Light Sensor
    // setUpSensor();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Select Command Register
*/
/**************************************************************************/
void TSL45315::setCommand(tslCommand_t command)
{
    tsl_command = command;
}

/**************************************************************************/
/*
        Gets the Select Command Register
*/
/**************************************************************************/
tslCommand_t TSL45315::getCommand()
{
    return tsl_command;
}

/**************************************************************************/
/*
        Sets the Operating Mode
*/
/**************************************************************************/
void TSL45315::setOperatingMode(tslOperatingMode_t operatingmode)
{
    tsl_operatingmode = operatingmode;
}

/**************************************************************************/
/*
        Gets the Operating Mode
*/
/**************************************************************************/
tslOperatingMode_t TSL45315::getOperatingMode()
{
    return tsl_operatingmode;
}

/**************************************************************************/
/*
        Sets the PowerSave Mode
*/
/**************************************************************************/
void TSL45315::setPowerSaveMode(tslPowerSaveMode_t powersavemode)
{
    tsl_powersavemode = powersavemode;
}

/**************************************************************************/
/*
        Gets the PowerSave Mode
*/
/**************************************************************************/
tslPowerSaveMode_t TSL45315::getPowerSaveMode()
{
    return tsl_powersavemode;
}

/**************************************************************************/
/*
        Sets the Timer Control sets the Integration Time
*/
/**************************************************************************/
void TSL45315::setTCntrl(tslTCntrl_t tcntrl)
{
    tsl_tcntrl = tcntrl;
}

/**************************************************************************/
/*
        Gets the Timer Control sets the Integration Time
*/
/**************************************************************************/
tslTCntrl_t TSL45315::getTCntrl()
{
    return tsl_tcntrl;
}

/**************************************************************************/
/*
        Sets up the Digital Ambient Light Sensor
*/
/**************************************************************************/
void TSL45315::setUpSensor(void)
{
    // Set Up the Configuration for the Digital Ambient Light Sensor Control Register
    /*
     // Set the Operating Mode
     uint8_t control = tsl_operatingmode;
    */
    
    uint8_t control = TSL45315_REG_ALS_CONTROL_MODE_NORMAL;     // Normal Operation
    
    // Write the configuration to the Digital Ambient Light Sensor Control Register
    writeRegister(tsl_i2cAddress, TSL45315_REG_ALS_CONTROL | TSL45315_REG_ALS_CMD_SELECT, control);
    
    // Wait for the configuration to complete
    delay(tsl_conversionDelay);
    
    // Set Up the Configuration for the Digital Ambient Light Sensor Configuration Register
    /*
     // Set the PowerSave Mode
     uint8_t config = tsl_powersavemode;
     
     // Set the Timer Control sets the Integration Time
     config |= tsl_tcntrl;
    */
    
    uint8_t config = TSL45315_REG_ALS_CONFIG_PSAVESKIP_NOT_ASSERT;  //  Not Asserted, the Power Save States are NOT Skipped Following a Light Integration Cycle for Shorter Sampling Rates
    
    // Set the Timer Control sets the Integration Time
    config |= tsl_tcntrl;
    
    // Write the configuration to the Digital Ambient Light Sensor Configuration Register
    writeRegister(tsl_i2cAddress, TSL45315_REG_ALS_CONFIG | TSL45315_REG_ALS_CMD_SELECT, config);
    
    // Wait for the configuration to complete
    delay(tsl_conversionDelay);
    
}

/**************************************************************************/
/*
        Reads the Digital Ambient Light Sensor
*/
/**************************************************************************/
void TSL45315::Measure_ALS()
{
    uint8_t dataLo, dataHi;
    uint16_t Data;
    uint8_t MULTIPLIER;
    
    // Check the ATIME
    switch (tsl_tcntrl)
    {
        case TCNTRL_400MS:
            MULTIPLIER = 1;
            break;
            
        case TCNTRL_200MS:
            MULTIPLIER = 2;
            break;
            
        case TCNTRL_100MS:
            MULTIPLIER = 4;
            break;
            
        default:
            break;
    }
    
    // Read the Data
    // Reading the ALS Data Low Register
    dataLo = readRegister(tsl_i2cAddress, TSL45315_REG_ALS_DATALOW | TSL45315_REG_ALS_CMD_SELECT);
    // Reading the ALS Data High Register
    dataHi = readRegister(tsl_i2cAddress, TSL45315_REG_ALS_DATAHIGH | TSL45315_REG_ALS_CMD_SELECT);
    // Conversion of the result
    // 16-bit unsigned result for ALS Data of TSL45315
    Data = (uint16_t)((dataHi << 8) | dataLo);
    

    /*
     Light Level (lux) = MULTIPLIER × [ (DATAHIGH << 8) + DATALOW ]
     
     Where: MULTIPLIER = 1 for TCNTRL = 00 (Tint = 400 ms),
            MULTIPLIER = 2 for TCNTRL = 01 (Tint = 200 ms), and
            MULTIPLIER = 4 for TCNTRL = 10 (Tint = 100 ms), and
            << 8 indicates a logical 8-bit shift left operation, and
            TCNTRL is a 2-bit field in the configuration register (0x01)
    */
    
    uint16_t Illuminance = MULTIPLIER * Data;
    
    tsl_alsData.L = Illuminance;
}
