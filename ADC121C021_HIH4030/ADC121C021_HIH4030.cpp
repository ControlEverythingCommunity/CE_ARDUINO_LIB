/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ADC121C021_HIH-4030
        This code is designed to work with the ADC121C021_I2CS_HIH-4030 I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "ADC121C021_HIH4030.h"

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
        Reads 16-bits from the specified destination register
*/
/**************************************************************************/
static uint16_t readRegister(uint8_t i2cAddress, uint8_t reg)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)2);
    return (int16_t)((i2cread() << 8) | i2cread());
}

/**************************************************************************/
/*
        Instantiates a new ADC121C021_HIH4030 class with appropriate properties
*/
/**************************************************************************/
void ADC121C021_HIH4030::getAddr_ADC121C021_HIH4030(uint8_t i2cAddress)
{
    adc_i2cAddress = i2cAddress;
    adc_conversionDelay = ADC121C021_HIH4030_CONVERSIONDELAY;
    adc_staticTemperature = ADC121C021_HIH4030_STATIC_TEMPERATURE;
    adc_supplyVoltage = ADC121C021_HIH4030_SUPPLY_VOLTAGE;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void ADC121C021_HIH4030::begin()
{
    Wire.begin();
}

/**************************************************************************/
/*
        Reads the result of the most recent conversion. 
        In the normal mode, a new conversion is started whenever this register is read
        And then convert to voltage value
*/
/**************************************************************************/
float ADC121C021_HIH4030::Measure_Voltage()
{
    // Read the conversion result
    // 12-bit unsigned result for the ADC121C021_HIH4030
    uint16_t raw_voltage = readRegister(adc_i2cAddress, ADC121C021_HIH4030_REG_CONVERSION);
    float vout = (raw_voltage * adc_supplyVoltage) / 4095.0;
    return (float)vout;
}

/**************************************************************************/
/*
        Gets the converted sensor reading into Relative Humidity (RH%) 
        using equation from Datasheet
        Voltage output (1st order curve fit)
        VOUT = (VSUPPLY)(0.0062(sensor RH) + 0.16), typical at 25 ºC
*/
/**************************************************************************/
float ADC121C021_HIH4030::getSensorRH()
{
    return (float)((Measure_Voltage() / (0.0062 * adc_supplyVoltage)) - 25.81);
}

/**************************************************************************/
/*
        Get True Relative Humidity (RH%) compensated with Static Temperature 
        or Measured Temperature using equation from Datasheet
        Temperature Compensation
        True RH = (Sensor RH) / (1.0546 – 0.00216T), T in ºC
*/
/**************************************************************************/
float ADC121C021_HIH4030::getTrueRH()
{
    return (float)(getSensorRH() / (1.0546 - (0.00216 * adc_staticTemperature)));
}
