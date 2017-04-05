/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TSL26711
        This code is designed to work with the TSL26711_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "TSL26711.h"

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
        Instantiates a new TSL26711 class with appropriate properties
*/
/**************************************************************************/
void TSL26711::getAddr_TSL26711(uint8_t i2cAddress)
{
    tsl_i2cAddress = i2cAddress;
    tsl_conversionDelay = TSL26711_CONVERSIONDELAY;
    
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool TSL26711::begin()
{
    Wire.begin();
    
    uint8_t devid = readRegister(tsl_i2cAddress, TSL26711_REG_PROX_ID);
    if (devid != TSL26711_PROX_DEV_ID)
        return false;
    
    // Set up for Digital Proximity Detector
    setUpProximity();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Select Command Register
*/
/**************************************************************************/
void TSL26711::setCommand(tslCommand_t command)
{
    tsl_command = command;
}

/**************************************************************************/
/*
        Gets the Select Command Register
*/
/**************************************************************************/
tslCommand_t TSL26711::getCommand()
{
    return tsl_command;
}

/**************************************************************************/
/*
        Sets the Proximity Interrupt Mask
*/
/**************************************************************************/
void TSL26711::setProximityInterrupt(tslProximityInterrupt_t proximityinterrupt)
{
    tsl_proximityinterrupt = proximityinterrupt;
}

/**************************************************************************/
/*
        Gets the Proximity Interrupt Mask
*/
/**************************************************************************/
tslProximityInterrupt_t TSL26711::getProximityInterrupt()
{
    return tsl_proximityinterrupt;
}

/**************************************************************************/
/*
        Sets the Wait Enable
*/
/**************************************************************************/
void TSL26711::setWaitEnable(tslWaitEnable_t waitenable)
{
    tsl_waitenable = waitenable;
}

/**************************************************************************/
/*
        Gets the Wait Enable
*/
/**************************************************************************/
tslWaitEnable_t TSL26711::getWaitEnable()
{
    return tsl_waitenable;
}

/**************************************************************************/
/*
        Sets the Proximity Enable
*/
/**************************************************************************/
void TSL26711::setProximityEnable(tslProximityEnable_t proximityenable)
{
    tsl_proximityenable = proximityenable;
}

/**************************************************************************/
/*
        Gets the Proximity Enable
*/
/**************************************************************************/
tslProximityEnable_t TSL26711::getProximityEnable()
{
    return tsl_proximityenable;
}

/**************************************************************************/
/*
        Sets the Power ON
*/
/**************************************************************************/
void TSL26711::setPowerEnable(tslPowerEnable_t powerenable)
{
    tsl_powerenable = powerenable;
}
                    
/**************************************************************************/
/*
        Gets the Power ON
*/
/**************************************************************************/
tslPowerEnable_t TSL26711::getPowerEnable()
{
    return tsl_powerenable;
}

/**************************************************************************/
/*
        Sets to Control the Internal Integration Time of the Proximity
        ADC’s in 2.72ms Increments
*/
/**************************************************************************/
void TSL26711::setPTime(tslPTime_t ptime)
{
    tsl_ptime = ptime;
}

/**************************************************************************/
/*
        Gets to Control the Internal Integration Time of the Proximity 
        ADC’s in 2.72ms Increments
*/
/**************************************************************************/
tslPTime_t TSL26711::getPTime()
{
    return tsl_ptime;
}

/**************************************************************************/
/*
        Sets to Control the Wait Time is Set in 2.72 ms Increments 
        Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
*/
/**************************************************************************/
void TSL26711::setWTime(tslWTime_t wtime)
{
    tsl_wtime = wtime;
}

/**************************************************************************/
/*
        Gets to Control the Wait Time is Set in 2.72 ms Increments 
        Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
*/
/**************************************************************************/
tslWTime_t TSL26711::getWTime()
{
    return tsl_wtime;
}

/**************************************************************************/
/*
        Sets the Wait Long
*/
/**************************************************************************/
void TSL26711::setWaitLong(tslWaitLong_t waitlong)
{
    tsl_waitlong = waitlong;
}

/**************************************************************************/
/*
        Gets the Wait Long
*/
/**************************************************************************/
tslWaitLong_t TSL26711::getWaitLong()
{
    return tsl_waitlong;
}

/**************************************************************************/
/*
        Sets the Proximity LED Drive Strength
*/
/**************************************************************************/
void TSL26711::setProximityDrive(tslProximityDrive_t proximitydrive)
{
    tsl_proximitydrive = proximitydrive;
}

/**************************************************************************/
/*
        Gets the Proximity LED Drive Strength
*/
/**************************************************************************/
tslProximityDrive_t TSL26711::getProximityDrive()
{
    return tsl_proximitydrive;
}

/**************************************************************************/
/*
        Sets the Proximity Diode Selector
*/
/**************************************************************************/
void TSL26711::setProximityDiode(tslProximityDiode_t proximitydiode)
{
    tsl_proximitydiode = proximitydiode;
}

/**************************************************************************/
/*
        Gets the Proximity Diode Selector
*/
/**************************************************************************/
tslProximityDiode_t TSL26711::getProximityDiode()
{
    return tsl_proximitydiode;
}

/**************************************************************************/
/*
        Sets up the Digital Proximity Detector
*/
/**************************************************************************/
void TSL26711::setUpProximity(void)
{
    // Set Up the Configuration for the Digital Proximity Detector Enable Register
    /*
     // Set the Proximity Interrupt Mask
     uint8_t enable = tsl_proximityinterrupt;
    
     // Set the Wait Enable
     enable |= tsl_waitenable;
     
     // Set the Proximity Enable
     enable |= tsl_proximityenable;
     
     // Set the Power ON
     enable |= tsl_powerenable;
    */
    
    uint8_t enable =    TSL26711_REG_PROX_ENABLE_PIEN_NOT_ASSERT      |   // Not Asserted, Did not Permits Proximity Interrupts to be Generated
                        TSL26711_REG_PROX_ENABLE_WEN_ENABLE           |   // Enables the Wait Timer
                        TSL26711_REG_PROX_ENABLE_PEN_ENABLE           |   // Activates the Proximity
                        TSL26711_REG_PROX_ENABLE_PON_ENABLE;              // Activates the Internal Oscillator to Permit the Timers and ADC Channels to Operate
    
    // Write the configuration to the Digital Proximity Detector Enable Register
    writeRegister(tsl_i2cAddress, TSL26711_REG_PROX_ENABLE | TSL26711_REG_PROX_CMD_SELECT | TSL26711_REG_PROX_CMD_TYPE_AUTO_INCR, enable);
    
    // Wait for the configuration to complete
    delay(tsl_conversionDelay);
    
    // Set Up the Configuration for the Digital Proximity Detector PTIME Register
    // Set the Internal Integration Time of the Proximity ADC’s in 2.72ms Increments
    uint8_t ptime = tsl_ptime;
    
    // Write the configuration to the Digital Proximity Detector PTIME Register
    writeRegister(tsl_i2cAddress, TSL26711_REG_PROX_PTIME | TSL26711_REG_PROX_CMD_SELECT | TSL26711_REG_PROX_CMD_TYPE_AUTO_INCR, ptime);
    
    // Wait for the configuration to complete
    delay(tsl_conversionDelay);
    
    // Set Up the Configuration for the Digital Proximity Detector WTIME Register
    // Set the Wait Time is Set in 2.72 ms Increments Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
    uint8_t wtime = tsl_wtime;
    
    // Write the configuration to the Digital Proximity Detector WTIME Register
    writeRegister(tsl_i2cAddress, TSL26711_REG_PROX_WTIME | TSL26711_REG_PROX_CMD_SELECT | TSL26711_REG_PROX_CMD_TYPE_AUTO_INCR, wtime);
    
    // Wait for the configuration to complete
    delay(tsl_conversionDelay);
    
    // Set Up the Configuration for the Digital Proximity Detector Configuration Register
    /*
     // Set the Wait Long
     uint8_t config = tsl_waitlong;
    */
    
    uint8_t config =    TSL26711_REG_PROX_CONFIG_WLONG_NOT_ASSERT;   // Not Asserted, the Wait Cycles are Not Increased by a Factor 12X from that Programmed in the WTIME Register
    
    // Write the configuration to the Digital Proximity Detector Configuration Register
    writeRegister(tsl_i2cAddress, TSL26711_REG_PROX_CONFIG | TSL26711_REG_PROX_CMD_SELECT | TSL26711_REG_PROX_CMD_TYPE_AUTO_INCR, config);
    
    // Wait for the configuration to complete
    delay(tsl_conversionDelay);
    
    // Set Up the Configuration for the Digital Proximity Detector Control Register
    // Proximity LED Drive Strength
    uint8_t control = tsl_proximitydrive;
    
    // Proximity Diode Selector
    control |= tsl_proximitydiode;
    
    // Write the configuration to the Digital Proximity Detector Control Register
    writeRegister(tsl_i2cAddress, TSL26711_REG_PROX_CONTROL | TSL26711_REG_PROX_CMD_SELECT | TSL26711_REG_PROX_CMD_TYPE_AUTO_INCR, control);
    
    // Wait for the configuration to complete
    delay(tsl_conversionDelay);
}
/**************************************************************************/
/*
        Reads up the Digital Proximity Detector
*/
/**************************************************************************/
void TSL26711::Measure_Sensor(void)
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
void TSL26711::Measure_Proximity()
{
    uint8_t PdataLo, PdataHi;
    
    // Read the Data
    // Reading the Proximity Data Low Register
    PdataLo = readRegister(tsl_i2cAddress, TSL26711_REG_PROX_PDATAL | TSL26711_REG_PROX_CMD_SELECT | TSL26711_REG_PROX_CMD_TYPE_AUTO_INCR);
    // Reading the Proximity Data High Register
    PdataHi = readRegister(tsl_i2cAddress, TSL26711_REG_PROX_PDATAH | TSL26711_REG_PROX_CMD_SELECT | TSL26711_REG_PROX_CMD_TYPE_AUTO_INCR);
    // Conversion of the result
    // 16-bit unsigned result for Proximity Data of TSL26711
    tsl_proxData.P = (float)((PdataHi << 8) | PdataLo);
}
