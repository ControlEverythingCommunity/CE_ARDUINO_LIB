/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        H3LIS331DL
        This code is designed to work with the H3LIS331DL_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelorometer?sku=H3LIS331DL_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "H3LIS331DL.h"

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
        Instantiates a new H3LIS331DL class with appropriate properties
*/
/**************************************************************************/
void H3LIS331DL::getAddr_H3LIS331DL(uint8_t i2cAddress)
{
    h3lis_i2cAddress = i2cAddress;
    h3lis_conversionDelay = H3LIS331DL_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool H3LIS331DL::begin()
{
    Wire.begin();
    
    if (readRegister(h3lis_i2cAddress, H3LIS331DL_REG_ACCEL_WHO_AM_I) != 0x32) return false;
    
    // Set up the sensor for Accelerometer
    setUpAccelerometer();
    
    return true;

}

/**************************************************************************/
/*
        Sets the Power mode and Low-Power Output Data Rate Configurations
*/
/**************************************************************************/
void H3LIS331DL::setAccelPowerMode(h3lisAccelPowerMode_t accelpowermode)
{
    h3lis_accelpowermode = accelpowermode;
}

/**************************************************************************/
/*
        Gets the Power Mode and Low-power Output Data Rate Configurations
*/
/**************************************************************************/
h3lisAccelPowerMode_t H3LIS331DL::getAccelPowerMode()
{
    return h3lis_accelpowermode;
}

/**************************************************************************/
/*
        Sets the Normal-Mode Output Data Rate Configurations and Low-Pass Cut-Off Frequencies
*/
/**************************************************************************/
void H3LIS331DL::setAccelDataRate(h3lisAccelDataRate_t acceldatarate)
{
    h3lis_acceldatarate = acceldatarate;
}
                    
/**************************************************************************/
/*
        Gets the Normal-Mode Output Data Rate Configurations and Low-Pass Cut-Off Frequencies
*/
/**************************************************************************/
h3lisAccelDataRate_t H3LIS331DL::getAccelDataRate()
{
    return h3lis_acceldatarate;
}

/**************************************************************************/
/*
        Sets the Acceleration Z-Axis Enable
*/
/**************************************************************************/
void H3LIS331DL::setAccelZen(h3lisAccelZen_t accelzen)
{
    h3lis_accelzen = accelzen;
}

/**************************************************************************/
/*
        Gets the Acceleration Z-Axis Enable
*/
/**************************************************************************/
h3lisAccelZen_t H3LIS331DL::getAccelZen()
{
    return h3lis_accelzen;
}

/**************************************************************************/
/*
        Sets the Acceleration Y-Axis Enable
*/
/**************************************************************************/
void H3LIS331DL::setAccelYen(h3lisAccelYen_t accelyen)
{
    h3lis_accelyen = accelyen;
}

/**************************************************************************/
/*
        Gets the Acceleration Y-Axis Enable
*/
/**************************************************************************/
h3lisAccelYen_t H3LIS331DL::getAccelYen()
{
    return h3lis_accelyen;
}

/**************************************************************************/
/*
        Sets the Acceleration X-Axis Enable
*/
/**************************************************************************/
void H3LIS331DL::setAccelXen(h3lisAccelXen_t accelxen)
{
    h3lis_accelxen = accelxen;
}

/**************************************************************************/
/*
        Gets the Acceleration X-Axis Enable
*/
/**************************************************************************/
h3lisAccelXen_t H3LIS331DL::getAccelXen()
{
    return h3lis_accelxen;
}

/**************************************************************************/
/*
        Sets the Block Data Update for Acceleration Data
*/
/**************************************************************************/
void H3LIS331DL::setAccelBlockData(h3lisAccelBlockData_t accelblockdata)
{
    h3lis_accelblockdata = accelblockdata;
}

/**************************************************************************/
/*
        Gets the Block Data Update for Acceleration Data
*/
/**************************************************************************/
h3lisAccelBlockData_t H3LIS331DL::getAccelBlockData()
{
    return h3lis_accelblockdata;
}

/**************************************************************************/
/*
        Sets the Big/Little Endian Data Selection
*/
/**************************************************************************/
void H3LIS331DL::setAccelEndianData(h3lisAccelEndianData_t accelendiandata)
{
    h3lis_accelendiandata = accelendiandata;
}

/**************************************************************************/
/*
        Gets the Big/Little Endian Data Selection
*/
/**************************************************************************/
h3lisAccelEndianData_t H3LIS331DL::getAccelEndianData()
{
    return h3lis_accelendiandata;
}

/**************************************************************************/
/*
        Sets the Acceleration Full-Scale Selection
*/
/**************************************************************************/
void H3LIS331DL::setAccelRange(h3lisAccelRange_t accelrange)
{
    h3lis_accelrange = accelrange;
}
                    
/**************************************************************************/
/*
        Gets the Acceleration Full-Scale Selection
*/
/**************************************************************************/
h3lisAccelRange_t H3LIS331DL::getAccelRange()
{
    return h3lis_accelrange;
}

/**************************************************************************/
/*
        Sets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
void H3LIS331DL::setAccelSPIWire(h3lisAccelSPIWire_t accelspiwire)
{
    h3lis_accelspiwire = accelspiwire;
}

/**************************************************************************/
/*
        Gets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
h3lisAccelSPIWire_t H3LIS331DL::getAccelSPIWire()
{
    return h3lis_accelspiwire;
}

/**************************************************************************/
/*
        Sets up the Accelerometer
*/
/**************************************************************************/
void H3LIS331DL::setUpAccelerometer(void)
{
    // Set Up the Configuration for the Accelerometer Control Register 1
    /*
    // Set the Power mode and Low-Power Output Data Rate Configurations
    uint8_t config1 = h3lis_accelpowermode;
     
    // Set the Acceleration Z-Axis Enable
    config1 |= h3lis_accelzen;
     
    // Set the Acceleration Y-Axis Enable
    config1 |= h3lis_accelyen;
     
    // Set the Acceleration X-Axis Enable
    config1 |= h3lis_accelxen;
    */
    
    uint8_t config1 =   H3LIS331DL_REG_ACCEL_CTRL_REG1_PM_NORMAL          |   // Normal Mode
                        H3LIS331DL_REG_ACCEL_CTRL_REG1_AZEN_ENABLE        |   // Acceleration Z-Axis Enabled
                        H3LIS331DL_REG_ACCEL_CTRL_REG1_AYEN_ENABLE        |   // Acceleration Y-Axis Enabled
                        H3LIS331DL_REG_ACCEL_CTRL_REG1_AXEN_ENABLE;           // Acceleration X-Axis Enabled
    
    // Set the Acceleration Data Rate Configuration
    config1 |= h3lis_acceldatarate;
        
    // Write the configuration to the Accelerometer Control Register 1
    writeRegister(h3lis_i2cAddress, H3LIS331DL_REG_ACCEL_CTRL_REG1, config1);
    
    // Wait for the configuration to complete
    delay(h3lis_conversionDelay);
    
    // Set Up the Configuration for the Accelerometer Control Register 4
    /*
    // Set the Accelerometer Block Data Update
    uint8_t config4 = h3lis_accelblockdata;
     
    // Set the Big/Little Endian Data Selection
    config4 |= h3lis_accelendiandata;
     
    // Set the SPI Serial Interface Mode Selection
    config4 |= h3lis_accelmagspiwire;
    */
    
    uint8_t config4 =   H3LIS331DL_REG_ACCEL_CTRL_REG4_BDU_CONTINUOUS     |   // Continuous Update
                        H3LIS331DL_REG_ACCEL_CTRL_REG4_BLE_LSB            |   // Data LSB @ lower address
                        H3LIS331DL_REG_ACCEL_CTRL_REG4_SIM_4WIRE;             // 4-Wire Interface
    
    // Set the Acceleration Full-Scale Selection
    config4 |= h3lis_accelrange;
        
    // Write the configuration to the Accelerometer Control Register 4
    writeRegister(h3lis_i2cAddress, H3LIS331DL_REG_ACCEL_CTRL_REG4, config4);
    
    // Wait for the configuration to complete
    delay(h3lis_conversionDelay);
}

/**************************************************************************/
/*
        Reads the results for the Accelerometer H3LIS331DL
*/
/**************************************************************************/
void H3LIS331DL::Measure_Accelerometer(void)
{
    // Read the Accelerometer
    uint8_t xAccelLo, xAccelHi, yAccelLo, yAccelHi, zAccelLo, zAccelHi;
    
    // Read the Data
    // Reading the Low X-Axis Acceleration Data Register
    xAccelLo = readRegister(h3lis_i2cAddress, H3LIS331DL_REG_ACCEL_OUT_X_L);
    // Reading the High X-Axis Acceleration Data Register
    xAccelHi = readRegister(h3lis_i2cAddress, H3LIS331DL_REG_ACCEL_OUT_X_H);
    // Conversion of the result
    // 16-bit signed result for X-Axis Acceleration Data of H3LIS331DL
    h3lis_accelData.X = (int16_t)((xAccelHi << 8) | xAccelLo);
    
    // Reading the Low Y-Axis Acceleration Data Register
    yAccelLo = readRegister(h3lis_i2cAddress, H3LIS331DL_REG_ACCEL_OUT_Y_L);
    // Reading the High Y-Axis Acceleration Data Register
    yAccelHi = readRegister(h3lis_i2cAddress, H3LIS331DL_REG_ACCEL_OUT_Y_H);
    // Conversion of the result
    // 16-bit signed result for Y-Axis Acceleration Data of H3LIS331DL
    h3lis_accelData.Y = (int16_t)((yAccelHi << 8) | yAccelLo);
    
    // Reading the Low Z-Axis Acceleration Data Register
    zAccelLo = readRegister(h3lis_i2cAddress, H3LIS331DL_REG_ACCEL_OUT_Z_L);
    // Reading the High Z-Axis Acceleration Data Register
    zAccelHi = readRegister(h3lis_i2cAddress, H3LIS331DL_REG_ACCEL_OUT_Z_H);
    // Conversion of the result
    // 16-bit signed result for Z-Axis Acceleration Data of H3LIS331DL
    h3lis_accelData.Z = (int16_t)((zAccelHi << 8) | zAccelLo);
}
