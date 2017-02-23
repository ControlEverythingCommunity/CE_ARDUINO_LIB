/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MMA7455
        This code is designed to work with the MMA7455_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "MMA7455.h"

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
        Instantiates a new MMA7455 class with appropriate properties
        Accelerometer Address
*/
/**************************************************************************/
void MMA7455::getAddr_MMA7455(uint8_t i2cAddress)
{
    mma_i2cAddress = i2cAddress;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool MMA7455::begin()
{
    Wire.begin();
    
    // Set up the sensor for Accelerometer
    // setUpAccelerometer();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Data Ready Status for the Accelerometer
*/
/**************************************************************************/
void MMA7455::setAccelDRPD(mmaAccelDRPD_t acceldrpd)
{
    mma_acceldrpd = acceldrpd;
}

/**************************************************************************/
/*
        Gets the Data Ready Status for the Accelerometer
*/
/**************************************************************************/
mmaAccelDRPD_t MMA7455::getAccelDRPD()
{
    return mma_acceldrpd;
}

/**************************************************************************/
/*
        Sets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
void MMA7455::setAccelSPIWire(mmaAccelSPIWire_t accelspiwire)
{
    mma_accelspiwire = accelspiwire;
}

/**************************************************************************/
/*
        Gets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
mmaAccelSPIWire_t MMA7455::getAccelSPIWire()
{
    return mma_accelspiwire;
}

/**************************************************************************/
/*
        Sets the Self-Test Mode Configuration
*/
/**************************************************************************/
void MMA7455::setAccelSelfTest(mmaAccelSelfTest_t accelselftest)
{
    mma_accelselftest = accelselftest;
}

/**************************************************************************/
/*
        Gets the Self-Test Mode Configuration
*/
/**************************************************************************/
mmaAccelSelfTest_t MMA7455::getAccelSelfTest()
{
    return mma_accelselftest;
}

/**************************************************************************/
/*
        Sets the Acceleration Full-Scale Selection
*/
/**************************************************************************/
void MMA7455::setAccelRange(mmaAccelRange_t accelrange)
{
    mma_accelrange = accelrange;
}

/**************************************************************************/
/*
        Gets the Acceleration Full-Scale Selection
*/
/**************************************************************************/
mmaAccelRange_t MMA7455::getAccelRange()
{
    return mma_accelrange;
}

/**************************************************************************/
/*
        Sets the Operating Mode Selection
*/
/**************************************************************************/
void MMA7455::setAccelMode(mmaAccelMode_t accelmode)
{
    mma_accelmode = accelmode;
}

/**************************************************************************/
/*
        Gets the Operating Mode Selection
*/
/**************************************************************************/
mmaAccelMode_t MMA7455::getAccelMode()
{
    return mma_accelmode;
}

/**************************************************************************/
/*
        Sets the Digital Filter Band Width
*/
/**************************************************************************/
void MMA7455::setAccelBandwidth(mmaAccelBandwidth_t accelbandwidth)
{
    mma_accelbandwidth = accelbandwidth;
}

/**************************************************************************/
/*
        Gets the Digital Filter Band Width
*/
/**************************************************************************/
mmaAccelBandwidth_t MMA7455::getAccelBandwidth()
{
    return mma_accelbandwidth;
}

/**************************************************************************/
/*
        Sets the Threshold Value
*/
/**************************************************************************/
void MMA7455::setAccelThreshold(mmaAccelThreshold_t accelthreshold)
{
    mma_accelthreshold = accelthreshold;
}

/**************************************************************************/
/*
        Gets the Threshold Value
*/
/**************************************************************************/
mmaAccelThreshold_t MMA7455::getAccelThreshold()
{
    return mma_accelthreshold;
}


/**************************************************************************/
/*
        Sets the Acceleration Z-Axis Enable
*/
/**************************************************************************/
void MMA7455::setAccelZda(mmaAccelZda_t accelzda)
{
    mma_accelzda = accelzda;
}

/**************************************************************************/
/*
        Gets the Acceleration Z-Axis Enable
*/
/**************************************************************************/
mmaAccelZda_t MMA7455::getAccelZda()
{
    return mma_accelzda;
}

/**************************************************************************/
/*
        Sets the Acceleration Y-Axis Enable
*/
/**************************************************************************/
void MMA7455::setAccelYda(mmaAccelYda_t accelyda)
{
    mma_accelyda = accelyda;
}

/**************************************************************************/
/*
        Gets the Acceleration Y-Axis Enable
*/
/**************************************************************************/
mmaAccelYda_t MMA7455::getAccelYda()
{
    return mma_accelyda;
}

/**************************************************************************/
/*
        Sets the Acceleration X-Axis Enable
*/
/**************************************************************************/
void MMA7455::setAccelXda(mmaAccelXda_t accelxda)
{
    mma_accelxda = accelxda;
}

/**************************************************************************/
/*
        Gets the Acceleration X-Axis Enable
*/
/**************************************************************************/
mmaAccelXda_t MMA7455::getAccelXda()
{
    return mma_accelxda;
}

/**************************************************************************/
/*
        Sets the Interrupt Settings
*/
/**************************************************************************/
void MMA7455::setAccelInterrupt(mmaAccelInterrupt_t accelinterrupt)
{
    mma_accelinterrupt = accelinterrupt;
}

/**************************************************************************/
/*
        Gets the Interrupt Settings
*/
/**************************************************************************/
mmaAccelInterrupt_t MMA7455::getAccelInterrupt()
{
    return mma_accelinterrupt;
}

/**************************************************************************/
/*
        Sets up the Accelerometer
*/
/**************************************************************************/
void MMA7455::setUpAccelerometer(void)
{
    // Set Up the Configuration for the Accelerometer Mode Control Register
    /*
     // Set the Data Ready Status
     uint8_t mode_control = mma_acceldrpd;
    
     // Set the SPI Serial Interface Mode Selection
     mode_control |= mma_sccelspiwire;
    
     // Set the Self-Test Mode
     mode_control |= mma_accelselftest;
    
     // Set the Acceleration Measurement Range
     mode_control |= mma_accelrange;
     
     // Set the Operating Mode
     mode_control |= mma_accelmode;
    */
    
    uint8_t mode_control =   MMA7455_REG_ACCEL_MCTL_DRPD_OUTPUT     |   // Data Ready Status is Output to INT1/DRDY PIN
                        MMA7455_REG_ACCEL_MCTL_SPI3W_4WIRE          |   // 4-Wire Interface
                        MMA7455_REG_ACCEL_MCTL_STON_DISABLE         |   // Self-Test Disabled
                        MMA7455_REG_ACCEL_MCTL_MODE_MEAS;               // Measurement Mode
    
    // Set the Acceleration Measurement Range
    mode_control |= mma_accelrange;
    
    // Write the configuration to the Accelerometer Mode Control Register
    writeRegister(mma_i2cAddress, MMA7455_REG_ACCEL_MCTL, mode_control);
    
    // Wait for the configuration to complete
    delay(mma_conversionDelay);
    
    // Set Up the Configuration for the Accelerometer Control Register 1
    /*
     // Set the Digital Filter Band Width
     uint8_t control1 = mma_accelbandwidth;
     
     // Set the  Threshold Value
     control1 |= mma_accelthreshold;
     
     // Set the Acceleration Z-Axis Enable
     control1 |= mma_accelyda;
     
     // Set the Acceleration Y-Axis Enable
     control1 |= mma_accelzda;
     
     // Set the Acceleration X-Axis Enable
     control1 |= mma_accelxda;
     
     // Set the Interrupt Settings
     control1 |= mma_accelinterrupt;
    */
    
    uint8_t control1 =  MMA7455_REG_ACCEL_CTL1_DFBW_62_5HZ          |   // Digital Filter Band Width: 62.5 Hz
                        MMA7455_REG_ACCEL_CTL1_THOPT_ABSOLUTE       |   // Threshold Value is Absolute Only
                        MMA7455_REG_ACCEL_CTL1_ZDA_ENABLE           |   // Acceleration Z-Axis Enabled
                        MMA7455_REG_ACCEL_CTL1_YDA_ENABLE           |   // Acceleration Y-Axis Enabled
                        MMA7455_REG_ACCEL_CTL1_XDA_ENABLE           |   // Acceleration X-Axis Enabled
                        MMA7455_REG_ACCEL_CTL1_INTREG_LEVEL;            // Level Detection
    
    
    // Write the configuration to the Accelerometer Control Register 1
    writeRegister(mma_i2cAddress, MMA7455_REG_ACCEL_CTL1, control1);
    
    // Wait for the configuration to complete
    delay(mma_conversionDelay);
}

/**************************************************************************/
/*
        Reads the 3 axes of the Accelerometer
        The value is expressed in 10 bit as two’s complement
*/
/**************************************************************************/
void MMA7455::Measure_Accelerometer()
{
    // Read the Accelerometer
    uint8_t xAccelLo, xAccelHi, yAccelLo, yAccelHi, zAccelLo, zAccelHi;
    
    // Read the Data
    // Reading the Low X-Axis Acceleration Data Register
    xAccelLo = readRegister(mma_i2cAddress, MMA7455_REG_ACCEL_XOUTL);
    // Reading the High X-Axis Acceleration Data Register
    xAccelHi = readRegister(mma_i2cAddress, MMA7455_REG_ACCEL_XOUTH);
    // Conversion of the result
    // 10-bit signed result for X-Axis Acceleration Data of MMA7455
    mma_accelData.X = (int16_t)((xAccelHi << 8) | xAccelLo);
    if (mma_accelData.X > 511)
        mma_accelData.X -= 1024;
    
    // Reading the Low Y-Axis Acceleration Data Register
    yAccelLo = readRegister(mma_i2cAddress, MMA7455_REG_ACCEL_YOUTL);
    // Reading the High Y-Axis Acceleration Data Register
    yAccelHi = readRegister(mma_i2cAddress, MMA7455_REG_ACCEL_YOUTH);
    // Conversion of the result
    // 10-bit signed result for Y-Axis Acceleration Data of MMA7455
    mma_accelData.Y = (int16_t)((yAccelHi << 8) | yAccelLo);
    if (mma_accelData.Y > 511)
        mma_accelData.Y -= 1024;
    
    // Reading the Low Z-Axis Acceleration Data Register
    zAccelLo = readRegister(mma_i2cAddress, MMA7455_REG_ACCEL_ZOUTL);
    // Reading the High Z-Axis Acceleration Data Register
    zAccelHi = readRegister(mma_i2cAddress, MMA7455_REG_ACCEL_ZOUTH);
    // Conversion of the result
    // 10-bit signed result for Z-Axis Acceleration Data of MMA7455
    mma_accelData.Z = (int16_t)((zAccelHi << 8) | zAccelLo);
    if (mma_accelData.Z > 511)
        mma_accelData.Z -= 1024;
}

