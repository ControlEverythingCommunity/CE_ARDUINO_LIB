/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MMC2460MT
        This code is designed to work with the MMC2460MT_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Magnetic-Sensor?sku=MMC2460MT_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "MMC2460MT.h"

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
        Instantiates a new MMC2460MT class with appropriate properties
        Magnetometer Address
*/
/**************************************************************************/
void MMC2460MT::getAddr_MMC2460MT(uint8_t i2cAddress)
{
    mmc_i2cAddress = i2cAddress;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool MMC2460MT::begin()
{
    Wire.begin();
    
    // Set up the sensor for Magnetometer
    // setUpMagnetometer();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Sensor to Recharge the Capacitor at CAP pin, it is Requested to be Issued Before SET/RESET Command
*/
/**************************************************************************/
void MMC2460MT::setMagRefill(mmcMagRefill_t magrefill)
{
    mmc_magrefill = magrefill;
}

/**************************************************************************/
/*
        Gets the Sensor to Recharge the Capacitor at CAP pin, it is Requested to be Issued Before SET/RESET Command
*/
/**************************************************************************/
mmcMagRefill_t MMC2460MT::getMagRefill()
{
    return mmc_magrefill;
}

/**************************************************************************/
/*
        Resets the Sensor by Passing a Large Current through Set/Reset Coil at a Reversed Direction
*/
/**************************************************************************/
void MMC2460MT::setMagReset(mmcMagReset_t magreset)
{
    mmc_magreset = magreset;
}

/**************************************************************************/
/*
        Gets the Reset Status of the Sensor by Passing a Large Current through Set/Reset Coil at a Reversed Direction
*/
/**************************************************************************/
mmcMagReset_t MMC2460MT::getMagReset()
{
    return mmc_magreset;
}

/**************************************************************************/
/*
        Sets the Sensor by Passing a Large Current through Set/Reset Coil
*/
/**************************************************************************/
void MMC2460MT::setMagSet(mmcMagSet_t magset)
{
    mmc_magset = magset;
}

/**************************************************************************/
/*
        Gets the Set Status of the Sensor by Passing a Large Current through Set/Reset Coil
*/
/**************************************************************************/
mmcMagSet_t MMC2460MT::getMagSet()
{
    return mmc_magset;
}

/**************************************************************************/
/*
        Sets the Sensor to Determine How Often the Chip will Take Measurements in Continuous Measurement Mode
*/
/**************************************************************************/
void MMC2460MT::setMagFrequency(mmcMagFrequency_t magfrequency)
{
    mmc_magfrequency = magfrequency;
}

/**************************************************************************/
/*
        Gets the Sensor to Determine How Often the Chip will Take Measurements in Continuous Measurement Mode
*/
/**************************************************************************/
mmcMagFrequency_t MMC2460MT::getMagFrequency()
{
    return mmc_magfrequency;
}

/**************************************************************************/
/*
        Sets the Enable Continuous Measurement Mode
*/
/**************************************************************************/
void MMC2460MT::setMagContMode(mmcMagContMode_t magcontmode)
{
    mmc_magcontmode = magcontmode;
}

/**************************************************************************/
/*
        Gets the Enable Continuous Measurement Mode
*/
/**************************************************************************/
mmcMagContMode_t MMC2460MT::getMagContMode()
{
    return mmc_magcontmode;
}

/**************************************************************************/
/*
        Sets the Initiate Measurementt Mode
*/
/**************************************************************************/
void MMC2460MT::setMagMeasurement(mmcMagMeasurement_t magmeasurement)
{
    mmc_magmeasurement = magmeasurement;
}

/**************************************************************************/
/*
        Gets the Initiate Measurement Mode
*/
/**************************************************************************/
mmcMagMeasurement_t MMC2460MT::getMagMeasurement()
{
    return mmc_magmeasurement;
}

/**************************************************************************/
/*
        Sets up the Magnetometer
*/
/**************************************************************************/
void MMC2460MT::setUpMagnetometer(void)
{
    // Set Up the Configuration for the Magnetometer Internal Control Register 0
    /*
     // Recharge the Capacitor at CAP pin, it is Requested to be Issued Before SET/RESET Command
     uint8_t control0 = mmc_magrefill;
     
     // Reset the Sensor by Passing a Large Current through Set/Reset Coil at a Reversed Direction
     control0 |= mmc_magreset;
     
     // Set the Sensor by Passing a Large Current through Set/Reset Coil
     control0 |= mmc_magset;
     
     // Set the Sensor to Determine How Often the Chip will Take Measurements in Continuous Measurement Mode
     control0 |= mmc_magfrequency;
     
     // Set to Enable the Continuous Measurement Mode
     control0 |= mmc_magcontmode;
     
     // Set to Initiate Measurement Mode
     control0 |= mmc_magmeasurement;
    */
    
    uint8_t control0 =  MMC2460MT_REG_MAG_CTRL_REG0_REFILL_NO_RECHARGE  |   // No Recharge pf the Capacitor
                        MMC2460MT_REG_MAG_CTRL_REG0_RESET_NO            |   // No Reset the Sensor
                        MMC2460MT_REG_MAG_CTRL_REG0_SET_YES             |   // Set the Sensor
                        MMC2460MT_REG_MAG_CTRL_REG0_CM_FREQ_50HZ        |   // Frequency: 50Hz
                        MMC2460MT_REG_MAG_CTRL_REG0_CONT_MODE_ENABLE    |   // Enable Continuous Measurement Mode
                        MMC2460MT_REG_MAG_CTRL_REG0_TM_INITIATE;           // Initiate Measurement
    
    // Write the configuration to the Magnetometer Internal Control Register 0
    writeRegister(mmc_i2cAddress, MMC2460MT_REG_MAG_CTRL_REG0, control0);
    
    // Wait for the configuration to complete
    delay(mmc_conversionDelay);
}

/**************************************************************************/
/*
        Reads the 2 axes of the Magnetometer
        The value is expressed in 14 bits, unsigned format
*/
/**************************************************************************/
void MMC2460MT::Measure_Magnetometer()
{
    // Read the Magnetometer
    uint8_t xMagLo, xMagHi, yMagLo, yMagHi;
    
    // Read the Data
    // Reading the Low X-Axis Magnetic Data Register
    xMagLo = readRegister(mmc_i2cAddress, MMC2460MT_REG_MAG_OUT_X_L);
    // Reading the High X-Axis Magnetic Data Register
    xMagHi = readRegister(mmc_i2cAddress, MMC2460MT_REG_MAG_OUT_X_H);
    // Conversion of the results
    // 16-bit signed result for X-Axis Magnetic Data of MMC2460MT
    mmc_magData.X = (uint16_t)((xMagHi << 8) | xMagLo);
    
    // Reading the Low Y-Axis Magnetic Data Register
    yMagLo = readRegister(mmc_i2cAddress, MMC2460MT_REG_MAG_OUT_Y_L);
    // Reading the High Y-Axis Magnetic Data Register
    yMagHi = readRegister(mmc_i2cAddress, MMC2460MT_REG_MAG_OUT_Y_H);
    // Conversion of the result
    // 16-bit signed result for Y-Axis Magnetic Data of MMC2460MT
    mmc_magData.Y = (uint16_t)((yMagHi << 8) | yMagLo);
}
