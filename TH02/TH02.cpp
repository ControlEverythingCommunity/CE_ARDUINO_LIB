/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TH02
        This code is designed to work with the TH02_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "TH02.h"

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
        Reads first 8-bits from the specified destination register
*/
/**************************************************************************/
static uint8_t readRegister8(uint8_t i2cAddress, uint8_t reg)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)3);
    return (uint8_t)(i2cread());
}

/**************************************************************************/
/*
        Reads last 16-bits from the specified destination register
*/
/**************************************************************************/
static uint16_t readRegister(uint8_t i2cAddress, uint8_t reg)
{
    uint8_t data[3];
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)3);
    data[0] = i2cread();
    data[1] = i2cread();
    data[2] = i2cread();
    return (uint16_t)((data[1] << 8) | data[2]);
}

/**************************************************************************/
/*
        Instantiates a new TH02 class with appropriate properties
*/
/**************************************************************************/
void TH02::getAddr_TH02(uint8_t i2cAddress)
{
    th_i2cAddress = i2cAddress;
    th_conversionDelay = TH02_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
boolean TH02::begin()
{
    Wire.begin();
    if (readRegister8(th_i2cAddress, TH02_REG_CONTROL_ID) != 0x50) return false;
    
    return true;
}

/**************************************************************************/
/*
        Sets the Fast Mode Enable
*/
/**************************************************************************/
void TH02::setFastEnable(thFastEnable_t fastenable)
{
    th_fastenable = fastenable;
}

/**************************************************************************/
/*
        Gets the Fast Mode Enable
*/
/**************************************************************************/
thFastEnable_t TH02::getFastEnable()
{
    return th_fastenable;
}

/**************************************************************************/
/*
        Sets the Heater Enable
*/
/**************************************************************************/
void TH02::setHeaterEnable(thHeaterEnable_t heaterenable)
{
    th_heaterenable = heaterenable;
}

/**************************************************************************/
/*
        Gets the Heater Enable
*/
/**************************************************************************/
thHeaterEnable_t TH02::getHeaterEnable()
{
    return th_heaterenable;
}

/**************************************************************************/
/*
        Sets the Conversion Start
*/
/**************************************************************************/
void TH02::setConversion(thConversion_t conversion)
{
    th_conversion = conversion;
}

/**************************************************************************/
/*
        Gets the Conversion Start
*/
/**************************************************************************/
thConversion_t TH02::getConversion()
{
    return th_conversion;
}

/**************************************************************************/
/*
        Setup the Config Register, Getting  16-bit rawdata from the registers
*/
/**************************************************************************/
uint16_t TH02::getMeasurement(uint8_t configValue)
{
    uint16_t rawData;
    uint8_t status;
    
    // Start with default values
    // Fast Mode Enable
    uint8_t config = th_fastenable;
    
    // Set Heater Mode
    config |= th_heaterenable;
    
    // Set Conversion Mode
    config |= th_conversion;

    // Setup config register
    writeRegister(th_i2cAddress, TH02_REG_CONTROL_CONFIG, config | configValue);
    
    // Wait for the conversion to complete
    delay(th_conversionDelay);

    status = TH02_REG_STATUS_NOT_READY;

    // Returns 0 on failure which is same as "ready" status
    // No timeout if device never gets to "ready" status
    // Wait for the measurement to finish
    while (status & TH02_REG_STATUS_NOT_READY)
    {
        status = readRegister8(th_i2cAddress, TH02_REG_CONTROL_STATUS);
    }

    // Read the conversion results
    // 16-bit unsigned results from the TH02
    rawData = readRegister(th_i2cAddress, TH02_REG_CONTROL_STATUS);
    return (uint16_t)rawData;
}

float TH02::Measure_Temperature()
{
    uint16_t rawTemperature = getMeasurement(TH02_REG_CONFIG_ENABLE_TEMPERATURE);
    rawTemperature = ((rawTemperature >> 2) & 0xFFFF);
    last_temperature = ((float)rawTemperature) / TH02_TEMPERATURE_SLOPE - TH02_TEMPERATURE_OFFSET;
    return (float)(last_temperature + 0.5);
}

float TH02::Measure_Humidity()
{
    uint16_t rawHumidity = getMeasurement(TH02_REG_CONFIG_ENABLE_HUMIDITY);
    rawHumidity = ((rawHumidity >> 4) & 0xFFFF);
    float linearHumidity = ((float)rawHumidity) / TH02_HUMIDITY_SLOPE - TH02_HUMIDITY_OFFSET;
    linearHumidity -= A2 * linearHumidity * linearHumidity + A1 * linearHumidity + A0;
    linearHumidity += ( last_temperature - 30 ) * ( Q1 * linearHumidity + Q0 );
    return (float)(linearHumidity + 0.5);
}
