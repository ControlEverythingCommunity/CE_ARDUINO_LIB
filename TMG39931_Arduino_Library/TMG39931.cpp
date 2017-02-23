/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TMG39931
        This code is designed to work with the TMG39931_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/light-sensor-gesture-color-als-and-proximity-sensor
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "TMG39931.h"

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
        Instantiates a new TMG39931 class with appropriate properties
*/
/**************************************************************************/
void TMG39931::getAddr_TMG39931(uint8_t i2cAddress)
{
    tmg_i2cAddress = i2cAddress;
    tmg_conversionDelay = TMG39931_CONVERSIONDELAY;
    
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool TMG39931::begin()
{
    Wire.begin();
    
    uint8_t devid = readRegister(tmg_i2cAddress, TMG39931_REG_COLORPROX_ID);
    if (devid != TMG39931_COLORPROX_DEV_ID)
        return false;
    
    // Set up for Light Sensor Gesture, Color, ALS and Proximity Sensor
    setUpColorProximity();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Pattern Burst Enable
*/
/**************************************************************************/
void TMG39931::setPatternBurst(tmgPatternBurst_t patternburst)
{
    tmg_patternburst = patternburst;
}

/**************************************************************************/
/*
        Gets the Pattern Burst Enable
*/
/**************************************************************************/
tmgPatternBurst_t TMG39931::getPatternBurst()
{
    return tmg_patternburst;
}

/**************************************************************************/
/*
        Sets the Gesture Enable
*/
/**************************************************************************/
void TMG39931::setGestureEnable(tmgGestureEnable_t gestureenable)
{
    tmg_gestureenable = gestureenable;
}

/**************************************************************************/
/*
        Gets the Gesture Enable
*/
/**************************************************************************/
tmgGestureEnable_t TMG39931::getGestureEnable()
{
    return tmg_gestureenable;
}

/**************************************************************************/
/*
        Sets the Proximity Interrupt Mask
*/
/**************************************************************************/
void TMG39931::setProximityInterrupt(tmgProximityInterrupt_t proximityinterrupt)
{
    tmg_proximityinterrupt = proximityinterrupt;
}

/**************************************************************************/
/*
        Gets the Proximity Interrupt Mask
*/
/**************************************************************************/
tmgProximityInterrupt_t TMG39931::getProximityInterrupt()
{
    return tmg_proximityinterrupt;
}

/**************************************************************************/
/*
        Sets the ALS/Color Interrupt Mask
*/
/**************************************************************************/
void TMG39931::setColorInterrupt(tmgColorInterrupt_t colorinterrupt)
{
    tmg_colorinterrupt = colorinterrupt;
}

/**************************************************************************/
/*
        Gets the ALS/Color Interrupt Mask
*/
/**************************************************************************/
tmgColorInterrupt_t TMG39931::getColorInterrupt()
{
    return tmg_colorinterrupt;
}

/**************************************************************************/
/*
        Sets the Wait Enable
*/
/**************************************************************************/
void TMG39931::setWaitEnable(tmgWaitEnable_t waitenable)
{
    tmg_waitenable = waitenable;
}

/**************************************************************************/
/*
        Gets the Wait Enable
*/
/**************************************************************************/
tmgWaitEnable_t TMG39931::getWaitEnable()
{
    return tmg_waitenable;
}

/**************************************************************************/
/*
        Sets the Proximity Enable
*/
/**************************************************************************/
void TMG39931::setProximityEnable(tmgProximityEnable_t proximityenable)
{
    tmg_proximityenable = proximityenable;
}

/**************************************************************************/
/*
        Gets the Proximity Enable
*/
/**************************************************************************/
tmgProximityEnable_t TMG39931::getProximityEnable()
{
    return tmg_proximityenable;
}

/**************************************************************************/
/*
        Sets the ALS/Color Enable
*/
/**************************************************************************/
void TMG39931::setColorEnable(tmgColorEnable_t colorenable)
{
    tmg_colorenable = colorenable;
}

/**************************************************************************/
/*
        Gets the ALS/Color Enable
*/
/**************************************************************************/
tmgColorEnable_t TMG39931::getColorEnable()
{
    return tmg_colorenable;
}

/**************************************************************************/
/*
        Sets the Power ON
*/
/**************************************************************************/
void TMG39931::setPowerEnable(tmgPowerEnable_t powerenable)
{
    tmg_powerenable = powerenable;
}
                    
/**************************************************************************/
/*
        Gets the Power ON
*/
/**************************************************************************/
tmgPowerEnable_t TMG39931::getPowerEnable()
{
    return tmg_powerenable;
}

/**************************************************************************/
/*
        Sets to Control the Internal Integration Time of the ALS Channel 
        ADC’s in 2.78 ms Increments
*/
/**************************************************************************/
void TMG39931::setATime(tmgATime_t atime)
{
    tmg_atime = atime;
}

/**************************************************************************/
/*
        Gets to Control the Internal Integration Time of the ALS/Color Channel
        ADC’s in 2.78 ms Increments
*/
/**************************************************************************/
tmgATime_t TMG39931::getATime()
{
    return tmg_atime;
}

/**************************************************************************/
/*
        Sets to Control the Wait Time is Set in 2.78 ms Increments 
        Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
*/
/**************************************************************************/
void TMG39931::setWTime(tmgWTime_t wtime)
{
    tmg_wtime = wtime;
}

/**************************************************************************/
/*
        Gets to Control the Wait Time is Set in 2.78 ms Increments 
        Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
*/
/**************************************************************************/
tmgWTime_t TMG39931::getWTime()
{
    return tmg_wtime;
}

/**************************************************************************/
/*
        Sets the Wait Long
*/
/**************************************************************************/
void TMG39931::setWaitLong(tmgWaitLong_t waitlong)
{
    tmg_waitlong = waitlong;
}

/**************************************************************************/
/*
        Gets the Wait Long
*/
/**************************************************************************/
tmgWaitLong_t TMG39931::getWaitLong()
{
    return tmg_waitlong;
}

/**************************************************************************/
/*
        Sets the Proximity LED Drive Strength
*/
/**************************************************************************/
void TMG39931::setProximityDrive(tmgProximityDrive_t proximitydrive)
{
    tmg_proximitydrive = proximitydrive;
}

/**************************************************************************/
/*
        Gets the Proximity LED Drive Strength
*/
/**************************************************************************/
tmgProximityDrive_t TMG39931::getProximityDrive()
{
    return tmg_proximitydrive;
}

/**************************************************************************/
/*
        Sets the Proximity Gain Value
*/
/**************************************************************************/
void TMG39931::setProximityGain(tmgProximityGain_t proximitygain)
{
    tmg_proximitygain = proximitygain;
}

/**************************************************************************/
/*
        Gets the Proximity Gain Value
*/
/**************************************************************************/
tmgProximityGain_t TMG39931::getProximityGain()
{
    return tmg_proximitygain;
}

/**************************************************************************/
/*
        Sets the RGBC Gain Value
*/
/**************************************************************************/
void TMG39931::setColorGain(tmgColorGain_t colorgain)
{
    tmg_colorgain = colorgain;
}

/**************************************************************************/
/*
        Gets the RGBC Gain Value
*/
/**************************************************************************/
tmgColorGain_t TMG39931::getColorGain()
{
    return tmg_colorgain;
}

/**************************************************************************/
/*
        Sets up the Light Sensor Gesture, Color, ALS and Proximity Sensor
*/
/**************************************************************************/
void TMG39931::setUpColorProximity(void)
{
    // Set Up the Configuration for the Light Sensor Gesture, Color, ALS and Proximity Sensor Enable Register
    /*
     // Set the Pattern Burst Enable
     uint8_t enable = tmg_patternburst;
     
     // Set the Gesture Enable
     enable |= tmg_gestureburst;
     
     // Set the Proximity Interrupt Mask
     enable |= tmg_proximityinterrupt;
    
     // Set the Color Interrupt Mask
     enable |= tmg_colorinterrupt;
    
     // Set the Wait Enable
     enable |= tmg_waitenable;
     
     // Set the Proximity Enable
     enable |= tmg_proximityenable;
     
     // Set the Color Enable
     enable |= tmg_colorinable;
     
     // Set the Power ON
     enable |= tmg_powerenable;
    */
    
    uint8_t enable =    TMG39931_REG_COLORPROX_ENABLE_PBEN_NOT_ASSERT       |   // Not Asserted, No Control of the LED Driver Pin (LDR) is Managed by the IRBeam State Machine
                        TMG39931_REG_COLORPROX_ENABLE_GEN_NOT_ASSERT        |   // Not Asserted, the Gesture State Machine can not be Activated
                        TMG39931_REG_COLORPROX_ENABLE_PIEN_NOT_ASSERT       |   // Not Asserted, Did not Permits Proximity Interrupts to be Generated
                        TMG39931_REG_COLORPROX_ENABLE_AIEN_NOT_ASSERT       |   // Not Asserted, Did not Permits ALS/Color Interrupts to be Generated
                        TMG39931_REG_COLORPROX_ENABLE_WEN_ENABLE            |   // Enables the Wait Timer
                        TMG39931_REG_COLORPROX_ENABLE_PEN_ENABLE            |   // Activates the Proximity
                        TMG39931_REG_COLORPROX_ENABLE_AEN_ENABLE            |   // Activates the ALS/Color ADC
                        TMG39931_REG_COLORPROX_ENABLE_PON_ENABLE;              // Activates the Internal Oscillator to Permit the Timers and ADC Channels to Operate
    
    // Write the configuration to the Light Sensor Gesture, Color, ALS and Proximity Sensor Enable Register
    writeRegister(tmg_i2cAddress, TMG39931_REG_COLORPROX_ENABLE, enable);
    
    // Wait for the configuration to complete
    delay(tmg_conversionDelay);
    
    // Set Up the Configuration for the Light Sensor Gesture, Color, ALS and Proximity Sensor ATIME Register
    // Set the Internal Integration Time of the ALS Channel ADC’s in 2.78 ms Increments
    uint8_t atime = tmg_atime;
    
    // Write the configuration to the Light Sensor Gesture, Color, ALS and Proximity Sensor ATIME Register
    writeRegister(tmg_i2cAddress, TMG39931_REG_COLORPROX_ATIME, atime);
    
    // Wait for the configuration to complete
    delay(tmg_conversionDelay);
    
    // Set Up the Configuration for the Light Sensor Gesture, Color, ALS and Proximity Sensor WTIME Register
    // Set the Wait Time is Set in 2.78 ms Increments Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
    uint8_t wtime = tmg_wtime;
    
    // Write the configuration to the Light Sensor Gesture, Color, ALS and Proximity Sensor WTIME Register
    writeRegister(tmg_i2cAddress, TMG39931_REG_COLORPROX_WTIME, wtime);
    
    // Wait for the configuration to complete
    delay(tmg_conversionDelay);
    
    // Set Up the Configuration for the Light Sensor Gesture, Color, ALS and Proximity Sensor Configuration Register
    /*
     // Set the Wait Long
     uint8_t config = tmg_waitlong;
    */
    
    uint8_t config = TMG39931_REG_COLORPROX_CONFIG_WLONG_NOT_ASSERT;  // Not Asserted, the Wait Cycles are Not Increased by a Factor 12X from that Programmed in the WTIME Register
    
    // Write the configuration to the Light Sensor Gesture, Color, ALS and Proximity Sensor Configuration Register One
    writeRegister(tmg_i2cAddress, TMG39931_REG_COLORPROX_CONFIG1, config);
    
    // Wait for the configuration to complete
    delay(tmg_conversionDelay);
    
    // Set Up the Configuration for the Light Sensor Gesture, Color, ALS and Proximity Sensor Control Register
    // Proximity LED Drive Strength
    uint8_t control = tmg_proximitydrive;
    
    // Proximity Gain Value
    control |= tmg_proximitygain;
    
    // ALS Gain
    control |= tmg_colorgain;
    
    // Write the configuration to the Light Sensor Gesture, Color, ALS and Proximity Sensor Control Register
    writeRegister(tmg_i2cAddress, TMG39931_REG_COLORPROX_CONTROL, control);
    
    // Wait for the configuration to complete
    delay(tmg_conversionDelay);
}
/**************************************************************************/
/*
        Reads up theLight Sensor Gesture, Color, ALS and Proximity Sensor
*/
/**************************************************************************/
void TMG39931::Measure_Sensor(void)
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
void TMG39931::Measure_Color()
{
    uint8_t CdataLo, CdataHi, RdataLo, RdataHi, GdataLo, GdataHi, BdataLo, BdataHi;
    uint16_t CData, RData, GData, BData;
    
    // Read the Data
    // Reading the Clear Data Low Register
    CdataLo = readRegister(tmg_i2cAddress, TMG39931_REG_COLORPROX_CDATAL);
    // Reading the Clear Data High Register
    CdataHi = readRegister(tmg_i2cAddress, TMG39931_REG_COLORPROX_CDATAH);
    // Conversion of the result
    // 16-bit unsigned result for Clear Data of TMG39931
    CData = (uint16_t)((CdataHi << 8) | CdataLo);
    
    // Reading the Red Data Low Register
    RdataLo = readRegister(tmg_i2cAddress, TMG39931_REG_COLORPROX_RDATAL);
    // Reading the Red Data High Register
    RdataHi = readRegister(tmg_i2cAddress, TMG39931_REG_COLORPROX_RDATAH);
    // Conversion of the result
    // 16-bit unsigned result for Red Data of TMG39931
    RData = (uint16_t)((RdataHi << 8) | RdataLo);
    
    // Reading the Green Data Low Register
    GdataLo = readRegister(tmg_i2cAddress, TMG39931_REG_COLORPROX_GDATAL);
    // Reading the Green Data High Register
    GdataHi = readRegister(tmg_i2cAddress, TMG39931_REG_COLORPROX_GDATAH);
    // Conversion of the result
    // 16-bit unsigned result for Green Data of TMG39931
    GData = (uint16_t)((GdataHi << 8) | GdataLo);
    
    // Reading the Blue Data Low Register
    BdataLo = readRegister(tmg_i2cAddress, TMG39931_REG_COLORPROX_BDATAL);
    // Reading the Blue Data High Register
    BdataHi = readRegister(tmg_i2cAddress, TMG39931_REG_COLORPROX_BDATAH);
    // Conversion of the result
    // 16-bit unsigned result for Blue Data of TMG39931
    BData = (uint16_t)((BdataHi << 8) | BdataLo);
    
    tmg_colorproxData.C = CData;
    tmg_colorproxData.R = RData;
    tmg_colorproxData.G = GData;
    tmg_colorproxData.B = BData;
}

/**************************************************************************/
/*
        Reads the Proximity Sensor
*/
/**************************************************************************/
void TMG39931::Measure_Proximity()
{
    uint8_t Pdata;
    
    // Read the Data
    // Reading the Proximity Data Register
    Pdata = readRegister(tmg_i2cAddress, TMG39931_REG_COLORPROX_PDATA);
    // Conversion of the result
    // 8-bit unsigned result for Proximity Data of TMG39931
    tmg_colorproxData.P = (uint8_t)(Pdata);
}
