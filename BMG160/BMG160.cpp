/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        BMG160
        This code is designed to work with the BMG160_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/3-axis-12-bit-8-bit-digital-Gyroscope
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "BMG160.h"

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
        Instantiates a new BMG160 class with appropriate properties
        Gyroscope Address
*/
/**************************************************************************/
void BMG160::getAddr_BMG160(uint8_t i2cAddress)
{
    bmg_i2cAddress = i2cAddress;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool BMG160::begin()
{
    Wire.begin();
    
    uint8_t chipid = readRegister(bmg_i2cAddress, BMG160_REG_GYRO_CHIP_ID);
    if (chipid != BMG160_DEFAULT_CHIP_ID)
        return false;
    
    // Set up the sensor for Gyroscope
    // setUpSensor();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Full Scale Range of the Gyroscope Outputs
*/
/**************************************************************************/
void BMG160::setGyroRange(bmgGyroRange_t gyrorange)
{
    bmg_gyrorange = gyrorange;
}

/**************************************************************************/
/*
        Gets the Full Scale Range of the Gyroscope Outputs
*/
/**************************************************************************/
bmgGyroRange_t BMG160::getGyroRange()
{
    return bmg_gyrorange;
}

/**************************************************************************/
/*
        Sets the Selection of the Bandwidth for the Gyroscope Data
*/
/**************************************************************************/
void BMG160::setGyroBandwidth(bmgGyroBandwidth_t gyrobandwidth)
{
    bmg_gyrobandwidth = gyrobandwidth;
}

/**************************************************************************/
/*
        Gets the Selection of the Bandwidth for the Gyroscope Data
*/
/**************************************************************************/
bmgGyroBandwidth_t BMG160::getGyroBandwidth()
{
    return bmg_gyrobandwidth;
}

/**************************************************************************/
/*
        Sets up the Sensor for Gyroscope
*/
/**************************************************************************/
void BMG160::setUpSensor(void)
{
    // Set Up the Configuration for the Gyroscope Angular Rate Range Register
    // Full Scale Range of the Gyroscope Outputs
    uint8_t range = bmg_gyrorange;
    
    // Write the configuration to the Gyroscope Angular Rate Range Register
    writeRegister(bmg_i2cAddress, BMG160_REG_GYRO_RANGE, range);
    
    // Wait for the configuration to complete
    delay(bmg_conversionDelay);
    
    // Set Up the Configuration for the Gyroscope Angular Rate Data Filter Bandwidth Register
    // Set the Selection of the Bandwidth for the Gyroscope Data
    uint8_t bandwidth = bmg_gyrobandwidth;
    
    // Write the configuration to the Gyroscope Angular Rate Data Filter Bandwidth Register
    writeRegister(bmg_i2cAddress, BMG160_REG_GYRO_BANDWIDTH, bandwidth);
    
    // Wait for the configuration to complete
    delay(bmg_conversionDelay);

}

/**************************************************************************/
/*
        Reads the 3 axes of the Gyroscope
        The value is expressed in 16 bit as two’s complement
*/
/**************************************************************************/
void BMG160::Measure_Gyroscope()
{
    // Read the Gyroscope
    uint8_t xGyroLo, xGyroHi, yGyroLo, yGyroHi, zGyroLo, zGyroHi;
    
    // Read the Data
    // Reading the Low X-Axis Gyroscope Data Register
    xGyroLo = readRegister(bmg_i2cAddress, BMG160_REG_GYRO_RATE_X_LSB);
    // Reading the High X-Axis Gyroscope Data Register
    xGyroHi = readRegister(bmg_i2cAddress, BMG160_REG_GYRO_RATE_X_MSB);
    // Conversion of the result
    // 16-bit signed result for X-Axis Gyroscope Data of BMG160
    bmg_gyroData.X = (int16_t)((xGyroHi << 8) | xGyroLo);
    
    // Reading the Low Y-Axis Gyroscope Data Register
    yGyroLo = readRegister(bmg_i2cAddress, BMG160_REG_GYRO_RATE_Y_LSB);
    // Reading the High Y-Axis Gyroscope Data Register
    yGyroHi = readRegister(bmg_i2cAddress, BMG160_REG_GYRO_RATE_Y_MSB);
    // Conversion of the result
    // 16-bit signed result for Y-Axis Gyroscope Data of BMG160
    bmg_gyroData.Y = (int16_t)((yGyroHi << 8) | yGyroLo);
    
    // Reading the Low Z-Axis Gyroscope Data Register
    zGyroLo = readRegister(bmg_i2cAddress, BMG160_REG_GYRO_RATE_Z_LSB);
    // Reading the High Z-Axis Gyroscope Data Register
    zGyroHi = readRegister(bmg_i2cAddress, BMG160_REG_GYRO_RATE_Z_MSB);
    // Conversion of the result
    // 16-bit signed result for Z-Axis Gyroscope Data of BMG160
    bmg_gyroData.Z = (int16_t)((zGyroHi << 8) | zGyroLo);
}

