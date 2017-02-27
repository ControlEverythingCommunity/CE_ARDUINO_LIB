/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        BMP180
        This code is designed to work with the BMP180_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelerometer?sku=BMP180_I2CS_A01#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "BMP180.h"

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
static void writeRegister8(uint8_t i2cAddress, uint8_t reg, uint8_t value)
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
static uint16_t readRegister16(uint8_t i2cAddress, uint8_t reg)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)2);
    return (uint16_t)((i2cread() << 8) | i2cread());
}

/**************************************************************************/
/*
    Reads 24-bits from the specified destination register
*/
/**************************************************************************/
static uint32_t readRegister24(uint8_t i2cAddress, uint8_t reg)
{
    uint32_t value;
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)3);
    value = i2cread();
    value <<= 8;
    value |= i2cread();
    value <<= 8;
    value |= i2cread();
    return value;
}

/**************************************************************************/
/*
        Reads Signed 16-bits from the specified destination register
*/
/**************************************************************************/
int16_t BMP180::readRegisterS16(uint8_t i2cAddress, uint8_t reg)
{
    return (int16_t)readRegister16(i2cAddress, reg);
}

/**************************************************************************/
/*
        Instantiates a new BMP180 class with appropriate properties
*/
/**************************************************************************/
void BMP180::getAddr_BMP180(uint8_t i2cAddress)
{
    bmp_i2cAddress = i2cAddress;
    bmp_conversionDelay = BMP180_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool BMP180::begin()
{
    Wire.begin();
    
    uint8_t chipid= readRegister8(bmp_i2cAddress, BMP180_CHIP_ID_REG);
    if (chipid != BMP180_CHIP_ID)
        return false;
    
    // Reset the Device Using Soft-Reset
    // Reset();
    
    // Set up the sensor for Pressure, Temperature and Humidity
    // setUpSampling();
    
    // Read Trimming Parameters
    readCoefficients();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Soft Reset Command
        The Device is Reset using the Complete Power-ON-Reset Procedure
*/
/**************************************************************************/
void BMP180::Reset()
{
    writeRegister8(bmp_i2cAddress, BMP180_SOFTRESET_REG, 0xB6);
    delay(bmp_conversionDelay);
}

/**************************************************************************/
/*
        Setup the Sensor and read the Uncompensated Value of Pressure
*/
/**************************************************************************/
int32_t BMP180::getUncompPressure()
{
    // Set Up the Configuration for the Measurement Control Register
    uint8_t meas_ctrl = BMP180_CTRL_MEAS_REG_MEASURE_PRESSURE               |   // Measurement Pressure
                        (BMP180_CTRL_MEAS_REG_OVERSAMPLING_ULTRA_HIGH_RESOLUTION << 6);      // Oversampling_Setting: 1
    
    // Write the configuration to the Measurement Control Register
    writeRegister8(bmp_i2cAddress, BMP180_CTRL_MEAS_REG, meas_ctrl);
    
    // Wait for the configuration to complete
    delay(bmp_conversionDelay);
    
    // Read the Uncompensated Pressure Data
    int32_t p = readRegister24(bmp_i2cAddress, BMP180_MEASURE_MSB_REG);
    p = p >> (8 - BMP180_CTRL_MEAS_REG_OVERSAMPLING_ULTRA_HIGH_RESOLUTION);
    return p;
}

/**************************************************************************/
/*
        Setup the Sensor and read the Uncompensated Value of Temperature
*/
/**************************************************************************/
int16_t BMP180::getUncompTemperature()
{
    // Set Up the Configuration for the Measurement Control Register
    // Set the Controls for Oversampling Ratio of the Temperature Measurement
    uint8_t meas_ctrl = BMP180_CTRL_MEAS_REG_MEASURE_TEMPERATURE;       // Measurement Temperature
    
    // Write the configuration to the Measurement Control Register
    writeRegister8(bmp_i2cAddress, BMP180_CTRL_MEAS_REG, meas_ctrl);
    
    // Wait for the configuration to complete
    delay(bmp_conversionDelay);
    
    // Read the Uncompensated Pressure Data
    return (int16_t)readRegister16(bmp_i2cAddress, BMP180_MEASURE_MSB_REG);
}

/**************************************************************************/
/*
        Reads up the Device which comprises of an Pressure and
        Temperature Sensor
*/
/**************************************************************************/
void BMP180::Measure_Sensor(void)
{
    // Read all the Sensors
    // Pressure, Temperature Data
    Measure_Pressure(BMP180_CTRL_MEAS_REG_OVERSAMPLING_ULTRA_HIGH_RESOLUTION);
    Measure_Temperature();
}

/**************************************************************************/
/*
        Reads Calibration Data (Coefficients that are Factory Calibrated) stored in PROM
*/
/**************************************************************************/
void BMP180::readCoefficients(void)
{
    bmp_calibdata.AC1 = readRegisterS16(bmp_i2cAddress, BMP180_CAL_AC1_LSB_REG);
    bmp_calibdata.AC2 = readRegisterS16(bmp_i2cAddress, BMP180_CAL_AC2_LSB_REG);
    bmp_calibdata.AC3 = readRegisterS16(bmp_i2cAddress, BMP180_CAL_AC3_LSB_REG);
    bmp_calibdata.AC4 = readRegister16(bmp_i2cAddress, BMP180_CAL_AC4_LSB_REG);
    bmp_calibdata.AC5 = readRegister16(bmp_i2cAddress, BMP180_CAL_AC5_LSB_REG);
    bmp_calibdata.AC6 = readRegister16(bmp_i2cAddress, BMP180_CAL_AC6_LSB_REG);
    
    bmp_calibdata._B1 = readRegisterS16(bmp_i2cAddress, BMP180_CAL_B1_LSB_REG);
    bmp_calibdata._B2 = readRegisterS16(bmp_i2cAddress, BMP180_CAL_B2_LSB_REG);
    
    bmp_calibdata.MB = readRegisterS16(bmp_i2cAddress, BMP180_CAL_MB_LSB_REG);
    bmp_calibdata.MC = readRegisterS16(bmp_i2cAddress, BMP180_CAL_MC_LSB_REG);
    bmp_calibdata.MD = readRegisterS16(bmp_i2cAddress, BMP180_CAL_MD_LSB_REG);
    
}

/**************************************************************************/
/*
        Starts the Measure of the Digital Temperarure
        The Measured Value Must be Compensated by the Calibration Data
*/
/**************************************************************************/
void BMP180::Measure_Temperature(void)
{
    long UT = getUncompTemperature();
    
    long X1 = (UT - (long)bmp_calibdata.AC6) * (long)bmp_calibdata.AC5 >> 15;
    long X2 = ((long)bmp_calibdata.MC << 11) / (X1 + bmp_calibdata.MD);
    bmp_calibdata.B5 = X1 + X2;
    bmp_sensorData.T = (bmp_calibdata.B5 + 8) >> 4;
    bmp_sensorData.T /= 10.0;
}

/**************************************************************************/
/*
        Reads 24-bits from the Destination Register
        Reads the Results for Digital Pressure Value
*/
/**************************************************************************/
void BMP180::Measure_Pressure(uint8_t oversampling)
{
    oversampling = BMP180_CTRL_MEAS_REG_OVERSAMPLING_ULTRA_HIGH_RESOLUTION;
    long UP = getUncompPressure();
    
    long B6, X1, X2, X3, B3, p;
    unsigned long B4, B7;
    B6 = bmp_calibdata.B5 - 4000;
    X1 = (bmp_calibdata._B2 * (B6 * B6 >> 12)) >> 11;
    X2 = (bmp_calibdata.AC2 * B6) >> 11;
    X3 = X1 + X2;
    B3 = ((((uint16_t)bmp_calibdata.AC1 * 4 + X3) << oversampling) + 2) >> 2;
    
    X1 = bmp_calibdata.AC3 * B6 >> 13;
    X2 = (bmp_calibdata._B1 * (B6 * B6 >> 12)) >> 16;
    X3 = ((X1 + X2) + 2) >> 2;
    B4 = bmp_calibdata.AC4 * (unsigned long)(X3 + 32768) >> 15;
    
    B7 = ((unsigned long)UP - B3) * (50000 >> oversampling);
    if (B7 < 0x80000000)
        p = (B7 * 2) / B4;
    else
        p = (B7 / B4) * 2;
    X1 = (p >> 8) * (p >> 8);
    X1 = (X1 * 3038) >> 16;
    X2 = (-7357 * p) >> 16;
    bmp_sensorData.P = p + ((X1 + X2 + 3791) >> 4);
}

/**************************************************************************/
/*
        Calculates the Altitude (in meters) from the specified Atmospheric
        Pressure (in hPa), and Sea-Level pressure (in hPa)
*/
/**************************************************************************/
float BMP180::readAltitude(float seaLevel)
{
    float atmospheric = bmp_sensorData.P / 100.0;
    return 44330.0 * (1.0 - pow(atmospheric / seaLevel, 0.1903));
}


/**************************************************************************/
/*
        Calculates the Pressure at Sea Level (in hPa) from the Specified Altitude
        (in meters), and Atmospheric Pressure (in hPa)
*/
/**************************************************************************/
float BMP180::seaLevelForAltitude(float altitude, float atmospheric)
{
    return atmospheric / pow(1.0 - (altitude/44330.0), 5.255);
}

