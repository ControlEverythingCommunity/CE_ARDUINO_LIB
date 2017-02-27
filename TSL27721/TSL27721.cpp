/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TSL27721
        This code is designed to work with the TSL27721_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "TSL27721.h"

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
        Instantiates a new TSL27721 class with appropriate properties
*/
/**************************************************************************/
void TSL27721::getAddr_TSL27721(uint8_t i2cAddress)
{
    tsl_i2cAddress = i2cAddress;
    tsl_conversionDelay = TSL27721_CONVERSIONDELAY;
    
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool TSL27721::begin()
{
    Wire.begin();
    
    uint8_t devid = readRegister(tsl_i2cAddress, TSL27721_REG_ALSPROX_ID);
    if (devid != TSL27721_ALSPROX_DEV_ID)
        return false;
    
    // Set up for Digital Ambient Light and Proximity Sensor
    // setUpALSProximity();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Select Command Register
*/
/**************************************************************************/
void TSL27721::setCommand(tslCommand_t command)
{
    tsl_command = command;
}

/**************************************************************************/
/*
        Gets the Select Command Register
*/
/**************************************************************************/
tslCommand_t TSL27721::getCommand()
{
    return tsl_command;
}

/**************************************************************************/
/*
        Sets the Sleep After Interrupt
*/
/**************************************************************************/
void TSL27721::setSleepInterrupt(tslSleepInterrupt_t sleepinterrupt)
{
    tsl_sleepinterrupt = sleepinterrupt;
}

/**************************************************************************/
/*
        Gets the Sleep After Interrupt
*/
/**************************************************************************/
tslSleepInterrupt_t TSL27721::getSleepInterrupt()
{
    return tsl_sleepinterrupt;
}

/**************************************************************************/
/*
        Sets the Proximity Interrupt Mask
*/
/**************************************************************************/
void TSL27721::setProximityInterrupt(tslProximityInterrupt_t proximityinterrupt)
{
    tsl_proximityinterrupt = proximityinterrupt;
}

/**************************************************************************/
/*
        Gets the Proximity Interrupt Mask
*/
/**************************************************************************/
tslProximityInterrupt_t TSL27721::getProximityInterrupt()
{
    return tsl_proximityinterrupt;
}

/**************************************************************************/
/*
        Sets the ALS Interrupt Mask
*/
/**************************************************************************/
void TSL27721::setALSInterrupt(tslALSInterrupt_t alsinterrupt)
{
    tsl_alsinterrupt = alsinterrupt;
}

/**************************************************************************/
/*
        Gets the ALS Interrupt Mask
*/
/**************************************************************************/
tslALSInterrupt_t TSL27721::getALSInterrupt()
{
    return tsl_alsinterrupt;
}

/**************************************************************************/
/*
        Sets the Wait Enable
*/
/**************************************************************************/
void TSL27721::setWaitEnable(tslWaitEnable_t waitenable)
{
    tsl_waitenable = waitenable;
}

/**************************************************************************/
/*
        Gets the Wait Enable
*/
/**************************************************************************/
tslWaitEnable_t TSL27721::getWaitEnable()
{
    return tsl_waitenable;
}

/**************************************************************************/
/*
        Sets the Proximity Enable
*/
/**************************************************************************/
void TSL27721::setProximityEnable(tslProximityEnable_t proximityenable)
{
    tsl_proximityenable = proximityenable;
}

/**************************************************************************/
/*
        Gets the Proximity Enable
*/
/**************************************************************************/
tslProximityEnable_t TSL27721::getProximityEnable()
{
    return tsl_proximityenable;
}

/**************************************************************************/
/*
        Sets the ALS Enable
*/
/**************************************************************************/
void TSL27721::setALSEnable(tslALSEnable_t alsenable)
{
    tsl_alsenable = alsenable;
}

/**************************************************************************/
/*
        Gets the ALS Enable
*/
/**************************************************************************/
tslALSEnable_t TSL27721::getALSEnable()
{
    return tsl_alsenable;
}

/**************************************************************************/
/*
        Sets the Power ON
*/
/**************************************************************************/
void TSL27721::setPowerEnable(tslPowerEnable_t powerenable)
{
    tsl_powerenable = powerenable;
}
                    
/**************************************************************************/
/*
        Gets the Power ON
*/
/**************************************************************************/
tslPowerEnable_t TSL27721::getPowerEnable()
{
    return tsl_powerenable;
}

/**************************************************************************/
/*
        Sets to Control the Internal Integration Time of the ALS Channel 
        ADC’s in 2.73ms Increments
*/
/**************************************************************************/
void TSL27721::setATime(tslATime_t atime)
{
    tsl_atime = atime;
}

/**************************************************************************/
/*
        Gets to Control the Internal Integration Time of the ALS Channel 
        ADC’s in 2.73ms Increments
*/
/**************************************************************************/
tslATime_t TSL27721::getATime()
{
    return tsl_atime;
}

/**************************************************************************/
/*
        Sets to Control the Internal Integration Time of the Proximity
        ADC’s in 2.73ms Increments
*/
/**************************************************************************/
void TSL27721::setPTime(tslPTime_t ptime)
{
    tsl_ptime = ptime;
}

/**************************************************************************/
/*
        Gets to Control the Internal Integration Time of the Proximity 
        ADC’s in 2.73ms Increments
*/
/**************************************************************************/
tslPTime_t TSL27721::getPTime()
{
    return tsl_ptime;
}

/**************************************************************************/
/*
        Sets to Control the Wait Time is Set in 2.73 ms Increments 
        Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
*/
/**************************************************************************/
void TSL27721::setWTime(tslWTime_t wtime)
{
    tsl_wtime = wtime;
}

/**************************************************************************/
/*
        Gets to Control the Wait Time is Set in 2.73 ms Increments 
        Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
*/
/**************************************************************************/
tslWTime_t TSL27721::getWTime()
{
    return tsl_wtime;
}

/**************************************************************************/
/*
        Sets the ALS Gain Level
*/
/**************************************************************************/
void TSL27721::setALSGainLevel(tslALSGainLevel_t alsgainlevel)
{
    tsl_alsgainlevel = alsgainlevel;
}

/**************************************************************************/
/*
        Gets the ALS Gain Level
*/
/**************************************************************************/
tslALSGainLevel_t TSL27721::getALSGainLevel()
{
    return tsl_alsgainlevel;
}

/**************************************************************************/
/*
        Sets the Wait Long
*/
/**************************************************************************/
void TSL27721::setWaitLong(tslWaitLong_t waitlong)
{
    tsl_waitlong = waitlong;
}

/**************************************************************************/
/*
        Gets the Wait Long
*/
/**************************************************************************/
tslWaitLong_t TSL27721::getWaitLong()
{
    return tsl_waitlong;
}

/**************************************************************************/
/*
        Sets the Proximity Drive Level
*/
/**************************************************************************/
void TSL27721::setProximityDriveLevel(tslProximityDriveLevel_t proximitydrivelevel)
{
    tsl_proximitydrivelevel = proximitydrivelevel;
}

/**************************************************************************/
/*
        Gets the Proximity Drive Level
*/
/**************************************************************************/
tslProximityDriveLevel_t TSL27721::getProximityDriveLevel()
{
    return tsl_proximitydrivelevel;
}

/**************************************************************************/
/*
        Sets the Proximity LED Drive Strength
*/
/**************************************************************************/
void TSL27721::setProximityDrive(tslProximityDrive_t proximitydrive)
{
    tsl_proximitydrive = proximitydrive;
}

/**************************************************************************/
/*
        Gets the Proximity LED Drive Strength
*/
/**************************************************************************/
tslProximityDrive_t TSL27721::getProximityDrive()
{
    return tsl_proximitydrive;
}

/**************************************************************************/
/*
        Sets the Proximity Diode Selector
*/
/**************************************************************************/
void TSL27721::setProximityDiode(tslProximityDiode_t proximitydiode)
{
    tsl_proximitydiode = proximitydiode;
}

/**************************************************************************/
/*
        Gets the Proximity Diode Selector
*/
/**************************************************************************/
tslProximityDiode_t TSL27721::getProximityDiode()
{
    return tsl_proximitydiode;
}

/**************************************************************************/
/*
        Sets the Proximity Gain
*/
/**************************************************************************/
void TSL27721::setProximityGain(tslProximityGain_t proximitygain)
{
    tsl_proximitygain = proximitygain;
}

/**************************************************************************/
/*
        Gets the Proximity Gain
*/
/**************************************************************************/
tslProximityGain_t TSL27721::getProximityGain()
{
    return tsl_proximitygain;
}

/**************************************************************************/
/*
        Sets the ALS Gain
*/
/**************************************************************************/
void TSL27721::setALSGain(tslALSGain_t alsgain)
{
    tsl_alsgain = alsgain;
}

/**************************************************************************/
/*
        Gets the ALS Gain
*/
/**************************************************************************/
tslALSGain_t TSL27721::getALSGain()
{
    return tsl_alsgain;
}

/**************************************************************************/
/*
        Sets up the Digital Ambient Light and Proximity Sensor
*/
/**************************************************************************/
void TSL27721::setUpALSProximity(void)
{
    // Set Up the Configuration for the Digital Ambient Light and Proximity Sensor Enable Register
    /*
     // Set the Sleep After Interrupt
     uint8_t enable = tsl_sleepinterrupt;
    
     // Set the Proximity Interrupt Mask
     enable |= tsl_proximityinterrupt;
    
     // Set the ALS Interrupt Mask
     enable |= tsl_alsinterrupt;
    
     // Set the Wait Enable
     enable |= tsl_waitenable;
     
     // Set the Proximity Enable
     enable |= tsl_proximityenable;
     
     // Set the ALS Enable
     enable |= tsl_alsinable;
     
     // Set the Power ON
     enable |= tsl_powerenable;
    */
    
    uint8_t enable =    TSL27721_REG_ALSPROX_ENABLE_SAI_NOT_ASSERT       |   // Not Asserted, the Device will not Power Down at the End of a Proximity or ALS Cycle if an Interrupt has been Generated
                        TSL27721_REG_ALSPROX_ENABLE_PIEN_NOT_ASSERT      |   // Not Asserted, Did not Permits Proximity Interrupts to be Generated
                        TSL27721_REG_ALSPROX_ENABLE_AIEN_NOT_ASSERT      |   // Not Asserted, Did not Permits ALS Interrupts to be Generated
                        TSL27721_REG_ALSPROX_ENABLE_WEN_ENABLE           |   // Enables the Wait Timer
                        TSL27721_REG_ALSPROX_ENABLE_PEN_ENABLE           |   // Activates the Proximity
                        TSL27721_REG_ALSPROX_ENABLE_AEN_ENABLE           |   // Activates the ALS
                        TSL27721_REG_ALSPROX_ENABLE_PON_ENABLE;              // Activates the Internal Oscillator to Permit the Timers and ADC Channels to Operate
    
    // Write the configuration to the Digital Ambient Light and Proximity Sensor Enable Register
    writeRegister(tsl_i2cAddress, TSL27721_REG_ALSPROX_ENABLE | TSL27721_REG_ALSPROX_CMD_SELECT | TSL27721_REG_ALSPROX_CMD_TYPE_AUTO_INCR, enable);
    
    // Wait for the configuration to complete
    delay(tsl_conversionDelay);
    
    // Set Up the Configuration for the Digital Ambient Light and Proximity Sensor ATIME Register
    // Set the Internal Integration Time of the ALS Channel ADC’s in 2.73ms Increments
    uint8_t atime = tsl_atime;
    
    // Write the configuration to the Digital Ambient Light and Proximity Sensor ATIME Register
    writeRegister(tsl_i2cAddress, TSL27721_REG_ALSPROX_ATIME | TSL27721_REG_ALSPROX_CMD_SELECT | TSL27721_REG_ALSPROX_CMD_TYPE_AUTO_INCR, atime);
    
    // Wait for the configuration to complete
    delay(tsl_conversionDelay);
    
    // Set Up the Configuration for the Digital Ambient Light and Proximity Sensor PTIME Register
    // Set the Internal Integration Time of the Proximity ADC’s in 2.73ms Increments
    uint8_t ptime = tsl_ptime;
    
    // Write the configuration to the Digital Ambient Light and Proximity Sensor PTIME Register
    writeRegister(tsl_i2cAddress, TSL27721_REG_ALSPROX_PTIME | TSL27721_REG_ALSPROX_CMD_SELECT | TSL27721_REG_ALSPROX_CMD_TYPE_AUTO_INCR, ptime);
    
    // Wait for the configuration to complete
    delay(tsl_conversionDelay);
    
    // Set Up the Configuration for the Digital Ambient Light and Proximity Sensor WTIME Register
    // Set the Wait Time is Set in 2.73 ms Increments Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
    uint8_t wtime = tsl_wtime;
    
    // Write the configuration to the Digital Ambient Light and Proximity Sensor WTIME Register
    writeRegister(tsl_i2cAddress, TSL27721_REG_ALSPROX_WTIME | TSL27721_REG_ALSPROX_CMD_SELECT | TSL27721_REG_ALSPROX_CMD_TYPE_AUTO_INCR, wtime);
    
    // Wait for the configuration to complete
    delay(tsl_conversionDelay);
    
    // Set Up the Configuration for the Digital Ambient Light and Proximity Sensor Configuration Register
    /*
     // Set the ALS Gain Level
     uint8_t config = tsl_alsgainlevel;
     
     // Set the Wait Long
     config |= tsl_waitlong;
     
     // Set the Proximity Drive Level
     config |= tsl_proximitydrivelevel;
    */
    
    uint8_t config =    TSL27721_REG_ALSPROX_CONFIG_AGL_NOT_ASSERT           |   // Not Asserted, the 1X and 8X ALS Gain (AGAIN) Modes are Scaled by 1
                        TSL27721_REG_ALSPROX_CONFIG_WLONG_NOT_ASSERT         |   // Not Asserted, the Wait Cycles are Not Increased by a Factor 12X from that Programmed in the WTIME Register
                        TSL27721_REG_ALSPROX_CONFIG_PDL_NOT_ASSERT;              // Not Asserted, the Proximity LDR Drive Current is Not Reduced by 9
    
    // Write the configuration to the Digital Ambient Light and Proximity Sensor Configuration Register
    writeRegister(tsl_i2cAddress, TSL27721_REG_ALSPROX_CONFIG | TSL27721_REG_ALSPROX_CMD_SELECT | TSL27721_REG_ALSPROX_CMD_TYPE_AUTO_INCR, config);
    
    // Wait for the configuration to complete
    delay(tsl_conversionDelay);
    
    // Set Up the Configuration for the Digital Ambient Light and Proximity Sensor Control Register
    // Proximity LED Drive Strength
    uint8_t control = tsl_proximitydrive;
    
    // Proximity Diode Selector
    control |= tsl_proximitydiode;
    
    // Proximity Gain
    control |= tsl_proximitygain;
    
    // ALS Gain
    control |= tsl_alsgain;
    
    // Write the configuration to the Digital Ambient Light and Proximity Sensor Control Register
    writeRegister(tsl_i2cAddress, TSL27721_REG_ALSPROX_CONTROL | TSL27721_REG_ALSPROX_CMD_SELECT | TSL27721_REG_ALSPROX_CMD_TYPE_AUTO_INCR, control);
    
    // Wait for the configuration to complete
    delay(tsl_conversionDelay);
}
/**************************************************************************/
/*
        Reads up the Digital Ambient Light and Proximity Sensor
*/
/**************************************************************************/
void TSL27721::Measure_Sensor(void)
{
    // Read all the Sensors
    // ALS and Proximity Data
    Measure_ALS();
    Measure_Proximity();
}

/**************************************************************************/
/*
        Reads the Digital Ambient Light Sensor
*/
/**************************************************************************/
void TSL27721::Measure_ALS()
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
        case ATIME_2_73MS:
            atime = 2.73;
            break;
            
        case ATIME_27_3MS:
            atime = 27.3;
            break;
            
        case ATIME_101MS:
            atime = 101.0;
            break;
            
        case ATIME_175MS:
            atime = 175.0;
            break;
            
        case ATIME_699MS:
            atime = 699;
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
    C0dataLo = readRegister(tsl_i2cAddress, TSL27721_REG_ALSPROX_C0DATAL | TSL27721_REG_ALSPROX_CMD_SELECT | TSL27721_REG_ALSPROX_CMD_TYPE_AUTO_INCR);
    // Reading the ALS CH0 Data High Register
    C0dataHi = readRegister(tsl_i2cAddress, TSL27721_REG_ALSPROX_C0DATAH | TSL27721_REG_ALSPROX_CMD_SELECT | TSL27721_REG_ALSPROX_CMD_TYPE_AUTO_INCR);
    // Conversion of the result
    // 16-bit unsigned result for ALS CH0 Data of TSL27721
    C0Data = (uint16_t)((C0dataHi << 8) | C0dataLo);
    
    // Reading the ALS CH1 Data Low Register
    C1dataLo = readRegister(tsl_i2cAddress, TSL27721_REG_ALSPROX_C1DATAL | TSL27721_REG_ALSPROX_CMD_SELECT | TSL27721_REG_ALSPROX_CMD_TYPE_AUTO_INCR);
    // Reading the ALS CH1 Data High Register
    C1dataHi = readRegister(tsl_i2cAddress, TSL27721_REG_ALSPROX_C1DATAH | TSL27721_REG_ALSPROX_CMD_SELECT | TSL27721_REG_ALSPROX_CMD_TYPE_AUTO_INCR);
    // Conversion of the result
    // 16-bit unsigned result for ALS CH1 Data of TSL27721
    C1Data = (uint16_t)((C1dataHi << 8) | C1dataLo);
    
    /*
     CPL = (ATIME_ms × AGAINx) / (GA × 60)
     Lux1 = (1 × C0DATA - 1.87 × C1DATA) / CPL
     Lux2 = (0.63 × C0DATA - 1 × C1DATA) / CPL
     Lux = MAX(Lux1, Lux2, 0)
     For a Device in Open Air with No Aperture or
     Glass/Plastic Above the Device, GA = 1
    */
    
    CPL = (atime * again) / 60.0;
    // Fluorescent and Incandescent Light
    Lux1 = (C0Data - (1.87 * C1Data)) / CPL;
    // Dimmed Incandescent Light
    Lux2 = ((0.63 * C0Data) - (1.00 * C1Data)) / CPL;
    
    // Calculate the Final Lux
    if((Lux1 > 0) && (Lux1 > Lux2))
    {
        Lux = Lux1;
    }
    else if((Lux2 > 0) && (Lux2 > Lux1))
    {
        Lux = Lux2;
    }
    
    tsl_alsproxData.L = Lux;
}

/**************************************************************************/
/*
        Reads the Proximity Sensor
*/
/**************************************************************************/
void TSL27721::Measure_Proximity()
{
    uint8_t PdataLo, PdataHi;
    
    // Read the Data
    // Reading the Proximity Data Low Register
    PdataLo = readRegister(tsl_i2cAddress, TSL27721_REG_ALSPROX_PDATAL | TSL27721_REG_ALSPROX_CMD_SELECT | TSL27721_REG_ALSPROX_CMD_TYPE_AUTO_INCR);
    // Reading the Proximity Data High Register
    PdataHi = readRegister(tsl_i2cAddress, TSL27721_REG_ALSPROX_PDATAH | TSL27721_REG_ALSPROX_CMD_SELECT | TSL27721_REG_ALSPROX_CMD_TYPE_AUTO_INCR);
    // Conversion of the result
    // 16-bit unsigned result for Proximity Data of TSL27721
    tsl_alsproxData.P = (float)((PdataHi << 8) | PdataLo);
}
