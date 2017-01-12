/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        SI7015
        This code is designed to work with the SI7015_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=SI7015_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "SI7015.h"

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
        Reads 8-bits from the specified destination register
*/
/**************************************************************************/
static uint8_t readRegister8(uint8_t i2cAddress, uint8_t reg)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)1);
    return (uint8_t)(i2cread());
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
    return (uint16_t)((i2cread() << 8) | i2cread());
}

/**************************************************************************/
/*
        Instantiates a new SI7015 class with appropriate properties
*/
/**************************************************************************/
void SI7015::getAddr_SI7015(uint8_t i2cAddress)
{
    si_i2cAddress = i2cAddress;
    si_conversionDelay = SI7015_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
boolean SI7015::begin()
{
    Wire.begin();
    if (readRegister8(si_i2cAddress, SI7015_REG_CONTROL_ID) != 0x50) return false;
    
    return true;
}

/**************************************************************************/
/*
        Sets the Fast Mode Enable
*/
/**************************************************************************/
void SI7015::setFastEnable(siFastEnable_t fastenable)
{
    si_fastenable = fastenable;
}

/**************************************************************************/
/*
        Gets the Fast Mode Enable
*/
/**************************************************************************/
siFastEnable_t SI7015::getFastEnable()
{
    return si_fastenable;
}

/**************************************************************************/
/*
        Sets the Heater Enable
*/
/**************************************************************************/
void SI7015::setHeaterEnable(siHeaterEnable_t heaterenable)
{
    si_heaterenable = heaterenable;
}

/**************************************************************************/
/*
        Gets the Heater Enable
*/
/**************************************************************************/
siHeaterEnable_t SI7015::getHeaterEnable()
{
    return si_heaterenable;
}

/**************************************************************************/
/*
        Sets the Conversion Start
*/
/**************************************************************************/
void SI7015::setConversion(siConversion_t conversion)
{
    si_conversion = conversion;
}

/**************************************************************************/
/*
        Gets the Conversion Start
*/
/**************************************************************************/
siConversion_t SI7015::getConversion()
{
    return si_conversion;
}

/**************************************************************************/
/*
        Setup the Config Register, Getting  16-bit rawdata from the registers
*/
/**************************************************************************/
uint16_t SI7015::getMeasurement(uint8_t configValue)
{
    uint16_t rawData;
    uint8_t status;
    
    // Start with default values
    // Fast Mode Enable
    uint8_t config = si_fastenable;
    
    // Set Heater Mode
    config |= si_heaterenable;
    
    // Set Conversion Mode
    config |= si_conversion;

    // Setup config register
    writeRegister(si_i2cAddress, SI7015_REG_CONTROL_CONFIG, config | configValue);
    
    // Wait for the conversion to complete
    delay(si_conversionDelay);

    status = SI7015_REG_STATUS_NOT_READY;

    // Returns 0 on failure which is same as "ready" status
    // No timeout if device never gets to "ready" status
    // Wait for the measurement to finish
    while (status & SI7015_REG_STATUS_NOT_READY)
    {
        status = readRegister8(si_i2cAddress, SI7015_REG_CONTROL_STATUS);
    }

    // Read the conversion results
    // 16-bit unsigned results from the SI7015
    rawData = readRegister(si_i2cAddress, SI7015_REG_CONTROL_DATA_HI);
    return (uint16_t)rawData;
}

float SI7015::Measure_Temperature()
{
    uint16_t rawTemperature = getMeasurement(SI7015_REG_CONFIG_ENABLE_TEMPERATURE);
    rawTemperature = ((rawTemperature >> 2) & 0xFFFF);
    last_temperature = ((float)rawTemperature) / SI7015_TEMPERATURE_SLOPE - SI7015_TEMPERATURE_OFFSET;
    return (float)(last_temperature + 0.5);
}

float SI7015::Measure_Humidity()
{
    uint16_t rawHumidity = getMeasurement(SI7015_REG_CONFIG_ENABLE_HUMIDITY);
    rawHumidity = ((rawHumidity >> 4) & 0xFFFF);
    float linearHumidity = ((float)rawHumidity) / SI7015_HUMIDITY_SLOPE - SI7015_HUMIDITY_OFFSET;
    linearHumidity -= A2 * linearHumidity * linearHumidity + A1 * linearHumidity + A0;
    linearHumidity += ( last_temperature - 30 ) * ( Q1 * linearHumidity + Q0 );
    return (float)(linearHumidity + 0.5);
}
