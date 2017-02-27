/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TCS34903
        This code is designed to work with the TCS34903FN_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Color?sku=TCS34903FN_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "TCS34903.h"

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
        Instantiates a new TCS34903 class with appropriate properties
*/
/**************************************************************************/
void TCS34903::getAddr_TCS34903(uint8_t i2cAddress)
{
    tcs_i2cAddress = i2cAddress;
    tcs_conversionDelay = TCS34903_CONVERSIONDELAY;
    
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool TCS34903::begin()
{
    Wire.begin();
    
    uint8_t devid = readRegister(tcs_i2cAddress, TCS34903_REG_COLOR_DEVID);
    if (devid != TCS34903_COLOR_DEV_ID)
        return false;
    
    // Set up for Color Light-to-Digital Converter
    // setUpColor();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Sleep After Interrupt
*/
/**************************************************************************/
void TCS34903::setSleepInterrupt(tcsSleepInterrupt_t sleepinterrupt)
{
    tcs_sleepinterrupt = sleepinterrupt;
}

/**************************************************************************/
/*
        Gets the Sleep After Interrupt
*/
/**************************************************************************/
tcsSleepInterrupt_t TCS34903::getSleepInterrupt()
{
    return tcs_sleepinterrupt;
}

/**************************************************************************/
/*
        Sets the ALS/Color Interrupt Mask
*/
/**************************************************************************/
void TCS34903::setColorInterrupt(tcsColorInterrupt_t colorinterrupt)
{
    tcs_colorinterrupt = colorinterrupt;
}

/**************************************************************************/
/*
        Gets the ALS/Color Interrupt Mask
*/
/**************************************************************************/
tcsColorInterrupt_t TCS34903::getColorInterrupt()
{
    return tcs_colorinterrupt;
}

/**************************************************************************/
/*
        Sets the Wait Enable
*/
/**************************************************************************/
void TCS34903::setWaitEnable(tcsWaitEnable_t waitenable)
{
    tcs_waitenable = waitenable;
}

/**************************************************************************/
/*
        Gets the Wait Enable
*/
/**************************************************************************/
tcsWaitEnable_t TCS34903::getWaitEnable()
{
    return tcs_waitenable;
}

/**************************************************************************/
/*
        Sets the ALS/Color Enable
*/
/**************************************************************************/
void TCS34903::setColorEnable(tcsColorEnable_t colorenable)
{
    tcs_colorenable = colorenable;
}

/**************************************************************************/
/*
        Gets the ALS/Color Enable
*/
/**************************************************************************/
tcsColorEnable_t TCS34903::getColorEnable()
{
    return tcs_colorenable;
}

/**************************************************************************/
/*
        Sets the Power ON
*/
/**************************************************************************/
void TCS34903::setPowerEnable(tcsPowerEnable_t powerenable)
{
    tcs_powerenable = powerenable;
}
                    
/**************************************************************************/
/*
        Gets the Power ON
*/
/**************************************************************************/
tcsPowerEnable_t TCS34903::getPowerEnable()
{
    return tcs_powerenable;
}

/**************************************************************************/
/*
        Sets to Control the Internal Integration Time of the ALS Channel 
        ADC’s in 2.78 ms Increments
*/
/**************************************************************************/
void TCS34903::setATime(tcsATime_t atime)
{
    tcs_atime = atime;
}

/**************************************************************************/
/*
        Gets to Control the Internal Integration Time of the ALS/Color Channel
        ADC’s in 2.78 ms Increments
*/
/**************************************************************************/
tcsATime_t TCS34903::getATime()
{
    return tcs_atime;
}

/**************************************************************************/
/*
        Sets to Control the Wait Time is Set in 2.78 ms Increments 
        Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
*/
/**************************************************************************/
void TCS34903::setWTime(tcsWTime_t wtime)
{
    tcs_wtime = wtime;
}

/**************************************************************************/
/*
        Gets to Control the Wait Time is Set in 2.78 ms Increments 
        Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
*/
/**************************************************************************/
tcsWTime_t TCS34903::getWTime()
{
    return tcs_wtime;
}

/**************************************************************************/
/*
        Sets the Wait Long
*/
/**************************************************************************/
void TCS34903::setWaitLong(tcsWaitLong_t waitlong)
{
    tcs_waitlong = waitlong;
}

/**************************************************************************/
/*
        Gets the Wait Long
*/
/**************************************************************************/
tcsWaitLong_t TCS34903::getWaitLong()
{
    return tcs_waitlong;
}

/**************************************************************************/
/*
        Sets the RGBC Gain Control
*/
/**************************************************************************/
void TCS34903::setColorGain(tcsColorGain_t colorgain)
{
    tcs_colorgain = colorgain;
}

/**************************************************************************/
/*
        Gets the RGBC Gain Control
*/
/**************************************************************************/
tcsColorGain_t TCS34903::getColorGain()
{
    return tcs_colorgain;
}

/**************************************************************************/
/*
        Sets the IR Sensor Access
*/
/**************************************************************************/
void TCS34903::setIRAccess(tcsIRAccess_t iraccess)
{
    tcs_iraccess = iraccess;
}

/**************************************************************************/
/*
        Gets the IR Sensor Access
*/
/**************************************************************************/
tcsIRAccess_t TCS34903::getIRAccess()
{
    return tcs_iraccess;
}

/**************************************************************************/
/*
        Sets up the Color Light-to-Digital Converter
*/
/**************************************************************************/
void TCS34903::setUpColor(void)
{
    // Set Up the Configuration for the Color Light-to-Digital Converter Enable Register
    /*
     // Set the Sleep After Interrupt
     uint8_t enable = tcs_sleepinterrupt;
    
     // Set the Color Interrupt Mask
     enable |= tcs_colorinterrupt;
    
     // Set the Wait Enable
     enable |= tcs_waitenable;
     
     // Set the Color Enable
     enable |= tcs_colorinable;
     
     // Set the Power ON
     enable |= tcs_powerenable;
    */
    
    uint8_t enable =    TCS34903_REG_COLOR_ENABLE_SAI_NOT_ASSERT       |   // Not Asserted, the Device will not Power Down at the End of a RGBC Cycle if an Interrupt has been Generated
                        TCS34903_REG_COLOR_ENABLE_AIEN_NOT_ASSERT      |   // Not Asserted, Did not Permits ALS/Color Interrupts to be Generated
                        TCS34903_REG_COLOR_ENABLE_WEN_ENABLE           |   // Enables the Wait Timer
                        TCS34903_REG_COLOR_ENABLE_AEN_ENABLE           |   // Activates the ALS/Color ADC
                        TCS34903_REG_COLOR_ENABLE_PON_ENABLE;              // Activates the Internal Oscillator to Permit the Timers and ADC Channels to Operate
    
    // Write the configuration to the Color Light-to-Digital Converter Enable Register
    writeRegister(tcs_i2cAddress, TCS34903_REG_COLOR_ENABLE, enable);
    
    // Wait for the configuration to complete
    delay(tcs_conversionDelay);
    
    // Set Up the Configuration for the Color Light-to-Digital Converter ATIME Register
    // Set the Internal Integration Time of the ALS Channel ADC’s in 2.78 ms Increments
    uint8_t atime = tcs_atime;
    
    // Write the configuration to the Color Light-to-Digital Converter ATIME Register
    writeRegister(tcs_i2cAddress, TCS34903_REG_COLOR_ATIME, atime);
    
    // Wait for the configuration to complete
    delay(tcs_conversionDelay);
    
    // Set Up the Configuration for the Color Light-to-Digital Converter WTIME Register
    // Set the Wait Time is Set in 2.78 ms Increments Unless the WLONG Bit is Asserted in Which Case the wait Times are 12x Longer
    uint8_t wtime = tcs_wtime;
    
    // Write the configuration to the Color Light-to-Digital Converter WTIME Register
    writeRegister(tcs_i2cAddress, TCS34903_REG_COLOR_WTIME, wtime);
    
    // Wait for the configuration to complete
    delay(tcs_conversionDelay);
    
    // Set Up the Configuration for the Color Light-to-Digital Converter Configuration Register
    /*
     // Set the Wait Long
     uint8_t config = tcs_waitlong;
    */
    
    uint8_t config = TCS34903_REG_COLOR_CONFIG_WLONG_NOT_ASSERT;  // Not Asserted, the Wait Cycles are Not Increased by a Factor 12X from that Programmed in the WTIME Register
    
    // Write the configuration to the Color Light-to-Digital Converter Configuration Register
    writeRegister(tcs_i2cAddress, TCS34903_REG_COLOR_CONFIG, config);
    
    // Wait for the configuration to complete
    delay(tcs_conversionDelay);
    
    // Set Up the Configuration for the Color Light-to-Digital Converter Control Register
    // ALS Gain
    uint8_t control = tcs_colorgain;
    
    // Write the configuration to the Color Light-to-Digital Converter Control Register
    writeRegister(tcs_i2cAddress, TCS34903_REG_COLOR_CONTROL, control);
    
    // Wait for the configuration to complete
    delay(tcs_conversionDelay);
    
    // Set Up the Configuration for the Color Light-to-Digital Converter IR Register
    /*
     // Set the IR Sensor Access
     uint8_t iraccess = tcs_iraccess;
     */
    
    uint8_t iraccess = TCS34903_REG_COLOR_IR_ACCESS_SET;  // Clear Channel Reports the Measurement from the IR Sensor
    
    // Write the configuration to the Color Light-to-Digital Converter IR Register
    writeRegister(tcs_i2cAddress, TCS34903_REG_COLOR_IR, iraccess);
    
    // Wait for the configuration to complete
    delay(tcs_conversionDelay);
}
/**************************************************************************/
/*
        Reads up the Color Light-to-Digital Converter
*/
/**************************************************************************/
void TCS34903::Measure_Sensor(void)
{
    // Read all the Sensors
    // RGBC Data
    Measure_Color();
}

/**************************************************************************/
/*
        Reads the Color Light-to-Digital Converter
*/
/**************************************************************************/
void TCS34903::Measure_Color()
{
    uint8_t CdataLo, CdataHi, RdataLo, RdataHi, GdataLo, GdataHi, BdataLo, BdataHi;
    uint16_t CData, RData, GData, BData;
    
    // Read the Data
    // Reading the Clear/IR Data Low Register
    CdataLo = readRegister(tcs_i2cAddress, TCS34903_REG_COLOR_CDATAL);
    // Reading the Clear/IR Data High Register
    CdataHi = readRegister(tcs_i2cAddress, TCS34903_REG_COLOR_CDATAH);
    // Conversion of the result
    // 16-bit unsigned result for Clear/IR Data of TCS34903
    CData = (uint16_t)((CdataHi << 8) | CdataLo);
    
    // Reading the Red Data Low Register
    RdataLo = readRegister(tcs_i2cAddress, TCS34903_REG_COLOR_RDATAL);
    // Reading the Red Data High Register
    RdataHi = readRegister(tcs_i2cAddress, TCS34903_REG_COLOR_RDATAH);
    // Conversion of the result
    // 16-bit unsigned result for Red Data of TCS34903
    RData = (uint16_t)((RdataHi << 8) | RdataLo);
    
    // Reading the Green Data Low Register
    GdataLo = readRegister(tcs_i2cAddress, TCS34903_REG_COLOR_GDATAL);
    // Reading the Green Data High Register
    GdataHi = readRegister(tcs_i2cAddress, TCS34903_REG_COLOR_GDATAH);
    // Conversion of the result
    // 16-bit unsigned result for Green Data of TCS34903
    GData = (uint16_t)((GdataHi << 8) | GdataLo);
    
    // Reading the Blue Data Low Register
    BdataLo = readRegister(tcs_i2cAddress, TCS34903_REG_COLOR_BDATAL);
    // Reading the Blue Data High Register
    BdataHi = readRegister(tcs_i2cAddress, TCS34903_REG_COLOR_BDATAH);
    // Conversion of the result
    // 16-bit unsigned result for Blue Data of TCS34903
    BData = (uint16_t)((BdataHi << 8) | BdataLo);
    
    tcs_colorData.C = CData;
    tcs_colorData.R = RData;
    tcs_colorData.G = GData;
    tcs_colorData.B = BData;
}
