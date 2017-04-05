/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MPL115A2
        This code is designed to work with the MPL115A2_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/collections/barometers/products/mpl115a2-digital-barometer-500-to-1150-mbar
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "MPL115A2.h"

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
static void writeRegister8(uint8_t i2cAddress, uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    i2cwrite((uint8_t)value);
    Wire.endTransmission();
}

/**************************************************************************/
/*
        Instantiates a new MPL115A2 class with appropriate properties
*/
/**************************************************************************/
void MPL115A2::getAddr_MPL115A2(uint8_t i2cAddress)
{
    mpl_i2cAddress = i2cAddress;
    mpl_conversionDelay = MPL115A2_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool MPL115A2::begin()
{
    Wire.begin();

    readCoefficients();
    return true;
}

/**************************************************************************/
/*
        Reads Coefficients Data
*
/**************************************************************************/
void MPL115A2::readCoefficients()
{
    uint8_t buffer[8];
    int16_t A0Coeff, B1Coeff, B2Coeff, C12Coeff;
    // Reading 4 Calibration Data Values
    // Read cofficients values stored in EPROM of the device
    // Pressure Offset Coefficient, a0
    // Pressure Sensitivity Coefficient, b1
    // Temperature coefficient of Offset (TCO), b2
    // Temperature coefficient of Sensitivity (TCS), c12
    
    for (int i = 0; i < 8; i++)
    {
        Wire.beginTransmission(mpl_i2cAddress);
        i2cwrite((uint8_t)MPL115A2_REG_A0_MSB + i);
        Wire.endTransmission();
        
        // Request 1 byte of data
        Wire.requestFrom(mpl_i2cAddress, (uint8_t)1);
        
        // Read 1 byte of data
        // a0 MSB, a0 LSB, b1 MSB, b1 LSB, b2 MSB, b2 LSB, c12 MSB, c12 LSB
        if (Wire.available() == 1)
        {
            buffer[i] = Wire.read();
        }
    }
    
    // Conversion of the Data
    A0Coeff = (int16_t)(buffer[0] << 8 | buffer[1]);
    B1Coeff = (int16_t)(buffer[2] << 8 | buffer[3]);
    B2Coeff = (int16_t)(buffer[4] << 8 | buffer[5]);
    C12Coeff = (int16_t)(buffer[6] << 8 | buffer[7]) >> 2;
    
    a0 = (float)A0Coeff / 8;
    b1 = (float)B1Coeff / 8192;
    b2 = (float)B2Coeff / 16384;
    c12 = (float)C12Coeff / 4194304.0;

}

/**************************************************************************/
/* 
        Calculate Pressure Calculations and perform compensation
        More info about these calculations are available in the datasheet
*/
/**************************************************************************/
void MPL115A2::Measure_PressureTemperature(void)
{
    uint8_t buffer[4];
    uint16_t pressure, temperature;
    float Pcomp;
    
    // Get Raw Pressure and Temperature Settings
    // Start the Pressure and Temperature Conversion
    for (int i = 0; i < 8; i++)
    {
        Wire.beginTransmission(mpl_i2cAddress);
        i2cwrite((uint8_t)MPL115A2_REG_P_ADC_MSB + i);
        Wire.endTransmission();
        
        // Request 1 byte of data
        Wire.requestFrom(mpl_i2cAddress, (uint8_t)1);
        
        // Read 1 byte of data
        // Pressure MSB, Pressure LSB, Temperature MSB, Temperature LSB
        if (Wire.available() == 1)
        {
            buffer[i] = Wire.read();
        }
    }
    
    // Read the 10-bit Pressure and Temperature Values
    pressure = (uint16_t)(buffer[0] << 8 | buffer[1]) >> 6;
    temperature = (uint16_t)(buffer[2] << 8 | buffer[3]) >> 6;
    
    // Calculate Pressure Compensation
    // The 10-bit Compensated Pressure Output, Pcomp, is Calculated as follows:
    // Pcomp = a0 + (b1 + c12 * Tadc) * Padc + b2 * Tadc
    Pcomp =  a0 + (b1 + c12 * temperature) * pressure + b2 * temperature;
    
    // Conversion of the Results
    mpl_sensorData.P = (65.0 / 1023.0) * Pcomp + 50.0;
    mpl_sensorData.T = (temperature - 498.0) / (-5.35) + 25.0;
}
