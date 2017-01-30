/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        AIS328DQTR
        This code is designed to work with the AIS328DQTR_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelorometer?sku=AIS328DQTR_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "AIS328DQTR.h"

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
        Instantiates a new AIS328DQTR class with appropriate properties
*/
/**************************************************************************/
void AIS328DQTR::getAddr_AIS328DQTR(uint8_t i2cAddress)
{
    ais_i2cAddress = i2cAddress;
    ais_conversionDelay = AIS328DQTR_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool AIS328DQTR::begin()
{
    Wire.begin();
    
    if (readRegister(ais_i2cAddress, AIS328DQTR_REG_ACCEL_WHO_AM_I) != 0x32) return false;
    
    // Set up the sensor for Accelerometer
    // setUpAccelerometer();
    
    return true;

}

/**************************************************************************/
/*
        Sets the Power mode and Low-Power Output Data Rate Configurations
*/
/**************************************************************************/
void AIS328DQTR::setAccelPowerMode(aisAccelPowerMode_t accelpowermode)
{
    ais_accelpowermode = accelpowermode;
}

/**************************************************************************/
/*
        Gets the Power Mode and Low-power Output Data Rate Configurations
*/
/**************************************************************************/
aisAccelPowerMode_t AIS328DQTR::getAccelPowerMode()
{
    return ais_accelpowermode;
}

/**************************************************************************/
/*
        Sets the Normal-Mode Output Data Rate Configurations and Low-Pass Cut-Off Frequencies
*/
/**************************************************************************/
void AIS328DQTR::setAccelDataRate(aisAccelDataRate_t acceldatarate)
{
    ais_acceldatarate = acceldatarate;
}
                    
/**************************************************************************/
/*
        Gets the Normal-Mode Output Data Rate Configurations and Low-Pass Cut-Off Frequencies
*/
/**************************************************************************/
aisAccelDataRate_t AIS328DQTR::getAccelDataRate()
{
    return ais_acceldatarate;
}

/**************************************************************************/
/*
        Sets the Acceleration Z-Axis Enable
*/
/**************************************************************************/
void AIS328DQTR::setAccelZen(aisAccelZen_t accelzen)
{
    ais_accelzen = accelzen;
}

/**************************************************************************/
/*
        Gets the Acceleration Z-Axis Enable
*/
/**************************************************************************/
aisAccelZen_t AIS328DQTR::getAccelZen()
{
    return ais_accelzen;
}

/**************************************************************************/
/*
        Sets the Acceleration Y-Axis Enable
*/
/**************************************************************************/
void AIS328DQTR::setAccelYen(aisAccelYen_t accelyen)
{
    ais_accelyen = accelyen;
}

/**************************************************************************/
/*
        Gets the Acceleration Y-Axis Enable
*/
/**************************************************************************/
aisAccelYen_t AIS328DQTR::getAccelYen()
{
    return ais_accelyen;
}

/**************************************************************************/
/*
        Sets the Acceleration X-Axis Enable
*/
/**************************************************************************/
void AIS328DQTR::setAccelXen(aisAccelXen_t accelxen)
{
    ais_accelxen = accelxen;
}

/**************************************************************************/
/*
        Gets the Acceleration X-Axis Enable
*/
/**************************************************************************/
aisAccelXen_t AIS328DQTR::getAccelXen()
{
    return ais_accelxen;
}

/**************************************************************************/
/*
        Sets the Block Data Update for Acceleration Data
*/
/**************************************************************************/
void AIS328DQTR::setAccelBlockData(aisAccelBlockData_t accelblockdata)
{
    ais_accelblockdata = accelblockdata;
}

/**************************************************************************/
/*
        Gets the Block Data Update for Acceleration Data
*/
/**************************************************************************/
aisAccelBlockData_t AIS328DQTR::getAccelBlockData()
{
    return ais_accelblockdata;
}

/**************************************************************************/
/*
        Sets the Big/Little Endian Data Selection
*/
/**************************************************************************/
void AIS328DQTR::setAccelEndianData(aisAccelEndianData_t accelendiandata)
{
    ais_accelendiandata = accelendiandata;
}

/**************************************************************************/
/*
        Gets the Big/Little Endian Data Selection
*/
/**************************************************************************/
aisAccelEndianData_t AIS328DQTR::getAccelEndianData()
{
    return ais_accelendiandata;
}

/**************************************************************************/
/*
        Sets the Acceleration Full-Scale Selection
*/
/**************************************************************************/
void AIS328DQTR::setAccelRange(aisAccelRange_t accelrange)
{
    ais_accelrange = accelrange;
}
                    
/**************************************************************************/
/*
        Gets the Acceleration Full-Scale Selection
*/
/**************************************************************************/
aisAccelRange_t AIS328DQTR::getAccelRange()
{
    return ais_accelrange;
}

/**************************************************************************/
/*
        Sets the Self-Test Sign Mode Configuration
*/
/**************************************************************************/
void AIS328DQTR::setAccelSelfTestSign(aisAccelSelfTestSign_t accelselftestsign)
{
    ais_accelselftestsign = accelselftestsign;
}

/**************************************************************************/
/*
        Gets the Self-Test Sign Mode Configuration
*/
/**************************************************************************/
aisAccelSelfTestSign_t AIS328DQTR::getAccelSelfTestSign()
{
    return ais_accelselftestsign;
}

/**************************************************************************/
/*
        Sets the Self-Test Mode Configuration
*/
/**************************************************************************/
void AIS328DQTR::setAccelSelfTest(aisAccelSelfTest_t accelselftest)
{
    ais_accelselftest = accelselftest;
}

/**************************************************************************/
/*
        Gets the Self-Test Mode Configuration
*/
/**************************************************************************/
aisAccelSelfTest_t AIS328DQTR::getAccelSelfTest()
{
    return ais_accelselftest;
}

/**************************************************************************/
/*
        Sets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
void AIS328DQTR::setAccelSPIWire(aisAccelSPIWire_t accelspiwire)
{
    ais_accelspiwire = accelspiwire;
}

/**************************************************************************/
/*
        Gets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
aisAccelSPIWire_t AIS328DQTR::getAccelSPIWire()
{
    return ais_accelspiwire;
}

/**************************************************************************/
/*
        Sets up the Accelerometer
*/
/**************************************************************************/
void AIS328DQTR::setUpAccelerometer(void)
{
    // Set Up the Configuration for the Accelerometer Control Register 1
    /*
    // Set the Power mode and Low-Power Output Data Rate Configurations
    uint8_t config1 = ais_accelpowermode;
     
    // Set the Acceleration Z-Axis Enable
    config1 |= ais_accelzen;
     
    // Set the Acceleration Y-Axis Enable
    config1 |= ais_accelyen;
     
    // Set the Acceleration X-Axis Enable
    config1 |= ais_accelxen;
    */
    
    uint8_t config1 =   AIS328DQTR_REG_ACCEL_CTRL_REG1_PM_NORMAL          |   // Normal Mode
                        AIS328DQTR_REG_ACCEL_CTRL_REG1_AZEN_ENABLE        |   // Acceleration Z-Axis Enabled
                        AIS328DQTR_REG_ACCEL_CTRL_REG1_AYEN_ENABLE        |   // Acceleration Y-Axis Enabled
                        AIS328DQTR_REG_ACCEL_CTRL_REG1_AXEN_ENABLE;           // Acceleration X-Axis Enabled
    
    // Set the Acceleration Data Rate Configuration
    config1 |= ais_acceldatarate;
        
    // Write the configuration to the Accelerometer Control Register 1
    writeRegister(ais_i2cAddress, AIS328DQTR_REG_ACCEL_CTRL_REG1, config1);
    
    // Wait for the configuration to complete
    delay(ais_conversionDelay);
    
    // Set Up the Configuration for the Accelerometer Control Register 4
    /*
    // Set the Accelerometer Block Data Update
    uint8_t config4 = ais_accelblockdata;
     
    // Set the Big/Little Endian Data Selection
    config4 |= ais_accelendiandata;
     
     // Set the Self-Test Sign
     config4 |= ais_accelselftestsign;
     
    // Set the Self-Test Enable
    config4 |= ais_accelselftest;
     
    // Set the SPI Serial Interface Mode Selection
    config4 |= ais_accelmagspiwire;
    */
    
    uint8_t config4 =   AIS328DQTR_REG_ACCEL_CTRL_REG4_BDU_CONTINUOUS     |   // Continuous Update
                        AIS328DQTR_REG_ACCEL_CTRL_REG4_BLE_LSB            |   // Data LSB @ lower address
                        AIS328DQTR_REG_ACCEL_CTRL_REG4_ST_SIGN_PLUS       |   // Self-Test Plus
                        AIS328DQTR_REG_ACCEL_CTRL_REG4_ST_DISABLED        |   // Self-Test Disabled
                        AIS328DQTR_REG_ACCEL_CTRL_REG4_SIM_4WIRE;             // 4-Wire Interface
    
    // Set the Acceleration Full-Scale Selection
    config4 |= ais_accelrange;
        
    // Write the configuration to the Accelerometer Control Register 4
    writeRegister(ais_i2cAddress, AIS328DQTR_REG_ACCEL_CTRL_REG4, config4);
    
    // Wait for the configuration to complete
    delay(ais_conversionDelay);
}

/**************************************************************************/
/*
        Reads the results for the Accelerometer AIS328DQTR
*/
/**************************************************************************/
void AIS328DQTR::Measure_Accelerometer(void)
{
    // Read the Accelerometer
    uint8_t xAccelLo, xAccelHi, yAccelLo, yAccelHi, zAccelLo, zAccelHi;
    
    // Read the Data
    // Reading the Low X-Axis Acceleration Data Register
    xAccelLo = readRegister(ais_i2cAddress, AIS328DQTR_REG_ACCEL_OUT_X_L);
    // Reading the High X-Axis Acceleration Data Register
    xAccelHi = readRegister(ais_i2cAddress, AIS328DQTR_REG_ACCEL_OUT_X_H);
    // Conversion of the result
    // 16-bit signed result for X-Axis Acceleration Data of AIS328DQTR
    ais_accelData.X = (int16_t)((xAccelHi << 8) | xAccelLo);
    
    // Reading the Low Y-Axis Acceleration Data Register
    yAccelLo = readRegister(ais_i2cAddress, AIS328DQTR_REG_ACCEL_OUT_Y_L);
    // Reading the High Y-Axis Acceleration Data Register
    yAccelHi = readRegister(ais_i2cAddress, AIS328DQTR_REG_ACCEL_OUT_Y_H);
    // Conversion of the result
    // 16-bit signed result for Y-Axis Acceleration Data of AIS328DQTR
    ais_accelData.Y = (int16_t)((yAccelHi << 8) | yAccelLo);
    
    // Reading the Low Z-Axis Acceleration Data Register
    zAccelLo = readRegister(ais_i2cAddress, AIS328DQTR_REG_ACCEL_OUT_Z_L);
    // Reading the High Z-Axis Acceleration Data Register
    zAccelHi = readRegister(ais_i2cAddress, AIS328DQTR_REG_ACCEL_OUT_Z_H);
    // Conversion of the result
    // 16-bit signed result for Z-Axis Acceleration Data of AIS328DQTR
    ais_accelData.Z = (int16_t)((zAccelHi << 8) | zAccelLo);
}
