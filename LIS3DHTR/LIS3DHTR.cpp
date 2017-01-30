/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LIS3DHTR
        This code is designed to work with the LIS3DHTR_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelorometer?sku=LIS3DHTR_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "LIS3DHTR.h"

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
        Instantiates a new LIS3DHTR class with appropriate properties
*/
/**************************************************************************/
void LIS3DHTR::getAddr_LIS3DHTR(uint8_t i2cAddress)
{
    lis_i2cAddress = i2cAddress;
    lis_conversionDelay = LIS3DHTR_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool LIS3DHTR::begin()
{
    Wire.begin();
    
    if (readRegister(lis_i2cAddress, LIS3DHTR_REG_ACCEL_WHO_AM_I) != 0x33) return false;
    
    // Set up the sensor for ADC, Temperature and Accelerometer
    // setUpADCTemperature();
    // setUpAccelerometer();
    
    return true;

}

/**************************************************************************/
/*
        Sets the ADC Enable
*/
/**************************************************************************/
void LIS3DHTR::setADCEnable(lisADCEnable_t adcenable)
{
    lis_adcenable = adcenable;
}

/**************************************************************************/
/*
        Gets the ADC Enable
*/
/**************************************************************************/
lisADCEnable_t LIS3DHTR::getADCEnable()
{
    return lis_adcenable;
}

/**************************************************************************/
/*
        Sets the Temperature Enable
*/
/**************************************************************************/
void LIS3DHTR::setTempEnable(lisTempEnable_t tempenable)
{
    lis_tempenable = tempenable;
}

/**************************************************************************/
/*
        Gets the Temperature Enable
*/
/**************************************************************************/
lisTempEnable_t LIS3DHTR::getTempEnable()
{
    return lis_tempenable;
}

/**************************************************************************/
/*
        Sets the Output Data Rate Selection for the Accelerometer
*/
/**************************************************************************/
void LIS3DHTR::setAccelDataRate(lisAccelDataRate_t acceldatarate)
{
    lis_acceldatarate = acceldatarate;
}
                    
/**************************************************************************/
/*
        Gets the Output Data Rate Selection for the Accelerometer
*/
/**************************************************************************/
lisAccelDataRate_t LIS3DHTR::getAccelDataRate()
{
    return lis_acceldatarate;
}

/**************************************************************************/
/*
        Sets the Low Power Mode Enable
*/
/**************************************************************************/
void LIS3DHTR::setAccelPowerMode(lisAccelPowerMode_t accelpowermode)
{
    lis_accelpowermode = accelpowermode;
}

/**************************************************************************/
/*
        Gets the Low Power Mode Enable
*/
/**************************************************************************/
lisAccelPowerMode_t LIS3DHTR::getAccelPowerMode()
{
    return lis_accelpowermode;
}

/**************************************************************************/
/*
        Sets the Acceleration Z-Axis Enable
*/
/**************************************************************************/
void LIS3DHTR::setAccelZen(lisAccelZen_t accelzen)
{
    lis_accelzen = accelzen;
}

/**************************************************************************/
/*
        Gets the Acceleration Z-Axis Enable
*/
/**************************************************************************/
lisAccelZen_t LIS3DHTR::getAccelZen()
{
    return lis_accelzen;
}

/**************************************************************************/
/*
        Sets the Acceleration Y-Axis Enable
*/
/**************************************************************************/
void LIS3DHTR::setAccelYen(lisAccelYen_t accelyen)
{
    lis_accelyen = accelyen;
}

/**************************************************************************/
/*
        Gets the Acceleration Y-Axis Enable
*/
/**************************************************************************/
lisAccelYen_t LIS3DHTR::getAccelYen()
{
    return lis_accelyen;
}

/**************************************************************************/
/*
        Sets the Acceleration X-Axis Enable
*/
/**************************************************************************/
void LIS3DHTR::setAccelXen(lisAccelXen_t accelxen)
{
    lis_accelxen = accelxen;
}

/**************************************************************************/
/*
        Gets the Acceleration X-Axis Enable
*/
/**************************************************************************/
lisAccelXen_t LIS3DHTR::getAccelXen()
{
    return lis_accelxen;
}

/**************************************************************************/
/*
        Sets the Block Data Update for Acceleration Data
*/
/**************************************************************************/
void LIS3DHTR::setAccelBlockData(lisAccelBlockData_t accelblockdata)
{
    lis_accelblockdata = accelblockdata;
}

/**************************************************************************/
/*
        Gets the Block Data Update for Acceleration Data
*/
/**************************************************************************/
lisAccelBlockData_t LIS3DHTR::getAccelBlockData()
{
    return lis_accelblockdata;
}

/**************************************************************************/
/*
        Sets the Big/Little Endian Data Selection
*/
/**************************************************************************/
void LIS3DHTR::setAccelEndianData(lisAccelEndianData_t accelendiandata)
{
    lis_accelendiandata = accelendiandata;
}

/**************************************************************************/
/*
        Gets the Big/Little Endian Data Selection
*/
/**************************************************************************/
lisAccelEndianData_t LIS3DHTR::getAccelEndianData()
{
    return lis_accelendiandata;
}

/**************************************************************************/
/*
        Sets the Acceleration Full-Scale Selection
*/
/**************************************************************************/
void LIS3DHTR::setAccelRange(lisAccelRange_t accelrange)
{
    lis_accelrange = accelrange;
}
                    
/**************************************************************************/
/*
        Gets the Acceleration Full-Scale Selection
*/
/**************************************************************************/
lisAccelRange_t LIS3DHTR::getAccelRange()
{
    return lis_accelrange;
}

/**************************************************************************/
/*
        Sets the Acceleration Resolution Selection
*/
/**************************************************************************/
void LIS3DHTR::setAccelResolution(lisAccelResolution_t accelresolution)
{
    lis_accelresolution = accelresolution;
}

/**************************************************************************/
/*
        Gets the Acceleration Resolution Selection
*/
/**************************************************************************/
lisAccelResolution_t LIS3DHTR::getAccelResolution()
{
    return lis_accelresolution;
}

/**************************************************************************/
/*
        Sets the Self-Test Mode Configuration
*/
/**************************************************************************/
void LIS3DHTR::setAccelSelfTest(lisAccelSelfTest_t accelselftest)
{
    lis_accelselftest = accelselftest;
}

/**************************************************************************/
/*
        Gets the Self-Test Mode Configuration
*/
/**************************************************************************/
lisAccelSelfTest_t LIS3DHTR::getAccelSelfTest()
{
    return lis_accelselftest;
}

/**************************************************************************/
/*
        Sets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
void LIS3DHTR::setAccelSPIWire(lisAccelSPIWire_t accelspiwire)
{
    lis_accelspiwire = accelspiwire;
}

/**************************************************************************/
/*
        Gets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
lisAccelSPIWire_t LIS3DHTR::getAccelSPIWire()
{
    return lis_accelspiwire;
}

/**************************************************************************/
/*
        Sets up the ADC and Temperature Enable
*/
/**************************************************************************/
void LIS3DHTR::setUpADCTemperature(void)
{
    // Set Up the Configuration for the ADC and Temperature Configuration Register
    /*
     // Set the ADC Enable
     uint8_t config = lis_adcenable;
     
     // Set the Temperature Enable
     uint8_t config |= lis_tempenable;
    */
    
    uint8_t config =    LIS3DHTR_REG_TEMP_ADC_PD_ENABLED        |       // ADC Enabled
                        LIS3DHTR_REG_TEMP_TEMP_EN_ENABLED;              // Temperature Enabled
    
    // Write the configuration to the ADC and Temperature Configuration Register
    writeRegister(lis_i2cAddress, LIS3DHTR_REG_TEMP_CFG, config);
    
    // Wait for the configuration to complete
    delay(lis_conversionDelay);
}
/**************************************************************************/
/*
        Sets up the Accelerometer
*/
/**************************************************************************/
void LIS3DHTR::setUpAccelerometer(void)
{
    // Set Up the Configuration for the Accelerometer Control Register 1
    /*
    // Set the Block Data Update for Acceleration Data
    uint8_t config1 = lis_accelpowermode;
     
    // Set the Acceleration Z-Axis Enable
    config1 |= lis_accelzen;
     
    // Set the Acceleration Y-Axis Enable
    config1 |= lis_accelyen;
     
    // Set the Acceleration X-Axis Enable
    config1 |= lis_accelxen;
    */
    
    uint8_t config1 =   LIS3DHTR_REG_ACCEL_CTRL_REG1_LPEN_NORMAL        |   // Normal Mode
                        LIS3DHTR_REG_ACCEL_CTRL_REG1_AZEN_ENABLE        |   // Acceleration Z-Axis Enabled
                        LIS3DHTR_REG_ACCEL_CTRL_REG1_AYEN_ENABLE        |   // Acceleration Y-Axis Enabled
                        LIS3DHTR_REG_ACCEL_CTRL_REG1_AXEN_ENABLE;           // Acceleration X-Axis Enabled
    
    // Set the Acceleration Data Rate Configuration
    config1 |= lis_acceldatarate;
        
    // Write the configuration to the Accelerometer Control Register 1
    writeRegister(lis_i2cAddress, LIS3DHTR_REG_ACCEL_CTRL_REG1, config1);
    
    // Wait for the configuration to complete
    delay(lis_conversionDelay);
    
    // Set Up the Configuration for the Accelerometer Control Register 4
    /*
    // Set the Accelerometer Block Data Update
    uint8_t config4 = lis_accelblockdata;
     
    // Set the Big/Little Endian Data Selection
    config4 |= lis_accelendiandata;
     
    // Set the Acceleration High Resolution Output Mode
    config4 |= lis_accelresolution;
     
    // Set the Self-Test Enable
    config4 |= lis_accelselftest;
     
    // Set the SPI Serial Interface Mode Selection
    config4 |= lis_accelspiwire;
    */
    
    uint8_t config4 =   LIS3DHTR_REG_ACCEL_CTRL_REG4_BDU_CONTINUOUS     |   // Continuous Update
                        LIS3DHTR_REG_ACCEL_CTRL_REG4_BLE_LSB            |   // Data LSB @ lower address
                        LIS3DHTR_REG_ACCEL_CTRL_REG4_HS_DISABLE         |   // High Resolution Disable
                        LIS3DHTR_REG_ACCEL_CTRL_REG4_ST_NORMAL          |   // Normal Mode
                        LIS3DHTR_REG_ACCEL_CTRL_REG4_SIM_4WIRE;             // 4-Wire Interface
    
    // Set the Acceleration Full-Scale Selection
    config4 |= lis_accelrange;
        
    // Write the configuration to the Accelerometer Control Register 4
    writeRegister(lis_i2cAddress, LIS3DHTR_REG_ACCEL_CTRL_REG4, config4);
    
    // Wait for the configuration to complete
    delay(lis_conversionDelay);
}

/**************************************************************************/
/*
        Reads the results for the Accelerometer LIS3DHTR
*/
/**************************************************************************/
void LIS3DHTR::Measure_Accelerometer(void)
{
    // Read the Accelerometer
    uint8_t xAccelLo, xAccelHi, yAccelLo, yAccelHi, zAccelLo, zAccelHi;
    
    // Read the Data
    // Reading the Low X-Axis Acceleration Data Register
    xAccelLo = readRegister(lis_i2cAddress, LIS3DHTR_REG_ACCEL_OUT_X_L);
    // Reading the High X-Axis Acceleration Data Register
    xAccelHi = readRegister(lis_i2cAddress, LIS3DHTR_REG_ACCEL_OUT_X_H);
    // Conversion of the result
    // 16-bit signed result for X-Axis Acceleration Data of LIS3DHTR
    lis_accelData.X = (int16_t)((xAccelHi << 8) | xAccelLo);
    
    // Reading the Low Y-Axis Acceleration Data Register
    yAccelLo = readRegister(lis_i2cAddress, LIS3DHTR_REG_ACCEL_OUT_Y_L);
    // Reading the High Y-Axis Acceleration Data Register
    yAccelHi = readRegister(lis_i2cAddress, LIS3DHTR_REG_ACCEL_OUT_Y_H);
    // Conversion of the result
    // 16-bit signed result for Y-Axis Acceleration Data of LIS3DHTR
    lis_accelData.Y = (int16_t)((yAccelHi << 8) | yAccelLo);
    
    // Reading the Low Z-Axis Acceleration Data Register
    zAccelLo = readRegister(lis_i2cAddress, LIS3DHTR_REG_ACCEL_OUT_Z_L);
    // Reading the High Z-Axis Acceleration Data Register
    zAccelHi = readRegister(lis_i2cAddress, LIS3DHTR_REG_ACCEL_OUT_Z_H);
    // Conversion of the result
    // 16-bit signed result for Z-Axis Acceleration Data of LIS3DHTR
    lis_accelData.Z = (int16_t)((zAccelHi << 8) | zAccelLo);
}
