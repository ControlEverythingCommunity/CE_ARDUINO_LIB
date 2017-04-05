/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TCS34725
        This code is designed to work with the TCS34725_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Color?sku=TCS34725_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "TCS34725.h"

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
        Instantiates a new TCS34725 class with appropriate properties
*/
/**************************************************************************/
void TCS34725::getAddr_TCS34725(uint8_t i2cAddress)
{
    tcs_i2cAddress = i2cAddress;
    tcs_conversionDelay = TCS34725_CONVERSIONDELAY;
    
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool TCS34725::begin()
{
    Wire.begin();
    
    uint8_t devid = readRegister(tcs_i2cAddress, TCS34725_REG_COLOR_ID);
    if (devid != TCS34725_COLOR_DEV_ID)
        return false;
    
    // Set up for Digital Color Sensor
    // setUpSensor();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Select Command Register
*/
/**************************************************************************/
void TCS34725::setCommand(tcsCommand_t command)
{
    tcs_command = command;
}

/**************************************************************************/
/*
        Gets the Select Command Register
*/
/**************************************************************************/
tcsCommand_t TCS34725::getCommand()
{
    return tcs_command;
}

/**************************************************************************/
/*
        Sets the ALS/Color Interrupt Mask
*/
/**************************************************************************/
void TCS34725::setColorInterrupt(tcsColorInterrupt_t colorinterrupt)
{
    tcs_colorinterrupt = colorinterrupt;
}

/**************************************************************************/
/*
        Gets the ALS/Color Interrupt Mask
*/
/**************************************************************************/
tcsColorInterrupt_t TCS34725::getColorInterrupt()
{
    return tcs_colorinterrupt;
}

/**************************************************************************/
/*
        Sets the Wait Enable
*/
/**************************************************************************/
void TCS34725::setWaitEnable(tcsWaitEnable_t waitenable)
{
    tcs_waitenable = waitenable;
}

/**************************************************************************/
/*
        Gets the Wait Enable
*/
/**************************************************************************/
tcsWaitEnable_t TCS34725::getWaitEnable()
{
    return tcs_waitenable;
}

/**************************************************************************/
/*
        Sets the ALS/Color Enable
*/
/**************************************************************************/
void TCS34725::setColorEnable(tcsColorEnable_t colorenable)
{
    tcs_colorenable = colorenable;
}

/**************************************************************************/
/*
        Gets the ALS/Color Enable
*/
/**************************************************************************/
tcsColorEnable_t TCS34725::getColorEnable()
{
    return tcs_colorenable;
}

/**************************************************************************/
/*
        Sets the Power ON
*/
/**************************************************************************/
void TCS34725::setPowerEnable(tcsPowerEnable_t powerenable)
{
    tcs_powerenable = powerenable;
}
                    
/**************************************************************************/
/*
        Gets the Power ON
*/
/**************************************************************************/
tcsPowerEnable_t TCS34725::getPowerEnable()
{
    return tcs_powerenable;
}

/**************************************************************************/
/*
        Sets to Control the Internal Integration Time of the ALS Channel 
        ADC’s in 2.4 ms Increments
*/
/**************************************************************************/
void TCS34725::setATime(tcsATime_t atime)
{
    tcs_atime = atime;
}

/**************************************************************************/
/*
        Gets to Control the Internal Integration Time of the ALS/Color Channel
        ADC’s in 2.4 ms Increments
*/
/**************************************************************************/
tcsATime_t TCS34725::getATime()
{
    return tcs_atime;
}

/**************************************************************************/
/*
        Sets to Control the Wait Time is Set in 2.4 ms Increments 
        Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
*/
/**************************************************************************/
void TCS34725::setWTime(tcsWTime_t wtime)
{
    tcs_wtime = wtime;
}

/**************************************************************************/
/*
        Gets to Control the Wait Time is Set in 2.4 ms Increments 
        Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
*/
/**************************************************************************/
tcsWTime_t TCS34725::getWTime()
{
    return tcs_wtime;
}

/**************************************************************************/
/*
        Sets the Interrupt Persistence
        Controls Rate of Interrupt to the Host Processor
*/
/**************************************************************************/
void TCS34725::setPersistence(tcsPersistence_t persistence)
{
    tcs_persistence = persistence;
}

/**************************************************************************/
/*
        Gets the Interrupt Persistence
        Controls Rate of Interrupt to the Host Processor
*/
/**************************************************************************/
tcsPersistence_t TCS34725::getPersistence()
{
    return tcs_persistence;
}

/**************************************************************************/
/*
        Sets the Wait Long
*/
/**************************************************************************/
void TCS34725::setWaitLong(tcsWaitLong_t waitlong)
{
    tcs_waitlong = waitlong;
}

/**************************************************************************/
/*
        Gets the Wait Long
*/
/**************************************************************************/
tcsWaitLong_t TCS34725::getWaitLong()
{
    return tcs_waitlong;
}

/**************************************************************************/
/*
        Sets the RGBC Gain Control
*/
/**************************************************************************/
void TCS34725::setColorGain(tcsColorGain_t colorgain)
{
    tcs_colorgain = colorgain;
}

/**************************************************************************/
/*
        Gets the RGBC Gain Control
*/
/**************************************************************************/
tcsColorGain_t TCS34725::getColorGain()
{
    return tcs_colorgain;
}

/**************************************************************************/
/*
        Sets up the Digital Color Sensor
*/
/**************************************************************************/
void TCS34725::setUpSensor(void)
{
    // Set Up the Configuration for the Digital Color Sensor Enable Register
    /*
     // Set the Color Interrupt Mask
     uint_8 enable = tcs_colorinterrupt;
    
     // Set the Wait Enable
     enable |= tcs_waitenable;
     
     // Set the Color Enable
     enable |= tcs_colorinable;
     
     // Set the Power ON
     enable |= tcs_powerenable;
    */
    
    uint8_t enable =    TCS34725_REG_COLOR_ENABLE_AIEN_NOT_ASSERT      |   // Not Asserted, Did not Permits ALS/Color Interrupts to be Generated
                        TCS34725_REG_COLOR_ENABLE_WEN_ENABLE           |   // Enables the Wait Timer
                        TCS34725_REG_COLOR_ENABLE_AEN_ENABLE           |   // Activates the ALS/Color ADC
                        TCS34725_REG_COLOR_ENABLE_PON_ENABLE;              // Activates the Internal Oscillator to Permit the Timers and ADC Channels to Operate
    
    // Write the configuration to the Digital Color Sensor Enable Register
    writeRegister(tcs_i2cAddress, TCS34725_REG_COLOR_ENABLE | TCS34725_REG_COLOR_CMD_SELECT | TCS34725_REG_COLOR_CMD_TYPE_AUTO_INCR, enable);
    
    // Wait for the configuration to complete
    delay(tcs_conversionDelay);
    
    // Set Up the Configuration for the Digital Color Sensor ATIME Register
    // Set the Internal Integration Time of the ALS Channel ADC’s in 2.4 ms Increments
    uint8_t atime = tcs_atime;
    
    // Write the configuration to the Digital Color Sensor ATIME Register
    writeRegister(tcs_i2cAddress, TCS34725_REG_COLOR_ATIME | TCS34725_REG_COLOR_CMD_SELECT | TCS34725_REG_COLOR_CMD_TYPE_AUTO_INCR, atime);
    
    // Wait for the configuration to complete
    delay(tcs_conversionDelay);
    
    // Set Up the Configuration for the Digital Color Sensor WTIME Register
    // Set the Wait Time is Set in 2.4 ms Increments Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
    uint8_t wtime = tcs_wtime;
    
    // Write the configuration to the Digital Color Sensor WTIME Register
    writeRegister(tcs_i2cAddress, TCS34725_REG_COLOR_WTIME | TCS34725_REG_COLOR_CMD_SELECT | TCS34725_REG_COLOR_CMD_TYPE_AUTO_INCR, wtime);
    
    // Wait for the configuration to complete
    delay(tcs_conversionDelay);
    
    // Set Up the Configuration for the Digital Color Sensor Persistence Register
    /*
     // Set the Interrupt Persistence
     uint8_t persistence = tcs_persistence;
     */
    uint8_t persistence = TCS34725_REG_COLOR_PERS_APERS_EVERY;  // Every RGBC Cycle Generates an Interrupt
    
    // Write the configuration to the Digital Color Sensor Persistence Register
    writeRegister(tcs_i2cAddress, TCS34725_REG_COLOR_PERS | TCS34725_REG_COLOR_CMD_SELECT | TCS34725_REG_COLOR_CMD_TYPE_AUTO_INCR, persistence);
    
    // Wait for the configuration to complete
    delay(tcs_conversionDelay);
    
    // Set Up the Configuration for the Digital Color Sensor Configuration Register
    /*
     // Set the Wait Long
     uint8_t config = tcs_waitlong;
    */
    
    uint8_t config = TCS34725_REG_COLOR_CONFIG_WLONG_NOT_ASSERT;  // Not Asserted, the Wait Cycles are Not Increased by a Factor 12X from that Programmed in the WTIME Register
    
    // Write the configuration to the Digital Color Sensor Configuration Register
    writeRegister(tcs_i2cAddress, TCS34725_REG_COLOR_CONFIG | TCS34725_REG_COLOR_CMD_SELECT | TCS34725_REG_COLOR_CMD_TYPE_AUTO_INCR, config);
    
    // Wait for the configuration to complete
    delay(tcs_conversionDelay);
    
    // Set Up the Configuration for the Digital Color Sensor Control Register
    // ALS Gain
    uint8_t control = tcs_colorgain;
    
    // Write the configuration to the Digital Color Sensor Control Register
    writeRegister(tcs_i2cAddress, TCS34725_REG_COLOR_CONTROL | TCS34725_REG_COLOR_CMD_SELECT | TCS34725_REG_COLOR_CMD_TYPE_AUTO_INCR, control);
    
    // Wait for the configuration to complete
    delay(tcs_conversionDelay);
}

/**************************************************************************/
/*
        Reads the Digital Color Sensor
*/
/**************************************************************************/
void TCS34725::Measure_Color()
{
    uint8_t CdataLo, CdataHi, RdataLo, RdataHi, GdataLo, GdataHi, BdataLo, BdataHi;
    uint16_t CData, RData, GData, BData;
    
    // Read the Data
    // Reading the Clear Data Low Register
    CdataLo = readRegister(tcs_i2cAddress, TCS34725_REG_COLOR_CDATAL | TCS34725_REG_COLOR_CMD_SELECT | TCS34725_REG_COLOR_CMD_TYPE_AUTO_INCR);
    // Reading the Clear Data High Register
    CdataHi = readRegister(tcs_i2cAddress, TCS34725_REG_COLOR_CDATAH | TCS34725_REG_COLOR_CMD_SELECT | TCS34725_REG_COLOR_CMD_TYPE_AUTO_INCR);
    // Conversion of the result
    // 16-bit unsigned result for Clear Data of TCS34725
    CData = (uint16_t)((CdataHi << 8) | CdataLo);
    
    // Reading the Red Data Low Register
    RdataLo = readRegister(tcs_i2cAddress, TCS34725_REG_COLOR_RDATAL | TCS34725_REG_COLOR_CMD_SELECT | TCS34725_REG_COLOR_CMD_TYPE_AUTO_INCR);
    // Reading the Red Data High Register
    RdataHi = readRegister(tcs_i2cAddress, TCS34725_REG_COLOR_RDATAH | TCS34725_REG_COLOR_CMD_SELECT | TCS34725_REG_COLOR_CMD_TYPE_AUTO_INCR);
    // Conversion of the result
    // 16-bit unsigned result for Red Data of TCS34725
    RData = (uint16_t)((RdataHi << 8) | RdataLo);
    
    // Reading the Green Data Low Register
    GdataLo = readRegister(tcs_i2cAddress, TCS34725_REG_COLOR_GDATAL | TCS34725_REG_COLOR_CMD_SELECT | TCS34725_REG_COLOR_CMD_TYPE_AUTO_INCR);
    // Reading the Green Data High Register
    GdataHi = readRegister(tcs_i2cAddress, TCS34725_REG_COLOR_GDATAH | TCS34725_REG_COLOR_CMD_SELECT | TCS34725_REG_COLOR_CMD_TYPE_AUTO_INCR);
    // Conversion of the result
    // 16-bit unsigned result for Green Data of TCS34725
    GData = (uint16_t)((GdataHi << 8) | GdataLo);
    
    // Reading the Blue Data Low Register
    BdataLo = readRegister(tcs_i2cAddress, TCS34725_REG_COLOR_BDATAL | TCS34725_REG_COLOR_CMD_SELECT | TCS34725_REG_COLOR_CMD_TYPE_AUTO_INCR);
    // Reading the Blue Data High Register
    BdataHi = readRegister(tcs_i2cAddress, TCS34725_REG_COLOR_BDATAH | TCS34725_REG_COLOR_CMD_SELECT | TCS34725_REG_COLOR_CMD_TYPE_AUTO_INCR);
    // Conversion of the result
    // 16-bit unsigned result for Blue Data of TCS34725
    BData = (uint16_t)((BdataHi << 8) | BdataLo);
    
    tcs_colorData.C = CData;
    tcs_colorData.R = RData;
    tcs_colorData.G = GData;
    tcs_colorData.B = BData;
}
