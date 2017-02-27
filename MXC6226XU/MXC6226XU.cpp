/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MXC6226XU
        This code is designed to work with the MXC6226XU_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelorometer?sku=MXC6226XU_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "MXC6226XU.h"

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
        Instantiates a new MXC6226XU class with appropriate properties
*/
/**************************************************************************/
void MXC6226XU::getAddr_MXC6226XU(uint8_t i2cAddress)
{
    mxc_i2cAddress = i2cAddress;
    mxc_conversionDelay = MXC6226XU_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool MXC6226XU::begin()
{
    Wire.begin();
    
    uint8_t chipid= readRegister(mxc_i2cAddress, MXC6226XU_CHIP_ID);
    if (chipid != MXC6226XU_CHIP_ID)
        return false;
    
    // Set up the sensor for Accelerometer
    // setUpAccelerometer();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Power Down DTOS
*/
/**************************************************************************/
void MXC6226XU::setDTOSEnable(mxcDTOSEnable_t dtosenable)
{
    mxc_dtosenable = dtosenable;
}

/**************************************************************************/
/*
        Gets the Power Down DTOS
*/
/**************************************************************************/
mxcDTOSEnable_t MXC6226XU::getDTOSEnable()
{
    return mxc_dtosenable;
}

/**************************************************************************/
/*
        Sets the Shake Mode Bit Status
*/
/**************************************************************************/
void MXC6226XU::setShakeMode(mxcShakeMode_t shakemode)
{
    mxc_shakemode = shakemode;
}

/**************************************************************************/
/*
        Gets the Shake Mode Bit Status
*/
/**************************************************************************/
mxcShakeMode_t MXC6226XU::getShakeMode()
{
    return mxc_shakemode;
}

/**************************************************************************/
/*
        Sets the Shake Threshold that the Perpendicular Acceleration
        must Exceed to Trigger the First Shake Event
*/
/**************************************************************************/
void MXC6226XU::setShakeThreshold(mxcShakeThreshold_t shakethreshold)
{
    mxc_shakethreshold = shakethreshold;
}

/**************************************************************************/
/*
        Gets the Shake Threshold that the Perpendicular Acceleration
        must Exceed to Trigger the First Shake Event
*/
/**************************************************************************/
mxcShakeThreshold_t MXC6226XU::getShakeThreshold()
{
    return mxc_shakethreshold;
}

/**************************************************************************/
/*
        Sets the Shake Events Time Window, Which Determines the Time Window 
        Allowed Between the First Shake Event (Perpendicular Acceleration 
        Exceeding the Threshold Set by SHTH[1:0]) and the Second Shake Event 
        (Acceleration Breaking the Threshold with Opposite Sign, SHM = 0, 
        or just Reversing Sign, SHM = 1)
*/
/**************************************************************************/
void MXC6226XU::setShakEvenTime(mxcShakEvenTime_t shakeventime)
{
    mxc_shakeventime = shakeventime;
}

/**************************************************************************/
/*
        Gets the Shake Events Time Window, Which Determines the Time Window
        Allowed Between the First Shake Event (Perpendicular Acceleration
        Exceeding the Threshold Set by SHTH[1:0]) and the Second Shake Event
        (Acceleration Breaking the Threshold with Opposite Sign, SHM = 0,
        or just Reversing Sign, SHM = 1)
*/
/**************************************************************************/
mxcShakEvenTime_t MXC6226XU::getShakEvenTime()
{
    return mxc_shakeventime;
}

/**************************************************************************/
/*
        Sets the Orientation Hysteresis Time Period, Which is the Time Period
        of Consecutive Valid New Instantaneous Orientation Measurements that 
        Must be Made Before a New Orientation Value is Written into Bits 
        OR[1:0] in the STATUS Register
*/
/**************************************************************************/
void MXC6226XU::setOrientationTime(mxcOrientationTime_t orientationtime)
{
    mxc_orientationtime = orientationtime;
}

/**************************************************************************/
/*
        Sets the Orientation Hysteresis Time Period, Which is the Time Period
        of Consecutive Valid New Instantaneous Orientation Measurements that
        Must be Made Before a New Orientation Value is Written into Bits
        OR[1:0] in the STATUS Register
*/
/**************************************************************************/
mxcOrientationTime_t MXC6226XU::getOrientationTime()
{
    return mxc_orientationtime;
}

/**************************************************************************/
/*
        Sets up the Accelerometer
*/
/**************************************************************************/
void MXC6226XU::setUpAccelerometer(void)
{
    // Set Up the Configuration for the Detection Register 1
    /*
     // Set the Power Down DTOS
     uint8_t detection = mxc_dtosenable;
     
     // Set the Shake Mode Bit Status
     detection |= mxc_shakemode;
     
     // Set the Shake Threshold
     detection |= mxc_shakethreshold;
     
     // Set the Shake Events Time Window
     detection |= mxc_shakeventime;
     
     // Set the Orientation Hysteresis Time Period
     detection |= mxc_orientationtime;
    */
    
    uint8_t detection =     MXC6226XU_REG_ACCEL_DETECTION_PD_DISABLED       |   // Disables the Power Down of the DTOS to a Non-Functional Low Power State with a Maximum Current Drain of 1 uA
                            MXC6226XU_REG_ACCEL_DETECTION_SHM_0             |   // For a Shake to be Detected, and Written to SH[1:0] in the STATUS Register, the Second Shake Event Must Break the Threshold Set by SHTH[1:0] with the Opposite Sign of the First Shake Event, Within the Time Window Set by SHC[1:0]
                            MXC6226XU_REG_ACCEL_DETECTION_SHTH_0_5          |   // Shake Threshold: 0.5 g
                            MXC6226XU_REG_ACCEL_DETECTION_SHC_80            |   // Shake Events Time Window: 80 ms
                            MXC6226XU_REG_ACCEL_DETECTION_ORC_160;              // Orientation Hysteresis Time Period: 160 ms
        
    // Write the configuration to the Detection Register 1
    writeRegister(mxc_i2cAddress, MXC6226XU_REG_ACCEL_DETECTION, detection);
    
    // Wait for the configuration to complete
    delay(mxc_conversionDelay);
}

/**************************************************************************/
/*
        Reads the results for the Accelerometer MXC6226XU
*/
/**************************************************************************/
void MXC6226XU::Measure_Accelerometer(void)
{
    // Read the Accelerometer Data
    // Reading the signed 8-bit X-Axis Acceleration Output Register
    mxc_accelData.X = readRegister(mxc_i2cAddress, MXC6226XU_REG_ACCEL_OUT_X);
    
    // Reading the signed 8-bit Y-Axis Acceleration Output Register
    mxc_accelData.Y = readRegister(mxc_i2cAddress, MXC6226XU_REG_ACCEL_OUT_Y);
}
