/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MS5611-01BXXX
        This code is designed to work with the MS5611-01BXXX_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/variometer-10-to-1200-mbar?variant=25687736203#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "MS5611_01BXXX.h"

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
        Writes 8-bits to the destination register
*/
/**************************************************************************/
static void writeRegister8(uint8_t i2cAddress, uint8_t reg)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    Wire.endTransmission();
}

/**************************************************************************/
/*
        Reads 16-bits from the specified destination register
*/
/**************************************************************************/
static uint16_t readRegister16(uint8_t i2cAddress, uint8_t reg)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)2);
    return (int16_t)((i2cread() << 8) | i2cread());
}

/**************************************************************************/
/*
    Reads 24-bits from the specified destination register
*/
/**************************************************************************/
static uint32_t readRegister24(uint8_t i2cAddress, uint8_t reg)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)3);
    return (uint32_t)((int32_t)i2cread() << 16) | ((int32_t)i2cread() << 8) | i2cread();
}

/**************************************************************************/
/*
        Instantiates a new MS5611-01BXXX class with appropriate properties
*/
/**************************************************************************/
void MS5611_01BXXX::getAddr_MS5611_01BXXX(uint8_t i2cAddress)
{
    ms_i2cAddress = i2cAddress;
    ms_conversionDelay = MS5611_01BXXX_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool MS5611_01BXXX::begin()
{
    Wire.begin();
    reset();
    delay(ms_conversionDelay);
    initialize();
    return true;
}

/**************************************************************************/
/*
        Sets the Reset Command
        The Reset sequence shall be sent once after power-on to make sure 
        that the calibration PROM gets loaded into the internal register
*/
/**************************************************************************/
void MS5611_01BXXX::reset()
{
    writeRegister8(ms_i2cAddress, MS5611_01BXXX_CMD_RESET);
    delay(ms_conversionDelay);
}

/**************************************************************************/
/*
        Sets the OSR command value (typ=D1) to intiate the pressure conversion
*/
/**************************************************************************/
void MS5611_01BXXX::setinitiatePressure(msD1OSR_t d1osr)
{
    ms_d1osr = d1osr;
}

/**************************************************************************/
/*
        Gets the OSR command value (typ=D1) to intiate the pressure conversion
 */
/**************************************************************************/
msD1OSR_t MS5611_01BXXX::getinitiatePressure()
{
    return ms_d1osr;
}

/**************************************************************************/
/*
        Sets the OSR command value (typ=D2) to intiate the temperature conversion
 */
/**************************************************************************/
void MS5611_01BXXX::setinitiateTemperature(msD2OSR_t d2osr)
{
    ms_d2osr = d2osr;
}

/**************************************************************************/
/*
        Gets the OSR command value (typ=D2) to intiate the temperature conversion
 */
/**************************************************************************/
msD2OSR_t MS5611_01BXXX::getinitiateTemperature()
{
    return ms_d2osr;
}

/**************************************************************************/
/*
        Power on and prepare for general usage
        Reads Calibration Data (coefficients that are factory calibrated) stored in PROM
*
/**************************************************************************/
void MS5611_01BXXX::initialize()
{
    // Reading 6 Calibration Data Values
    // Read cofficients values stored in EPROM of the device
    // Pressure sensitivity, SENST1
    // Pressure offset, OFFT1
    // Temperature coefficient of pressure sensitivity, TCS
    // Temperature coefficient of pressure offset, TCO
    // Reference temperature, TREF
    // Temperature coefficient of the temperature, TEMPSENS
    
    for (uint8_t offset = 0; offset < 6; offset++)
    {
        C[offset] = readRegister16(ms_i2cAddress, MS5611_01BXXX_CMD_C1 + (offset * 2));
    }
}

/**************************************************************************/
/*
        Reads 24-bits from the destination register
        Reads the results for Digital Pressure Value (D1)
*/
/**************************************************************************/
uint32_t MS5611_01BXXX::readPressure()
{
    uint8_t pressure_osr = ms_d1osr;
    // Initiate the process of pressure measurement
    writeRegister8(ms_i2cAddress, pressure_osr);
    // Waiting for pressure data ready
    delay(ms_conversionDelay);
    
    // Reads the pressure value
    return readRegister24(ms_i2cAddress, MS5611_01BXXX_CMD_ADC);
}

/**************************************************************************/
/*
        Reads 24-bits from the destination register
        Reads the results for Digital Temperarure Value (D2)
*/
/**************************************************************************/
uint32_t MS5611_01BXXX::readTemperature()
{
    uint8_t temperature_osr = ms_d2osr;
    // Initiate the process of temperarure measurement
    writeRegister8(ms_i2cAddress, temperature_osr);
    // Waiting for temperarure data ready
    delay(ms_conversionDelay);
    
    // Reads the temperarure value
   return readRegister24(ms_i2cAddress, MS5611_01BXXX_CMD_ADC);

}

/**************************************************************************/
/* 
        Calculate Pressure Calculations and perform compensation
        More info about these calculations is available in the datasheet
*/
/**************************************************************************/
double MS5611_01BXXX::Measure_Pressure(void)
{
    uint32_t D1 = readPressure();
    
    uint32_t D2 = readTemperature();
    
    // Difference between actual and reference temperature
    int32_t dT = D2 - (uint32_t)C[4] * pow(2, 8);

    // Actual temperature (-40…85°C with 0.01°C resolution)
    int32_t TEMP = 2000 + ((int64_t) dT * C[5]) / pow(2, 23);
    // Offset at actual temperature
    int64_t OFF = (int64_t)C[1] * pow(2, 16) + (int64_t)C[3] * dT / pow(2, 7);
    // Sensitivity at actual temperature
    int64_t SENS = (int64_t)C[0] * pow(2, 15) + (int64_t)C[2] * dT / pow(2, 8);
    
    // Second Order Temperature Compensation
        
    if (TEMP >= 2000)
    {
        // High temperature
        OFF2 = 0;
        SENS2 = 0;
    }
    else if (TEMP < 2000)
    {
        // Low Temperature
        OFF2 = 5 * (pow(TEMP - 2000, 2)) / 2;
        SENS2 = 5 * (pow(TEMP - 2000, 2)) / 4;
    }
    else if (TEMP < -1500)
    {
        // Very Low Temperature
        OFF2 = OFF2 + 7 * (pow(TEMP + 1500, 2));
        SENS2 = SENS2 + 11 * (pow(TEMP + 1500, 2)) / 2;
    }
    
    // Calculate pressure
    OFF = OFF - OFF2;
    SENS = SENS - SENS2;
    
    // Final calculations
    // Temperature Compensated Pressure (10…1200mbar with 0.01mbar resolution)
    uint32_t PRES = ((D1 * SENS) / pow(2, 21) - OFF) / pow(2, 15) / 100.0;
    return ((double)PRES);
}

/**************************************************************************/
/*
        Calculate Temperature calculations and perform compensation
        More info about these calculations is available in the datasheet
*/
/**************************************************************************/
double MS5611_01BXXX::Measure_Temperature(void)
{
    uint32_t D2 = readTemperature();
    
    // Difference between actual and reference temperature
    int32_t dT = D2 - (uint32_t)C[4] * pow(2, 8);
    
    // Actual temperature (-40…85°C with 0.01°C resolution)
    int32_t TEMP = 2000 + ((int64_t) dT * C[5]) / pow(2, 23);
    
    T2 = 0;
    
    if (TEMP < 2000)
    {
        T2 = (pow(dT, 2)) / pow(2, 31);
    }
    
    TEMP = TEMP - T2;
    
    // Compensated Temperature
    TEMP = TEMP / 100.0;
    return ((double)TEMP);
}
