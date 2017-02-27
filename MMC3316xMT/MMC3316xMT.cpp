/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MMC3316xMT
        This code is designed to work with the MMC3316xMT_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Magnetic-Sensor?sku=MMC3316xMT_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "MMC3316xMT.h"

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
        Instantiates a new MMC3316xMT class with appropriate properties
        Magnetometer Address
*/
/**************************************************************************/
void MMC3316xMT::getAddr_MMC3316xMT(uint8_t i2cAddress)
{
    mmc_i2cAddress = i2cAddress;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool MMC3316xMT::begin()
{
    Wire.begin();
    
    // Set up the sensor for Magnetometer
    // setUpMagnetometer();
    
    return true;
}

/**************************************************************************/
/*
        Resets the Sensor by Passing a Large Current through Set/Reset Coil at a Reversed Direction
*/
/**************************************************************************/
void MMC3316xMT::setMagReset(mmcMagReset_t magreset)
{
    mmc_magreset = magreset;
}

/**************************************************************************/
/*
        Gets the Reset Status of the Sensor by Passing a Large Current through Set/Reset Coil at a Reversed Direction
*/
/**************************************************************************/
mmcMagReset_t MMC3316xMT::getMagReset()
{
    return mmc_magreset;
}

/**************************************************************************/
/*
        Sets the Sensor by Passing a Large Current through Set/Reset Coil
*/
/**************************************************************************/
void MMC3316xMT::setMagSet(mmcMagSet_t magset)
{
    mmc_magset = magset;
}

/**************************************************************************/
/*
        Gets the Set Status of the Sensor by Passing a Large Current through Set/Reset Coil
*/
/**************************************************************************/
mmcMagSet_t MMC3316xMT::getMagSet()
{
    return mmc_magset;
}

/**************************************************************************/
/*
        Sets the Sensor to Determine How Often the Chip will Take Measurements in Continuous Measurement Mode
*/
/**************************************************************************/
void MMC3316xMT::setMagFrequency(mmcMagFrequency_t magfrequency)
{
    mmc_magfrequency = magfrequency;
}

/**************************************************************************/
/*
        Gets the Sensor to Determine How Often the Chip will Take Measurements in Continuous Measurement Mode
*/
/**************************************************************************/
mmcMagFrequency_t MMC3316xMT::getMagFrequency()
{
    return mmc_magfrequency;
}

/**************************************************************************/
/*
        Sets the Enable Continuous Measurement Mode
*/
/**************************************************************************/
void MMC3316xMT::setMagContMode(mmcMagContMode_t magcontmode)
{
    mmc_magcontmode = magcontmode;
}

/**************************************************************************/
/*
        Gets the Enable Continuous Measurement Mode
*/
/**************************************************************************/
mmcMagContMode_t MMC3316xMT::getMagContMode()
{
    return mmc_magcontmode;
}

/**************************************************************************/
/*
        Sets the Initiate Measurementt Mode
*/
/**************************************************************************/
void MMC3316xMT::setMagMeasurement(mmcMagMeasurement_t magmeasurement)
{
    mmc_magmeasurement = magmeasurement;
}

/**************************************************************************/
/*
        Gets the Initiate Measurement Mode
*/
/**************************************************************************/
mmcMagMeasurement_t MMC3316xMT::getMagMeasurement()
{
    return mmc_magmeasurement;
}

/**************************************************************************/
/*
        Sets up the Magnetometer
*/
/**************************************************************************/
void MMC3316xMT::setUpMagnetometer(void)
{
    // Set Up the Configuration for the Magnetometer Internal Control Register 0
    /*
     // Reset the Sensor by Passing a Large Current through Set/Reset Coil at a Reversed Direction
     uint8_t control0 = mmc_magreset;
     
     // Set the Sensor by Passing a Large Current through Set/Reset Coil
     control0 |= mmc_magset;
     
     // Set the Sensor to Determine How Often the Chip will Take Measurements in Continuous Measurement Mode
     control0 |= mmc_magfrequency;
     
     // Set to Enable the Continuous Measurement Mode
     control0 |= mmc_magcontmode;
     
     // Set to Initiate Measurement Mode
     control0 |= mmc_magmeasurement;
    */
    
    uint8_t control0 =  MMC3316xMT_REG_MAG_CTRL_REG0_RESET_NO           |   // No Reset the Sensor
                        MMC3316xMT_REG_MAG_CTRL_REG0_SET_NO             |   // Don't Set the Sensor
                        MMC3316xMT_REG_MAG_CTRL_REG0_CM_FREQ_50HZ       |   // Frequency: 50Hz
                        MMC3316xMT_REG_MAG_CTRL_REG0_CONT_MODE_DISABLE  |   // Disable Continuous Measurement Mode
                        MMC3316xMT_REG_MAG_CTRL_REG0_TM_INITIATE;           // Initiate Measurement
    
    // Write the configuration to the Magnetometer Internal Control Register 0
    writeRegister(mmc_i2cAddress, MMC3316xMT_REG_MAG_CTRL_REG0, control0);
    
    // Wait for the configuration to complete
    delay(mmc_conversionDelay);
    
    uint8_t control1 =  MMC3316xMT_REG_MAG_CTRL_REG0_RESET_NO           |   // No Reset the Sensor
                        MMC3316xMT_REG_MAG_CTRL_REG0_SET_YES            |   // Set the Sensor
                        MMC3316xMT_REG_MAG_CTRL_REG0_CM_FREQ_50HZ       |   // Frequency: 50Hz
                        MMC3316xMT_REG_MAG_CTRL_REG0_CONT_MODE_DISABLE  |   // Disable Continuous Measurement Mode
                        MMC3316xMT_REG_MAG_CTRL_REG0_TM_INITIATE;           // Initiate Measurement
    
    // Write the configuration to the Magnetometer Internal Control Register 0
    writeRegister(mmc_i2cAddress, MMC3316xMT_REG_MAG_CTRL_REG0, control1);
    
    // Wait for the configuration to complete
    delay(mmc_conversionDelay);
    
    uint8_t control2 =  MMC3316xMT_REG_MAG_CTRL_REG0_RESET_NO           |   // No Reset the Sensor
                        MMC3316xMT_REG_MAG_CTRL_REG0_SET_NO             |   // Don't Set the Sensor
                        MMC3316xMT_REG_MAG_CTRL_REG0_CM_FREQ_50HZ       |   // Frequency: 50Hz
                        MMC3316xMT_REG_MAG_CTRL_REG0_CONT_MODE_DISABLE  |   // Disable Continuous Measurement Mode
                        MMC3316xMT_REG_MAG_CTRL_REG0_TM_NOT;                // No Initiate Measurement
    
    // Write the configuration to the Magnetometer Internal Control Register 0
    writeRegister(mmc_i2cAddress, MMC3316xMT_REG_MAG_CTRL_REG0, control2);
    
    // Wait for the configuration to complete
    delay(mmc_conversionDelay);
    
    uint8_t control3 =  MMC3316xMT_REG_MAG_CTRL_REG0_RESET_YES          |   // Reset the Sensor
                        MMC3316xMT_REG_MAG_CTRL_REG0_SET_NO             |   // Don't Set the Sensor
                        MMC3316xMT_REG_MAG_CTRL_REG0_CM_FREQ_50HZ       |   // Frequency: 50Hz
                        MMC3316xMT_REG_MAG_CTRL_REG0_CONT_MODE_DISABLE  |   // Disable Continuous Measurement Mode
                        MMC3316xMT_REG_MAG_CTRL_REG0_TM_NOT;                // No Initiate Measurement
    
    // Write the configuration to the Magnetometer Internal Control Register 0
    writeRegister(mmc_i2cAddress, MMC3316xMT_REG_MAG_CTRL_REG0, control3);
    
    // Wait for the configuration to complete
    delay(mmc_conversionDelay);
    
    uint8_t control4 =  MMC3316xMT_REG_MAG_CTRL_REG0_RESET_YES          |   // Reset the Sensor
                        MMC3316xMT_REG_MAG_CTRL_REG0_SET_NO             |   // Don't Set the Sensor
                        MMC3316xMT_REG_MAG_CTRL_REG0_CM_FREQ_50HZ       |   // Frequency: 50Hz
                        MMC3316xMT_REG_MAG_CTRL_REG0_CONT_MODE_ENABLE   |   // Enable Continuous Measurement Mode
                        MMC3316xMT_REG_MAG_CTRL_REG0_TM_INITIATE;           // Initiate Measurement
    
    // Write the configuration to the Magnetometer Internal Control Register 0
    writeRegister(mmc_i2cAddress, MMC3316xMT_REG_MAG_CTRL_REG0, control4);
    
    // Wait for the configuration to complete
    delay(mmc_conversionDelay);
}

/**************************************************************************/
/*
        Reads up the Sensor which comprises of a Magnetometer
*/
/**************************************************************************/
void MMC3316xMT::Measure_Sensor(void)
{
    // Read the Sensor
    // Magnetometer Data
    Measure_Magnetometer();
}

/**************************************************************************/
/*
        Reads the 3 axes of the Magnetometer
        The value is expressed in 14 bits, unsigned format
*/
/**************************************************************************/
void MMC3316xMT::Measure_Magnetometer()
{
    // Read the Magnetometer
    uint8_t xMagLo, xMagHi, yMagLo, yMagHi, zMagLo, zMagHi;
    
    // Read the Data
    // Reading the Low X-Axis Magnetic Data Register
    xMagLo = readRegister(mmc_i2cAddress, MMC3316xMT_REG_MAG_OUT_X_L);
    // Reading the High X-Axis Magnetic Data Register
    xMagHi = readRegister(mmc_i2cAddress, MMC3316xMT_REG_MAG_OUT_X_H);
    // Conversion of the results
    // 16-bit signed result for X-Axis Magnetic Data of MMC3316xMT
    mmc_magData.X = (int16_t)(xMagHi << 8 | xMagLo);
    if (mmc_magData.X > 8191)
    {
        mmc_magData.X -= 16384;
    }
    
    // Reading the Low Y-Axis Magnetic Data Register
    yMagLo = readRegister(mmc_i2cAddress, MMC3316xMT_REG_MAG_OUT_Y_L);
    // Reading the High Y-Axis Magnetic Data Register
    yMagHi = readRegister(mmc_i2cAddress, MMC3316xMT_REG_MAG_OUT_Y_H);
    // Conversion of the result
    // 16-bit signed result for Y-Axis Magnetic Data of MMC3316xMT
    mmc_magData.Y = (int16_t)(yMagHi << 8 | yMagLo);
    if (mmc_magData.Y > 8191)
    {
        mmc_magData.Y -= 16384;
    }
    
    // Reading the Low Z-Axis Magnetic Data Register
    zMagLo = readRegister(mmc_i2cAddress, MMC3316xMT_REG_MAG_OUT_Z_L);
    // Reading the High Z-Axis Magnetic Data Register
    zMagHi = readRegister(mmc_i2cAddress, MMC3316xMT_REG_MAG_OUT_Z_H);
    // Conversion of the result
    // 16-bit signed result for Z-Axis Magnetic Data of MMC3316xMT
    mmc_magData.Z = (int16_t)(zMagHi << 8 | zMagLo);
    if (mmc_magData.Z > 8191)
    {
        mmc_magData.Z -= 16384;
    }
}
