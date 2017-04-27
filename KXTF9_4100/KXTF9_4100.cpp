/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        KXTF9_4100
        This code is designed to work with the KXTF9_4100_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "KXTF9_4100.h"

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
        Instantiates a new KXTF9_4100 class with appropriate properties
        Accelerometer Address
*/
/**************************************************************************/
void KXTF9_4100::getAddr_KXTF9_4100(uint8_t i2cAddress)
{
    kxtf_i2cAddress = i2cAddress;
    kxtf_conversionDelay = KXTF9_4100_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool KXTF9_4100::begin()
{
    Wire.begin();
    
    uint8_t devid = readRegister(kxtf_i2cAddress, KXTF9_4100_REG_ACCEL_WHO_AM_I);
    if (devid != KXTF9_4100_DEFAULT_DEV_ID)
        return false;
    
    // Set up the sensor for Accelerometer
    // setUpAccelerometer();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Operating Mode of the KXTF9
*/
/**************************************************************************/
void KXTF9_4100::setAccelOpMode(kxtfAccelOpMode_t accelopmode)
{
    kxtf_accelopmode = accelopmode;
}

/**************************************************************************/
/*
        Gets the Operating Mode of the KXTF9
*/
/**************************************************************************/
kxtfAccelOpMode_t KXTF9_4100::getAccelOpMode()
{
    return kxtf_accelopmode;
}

/**************************************************************************/
/*
        Sets the Performance Mode of the KXTF9
*/
/**************************************************************************/
void KXTF9_4100::setAccelPerformMode(kxtfAccelPerformMode_t accelperformmode)
{
    kxtf_accelperformmode = accelperformmode;
}

/**************************************************************************/
/*
        Gets the Performance Mode of the KXTF9
*/
/**************************************************************************/
kxtfAccelPerformMode_t KXTF9_4100::getAccelPerformMode()
{
    return kxtf_accelperformmode;
}

/**************************************************************************/
/*
        Sets the Reporting of the Availability of New Acceleration Data 
        on the Interrupt
*/
/**************************************************************************/
void KXTF9_4100::setAccelData(kxtfAccelData_t acceldata)
{
    kxtf_acceldata = acceldata;
}

/**************************************************************************/
/*
        Gets the Reporting of the Availability of New Acceleration Data 
        on the Interrupt
*/
/**************************************************************************/
kxtfAccelData_t KXTF9_4100::getAccelData()
{
    return kxtf_acceldata;
}

/**************************************************************************/
/*
        Sets the Acceleration Range of the Accelerometer Outputs
*/
/**************************************************************************/
void KXTF9_4100::setAccelRange(kxtfAccelRange_t accelrange)
{
    kxtf_accelrange = accelrange;
}

/**************************************************************************/
/*
        Gets the Acceleration Range of the Accelerometer Outputs
*/
/**************************************************************************/
kxtfAccelRange_t KXTF9_4100::getAccelRange()
{
    return kxtf_accelrange;
}

/**************************************************************************/
/*
        Sets the Directional TapTM Function
*/
/**************************************************************************/
void KXTF9_4100::setAccelTapTM(kxtfAccelTapTM_t acceltaptm)
{
    kxtf_acceltaptm = acceltaptm;
}

/**************************************************************************/
/*
        Gets the Directional TapTM Function
*/
/**************************************************************************/
kxtfAccelTapTM_t KXTF9_4100::getAccelTapTM()
{
    return kxtf_acceltaptm;
}

/**************************************************************************/
/*
        Sets the Wake Up (Motion Detect) Function
*/
/**************************************************************************/
void KXTF9_4100::setAccelWakeUp(kxtfAccelWakeUp_t accelwakeup)
{
    kxtf_accelwakeup = accelwakeup;
}

/**************************************************************************/
/*
        Gets the Wake Up (Motion Detect) Function
*/
/**************************************************************************/
kxtfAccelWakeUp_t KXTF9_4100::getAccelWakeUp()
{
    return kxtf_accelwakeup;
}

/**************************************************************************/
/*
        Sets the Tilt Position Function
*/
/**************************************************************************/
void KXTF9_4100::setAccelTilt(kxtfAccelTilt_t acceltilt)
{
    kxtf_acceltilt = acceltilt;
}

/**************************************************************************/
/*
        Gets the Tilt Position Function
*/
/**************************************************************************/
kxtfAccelTilt_t KXTF9_4100::getAccelTilt()
{
    return kxtf_acceltilt;
}

/**************************************************************************/
/*
        Sets up the Accelerometer
*/
/**************************************************************************/
void KXTF9_4100::setUpAccelerometer(void)
{
    // Set Up the Configuration for the Accelerometer Control Register 1
    /*
     // Operating Mode of the KXTF9
     uint8_t control1 = kxtf_opmode;
    
     // Performance Mode of the KXTF9
     control1 |= kxtf_performmode;
     
     // Reporting of the Availability of New Acceleration Data on the Interrupt
     control1 |= kxtf_acceldata;
     
     // Acceleration Range of the Accelerometer Outputs
     control1 |= kxtf_accelrange;
     
     // Directional TapTM Function
     control1 |= kxtf_acceltaptm;
     
     // Wake Up (Motion Detect) Function
     control1 |= kxtf_accelwakeup;
     
     // Tilt Position Function
     control1 |= kxtf_acceltilt;
    */
    
    uint8_t control1 =  KXTF9_4100_REG_ACCEL_CTRL_REG1_PC1_OPERATE      |   // Operating Mode
                        KXTF9_4100_REG_ACCEL_CTRL_REG1_RES_1            |   // High Current, 12-bit Valid
                        KXTF9_4100_REG_ACCEL_CTRL_REG1_DRDYE_NOT        |   // Availability of New Acceleration Data Not Reflected on Interrupt Pin (7)
                        KXTF9_4100_REG_ACCEL_CTRL_REG1_TDTE_ENABLE      |   // Directional TapTM Function Enable
                        KXTF9_4100_REG_ACCEL_CTRL_REG1_WUFE_ENABLE      |   // Wake Up (Motion Detect) Function Enable
                        KXTF9_4100_REG_ACCEL_CTRL_REG1_TPE_ENABLE;          // Tilt Position Function Enable
    
    // Set the Acceleration Range of the Accelerometer Outputs
    control1 |= kxtf_accelrange;
    
    // Write the configuration to the Accelerometer Control Register 1
    writeRegister(kxtf_i2cAddress, KXTF9_4100_REG_ACCEL_CTRL_REG1, control1);
    
    // Wait for the configuration to complete
    delay(kxtf_conversionDelay);
    
}

/**************************************************************************/
/*
        Reads the 3 axes of the Accelerometer
        The value is expressed in 12 bit as two’s complement
*/
/**************************************************************************/
void KXTF9_4100::Measure_Accelerometer()
{
    // Read the Accelerometer
    uint8_t xAccelLo, xAccelHi, yAccelLo, yAccelHi, zAccelLo, zAccelHi;
    
    // Read the Data
    // Reading the Low X-Axis Acceleration Data Register
    xAccelLo = readRegister(kxtf_i2cAddress, KXTF9_4100_REG_ACCEL_XOUT_L);
    // Reading the High X-Axis Acceleration Data Register
    xAccelHi = readRegister(kxtf_i2cAddress, KXTF9_4100_REG_ACCEL_XOUT_H);
    // Conversion of the result
    // 12-bit signed result for X-Axis Acceleration Data of KXTF9-4100
    kxtf_accelData.X = (int16_t)((xAccelHi << 8) | xAccelLo);
    kxtf_accelData.X >>= 4;

    // Reading the Low Y-Axis Acceleration Data Register
    yAccelLo = readRegister(kxtf_i2cAddress, KXTF9_4100_REG_ACCEL_YOUT_L);
    // Reading the High Y-Axis Acceleration Data Register
    yAccelHi = readRegister(kxtf_i2cAddress, KXTF9_4100_REG_ACCEL_YOUT_H);
    // Conversion of the result
    // 12-bit signed result for Y-Axis Acceleration Data of KXTF9-4100
    kxtf_accelData.Y = (int16_t)((yAccelHi << 8) | yAccelLo);
    kxtf_accelData.Y >>= 4;
    
    // Reading the Low Z-Axis Acceleration Data Register
    zAccelLo = readRegister(kxtf_i2cAddress, KXTF9_4100_REG_ACCEL_ZOUT_L);
    // Reading the High Z-Axis Acceleration Data Register
    zAccelHi = readRegister(kxtf_i2cAddress, KXTF9_4100_REG_ACCEL_ZOUT_H);
    // Conversion of the result
    // 12-bit signed result for Z-Axis Acceleration Data of KXTF9-4100
    kxtf_accelData.Z = (int16_t)((zAccelHi << 8) | zAccelLo);
    kxtf_accelData.Z >>= 4;
    
}

