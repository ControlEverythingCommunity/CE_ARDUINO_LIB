/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LIS331HH
        This code is designed to work with the LIS331HH_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelorometer?sku=LIS331HH_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "LIS331HH.h"

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
        Instantiates a new LIS331HH class with appropriate properties
*/
/**************************************************************************/
void LIS331HH::getAddr_LIS331HH(uint8_t i2cAddress)
{
    lis_i2cAddress = i2cAddress;
    lis_conversionDelay = LIS331HH_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool LIS331HH::begin()
{
    Wire.begin();
    
    // Set up the sensor for Accelerometer
    // setUpAccelerometer();
    
    return true;

}

/**************************************************************************/
/*
        Sets the Power mode and Low-Power Output Data Rate Configurations
*/
/**************************************************************************/
void LIS331HH::setAccelPowerMode(lisAccelPowerMode_t accelpowermode)
{
    lis_accelpowermode = accelpowermode;
}

/**************************************************************************/
/*
        Gets the Power Mode and Low-power Output Data Rate Configurations
*/
/**************************************************************************/
lisAccelPowerMode_t LIS331HH::getAccelPowerMode()
{
    return lis_accelpowermode;
}

/**************************************************************************/
/*
        Sets the Normal-Mode Output Data Rate Configurations and Low-Pass Cut-Off Frequencies
*/
/**************************************************************************/
void LIS331HH::setAccelDataRate(lisAccelDataRate_t acceldatarate)
{
    lis_acceldatarate = acceldatarate;
}
                    
/**************************************************************************/
/*
        Gets the Normal-Mode Output Data Rate Configurations and Low-Pass Cut-Off Frequencies
*/
/**************************************************************************/
lisAccelDataRate_t LIS331HH::getAccelDataRate()
{
    return lis_acceldatarate;
}

/**************************************************************************/
/*
        Sets the Acceleration Z-Axis Enable
*/
/**************************************************************************/
void LIS331HH::setAccelZen(lisAccelZen_t accelzen)
{
    lis_accelzen = accelzen;
}

/**************************************************************************/
/*
        Gets the Acceleration Z-Axis Enable
*/
/**************************************************************************/
lisAccelZen_t LIS331HH::getAccelZen()
{
    return lis_accelzen;
}

/**************************************************************************/
/*
        Sets the Acceleration Y-Axis Enable
*/
/**************************************************************************/
void LIS331HH::setAccelYen(lisAccelYen_t accelyen)
{
    lis_accelyen = accelyen;
}

/**************************************************************************/
/*
        Gets the Acceleration Y-Axis Enable
*/
/**************************************************************************/
lisAccelYen_t LIS331HH::getAccelYen()
{
    return lis_accelyen;
}

/**************************************************************************/
/*
        Sets the Acceleration X-Axis Enable
*/
/**************************************************************************/
void LIS331HH::setAccelXen(lisAccelXen_t accelxen)
{
    lis_accelxen = accelxen;
}

/**************************************************************************/
/*
        Gets the Acceleration X-Axis Enable
*/
/**************************************************************************/
lisAccelXen_t LIS331HH::getAccelXen()
{
    return lis_accelxen;
}

/**************************************************************************/
/*
        Sets the Block Data Update for Acceleration Data
*/
/**************************************************************************/
void LIS331HH::setAccelBlockData(lisAccelBlockData_t accelblockdata)
{
    lis_accelblockdata = accelblockdata;
}

/**************************************************************************/
/*
        Gets the Block Data Update for Acceleration Data
*/
/**************************************************************************/
lisAccelBlockData_t LIS331HH::getAccelBlockData()
{
    return lis_accelblockdata;
}

/**************************************************************************/
/*
        Sets the Big/Little Endian Data Selection
*/
/**************************************************************************/
void LIS331HH::setAccelEndianData(lisAccelEndianData_t accelendiandata)
{
    lis_accelendiandata = accelendiandata;
}

/**************************************************************************/
/*
        Gets the Big/Little Endian Data Selection
*/
/**************************************************************************/
lisAccelEndianData_t LIS331HH::getAccelEndianData()
{
    return lis_accelendiandata;
}

/**************************************************************************/
/*
        Sets the Acceleration Full-Scale Selection
*/
/**************************************************************************/
void LIS331HH::setAccelRange(lisAccelRange_t accelrange)
{
    lis_accelrange = accelrange;
}
                    
/**************************************************************************/
/*
        Gets the Acceleration Full-Scale Selection
*/
/**************************************************************************/
lisAccelRange_t LIS331HH::getAccelRange()
{
    return lis_accelrange;
}

/**************************************************************************/
/*
        Sets the Self-Test Sign Mode Configuration
*/
/**************************************************************************/
void LIS331HH::setAccelSelfTestSign(lisAccelSelfTestSign_t accelselftestsign)
{
    lis_accelselftestsign = accelselftestsign;
}

/**************************************************************************/
/*
        Gets the Self-Test Sign Mode Configuration
*/
/**************************************************************************/
lisAccelSelfTestSign_t LIS331HH::getAccelSelfTestSign()
{
    return lis_accelselftestsign;
}

/**************************************************************************/
/*
        Sets the Self-Test Mode Configuration
*/
/**************************************************************************/
void LIS331HH::setAccelSelfTest(lisAccelSelfTest_t accelselftest)
{
    lis_accelselftest = accelselftest;
}

/**************************************************************************/
/*
        Gets the Self-Test Mode Configuration
*/
/**************************************************************************/
lisAccelSelfTest_t LIS331HH::getAccelSelfTest()
{
    return lis_accelselftest;
}

/**************************************************************************/
/*
        Sets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
void LIS331HH::setAccelSPIWire(lisAccelSPIWire_t accelspiwire)
{
    lis_accelspiwire = accelspiwire;
}

/**************************************************************************/
/*
        Gets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
lisAccelSPIWire_t LIS331HH::getAccelSPIWire()
{
    return lis_accelspiwire;
}

/**************************************************************************/
/*
        Sets up the Accelerometer
*/
/**************************************************************************/
void LIS331HH::setUpAccelerometer(void)
{
    // Set Up the Configuration for the Accelerometer Control Register 1
    /*
    // Set the Power mode and Low-Power Output Data Rate Configurations
    uint8_t config1 = lis_accelpowermode;
     
    // Set the Acceleration Z-Axis Enable
    config1 |= lis_accelzen;
     
    // Set the Acceleration Y-Axis Enable
    config1 |= lis_accelyen;
     
    // Set the Acceleration X-Axis Enable
    config1 |= lis_accelxen;
    */
    
    uint8_t config1 =   LIS331HH_REG_ACCEL_CTRL_REG1_PM_NORMAL          |   // Normal Mode
                        LIS331HH_REG_ACCEL_CTRL_REG1_AZEN_ENABLE        |   // Acceleration Z-Axis Enabled
                        LIS331HH_REG_ACCEL_CTRL_REG1_AYEN_ENABLE        |   // Acceleration Y-Axis Enabled
                        LIS331HH_REG_ACCEL_CTRL_REG1_AXEN_ENABLE;           // Acceleration X-Axis Enabled
    
    // Set the Acceleration Data Rate Configuration
    config1 |= lis_acceldatarate;
        
    // Write the configuration to the Accelerometer Control Register 1
    writeRegister(lis_i2cAddress, LIS331HH_REG_ACCEL_CTRL_REG1, config1);
    
    // Wait for the configuration to complete
    delay(lis_conversionDelay);
    
    // Set Up the Configuration for the Accelerometer Control Register 4
    /*
    // Set the Accelerometer Block Data Update
    uint8_t config4 = lis_accelblockdata;
     
    // Set the Big/Little Endian Data Selection
    config4 |= lis_accelendiandata;
     
     // Set the Self-Test Sign
     config4 |= lis_accelselftestsign;
     
    // Set the Self-Test Enable
    config4 |= lis_accelselftest;
     
    // Set the SPI Serial Interface Mode Selection
    config4 |= lis_accelmagspiwire;
    */
    
    uint8_t config4 =   LIS331HH_REG_ACCEL_CTRL_REG4_BDU_CONTINUOUS     |   // Continuous Update
                        LIS331HH_REG_ACCEL_CTRL_REG4_BLE_LSB            |   // Data LSB @ lower address
                        LIS331HH_REG_ACCEL_CTRL_REG4_ST_SIGN_PLUS       |   // Self-Test Plus
                        LIS331HH_REG_ACCEL_CTRL_REG4_ST_DISABLED        |   // Self-Test Disabled
                        LIS331HH_REG_ACCEL_CTRL_REG4_SIM_4WIRE;             // 4-Wire Interface
    
    // Set the Acceleration Full-Scale Selection
    config4 |= lis_accelrange;
        
    // Write the configuration to the Accelerometer Control Register 4
    writeRegister(lis_i2cAddress, LIS331HH_REG_ACCEL_CTRL_REG4, config4);
    
    // Wait for the configuration to complete
    delay(lis_conversionDelay);
}

/**************************************************************************/
/*
        Reads the results for the Accelerometer LIS331HH
*/
/**************************************************************************/
void LIS331HH::Measure_Accelerometer(void)
{
    // Read the Accelerometer
    uint8_t xAccelLo, xAccelHi, yAccelLo, yAccelHi, zAccelLo, zAccelHi;
    
    // Read the Data
    // Reading the Low X-Axis Acceleration Data Register
    xAccelLo = readRegister(lis_i2cAddress, LIS331HH_REG_ACCEL_OUT_X_L);
    // Reading the High X-Axis Acceleration Data Register
    xAccelHi = readRegister(lis_i2cAddress, LIS331HH_REG_ACCEL_OUT_X_H);
    // Conversion of the result
    // 16-bit signed result for X-Axis Acceleration Data of LIS331HH
    lis_accelData.X = (int16_t)((xAccelHi << 8) | xAccelLo);
    
    // Reading the Low Y-Axis Acceleration Data Register
    yAccelLo = readRegister(lis_i2cAddress, LIS331HH_REG_ACCEL_OUT_Y_L);
    // Reading the High Y-Axis Acceleration Data Register
    yAccelHi = readRegister(lis_i2cAddress, LIS331HH_REG_ACCEL_OUT_Y_H);
    // Conversion of the result
    // 16-bit signed result for Y-Axis Acceleration Data of LIS331HH
    lis_accelData.Y = (int16_t)((yAccelHi << 8) | yAccelLo);
    
    // Reading the Low Z-Axis Acceleration Data Register
    zAccelLo = readRegister(lis_i2cAddress, LIS331HH_REG_ACCEL_OUT_Z_L);
    // Reading the High Z-Axis Acceleration Data Register
    zAccelHi = readRegister(lis_i2cAddress, LIS331HH_REG_ACCEL_OUT_Z_H);
    // Conversion of the result
    // 16-bit signed result for Z-Axis Acceleration Data of LIS331HH
    lis_accelData.Z = (int16_t)((zAccelHi << 8) | zAccelLo);
}
