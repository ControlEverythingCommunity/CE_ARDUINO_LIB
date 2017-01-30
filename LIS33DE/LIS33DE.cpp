/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LIS33DE
        This code is designed to work with the LIS33DE_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelorometer?sku=LIS33DE_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "LIS33DE.h"

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
        Instantiates a new LIS33DE class with appropriate properties
*/
/**************************************************************************/
void LIS33DE::getAddr_LIS33DE(uint8_t i2cAddress)
{
    lis_i2cAddress = i2cAddress;
    lis_conversionDelay = LIS33DE_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool LIS33DE::begin()
{
    Wire.begin();
    
    // Set up the sensor for Accelerometer
    // setUpAccelerometer();
    
    return true;

}

/**************************************************************************/
/*
        Sets the Output Data Rate Selection
*/
/**************************************************************************/
void LIS33DE::setAccelDataRate(lisAccelDataRate_t acceldatarate)
{
    lis_acceldatarate = acceldatarate;
}
                    
/**************************************************************************/
/*
        Gets the Output Data Rate Selection
*/
/**************************************************************************/
lisAccelDataRate_t LIS33DE::getAccelDataRate()
{
    return lis_acceldatarate;
}

/**************************************************************************/
/*
        Sets the Power Down Control
*/
/**************************************************************************/
void LIS33DE::setAccelPowerMode(lisAccelPowerMode_t accelpowermode)
{
    lis_accelpowermode = accelpowermode;
}

/**************************************************************************/
/*
        Gets the Power Down Control
*/
/**************************************************************************/
lisAccelPowerMode_t LIS33DE::getAccelPowerMode()
{
    return lis_accelpowermode;
}

/**************************************************************************/
/*
        Sets the Acceleration Full-Scale Selection
*/
/**************************************************************************/
void LIS33DE::setAccelRange(lisAccelRange_t accelrange)
{
    lis_accelrange = accelrange;
}

/**************************************************************************/
/*
        Gets the Acceleration Full-Scale Selection
*/
/**************************************************************************/
lisAccelRange_t LIS33DE::getAccelRange()
{
    return lis_accelrange;
}

/**************************************************************************/
/*
        Sets the Self-Test P Mode Configuration
*/
/**************************************************************************/
void LIS33DE::setAccelSelfTestP(lisAccelSelfTestP_t accelselftestp)
{
    lis_accelselftestp = accelselftestp;
}

/**************************************************************************/
/*
        Gets the Self-Test P Mode Configuration
*/
/**************************************************************************/
lisAccelSelfTestP_t LIS33DE::getAccelSelfTestP()
{
    return lis_accelselftestp;
}

/**************************************************************************/
/*
        Sets the Self-Test M Mode Configuration
*/
/**************************************************************************/
void LIS33DE::setAccelSelfTestM(lisAccelSelfTestM_t accelselftestm)
{
    lis_accelselftestm = accelselftestm;
}

/**************************************************************************/
/*
        Gets the Self-Test M Mode Configuration
*/
/**************************************************************************/
lisAccelSelfTestM_t LIS33DE::getAccelSelfTestM()
{
    return lis_accelselftestm;
}

/**************************************************************************/
/*
        Sets the Acceleration Z-Axis Enable
*/
/**************************************************************************/
void LIS33DE::setAccelZen(lisAccelZen_t accelzen)
{
    lis_accelzen = accelzen;
}

/**************************************************************************/
/*
        Gets the Acceleration Z-Axis Enable
*/
/**************************************************************************/
lisAccelZen_t LIS33DE::getAccelZen()
{
    return lis_accelzen;
}

/**************************************************************************/
/*
        Sets the Acceleration Y-Axis Enable
*/
/**************************************************************************/
void LIS33DE::setAccelYen(lisAccelYen_t accelyen)
{
    lis_accelyen = accelyen;
}

/**************************************************************************/
/*
        Gets the Acceleration Y-Axis Enable
*/
/**************************************************************************/
lisAccelYen_t LIS33DE::getAccelYen()
{
    return lis_accelyen;
}

/**************************************************************************/
/*
        Sets the Acceleration X-Axis Enable
*/
/**************************************************************************/
void LIS33DE::setAccelXen(lisAccelXen_t accelxen)
{
    lis_accelxen = accelxen;
}

/**************************************************************************/
/*
        Gets the Acceleration X-Axis Enable
*/
/**************************************************************************/
lisAccelXen_t LIS33DE::getAccelXen()
{
    return lis_accelxen;
}

/**************************************************************************/
/*
        Sets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
void LIS33DE::setAccelSPIWire(lisAccelSPIWire_t accelspiwire)
{
    lis_accelspiwire = accelspiwire;
}

/**************************************************************************/
/*
        Gets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
lisAccelSPIWire_t LIS33DE::getAccelSPIWire()
{
    return lis_accelspiwire;
}

/**************************************************************************/
/*
        Sets the Reboot Memory Content
*/
/**************************************************************************/
void LIS33DE::setAccelReboot(lisAccelReboot_t accelreboot)
{
    lis_accelreboot = accelreboot;
}

/**************************************************************************/
/*
        Gets the Reboot Memory Content
*/
/**************************************************************************/
lisAccelReboot_t LIS33DE::getAccelReboot()
{
    return lis_accelreboot;
}


/**************************************************************************/
/*
        Sets up the Accelerometer
*/
/**************************************************************************/
void LIS33DE::setUpAccelerometer(void)
{
    // Set Up the Configuration for the Accelerometer Control Register 1
    /*
    // Set the Power Down Control Configurations
    uint8_t config1 = lis_accelpowermode;
     
    // Set the Self-Test P Enable
    config1 |= lis_accelselftestp;
     
    // Set the Self-Test M Enable
    config1 |= lis_accelselftestm;
     
    // Set the Acceleration Z-Axis Enable
    config1 |= lis_accelzen;
     
    // Set the Acceleration Y-Axis Enable
    config1 |= lis_accelyen;
     
    // Set the Acceleration X-Axis Enable
    config1 |= lis_accelxen;
    */
    
    uint8_t config1 =   LIS33DE_REG_ACCEL_CTRL_REG1_PD_ACTIVE           |   // Active Mode
                        LIS33DE_REG_ACCEL_CTRL_REG1_STP_NORMAL          |   // Self Test P Normal
                        LIS33DE_REG_ACCEL_CTRL_REG1_STM_NORMAL          |   // Self Test M Normal
                        LIS33DE_REG_ACCEL_CTRL_REG1_AZEN_ENABLE         |   // Acceleration Z-Axis Enabled
                        LIS33DE_REG_ACCEL_CTRL_REG1_AYEN_ENABLE         |   // Acceleration Y-Axis Enabled
                        LIS33DE_REG_ACCEL_CTRL_REG1_AXEN_ENABLE;            // Acceleration X-Axis Enabled
    
    // Set the Acceleration Data Rate Configuration
    config1 |= lis_acceldatarate;
        
    // Write the configuration to the Accelerometer Control Register 1
    writeRegister(lis_i2cAddress, LIS33DE_REG_ACCEL_CTRL_REG1, config1);
    
    // Wait for the configuration to complete
    delay(lis_conversionDelay);
    
    // Set Up the Configuration for the Accelerometer Control Register 2
    /*
    // Set the SPI Serial Interface Mode Selection
    uint8_t config4 = lis_accelspiwire;
     
    // Set the Reboot Memory Content 
    config2 |= lis_accelreboot;
    */
    
    uint8_t config2 =   LIS33DE_REG_ACCEL_CTRL_REG2_SIM_4WIRE           |   // 4-Wire Interface
                        LIS33DE_REG_ACCEL_CTRL_REG2_BOOT_NORMAL;            // Reboot Normal Mode
        
    // Write the configuration to the Accelerometer Control Register 4
    writeRegister(lis_i2cAddress, LIS33DE_REG_ACCEL_CTRL_REG2, config2);
    
    // Wait for the configuration to complete
    delay(lis_conversionDelay);
}

/**************************************************************************/
/*
        Reads the results for the Accelerometer LIS33DE
*/
/**************************************************************************/
void LIS33DE::Measure_Accelerometer(void)
{
    // Read the Accelerometer
    uint8_t xAccel, yAccel, zAccel;
    
    // Read the Data
    // Reading the X-Axis Acceleration Data Register
    xAccel = readRegister(lis_i2cAddress, LIS33DE_REG_ACCEL_OUT_X);
    // Conversion of the result
    // 8-bit signed result for X-Axis Acceleration Data of LIS33DE
    lis_accelData.X = (int8_t)xAccel;
    
    // Reading the Y-Axis Acceleration Data Register
    yAccel = readRegister(lis_i2cAddress, LIS33DE_REG_ACCEL_OUT_Y);
    // Conversion of the result
    // 8-bit signed result for Y-Axis Acceleration Data of LIS33DE
    lis_accelData.Y = (int8_t)yAccel;
    
    // Reading the Z-Axis Acceleration Data Register
    zAccel = readRegister(lis_i2cAddress, LIS33DE_REG_ACCEL_OUT_Z);
    // Conversion of the result
    // 8-bit signed result for Z-Axis Acceleration Data of LIS33DE
    lis_accelData.Z = (int8_t)zAccel;
}
