/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        BMA250
        This code is designed to work with the BMA250_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/3-axis-12-bit-8-bit-digital-accelerometer
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "BMA250.h"

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
        Instantiates a new BMA250 class with appropriate properties
        Accelerometer Address
*/
/**************************************************************************/
void BMA250::getAddr_BMA250(uint8_t i2cAddress)
{
    bma_i2cAddress = i2cAddress;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool BMA250::begin()
{
    Wire.begin();
    
    uint8_t chipid = readRegister(bma_i2cAddress, BMA250_REG_ACCEL_CHIP_ID);
    if (chipid != BMA250_DEFAULT_CHIP_ID)
        return false;
    
    // Set up the sensor for Accelerometer
    // setUpSensor();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Full Scale Range of the Accelerometer Outputs
*/
/**************************************************************************/
void BMA250::setAccelRange(bmaAccelRange_t accelrange)
{
    bma_accelrange = accelrange;
}

/**************************************************************************/
/*
        Gets the Full Scale Range of the Accelerometer Outputs
*/
/**************************************************************************/
bmaAccelRange_t BMA250::getAccelRange()
{
    return bma_accelrange;
}

/**************************************************************************/
/*
        Sets the Selection of the Bandwidth for the Acceleration Data
*/
/**************************************************************************/
void BMA250::setAccelBandwidth(bmaAccelBandwidth_t accelbandwidth)
{
    bma_accelbandwidth = accelbandwidth;
}

/**************************************************************************/
/*
        Gets the Selection of the Bandwidth for the Acceleration Data
*/
/**************************************************************************/
bmaAccelBandwidth_t BMA250::getAccelBandwidth()
{
    return bma_accelbandwidth;
}

/**************************************************************************/
/*
        Sets up the Sensor for Accelerometer
*/
/**************************************************************************/
void BMA250::setUpSensor(void)
{
    // Set Up the Configuration for the Accelerometer g-Range Register
    // Full Scale Range of the Accelerometer Outputs
    uint8_t accel_range = bma_accelrange;
    
    // Write the configuration to the Accelerometer g-Range Register
    writeRegister(bma_i2cAddress, BMA250_REG_ACCEL_G_RANGE, accel_range);
    
    // Wait for the configuration to complete
    delay(bma_conversionDelay);
    
    // Set Up the Configuration for the Accelerometer Bandwidth Register
    // Set the Selection of the Bandwidth for the Acceleration Data
    uint8_t bandwidth = bma_accelbandwidth;
    
    // Write the configuration to the Accelerometer Bandwidth Register
    writeRegister(bma_i2cAddress, BMA250_REG_ACCEL_BANDWIDTH, bandwidth);
    
    // Wait for the configuration to complete
    delay(bma_conversionDelay);

}

/**************************************************************************/
/*
        Reads the 3 axes of the Accelerometer
        The value is expressed in 16 bit as two’s complement
*/
/**************************************************************************/
void BMA250::Measure_Accelerometer()
{
    // Read the Accelerometer
    uint8_t xAccelLo, xAccelHi, yAccelLo, yAccelHi, zAccelLo, zAccelHi;
    
    // Read the Data
    // Reading the Low X-Axis Acceleration Data Register
    xAccelLo = readRegister(bma_i2cAddress, BMA250_REG_ACCEL_ACCEL_X_LSB);
    // Reading the High X-Axis Acceleration Data Register
    xAccelHi = readRegister(bma_i2cAddress, BMA250_REG_ACCEL_ACCEL_X_MSB);
    // Conversion of the result
    // 16-bit signed result for X-Axis Acceleration Data of BMA250
    bma_accelData.X = (int16_t)((xAccelHi << 8) | xAccelLo);
    bma_accelData.X >>= 6;
    
    // Reading the Low Y-Axis Acceleration Data Register
    yAccelLo = readRegister(bma_i2cAddress, BMA250_REG_ACCEL_ACCEL_Y_LSB);
    // Reading the High Y-Axis Acceleration Data Register
    yAccelHi = readRegister(bma_i2cAddress, BMA250_REG_ACCEL_ACCEL_Y_MSB);
    // Conversion of the result
    // 16-bit signed result for Y-Axis Acceleration Data of BMA250
    bma_accelData.Y = (int16_t)((yAccelHi << 8) | yAccelLo);
    bma_accelData.Y >>= 6;
    
    // Reading the Low Z-Axis Acceleration Data Register
    zAccelLo = readRegister(bma_i2cAddress, BMA250_REG_ACCEL_ACCEL_Z_LSB);
    // Reading the High Z-Axis Acceleration Data Register
    zAccelHi = readRegister(bma_i2cAddress, BMA250_REG_ACCEL_ACCEL_Z_MSB);
    // Conversion of the result
    // 16-bit signed result for Z-Axis Acceleration Data of BMA250
    bma_accelData.Z = (int16_t)((zAccelHi << 8) | zAccelLo);
    bma_accelData.Z >>= 6;
}

