/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TMD2671
        This code is designed to work with the TMD2671_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Proximity?sku=TMD2671_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "TMD2671.h"

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
        Instantiates a new TMD2671 class with appropriate properties
*/
/**************************************************************************/
void TMD2671::getAddr_TMD2671(uint8_t i2cAddress)
{
    tmd_i2cAddress = i2cAddress;
    tmd_conversionDelay = TMD2671_CONVERSIONDELAY;
    
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool TMD2671::begin()
{
    Wire.begin();
    
    uint8_t devid = readRegister(tmd_i2cAddress, TMD2671_REG_PROX_ID);
    if (devid != TMD2671_PROX_DEV_ID)
        return false;
    
    // Set up for Infrared Digital Proximity Detector
    // setUpProximity();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Select Command Register
*/
/**************************************************************************/
void TMD2671::setCommand(tmdCommand_t command)
{
    tmd_command = command;
}

/**************************************************************************/
/*
        Gets the Select Command Register
*/
/**************************************************************************/
tmdCommand_t TMD2671::getCommand()
{
    return tmd_command;
}

/**************************************************************************/
/*
        Sets the Proximity Interrupt Mask
*/
/**************************************************************************/
void TMD2671::setProximityInterrupt(tmdProximityInterrupt_t proximityinterrupt)
{
    tmd_proximityinterrupt = proximityinterrupt;
}

/**************************************************************************/
/*
        Gets the Proximity Interrupt Mask
*/
/**************************************************************************/
tmdProximityInterrupt_t TMD2671::getProximityInterrupt()
{
    return tmd_proximityinterrupt;
}

/**************************************************************************/
/*
        Sets the Wait Enable
*/
/**************************************************************************/
void TMD2671::setWaitEnable(tmdWaitEnable_t waitenable)
{
    tmd_waitenable = waitenable;
}

/**************************************************************************/
/*
        Gets the Wait Enable
*/
/**************************************************************************/
tmdWaitEnable_t TMD2671::getWaitEnable()
{
    return tmd_waitenable;
}

/**************************************************************************/
/*
        Sets the Proximity Enable
*/
/**************************************************************************/
void TMD2671::setProximityEnable(tmdProximityEnable_t proximityenable)
{
    tmd_proximityenable = proximityenable;
}

/**************************************************************************/
/*
        Gets the Proximity Enable
*/
/**************************************************************************/
tmdProximityEnable_t TMD2671::getProximityEnable()
{
    return tmd_proximityenable;
}

/**************************************************************************/
/*
        Sets the Power ON
*/
/**************************************************************************/
void TMD2671::setPowerEnable(tmdPowerEnable_t powerenable)
{
    tmd_powerenable = powerenable;
}
                    
/**************************************************************************/
/*
        Gets the Power ON
*/
/**************************************************************************/
tmdPowerEnable_t TMD2671::getPowerEnable()
{
    return tmd_powerenable;
}

/**************************************************************************/
/*
        Sets to Control the Internal Integration Time of the ALS Channel
        ADC’s in 2.72 ms Increments
*/
/**************************************************************************/
void TMD2671::setATime(tmdATime_t atime)
{
    tmd_atime = atime;
}

/**************************************************************************/
/*
        Gets to Control the Internal Integration Time of the ALS Channel
        ADC’s in 2.72 ms Increments
*/
/**************************************************************************/
tmdATime_t TMD2671::getATime()
{
    return tmd_atime;
}

/**************************************************************************/
/*
        Sets to Control the Internal Integration Time of the Proximity
        ADC’s in 2.72 ms Increments
*/
/**************************************************************************/
void TMD2671::setPTime(tmdPTime_t ptime)
{
    tmd_ptime = ptime;
}

/**************************************************************************/
/*
        Gets to Control the Internal Integration Time of the Proximity 
        ADC’s in 2.72 ms Increments
*/
/**************************************************************************/
tmdPTime_t TMD2671::getPTime()
{
    return tmd_ptime;
}

/**************************************************************************/
/*
        Sets to Control the Wait Time is Set in 2.72 ms Increments 
        Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
*/
/**************************************************************************/
void TMD2671::setWTime(tmdWTime_t wtime)
{
    tmd_wtime = wtime;
}

/**************************************************************************/
/*
        Gets to Control the Wait Time is Set in 2.72 ms Increments 
        Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
*/
/**************************************************************************/
tmdWTime_t TMD2671::getWTime()
{
    return tmd_wtime;
}

/**************************************************************************/
/*
        Sets the Wait Long
*/
/**************************************************************************/
void TMD2671::setWaitLong(tmdWaitLong_t waitlong)
{
    tmd_waitlong = waitlong;
}

/**************************************************************************/
/*
        Gets the Wait Long
*/
/**************************************************************************/
tmdWaitLong_t TMD2671::getWaitLong()
{
    return tmd_waitlong;
}

/**************************************************************************/
/*
        Sets the Proximity LED Drive Strength
*/
/**************************************************************************/
void TMD2671::setProximityDrive(tmdProximityDrive_t proximitydrive)
{
    tmd_proximitydrive = proximitydrive;
}

/**************************************************************************/
/*
        Gets the Proximity LED Drive Strength
*/
/**************************************************************************/
tmdProximityDrive_t TMD2671::getProximityDrive()
{
    return tmd_proximitydrive;
}

/**************************************************************************/
/*
        Sets the Proximity Diode Selector
*/
/**************************************************************************/
void TMD2671::setProximityDiode(tmdProximityDiode_t proximitydiode)
{
    tmd_proximitydiode = proximitydiode;
}

/**************************************************************************/
/*
        Gets the Proximity Diode Selector
*/
/**************************************************************************/
tmdProximityDiode_t TMD2671::getProximityDiode()
{
    return tmd_proximitydiode;
}

/**************************************************************************/
/*
        Sets up the Infrared Digital Proximity Detector
*/
/**************************************************************************/
void TMD2671::setUpProximity(void)
{
    // Set Up the Configuration for the Infrared Digital Proximity Detector Enable Register
    /*
    // Set the Proximity Interrupt Mask
     uint8_t enable = tmd_proximityinterrupt;
    
     // Set the Wait Enable
     enable |= tmd_waitenable;
     
     // Set the Proximity Enable
     enable |= tmd_proximityenable;
     
     // Set the Power ON
     enable |= tmd_powerenable;
    */
    
    uint8_t enable =    TMD2671_REG_PROX_ENABLE_PIEN_NOT_ASSERT      |   // Not Asserted, Did not Permits Proximity Interrupts to be Generated
                        TMD2671_REG_PROX_ENABLE_WEN_ENABLE           |   // Enables the Wait Timer
                        TMD2671_REG_PROX_ENABLE_PEN_ENABLE           |   // Activates the Proximity
                        TMD2671_REG_PROX_ENABLE_PON_ENABLE;              // Activates the Internal Oscillator to Permit the Timers and ADC Channels to Operate
    
    // Write the configuration to the Infrared Digital Proximity Detector Enable Register
    writeRegister(tmd_i2cAddress, TMD2671_REG_PROX_ENABLE | TMD2671_REG_PROX_CMD_SELECT | TMD2671_REG_PROX_CMD_TYPE_AUTO_INCR, enable);
    
    // Wait for the configuration to complete
    delay(tmd_conversionDelay);
    
    // Set Up the Configuration for the Infrared Digital Proximity Detector ATIME Register
    // Set the Internal Integration Time of the ALS Channel ADC’s in 2.72 ms Increments
    uint8_t atime = tmd_atime;
    
    // Write the configuration to the Infrared Digital Proximity Detector ATIME Register
    writeRegister(tmd_i2cAddress, TMD2671_REG_PROX_ATIME | TMD2671_REG_PROX_CMD_SELECT | TMD2671_REG_PROX_CMD_TYPE_AUTO_INCR, atime);
    
    // Wait for the configuration to complete
    delay(tmd_conversionDelay);
    
    // Set Up the Configuration for the Infrared Digital Proximity Detector PTIME Register
    // Set the Internal Integration Time of the Proximity ADC’s in 2.72 ms Increments
    uint8_t ptime = tmd_ptime;
    
    // Write the configuration to the Infrared Digital Proximity Detector PTIME Register
    writeRegister(tmd_i2cAddress, TMD2671_REG_PROX_PTIME | TMD2671_REG_PROX_CMD_SELECT | TMD2671_REG_PROX_CMD_TYPE_AUTO_INCR, ptime);
    
    // Wait for the configuration to complete
    delay(tmd_conversionDelay);
    
    // Set Up the Configuration for the Infrared Digital Proximity Detector WTIME Register
    // Set the Wait Time is Set in 2.72 ms Increments Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
    uint8_t wtime = tmd_wtime;
    
    // Write the configuration to the Infrared Digital Proximity Detector WTIME Register
    writeRegister(tmd_i2cAddress, TMD2671_REG_PROX_WTIME | TMD2671_REG_PROX_CMD_SELECT | TMD2671_REG_PROX_CMD_TYPE_AUTO_INCR, wtime);
    
    // Wait for the configuration to complete
    delay(tmd_conversionDelay);
    
    // Set Up the Configuration for the Infrared Digital Proximity Detector Configuration Register
    /*
     // Set the Wait Long
     uint8_t config = tmd_waitlong;
    */
    
    uint8_t config = TMD2671_REG_PROX_CONFIG_WLONG_NOT_ASSERT;   // Not Asserted, the Wait Cycles are Not Increased by a Factor 12X from that Programmed in the WTIME Register
    
    // Write the configuration to the Infrared Digital Proximity Detector Configuration Register
    writeRegister(tmd_i2cAddress, TMD2671_REG_PROX_CONFIG | TMD2671_REG_PROX_CMD_SELECT | TMD2671_REG_PROX_CMD_TYPE_AUTO_INCR, config);
    
    // Wait for the configuration to complete
    delay(tmd_conversionDelay);
    
    // Set Up the Configuration for the Infrared Digital Proximity Detector Control Register
    // Proximity LED Drive Strength
    uint8_t control = tmd_proximitydrive;
    
    // Proximity Diode Selector
    control |= tmd_proximitydiode;
    
    // Write the configuration to the Infrared Digital Proximity Detector Control Register
    writeRegister(tmd_i2cAddress, TMD2671_REG_PROX_CONTROL | TMD2671_REG_PROX_CMD_SELECT | TMD2671_REG_PROX_CMD_TYPE_AUTO_INCR, control);
    
    // Wait for the configuration to complete
    delay(tmd_conversionDelay);
}
/**************************************************************************/
/*
        Reads up the Infrared Digital Proximity Detector
*/
/**************************************************************************/
void TMD2671::Measure_Sensor(void)
{
    // Read all the Sensors
    // Proximity Data
    Measure_Proximity();
}

/**************************************************************************/
/*
        Reads the Proximity Sensor
*/
/**************************************************************************/
void TMD2671::Measure_Proximity()
{
    uint8_t PdataLo, PdataHi;
    
    // Read the Data
    // Reading the Proximity Data Low Register
    PdataLo = readRegister(tmd_i2cAddress, TMD2671_REG_PROX_PDATAL | TMD2671_REG_PROX_CMD_SELECT | TMD2671_REG_PROX_CMD_TYPE_AUTO_INCR);
    // Reading the Proximity Data High Register
    PdataHi = readRegister(tmd_i2cAddress, TMD2671_REG_PROX_PDATAH | TMD2671_REG_PROX_CMD_SELECT | TMD2671_REG_PROX_CMD_TYPE_AUTO_INCR);
    // Conversion of the result
    // 16-bit unsigned result for Proximity Data of TMD2671
    tmd_proxData.P = (float)((PdataHi << 8) | PdataLo);
}
