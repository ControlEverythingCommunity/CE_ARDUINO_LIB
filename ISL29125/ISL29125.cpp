/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ISL29125
        This code is designed to work with the ISL29125_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/isl29125-digital-rgb-color-light-sensor-with-ir-blocking-filter
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "ISL29125.h"

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
        Reads 16-bits from the specified destination register
*/
/**************************************************************************/
static uint16_t readRegister(uint8_t i2cAddress, uint8_t reg)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)2);
    return (uint16_t)(i2cread() | (i2cread() << 8));
}

/**************************************************************************/
/*
        Instantiates a new ISL29125 class with appropriate properties
*/
/**************************************************************************/
void ISL29125::getAddr_ISL29125(uint8_t i2cAddress)
{
    isl_i2cAddress = i2cAddress;
    isl_conversionDelay = ISL29125_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void ISL29125::begin()
{
    Wire.begin();
}

/**************************************************************************/
/*
        Sets the SYNC Mode
*/
/**************************************************************************/
void ISL29125::setSync(islSync_t sync)
{
    isl_sync = sync;
}

/**************************************************************************/
/*
        Gets the SYNC Mode
*/
/**************************************************************************/
islSync_t ISL29125::getSync()
{
    return isl_sync;
}

/**************************************************************************/
/*
        Sets the ADC Resolution
*/
/**************************************************************************/
void ISL29125::setResolution(islResolution_t resolution)
{
    isl_resolution = resolution;
}

/**************************************************************************/
/*
        Gets the ADC Resolution
*/
/**************************************************************************/
islResolution_t ISL29125::getResolution()
{
    return isl_resolution;
}

/**************************************************************************/
/*
        Sets the RGB Data Sensing Range
*/
/**************************************************************************/
void ISL29125::setRange(islRange_t range)
{
    isl_range = range;
}

/**************************************************************************/
/*
        Gets the RGB Data Sensing Range
*/
/**************************************************************************/
islRange_t ISL29125::getRange()
{
    return isl_range;
}

/**************************************************************************/
/*
        Sets the RGB Operating Modes
*/
/**************************************************************************/
void ISL29125::setMode(islMode_t mode)
{
    isl_mode = mode;
}

/**************************************************************************/
/*
        Gets the RGB Operating Modes
*/
/**************************************************************************/
islMode_t ISL29125::getMode()
{
    return isl_mode;
}

/**************************************************************************/
/*
        Sets the Active Infrared (IR) Compensation Offset
*/
/**************************************************************************/
void ISL29125::setIROffset(islIROffset_t iroffset)
{
    isl_iroffset = iroffset;
}

/**************************************************************************/
/*
        Gets the Active Infrared (IR) Compensation Offset
*/
/**************************************************************************/
islIROffset_t ISL29125::getIROffset()
{
    return isl_iroffset;
}

/**************************************************************************/
/*
        Sets the Active Infrared (IR) Compensation Adjust
*/
/**************************************************************************/
void ISL29125::setIRAdjust(islIRAdjust_t iradjust)
{
    isl_iradjust = iradjust;
}

/**************************************************************************/
/*
        Gets the Active Infrared (IR) Compensation Adjust
*/
/**************************************************************************/
islIRAdjust_t ISL29125::getIRAdjust()
{
    return isl_iradjust;
}

/**************************************************************************/
/*
        Sets the RGB Conversion Done to INT Control
*/
/**************************************************************************/
void ISL29125::setConversion(islConversion_t conversion)
{
    isl_conversion = conversion;
}

/**************************************************************************/
/*
        Gets the RGB Conversion Done to INT Control
*/
/**************************************************************************/
islConversion_t ISL29125::getConversion()
{
    return isl_conversion;
}

/**************************************************************************/
/*
        Sets the Interrupt Persist Control
*/
/**************************************************************************/
void ISL29125::setIntegrationCycle(islIntegrationCycle_t integrationcycle)
{
    isl_integrationcycle = integrationcycle;
}

/**************************************************************************/
/*
        Gets the Interrupt Persist Control
*/
/**************************************************************************/
islIntegrationCycle_t ISL29125::getIntegrationCycle()
{
    return isl_integrationcycle;
}

/**************************************************************************/
/*
        Sets the Interrupt Threshold Assignment
*/
/**************************************************************************/
void ISL29125::setInterrupt(islInterrupt_t interrupt)
{
    isl_interrupt = interrupt;
}

/**************************************************************************/
/*
        Gets the Interrupt Threshold Assignment
*/
/**************************************************************************/
islInterrupt_t ISL29125::getInterrupt()
{
    return isl_interrupt;
}

/**************************************************************************/
/*
    Sets up the Digital RGB Color Light Sensor with IR Blocking Filter
*/
/**************************************************************************/
void ISL29125::setUpColor()
{
    // Set Up the Configuration for the Configuration Register 1
    /*
     // Set the SYNC Mode
     uint8_t config1 = isl_sync;
     
     // Set the ADC Resolution
     config1 |= isl_resolution;
     
     // Set the RGB Data Sensing Range
     config1 |= isl_range;
     
     // Set the RGB Data Sensing Range
     config1 |= isl_mode;
    */
    
    uint8_t config1 = ISL29125_REG_CONFIG1_SYNC_NORMAL;     // INT pin Sets to Normal Mode
    
    // Set the ADC Resolution
    config1 |= isl_resolution;
    
    // Set the RGB Data Sensing Range
    config1 |= isl_range;
    
    // Set the RGB Data Sensing Range
    config1 |= isl_mode;
    
    // Write the configuration to the Configuration Register 1
    writeRegister(isl_i2cAddress, ISL29125_REG_CONFIG_1, config1);
    
    // Wait for the configuration to complete
    delay(isl_conversionDelay);
    
    // Set Up the Configuration for the Configuration Register 2
    /*
     // Set the Active Infrared (IR) Compensation Offset
     uint8_t config2 = isl_iroffset;
     
     // Set the Active Infrared (IR) Compensation Adjust
     config2 |= isl_iradjust;
    */
    
    uint8_t config2 =   ISL29125_REG_CONFIG2_IR_OFFSET_OFF      |   // INT pin Sets to Normal Mode
                        ISL29125_REG_CONFIG2_IR_ADJUST_LOW;         // INT pin Sets to Normal Mode
    
    // Write the configuration to the Configuration Register 2
    writeRegister(isl_i2cAddress, ISL29125_REG_CONFIG_2, config2);
    
    // Wait for the configuration to complete
    delay(isl_conversionDelay);
    
    // Set Up the Configuration for the Configuration Register 3
    /*
     // Set the RGB Conversion Done to INT Control
     uint8_t config3 = isl_conversion;
     
     // Set the Interrupt Persist Control
     config3 |= isl_integrationcycle;
     
     // Set the Interrupt Threshold Assignment
     config3 |= isl_interrupt;
    */
    
    uint8_t config3 =   ISL29125_REG_CONFIG3_CONVEN_DISABLE     |   // Disable
                        ISL29125_REG_CONFIG3_PRST_1             |   // No. of Integration Cycle: 1
                        ISL29125_REG_CONFIG3_INTSEL_NO_INT;         // No Interrupt
    
    // Write the configuration to the Configuration Register 3
    writeRegister(isl_i2cAddress, ISL29125_REG_CONFIG_3, config3);
    
    // Wait for the configuration to complete
    delay(isl_conversionDelay);
}

/**************************************************************************/
/*
        Reads up the Digital RGB Color Light Sensor with IR Blocking Filter
*/
/**************************************************************************/
void ISL29125::Measure_Sensor(void)
{
    // Read all the Sensors
    // RGB Data
    Measure_Color();
}

/**************************************************************************/
/*
        Reads the Digital RGB Color Light Sensor with IR Blocking Filter
*/
/**************************************************************************/
void ISL29125::Measure_Color()
{
    uint8_t GdataLo, GdataHi, RdataLo, RdataHi, BdataLo, BdataHi;
    uint16_t GData, RData, BData;
    
    // Reading the Green Data Low Register
    GdataLo = readRegister(isl_i2cAddress, ISL29125_REG_GREEN_LOW);
    // Reading the Green Data High Register
    GdataHi = readRegister(isl_i2cAddress, ISL29125_REG_GREEN_HIGH);
    // Conversion of the result
    // 16-bit unsigned result for Green Data of ISL29125
    GData = (uint16_t)((GdataHi << 8) | GdataLo);
    
    // Reading the Red Data Low Register
    RdataLo = readRegister(isl_i2cAddress, ISL29125_REG_RED_LOW);
    // Reading the Red Data High Register
    RdataHi = readRegister(isl_i2cAddress, ISL29125_REG_RED_HIGH);
    // Conversion of the result
    // 16-bit unsigned result for Red Data of ISL29125
    RData = (uint16_t)((RdataHi << 8) | RdataLo);
    
    // Reading the Blue Data Low Register
    BdataLo = readRegister(isl_i2cAddress, ISL29125_REG_BLUE_LOW);
    // Reading the Blue Data High Register
    BdataHi = readRegister(isl_i2cAddress, ISL29125_REG_BLUE_HIGH);
    // Conversion of the result
    // 16-bit unsigned result for Blue Data of ISL29125
    BData = (uint16_t)((BdataHi << 8) | BdataLo);
    
    isl_colorData.G = GData;
    isl_colorData.R = RData;
    isl_colorData.B = BData;
}
