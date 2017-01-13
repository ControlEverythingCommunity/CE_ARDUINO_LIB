/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ISL29003
        This code is designed to work with the ISL29003_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Light?sku=ISL29003_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "ISL29003.h"

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
        Instantiates a new ISL29003 class with appropriate properties
*/
/**************************************************************************/
void ISL29003::getAddr_ISL29003(uint8_t i2cAddress)
{
    isl_i2cAddress = i2cAddress;
    isl_conversionDelay = ISL29003_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void ISL29003::begin()
{
    Wire.begin();
}

/**************************************************************************/
/*
        Sets the ADC Core Output
*/
/**************************************************************************/
void ISL29003::setADCCore(islADCCore_t core)
{
    isl_adccore = core;
}

/**************************************************************************/
/*
        Gets the ADC Core Output
*/
/**************************************************************************/
islADCCore_t ISL29003::getADCCore()
{
    return isl_adccore;
}

/**************************************************************************/
/*
        Sets the ADC Power Down Mode
*/
/**************************************************************************/
void ISL29003::setADCPD(islADCPD_t power)
{
    isl_adcpd = power;
}

/**************************************************************************/
/*
        Gets the ADC Power Down Mode
*/
/**************************************************************************/
islADCPD_t ISL29003::getADCPD()
{
    return isl_adcpd;
}

/**************************************************************************/
/*
        Sets the Timing Mode
*/
/**************************************************************************/
void ISL29003::setTMode(islTMode_t tmode)
{
    isl_tmode = tmode;
}

/**************************************************************************/
/*
        Gets the Timing Mode
*/
/**************************************************************************/
islTMode_t ISL29003::getTMode()
{
    return isl_tmode;
}

/**************************************************************************/
/*
        Sets the ADC Work Mode
*/
/**************************************************************************/
void ISL29003::setADCMode(islADCMode_t adcmode)
{
    isl_adcmode = adcmode;
}

/**************************************************************************/
/*
        Gets the ADC Work Mode
*/
/**************************************************************************/
islADCMode_t ISL29003::getADCMode()
{
    return isl_adcmode;
}

/**************************************************************************/
/*
        Sets the Interrupt Flag Status
*/
/**************************************************************************/
void ISL29003::setIntrFlag(islIntrFlag_t flag)
{
    isl_intrflag = flag;
}

/**************************************************************************/
/*
        Gets the Interrupt Flag Status
*/
/**************************************************************************/
islIntrFlag_t ISL29003::getIntrFlag()
{
    return isl_intrflag;
}

/**************************************************************************/
/*
        Sets the ADC Gain Range
*/
/**************************************************************************/
void ISL29003::setADCGain(islADCGain_t gain)
{
    isl_adcgain = gain;
}

/**************************************************************************/
/*
        Gets the ADC Gain Range
*/
/**************************************************************************/
islADCGain_t ISL29003::getADCGain()
{
    return isl_adcgain;
}

/**************************************************************************/
/*
        Sets the Interrupt Persist Time
*/
/**************************************************************************/
void ISL29003::setIntrCycle(islIntrCycle_t cycle)
{
    isl_intrcycle = cycle;
}

/**************************************************************************/
/*
        Gets the Interrupt Persist Time
*/
/**************************************************************************/
islIntrCycle_t ISL29003::getIntrCycle()
{
    return isl_intrcycle;
}

/**************************************************************************/
/*
        Reads the results, measuring the 16-bit Light register
*/
/**************************************************************************/
uint16_t ISL29003::Measure_Light()
{
    // Start with default values
    uint8_t command = ISL29003_REG_CAPABL_TMODE_INT;       // Integration is Internally Timed

    // Set the ADC Core Output
    command |= isl_adccore;
    
    // Set the ADC Power Down Mode
    command |= isl_adcpd;
    
    // Set the ADC Power Work Mode
    command |= isl_adcmode;
    
    // Write command register to the device
    writeRegister(isl_i2cAddress, ISL29003_REG_COMMAND, command);
    
    // Wait for the conversion to complete
    delay(isl_conversionDelay);
    
    uint8_t control = ISL29003_REG_CONTROL_INTRFLAG_NOTRIG;   // Interrupt is cleared or not yet Triggered
    
    // Set the ADC Gain Range
    control |= isl_adcgain;
    
    // Set the Interrupt Persist Time
    control |= isl_intrcycle;

    // Write config register to the isl
    writeRegister(isl_i2cAddress, ISL29003_REG_CONTROL, control);

    // Wait for the conversion to complete
    delay(isl_conversionDelay);

    // Read the conversion results
    // 16-bit signed results for the ISL29003
    uint16_t raw_light = readRegister(isl_i2cAddress, ISL29003_REG_SENSOR_LSB);
    return (uint16_t)raw_light;
}
