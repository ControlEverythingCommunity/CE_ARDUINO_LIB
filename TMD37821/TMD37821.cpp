/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TMD37821
        This code is designed to work with the TMD37821_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Color?sku=TMD37821_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "TMD37821.h"

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
        Instantiates a new TMD37821 class with appropriate properties
*/
/**************************************************************************/
void TMD37821::getAddr_TMD37821(uint8_t i2cAddress)
{
    tmd_i2cAddress = i2cAddress;
    tmd_conversionDelay = TMD37821_CONVERSIONDELAY;
    
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool TMD37821::begin()
{
    Wire.begin();
    
    uint8_t devid = readRegister(tmd_i2cAddress, TMD37821_REG_COLORPROX_ID);
    if (devid != TMD37821_COLORPROX_DEV_ID)
        return false;
    
    // Set up for Digital Color and Proximity Sensor
    setUpColorProximity();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Select Command Register
*/
/**************************************************************************/
void TMD37821::setCommand(tmdCommand_t command)
{
    tmd_command = command;
}

/**************************************************************************/
/*
        Gets the Select Command Register
*/
/**************************************************************************/
tmdCommand_t TMD37821::getCommand()
{
    return tmd_command;
}

/**************************************************************************/
/*
        Sets the Proximity Interrupt Mask
*/
/**************************************************************************/
void TMD37821::setProximityInterrupt(tmdProximityInterrupt_t proximityinterrupt)
{
    tmd_proximityinterrupt = proximityinterrupt;
}

/**************************************************************************/
/*
        Gets the Proximity Interrupt Mask
*/
/**************************************************************************/
tmdProximityInterrupt_t TMD37821::getProximityInterrupt()
{
    return tmd_proximityinterrupt;
}

/**************************************************************************/
/*
        Sets the ALS/Color Interrupt Mask
*/
/**************************************************************************/
void TMD37821::setColorInterrupt(tmdColorInterrupt_t colorinterrupt)
{
    tmd_colorinterrupt = colorinterrupt;
}

/**************************************************************************/
/*
        Gets the ALS/Color Interrupt Mask
*/
/**************************************************************************/
tmdColorInterrupt_t TMD37821::getColorInterrupt()
{
    return tmd_colorinterrupt;
}

/**************************************************************************/
/*
        Sets the Wait Enable
*/
/**************************************************************************/
void TMD37821::setWaitEnable(tmdWaitEnable_t waitenable)
{
    tmd_waitenable = waitenable;
}

/**************************************************************************/
/*
        Gets the Wait Enable
*/
/**************************************************************************/
tmdWaitEnable_t TMD37821::getWaitEnable()
{
    return tmd_waitenable;
}

/**************************************************************************/
/*
        Sets the Proximity Enable
*/
/**************************************************************************/
void TMD37821::setProximityEnable(tmdProximityEnable_t proximityenable)
{
    tmd_proximityenable = proximityenable;
}

/**************************************************************************/
/*
        Gets the Proximity Enable
*/
/**************************************************************************/
tmdProximityEnable_t TMD37821::getProximityEnable()
{
    return tmd_proximityenable;
}

/**************************************************************************/
/*
        Sets the ALS/Color Enable
*/
/**************************************************************************/
void TMD37821::setColorEnable(tmdColorEnable_t colorenable)
{
    tmd_colorenable = colorenable;
}

/**************************************************************************/
/*
        Gets the ALS/Color Enable
*/
/**************************************************************************/
tmdColorEnable_t TMD37821::getColorEnable()
{
    return tmd_colorenable;
}

/**************************************************************************/
/*
        Sets the Power ON
*/
/**************************************************************************/
void TMD37821::setPowerEnable(tmdPowerEnable_t powerenable)
{
    tmd_powerenable = powerenable;
}
                    
/**************************************************************************/
/*
        Gets the Power ON
*/
/**************************************************************************/
tmdPowerEnable_t TMD37821::getPowerEnable()
{
    return tmd_powerenable;
}

/**************************************************************************/
/*
        Sets to Control the Internal Integration Time of the ALS Channel 
        ADC’s in 2.38 ms Increments
*/
/**************************************************************************/
void TMD37821::setATime(tmdATime_t atime)
{
    tmd_atime = atime;
}

/**************************************************************************/
/*
        Gets to Control the Internal Integration Time of the ALS/Color Channel
        ADC’s in 2.38 ms Increments
*/
/**************************************************************************/
tmdATime_t TMD37821::getATime()
{
    return tmd_atime;
}

/**************************************************************************/
/*
        Sets to Control the Wait Time is Set in 2.38 ms Increments 
        Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
*/
/**************************************************************************/
void TMD37821::setWTime(tmdWTime_t wtime)
{
    tmd_wtime = wtime;
}

/**************************************************************************/
/*
        Gets to Control the Wait Time is Set in 2.38 ms Increments 
        Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
*/
/**************************************************************************/
tmdWTime_t TMD37821::getWTime()
{
    return tmd_wtime;
}

/**************************************************************************/
/*
        Sets the Wait Long
*/
/**************************************************************************/
void TMD37821::setWaitLong(tmdWaitLong_t waitlong)
{
    tmd_waitlong = waitlong;
}

/**************************************************************************/
/*
        Gets the Wait Long
*/
/**************************************************************************/
tmdWaitLong_t TMD37821::getWaitLong()
{
    return tmd_waitlong;
}

/**************************************************************************/
/*
        Sets the Proximity LED Drive Strength
*/
/**************************************************************************/
void TMD37821::setProximityDrive(tmdProximityDrive_t proximitydrive)
{
    tmd_proximitydrive = proximitydrive;
}

/**************************************************************************/
/*
        Gets the Proximity LED Drive Strength
*/
/**************************************************************************/
tmdProximityDrive_t TMD37821::getProximityDrive()
{
    return tmd_proximitydrive;
}

/**************************************************************************/
/*
        Sets the Proximity Output Selector
*/
/**************************************************************************/
void TMD37821::setProximityOutput(tmdProximityOutput_t proximityoutput)
{
    tmd_proximityoutput = proximityoutput;
}

/**************************************************************************/
/*
        Gets the Proximity Output Selector
*/
/**************************************************************************/
tmdProximityOutput_t TMD37821::getProximityOutput()
{
    return tmd_proximityoutput;
}

/**************************************************************************/
/*
        Sets the RGBC Gain Control
*/
/**************************************************************************/
void TMD37821::setColorGain(tmdColorGain_t colorgain)
{
    tmd_colorgain = colorgain;
}

/**************************************************************************/
/*
        Gets the RGBC Gain Control
*/
/**************************************************************************/
tmdColorGain_t TMD37821::getColorGain()
{
    return tmd_colorgain;
}

/**************************************************************************/
/*
        Sets up the Digital Color and Proximity Sensor
*/
/**************************************************************************/
void TMD37821::setUpColorProximity(void)
{
    // Set Up the Configuration for the Digital Color and Proximity Sensor Enable Register
    /*
     // Set the Proximity Interrupt Mask
     uint8_t enable = tmd_proximityinterrupt;
    
     // Set the Color Interrupt Mask
     enable |= tmd_colorinterrupt;
    
     // Set the Wait Enable
     enable |= tmd_waitenable;
     
     // Set the Proximity Enable
     enable |= tmd_proximityenable;
     
     // Set the Color Enable
     enable |= tmd_colorinable;
     
     // Set the Power ON
     enable |= tmd_powerenable;
    */
    
    uint8_t enable =    TMD37821_REG_COLORPROX_ENABLE_PIEN_NOT_ASSERT      |   // Not Asserted, Did not Permits Proximity Interrupts to be Generated
                        TMD37821_REG_COLORPROX_ENABLE_AIEN_NOT_ASSERT      |   // Not Asserted, Did not Permits ALS/Color Interrupts to be Generated
                        TMD37821_REG_COLORPROX_ENABLE_WEN_ENABLE           |   // Enables the Wait Timer
                        TMD37821_REG_COLORPROX_ENABLE_PEN_ENABLE           |   // Activates the Proximity
                        TMD37821_REG_COLORPROX_ENABLE_AEN_ENABLE           |   // Activates the ALS/Color ADC
                        TMD37821_REG_COLORPROX_ENABLE_PON_ENABLE;              // Activates the Internal Oscillator to Permit the Timers and ADC Channels to Operate
    
    // Write the configuration to the Digital Color and Proximity Sensor Enable Register
    writeRegister(tmd_i2cAddress, TMD37821_REG_COLORPROX_ENABLE | TMD37821_REG_COLORPROX_CMD_SELECT | TMD37821_REG_COLORPROX_CMD_TYPE_AUTO_INCR, enable);
    
    // Wait for the configuration to complete
    delay(tmd_conversionDelay);
    
    // Set Up the Configuration for the Digital Color and Proximity Sensor ATIME Register
    // Set the Internal Integration Time of the ALS Channel ADC’s in 2.38 ms Increments
    uint8_t atime = tmd_atime;
    
    // Write the configuration to the Digital Color and Proximity Sensor ATIME Register
    writeRegister(tmd_i2cAddress, TMD37821_REG_COLORPROX_ATIME | TMD37821_REG_COLORPROX_CMD_SELECT | TMD37821_REG_COLORPROX_CMD_TYPE_AUTO_INCR, atime);
    
    // Wait for the configuration to complete
    delay(tmd_conversionDelay);
    
    // Set Up the Configuration for the Digital Color and Proximity Sensor WTIME Register
    // Set the Wait Time is Set in 2.38 ms Increments Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
    uint8_t wtime = tmd_wtime;
    
    // Write the configuration to the Digital Color and Proximity Sensor WTIME Register
    writeRegister(tmd_i2cAddress, TMD37821_REG_COLORPROX_WTIME | TMD37821_REG_COLORPROX_CMD_SELECT | TMD37821_REG_COLORPROX_CMD_TYPE_AUTO_INCR, wtime);
    
    // Wait for the configuration to complete
    delay(tmd_conversionDelay);
    
    // Set Up the Configuration for the Digital Color and Proximity Sensor Configuration Register
    /*
     // Set the Wait Long
     uint8_t config = tmd_waitlong;
    */
    
    uint8_t config = TMD37821_REG_COLORPROX_CONFIG_WLONG_NOT_ASSERT;  // Not Asserted, the Wait Cycles are Not Increased by a Factor 12X from that Programmed in the WTIME Register
    
    // Write the configuration to the Digital Color and Proximity Sensor Configuration Register
    writeRegister(tmd_i2cAddress, TMD37821_REG_COLORPROX_CONFIG | TMD37821_REG_COLORPROX_CMD_SELECT | TMD37821_REG_COLORPROX_CMD_TYPE_AUTO_INCR, config);
    
    // Wait for the configuration to complete
    delay(tmd_conversionDelay);
    
    // Set Up the Configuration for the Digital Color and Proximity Sensor Control Register
    // Proximity LED Drive Strength
    uint8_t control = tmd_proximitydrive;
    
    // Proximity Output Selector
    control |= tmd_proximityoutput;
    
    // ALS Gain
    control |= tmd_colorgain;
    
    // Write the configuration to the Digital Color and Proximity Sensor Control Register
    writeRegister(tmd_i2cAddress, TMD37821_REG_COLORPROX_CONTROL | TMD37821_REG_COLORPROX_CMD_SELECT | TMD37821_REG_COLORPROX_CMD_TYPE_AUTO_INCR, control);
    
    // Wait for the configuration to complete
    delay(tmd_conversionDelay);
}
/**************************************************************************/
/*
        Reads up the Digital Color and Proximity Sensor
*/
/**************************************************************************/
void TMD37821::Measure_Sensor(void)
{
    // Read all the Sensors
    // ALS and Proximity Data
    Measure_Color();
    Measure_Proximity();
}

/**************************************************************************/
/*
        Reads the Digital Color Sensor
*/
/**************************************************************************/
void TMD37821::Measure_Color()
{
    uint8_t CdataLo, CdataHi, RdataLo, RdataHi, GdataLo, GdataHi, BdataLo, BdataHi;
    uint16_t CData, RData, GData, BData;
    
    // Read the Data
    // Reading the Clear Data Low Register
    CdataLo = readRegister(tmd_i2cAddress, TMD37821_REG_COLORPROX_CDATAL | TMD37821_REG_COLORPROX_CMD_SELECT | TMD37821_REG_COLORPROX_CMD_TYPE_AUTO_INCR);
    // Reading the Clear Data High Register
    CdataHi = readRegister(tmd_i2cAddress, TMD37821_REG_COLORPROX_CDATAH | TMD37821_REG_COLORPROX_CMD_SELECT | TMD37821_REG_COLORPROX_CMD_TYPE_AUTO_INCR);
    // Conversion of the result
    // 16-bit unsigned result for Clear Data of TMD37821
    CData = (uint16_t)((CdataHi << 8) | CdataLo);
    
    // Reading the Red Data Low Register
    RdataLo = readRegister(tmd_i2cAddress, TMD37821_REG_COLORPROX_RDATAL | TMD37821_REG_COLORPROX_CMD_SELECT | TMD37821_REG_COLORPROX_CMD_TYPE_AUTO_INCR);
    // Reading the Red Data High Register
    RdataHi = readRegister(tmd_i2cAddress, TMD37821_REG_COLORPROX_RDATAH | TMD37821_REG_COLORPROX_CMD_SELECT | TMD37821_REG_COLORPROX_CMD_TYPE_AUTO_INCR);
    // Conversion of the result
    // 16-bit unsigned result for Red Data of TMD37821
    RData = (uint16_t)((RdataHi << 8) | RdataLo);
    
    // Reading the Green Data Low Register
    GdataLo = readRegister(tmd_i2cAddress, TMD37821_REG_COLORPROX_GDATAL | TMD37821_REG_COLORPROX_CMD_SELECT | TMD37821_REG_COLORPROX_CMD_TYPE_AUTO_INCR);
    // Reading the Green Data High Register
    GdataHi = readRegister(tmd_i2cAddress, TMD37821_REG_COLORPROX_GDATAH | TMD37821_REG_COLORPROX_CMD_SELECT | TMD37821_REG_COLORPROX_CMD_TYPE_AUTO_INCR);
    // Conversion of the result
    // 16-bit unsigned result for Green Data of TMD37821
    GData = (uint16_t)((GdataHi << 8) | GdataLo);
    
    // Reading the Blue Data Low Register
    BdataLo = readRegister(tmd_i2cAddress, TMD37821_REG_COLORPROX_BDATAL | TMD37821_REG_COLORPROX_CMD_SELECT | TMD37821_REG_COLORPROX_CMD_TYPE_AUTO_INCR);
    // Reading the Blue Data High Register
    BdataHi = readRegister(tmd_i2cAddress, TMD37821_REG_COLORPROX_BDATAH | TMD37821_REG_COLORPROX_CMD_SELECT | TMD37821_REG_COLORPROX_CMD_TYPE_AUTO_INCR);
    // Conversion of the result
    // 16-bit unsigned result for Blue Data of TMD37821
    BData = (uint16_t)((BdataHi << 8) | BdataLo);
    
    tmd_colorproxData.C = CData;
    tmd_colorproxData.R = RData;
    tmd_colorproxData.G = GData;
    tmd_colorproxData.B = BData;
}

/**************************************************************************/
/*
        Reads the Proximity Sensor
*/
/**************************************************************************/
void TMD37821::Measure_Proximity()
{
    uint8_t PdataLo, PdataHi;
    
    // Read the Data
    // Reading the Proximity Data Low Register
    PdataLo = readRegister(tmd_i2cAddress, TMD37821_REG_COLORPROX_PDATAL | TMD37821_REG_COLORPROX_CMD_SELECT | TMD37821_REG_COLORPROX_CMD_TYPE_AUTO_INCR);
    // Reading the Proximity Data High Register
    PdataHi = readRegister(tmd_i2cAddress, TMD37821_REG_COLORPROX_PDATAH | TMD37821_REG_COLORPROX_CMD_SELECT | TMD37821_REG_COLORPROX_CMD_TYPE_AUTO_INCR);
    // Conversion of the result
    // 16-bit unsigned result for Proximity Data of TMD37821
    tmd_colorproxData.P = (uint16_t)((PdataHi << 8) | PdataLo);
}
