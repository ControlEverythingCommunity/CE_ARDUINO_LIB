/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TSL2571
        This code is designed to work with the TSL2571_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Light?sku=TSL2571_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "TSL2571.h"

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
        Instantiates a new TSL2571 class with appropriate properties
*/
/**************************************************************************/
void TSL2571::getAddr_TSL2571(uint8_t i2cAddress)
{
    tsl_i2cAddress = i2cAddress;
    tsl_conversionDelay = TSL2571_CONVERSIONDELAY;
    
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool TSL2571::begin()
{
    Wire.begin();
    
    uint8_t devid = readRegister(tsl_i2cAddress, TSL2571_REG_ALS_ID);
    //if (devid != TSL2571_ALS_DEV_ID)
      //  return false;
    
    // Set up for Light-to-Digital Converter
    setUpALS();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Select Command Register
*/
/**************************************************************************/
void TSL2571::setCommand(tslCommand_t command)
{
    tsl_command = command;
}

/**************************************************************************/
/*
        Gets the Select Command Register
*/
/**************************************************************************/
tslCommand_t TSL2571::getCommand()
{
    return tsl_command;
}

/**************************************************************************/
/*
        Sets the ALS Interrupt Mask
*/
/**************************************************************************/
void TSL2571::setALSInterrupt(tslALSInterrupt_t alsinterrupt)
{
    tsl_alsinterrupt = alsinterrupt;
}

/**************************************************************************/
/*
        Gets the ALS Interrupt Mask
*/
/**************************************************************************/
tslALSInterrupt_t TSL2571::getALSInterrupt()
{
    return tsl_alsinterrupt;
}

/**************************************************************************/
/*
        Sets the Wait Enable
*/
/**************************************************************************/
void TSL2571::setWaitEnable(tslWaitEnable_t waitenable)
{
    tsl_waitenable = waitenable;
}

/**************************************************************************/
/*
        Gets the Wait Enable
*/
/**************************************************************************/
tslWaitEnable_t TSL2571::getWaitEnable()
{
    return tsl_waitenable;
}

/**************************************************************************/
/*
        Sets the ALS Enable
*/
/**************************************************************************/
void TSL2571::setALSEnable(tslALSEnable_t alsenable)
{
    tsl_alsenable = alsenable;
}

/**************************************************************************/
/*
        Gets the ALS Enable
*/
/**************************************************************************/
tslALSEnable_t TSL2571::getALSEnable()
{
    return tsl_alsenable;
}

/**************************************************************************/
/*
        Sets the Power ON
*/
/**************************************************************************/
void TSL2571::setPowerEnable(tslPowerEnable_t powerenable)
{
    tsl_powerenable = powerenable;
}
                    
/**************************************************************************/
/*
        Gets the Power ON
*/
/**************************************************************************/
tslPowerEnable_t TSL2571::getPowerEnable()
{
    return tsl_powerenable;
}

/**************************************************************************/
/*
        Sets to Control the Internal Integration Time of the ALS Channel 
        ADC’s in 2.72 ms Increments
*/
/**************************************************************************/
void TSL2571::setATime(tslATime_t atime)
{
    tsl_atime = atime;
}

/**************************************************************************/
/*
        Gets to Control the Internal Integration Time of the ALS Channel 
        ADC’s in 2.72 ms Increments
*/
/**************************************************************************/
tslATime_t TSL2571::getATime()
{
    return tsl_atime;
}

/**************************************************************************/
/*
        Sets to Control the Wait Time is Set in 2.72 ms Increments
        Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
*/
/**************************************************************************/
void TSL2571::setWTime(tslWTime_t wtime)
{
    tsl_wtime = wtime;
}

/**************************************************************************/
/*
        Gets to Control the Wait Time is Set in 2.72 ms Increments
        Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
*/
/**************************************************************************/
tslWTime_t TSL2571::getWTime()
{
    return tsl_wtime;
}

/**************************************************************************/
/*
        Sets the Wait Long
*/
/**************************************************************************/
void TSL2571::setWaitLong(tslWaitLong_t waitlong)
{
    tsl_waitlong = waitlong;
}

/**************************************************************************/
/*
        Gets the Wait Long
*/
/**************************************************************************/
tslWaitLong_t TSL2571::getWaitLong()
{
    return tsl_waitlong;
}

/**************************************************************************/
/*
        Sets the ALS Gain
*/
/**************************************************************************/
void TSL2571::setALSGain(tslALSGain_t alsgain)
{
    tsl_alsgain = alsgain;
}

/**************************************************************************/
/*
        Gets the ALS Gain
*/
/**************************************************************************/
tslALSGain_t TSL2571::getALSGain()
{
    return tsl_alsgain;
}

/**************************************************************************/
/*
        Sets up the Light-to-Digital Converter
*/
/**************************************************************************/
void TSL2571::setUpALS(void)
{
    // Set Up the Configuration for the Light-to-Digital Converter Enable Register
    /*
     // Set the ALS Interrupt Mask
     uint8_t enable = tsl_alsinterrupt;
    
     // Set the Wait Enable
     enable |= tsl_waitenable;
     
     // Set the ALS Enable
     enable |= tsl_alsinable;
     
     // Set the Power ON
     enable |= tsl_powerenable;
    */
    
    uint8_t enable =    TSL2571_REG_ALS_ENABLE_AIEN_NOT_ASSERT      |   // Not Asserted, Did not Permits ALS Interrupts to be Generated
                        TSL2571_REG_ALS_ENABLE_WEN_ENABLE           |   // Enables the Wait Timer
                        TSL2571_REG_ALS_ENABLE_AEN_ENABLE           |   // Activates the ALS
                        TSL2571_REG_ALS_ENABLE_PON_ENABLE;              // Activates the Internal Oscillator to Permit the Timers and ADC Channels to Operate
    
    // Write the configuration to the Light-to-Digital Converter Enable Register
    writeRegister(tsl_i2cAddress, TSL2571_REG_ALS_ENABLE | TSL2571_REG_ALS_CMD_SELECT | TSL2571_REG_ALS_CMD_TYPE_AUTO_INCR, enable);
    
    // Wait for the configuration to complete
    delay(tsl_conversionDelay);
    
    // Set Up the Configuration for the Light-to-Digital Converter ATIME Register
    // Set the Internal Integration Time of the ALS Channel ADC’s in 2.72 ms Increments
    uint8_t atime = tsl_atime;
    
    // Write the configuration to the Light-to-Digital Converter ATIME Register
    writeRegister(tsl_i2cAddress, TSL2571_REG_ALS_ATIME | TSL2571_REG_ALS_CMD_SELECT | TSL2571_REG_ALS_CMD_TYPE_AUTO_INCR, atime);
    
    // Wait for the configuration to complete
    delay(tsl_conversionDelay);
    
    // Set Up the Configuration for the Light-to-Digital Converter WTIME Register
    // Set the Wait Time is Set in 2.72 ms Increments Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
    uint8_t wtime = tsl_wtime;
    
    // Write the configuration to the Light-to-Digital Converter WTIME Register
    writeRegister(tsl_i2cAddress, TSL2571_REG_ALS_WTIME | TSL2571_REG_ALS_CMD_SELECT | TSL2571_REG_ALS_CMD_TYPE_AUTO_INCR, wtime);
    
    // Wait for the configuration to complete
    delay(tsl_conversionDelay);
    
    // Set Up the Configuration for the Light-to-Digital Converter Configuration Register
    /*
     // Set the Wait Long
     uint8_t config |= tsl_waitlong;
    */
    
    uint8_t config = TSL2571_REG_ALS_CONFIG_WLONG_NOT_ASSERT;       // Not Asserted, the Wait Cycles are Not Increased by a Factor 12X from that Programmed in the WTIME Register
    
    // Write the configuration to the Light-to-Digital Converter Configuration Register
    writeRegister(tsl_i2cAddress, TSL2571_REG_ALS_CONFIG | TSL2571_REG_ALS_CMD_SELECT | TSL2571_REG_ALS_CMD_TYPE_AUTO_INCR, config);
    
    // Wait for the configuration to complete
    delay(tsl_conversionDelay);
    
    // Set Up the Configuration for the Light-to-Digital Converter Control Register
    // ALS Gain
    uint8_t control = tsl_alsgain;
    
    // Write the configuration to the Light-to-Digital Converter Control Register
    writeRegister(tsl_i2cAddress, TSL2571_REG_ALS_CONTROL | TSL2571_REG_ALS_CMD_SELECT | TSL2571_REG_ALS_CMD_TYPE_AUTO_INCR, control);
    
    // Wait for the configuration to complete
    delay(tsl_conversionDelay);
}

/**************************************************************************/
/*
        Reads the Light-to-Digital Converter
*/
/**************************************************************************/
void TSL2571::Measure_ALS()
{
    uint8_t C0dataLo, C0dataHi, C1dataLo, C1dataHi;
    uint16_t C0Data, C1Data;
    float atime, again, CPL, Lux1, Lux2, Lux;
    
    // Counts per Lux
    CPL = 0.0;
    // The Final Lux is the Maximum of Lux1, Lux2, or 0
    Lux = 0.0;
    
    // Check the ATIME
    switch (tsl_atime)
    {
        case ATIME_2_72MS:
            atime = 2.72;
            break;
            
        case ATIME_27_2MS:
            atime = 27.2;
            break;
            
        case ATIME_101MS:
            atime = 101.0;
            break;
            
        case ATIME_174MS:
            atime = 174.0;
            break;
            
        case ATIME_696MS:
            atime = 696;
            break;
            
        default:
            break;
    }
    
    // Check the AGAIN
    switch (tsl_alsgain)
    {
        case AGAIN_1X:
            again = 1.0;
            break;
        case AGAIN_8X:
            again = 8.0;
            break;
            
        case AGAIN_16X:
            again = 16.0;
            break;
            
        case AGAIN_120X:
            again = 120.0;
            break;
            
        default:
            break;
    }
    
    // Read the Data
    // Reading the ALS CH0 Data Low Register
    C0dataLo = readRegister(tsl_i2cAddress, TSL2571_REG_ALS_C0DATAL | TSL2571_REG_ALS_CMD_SELECT | TSL2571_REG_ALS_CMD_TYPE_AUTO_INCR);
    // Reading the ALS CH0 Data High Register
    C0dataHi = readRegister(tsl_i2cAddress, TSL2571_REG_ALS_C0DATAH | TSL2571_REG_ALS_CMD_SELECT | TSL2571_REG_ALS_CMD_TYPE_AUTO_INCR);
    // Conversion of the result
    // 16-bit unsigned result for ALS CH0 Data of TSL2571
    C0Data = (int16_t)((C0dataHi << 8) | C0dataLo);
    
    // Reading the ALS CH1 Data Low Register
    C1dataLo = readRegister(tsl_i2cAddress, TSL2571_REG_ALS_C1DATAL | TSL2571_REG_ALS_CMD_SELECT | TSL2571_REG_ALS_CMD_TYPE_AUTO_INCR);
    // Reading the ALS CH1 Data High Register
    C1dataHi = readRegister(tsl_i2cAddress, TSL2571_REG_ALS_C1DATAH | TSL2571_REG_ALS_CMD_SELECT | TSL2571_REG_ALS_CMD_TYPE_AUTO_INCR);
    // Conversion of the result
    // 16-bit unsigned result for ALS CH1 Data of TSL2571
    C1Data = (int16_t)((C1dataHi << 8) | C1dataLo);
    
    /*
     CPL = (ATIME_ms × AGAINx) / (GA × 23)
     Lux1 = (C0DATA − 2 × C1DATA) / CPL
     Lux2 = (0.6 × C0DATA − C1DATA) / CPL
     Lux = MAX(Lux1, Lux2, 0)
     For Open Air with No Aperture, GA = 1
    */
    
    CPL = (atime * again) / 23.0;
    // Fluorescent and Incandescent Light
    Lux1 = (C0Data - (2.0 * C1Data)) / CPL;
    // Dimmed Incandescent Light
    Lux2 = ((0.6 * C0Data) - (1.00 * C1Data)) / CPL;
    
    // Calculate the Final Lux
    if((Lux1 > 0) && (Lux1 > Lux2))
    {
        Lux = Lux1;
    }
    else if((Lux2 > 0) && (Lux2 > Lux1))
    {
        Lux = Lux2;
    }
    
    tsl_alsData.L = Lux;
}
