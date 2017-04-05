/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MXC6232xM
        This code is designed to work with the MXC6232xM_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/low-power-2g-dual-axis-accelerometer?variant=25687720907#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "MXC6232xM.h"

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
static void writeRegister(uint8_t i2cAddress, uint8_t value)
{
    Wire.beginTransmission(i2cAddress);
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
    return (int8_t)(i2cread());
}


/**************************************************************************/
/*
        Instantiates a new MXC6232xM class with appropriate properties
        Accelerometer Address
*/
/**************************************************************************/
void MXC6232xM::getAddr_MXC6232xM(uint8_t i2cAddress)
{
    mxc_i2cAddress = i2cAddress;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool MXC6232xM::begin()
{
    Wire.begin();
    
    // Set up the sensor for Accelerometer
    // setUpAccelerometer();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Temperature Out Status
*/
/**************************************************************************/
void MXC6232xM::setAccelTempOut(mxcAccelTempOut_t acceltempout)
{
    mxc_acceltempout = acceltempout;
}

/**************************************************************************/
/*
        Gets the Temperature Out Status
*/
/**************************************************************************/
mxcAccelTempOut_t MXC6232xM::getAccelTempOut()
{
    return mxc_acceltempout;
}

/**************************************************************************/
/*
        Sets the BandGap Test Status
*/
/**************************************************************************/
void MXC6232xM::setAccelBandgap(mxcAccelBandgap_t accelbandgap)
{
    mxc_accelbandgap = accelbandgap;
}

/**************************************************************************/
/*
        Gets the BandGap Test Status
*/
/**************************************************************************/
mxcAccelBandgap_t MXC6232xM::getAccelBandgap()
{
    return mxc_accelbandgap;
}

/**************************************************************************/
/*
        Sets the Self Test Status
*/
/**************************************************************************/
void MXC6232xM::setAccelSelfTest(mxcAccelSelfTest_t accelselftest)
{
    mxc_accelselftest = accelselftest;
}

/**************************************************************************/
/*
        Gets the Self Test Status
*/
/**************************************************************************/
mxcAccelSelfTest_t MXC6232xM::getAccelSelfTest()
{
    return mxc_accelselftest;
}

/**************************************************************************/
/*
        Sets the Power Status
*/
/**************************************************************************/
void MXC6232xM::setAccelPower(mxcAccelPower_t accelpower)
{
    mxc_accelpower = accelpower;
}

/**************************************************************************/
/*
        Gets the Power Status
*/
/**************************************************************************/
mxcAccelPower_t MXC6232xM::getAccelPower()
{
    return mxc_accelpower;
}

/**************************************************************************/
/*
        Sets up the Accelerometer
*/
/**************************************************************************/
void MXC6232xM::setUpAccelerometer(void)
{
    // Set Up the Configuration for the Accelerometer Control Register
    /*
     // Set the Temperature Out Status
     uint8_t control = mxc_acceltempout;
    
     // Set the BandGap Test Status
     control |= mxc_accelbandgap;
    
     // Set the Self Test Status
     control |= mxc_accelselftest;
    
     // Set the Power Status
     control |= mxc_accelpower;
    */
    
    uint8_t control =   MXC6232xM_REG_ACCEL_IR_TOEN_DISABLE     |   // Temperature Out Disable
                        MXC6232xM_REG_ACCEL_IR_BGTST_NORMAL     |   // Normal Test
                        MXC6232xM_REG_ACCEL_IR_ST_OFF           |   // Self Test Off
                        MXC6232xM_REG_ACCEL_IR_PD_ON;               // Power On
    
    // Write the configuration to the Accelerometer Control Register
    writeRegister(mxc_i2cAddress, control);
    
    // Wait for the configuration to complete
    delay(mxc_conversionDelay);
}

/**************************************************************************/
/*
        Reads the 2 axes of the Accelerometer
        The value is expressed in 16 bit as two’s complement
*/
/**************************************************************************/
void MXC6232xM::Measure_Accelerometer()
{
    // Read the Accelerometer
    int8_t internal, xAccelHi, xAccelLo, yAccelHi, yAccelLo;
    
    // Read the Data
    // Reading the Internal Register and the 2 Axes Acceleration Data Registers
    Wire.beginTransmission(mxc_i2cAddress);
    i2cwrite((uint8_t)MXC6232xM_REG_ACCEL_DATA);
    Wire.endTransmission();
    Wire.requestFrom(mxc_i2cAddress, (uint8_t)5);
    internal = i2cread();
    xAccelHi = i2cread();
    xAccelLo = i2cread();
    yAccelHi = i2cread();
    yAccelLo = i2cread();
    
    // Conversion of the result
    // 16-bit signed result for X-Axis Acceleration Data of MXC6232xM
    mxc_accelData.X = (int16_t)((xAccelHi << 8) | xAccelLo);
    
    // Conversion of the result
    // 16-bit signed result for Y-Axis Acceleration Data of MXC6232xM
    mxc_accelData.Y = (int16_t)((yAccelHi << 8) | yAccelLo);
    
}

