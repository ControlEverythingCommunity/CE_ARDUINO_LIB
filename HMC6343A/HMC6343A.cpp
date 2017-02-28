/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HMC6343A
        This code is designed to work with the HMC6343A_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Compass?sku=HMC6343A_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "HMC6343A.h"

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
        Writes 8-bits to the Specified Destination Register
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
        Reads 8-bits to the Specified Destination Register
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
        Instantiates a new HMC6343A class with appropriate properties
*/
/**************************************************************************/
void HMC6343A::getAddr_HMC6343A(uint8_t i2cAddress)
{
    hmc_i2cAddress = i2cAddress;
    hmc_ConversionDelay = HMC6343A_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool HMC6343A::begin()
{
    Wire.begin();
    
    // Set up the Sensor for Accelerometer and Magnetometer
    // setUpSensor();
    
    return true;
}

/**************************************************************************/
/*
        Set up the Sensor comprising of an Accelerometer and Magnetometer
*/
/**************************************************************************/
void HMC6343A::setUpSensor(void)
{
    // Set up the Configuration for Operational Mode Register 1
    uint8_t opmode1 =   HMC6343A_REG_ACCELMAG_OP_MODE1_COMP_NOSET       |   // No Set for Calculating Compass Data
                        HMC6343A_REG_ACCELMAG_OP_MODE1_CAL_NOSET        |   // No Set for Calculating Calibration Offsets
                        HMC6343A_REG_ACCELMAG_OP_MODE1_FILTER_NOSET     |   // No Set for IIR Heading Filter
                        HMC6343A_REG_ACCELMAG_OP_MODE1_RUN_SET          |   // Set for Run Mode
                        HMC6343A_REG_ACCELMAG_OP_MODE1_STANDBY_NOSET    |   // No Set for Standby Mode
                        HMC6343A_REG_ACCELMAG_OP_MODE1_UF_NOSET         |   // No Set for Upright Front Orientation
                        HMC6343A_REG_ACCELMAG_OP_MODE1_UE_NOSET         |   // No Set for Upright Edge Orientation
                        HMC6343A_REG_ACCELMAG_OP_MODE1_LEVEL_SET;           // Set for Level Orientation
    
    // Write the Configuration to the Operational Mode Register 1
    writeRegister(hmc_i2cAddress, HMC6343A_REG_ACCELMAG_OP_MODE1, opmode1);
    
    // Wait for the Configuration to Complete
    delay(hmc_ConversionDelay);
    
    // Set up the Configuration for Operational Mode Register 2
    uint8_t opmode2 = HMC6343A_REG_ACCELMAG_OP_MODE2_MR_5HZ;    // Measurement Rate: 5 Hz
    
    // Write the Configuration to the Operational Mode Register 2
    writeRegister(hmc_i2cAddress, HMC6343A_REG_ACCELMAG_OP_MODE2, opmode2);
    
    // Wait for the Configuration to Complete
    delay(hmc_ConversionDelay);
}

/**************************************************************************/
/*
        Reads the Results for the Accelerometer and Magentometer Sensors in HMC6343A
*/
/**************************************************************************/
void HMC6343A::Measure_Sensor(void)
{
    // Reads Accelerometer and Magentometer
    Measure_Accelerometer();
    Measure_Magnetometer();
}

/**************************************************************************/
/*
        Reads the Results for the Accelerometer Sensor in HMC6343A
 */
/**************************************************************************/
void HMC6343A::Measure_Accelerometer(void)
{
    // Read the Accelerometer
    uint8_t xAccelHi, xAccelLo, yAccelHi, yAccelLo, zAccelHi, zAccelLo;
    
    // Read the Conversion Results
    // Reading the MSB Data of Output X Register
    xAccelHi = readRegister(hmc_i2cAddress, HMC6343A_REG_ACCELMAG_POST_ACCEL);
    // Reading the LSB Data of Output X Register
    xAccelLo = readRegister(hmc_i2cAddress, HMC6343A_REG_ACCELMAG_POST_ACCEL + 1);
    // Conversion of the result
    // 16-bit signed result for Channel X of HMC6343A
    hmc_accelData.X = (int16_t)((xAccelHi << 8) | xAccelLo);
    
    // Reading the MSB Data of Output Y Register
    yAccelHi = readRegister(hmc_i2cAddress, HMC6343A_REG_ACCELMAG_POST_ACCEL + 2);
    // Reading the LSB Data of Output Y Register
    yAccelLo = readRegister(hmc_i2cAddress, HMC6343A_REG_ACCELMAG_POST_ACCEL + 3);
    // Conversion of the result
    // 16-bit signed result for Channel Y of HMC6343A
    hmc_accelData.Y = (int16_t)((yAccelHi << 8) | yAccelLo);
    
    // Reading the MSB Data of Output Z Register
    zAccelHi = readRegister(hmc_i2cAddress, HMC6343A_REG_ACCELMAG_POST_ACCEL + 4);
    // Reading the LSB Data of Output Z Register
    zAccelLo = readRegister(hmc_i2cAddress, HMC6343A_REG_ACCELMAG_POST_ACCEL + 5);
    // Conversion of the result
    // 16-bit signed result for Channel Z of HMC6343A
    hmc_accelData.Z = (int16_t)((zAccelHi << 8) | zAccelLo);
}


/**************************************************************************/
/*
        Reads the Results for the Magnetometer Sensor in HMC6343A
*/
/**************************************************************************/
void HMC6343A::Measure_Magnetometer(void)
{
    // Read the Magnetometer
    uint8_t xMagHi, xMagLo, yMagHi, yMagLo, zMagHi, zMagLo;
    
    // Read the Conversion Results
    // Reading the MSB Data of Output X Register
    xMagHi = readRegister(hmc_i2cAddress, HMC6343A_REG_ACCELMAG_POST_MAG);
    // Reading the LSB Data of Output X Register
    xMagLo = readRegister(hmc_i2cAddress, HMC6343A_REG_ACCELMAG_POST_MAG + 1);
    // Conversion of the result
    // 16-bit signed result for Channel X of HMC6343A
    hmc_magData.X = (int16_t)((xMagHi << 8) | xMagLo);
    
    // Reading the MSB Data of Output Y Register
    yMagHi = readRegister(hmc_i2cAddress, HMC6343A_REG_ACCELMAG_POST_MAG + 2);
    // Reading the LSB Data of Output Y Register
    yMagLo = readRegister(hmc_i2cAddress, HMC6343A_REG_ACCELMAG_POST_MAG + 3);
    // Conversion of the result
    // 16-bit signed result for Channel Y of HMC6343A
    hmc_magData.Y = (int16_t)((yMagHi << 8) | yMagLo);
    
    // Reading the MSB Data of Output Z Register
    zMagHi = readRegister(hmc_i2cAddress, HMC6343A_REG_ACCELMAG_POST_MAG + 4);
    // Reading the LSB Data of Output Z Register
    zMagLo = readRegister(hmc_i2cAddress, HMC6343A_REG_ACCELMAG_POST_MAG + 5);
    // Conversion of the result
    // 16-bit signed result for Channel Z of HMC6343A
    hmc_magData.Z = (int16_t)((zMagHi << 8) | zMagLo);
}
