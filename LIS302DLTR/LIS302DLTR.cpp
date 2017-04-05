/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LIS302DLTR
        This code is designed to work with the LIS302DLTR_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelorometer?sku=LIS302DLTR_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "LIS302DLTR.h"

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
        Instantiates a new LIS302DLTR class with appropriate properties
*/
/**************************************************************************/
void LIS302DLTR::getAddr_LIS302DLTR(uint8_t i2cAddress)
{
    lis_i2cAddress = i2cAddress;
    lis_conversionDelay = LIS302DLTR_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool LIS302DLTR::begin()
{
    Wire.begin();
    
    uint8_t devid = readRegister(lis_i2cAddress, LIS302DLTR_REG_ACCEL_WHO_AM_I);
    if (devid != LIS302DLTR_ACCEL_DEV_ID)
        return false;
    
    // Set up the sensor for Accelerometer
    // setUpAccelerometer();
    
    return true;

}

/**************************************************************************/
/*
        Sets the Output Data Rate Selection
*/
/**************************************************************************/
void LIS302DLTR::setAccelDataRate(lisAccelDataRate_t acceldatarate)
{
    lis_acceldatarate = acceldatarate;
}
                    
/**************************************************************************/
/*
        Gets the Output Data Rate Selection
*/
/**************************************************************************/
lisAccelDataRate_t LIS302DLTR::getAccelDataRate()
{
    return lis_acceldatarate;
}

/**************************************************************************/
/*
        Sets the Power Down Control
*/
/**************************************************************************/
void LIS302DLTR::setAccelPowerMode(lisAccelPowerMode_t accelpowermode)
{
    lis_accelpowermode = accelpowermode;
}

/**************************************************************************/
/*
        Gets the Power Down Control
*/
/**************************************************************************/
lisAccelPowerMode_t LIS302DLTR::getAccelPowerMode()
{
    return lis_accelpowermode;
}

/**************************************************************************/
/*
        Sets the Acceleration Full-Scale Selection
*/
/**************************************************************************/
void LIS302DLTR::setAccelRange(lisAccelRange_t accelrange)
{
    lis_accelrange = accelrange;
}

/**************************************************************************/
/*
        Gets the Acceleration Full-Scale Selection
*/
/**************************************************************************/
lisAccelRange_t LIS302DLTR::getAccelRange()
{
    return lis_accelrange;
}

/**************************************************************************/
/*
        Sets the Self-Test P Mode Configuration
*/
/**************************************************************************/
void LIS302DLTR::setAccelSelfTestP(lisAccelSelfTestP_t accelselftestp)
{
    lis_accelselftestp = accelselftestp;
}

/**************************************************************************/
/*
        Gets the Self-Test P Mode Configuration
*/
/**************************************************************************/
lisAccelSelfTestP_t LIS302DLTR::getAccelSelfTestP()
{
    return lis_accelselftestp;
}

/**************************************************************************/
/*
        Sets the Self-Test M Mode Configuration
*/
/**************************************************************************/
void LIS302DLTR::setAccelSelfTestM(lisAccelSelfTestM_t accelselftestm)
{
    lis_accelselftestm = accelselftestm;
}

/**************************************************************************/
/*
        Gets the Self-Test M Mode Configuration
*/
/**************************************************************************/
lisAccelSelfTestM_t LIS302DLTR::getAccelSelfTestM()
{
    return lis_accelselftestm;
}

/**************************************************************************/
/*
        Sets the Acceleration Z-Axis Enable
*/
/**************************************************************************/
void LIS302DLTR::setAccelZen(lisAccelZen_t accelzen)
{
    lis_accelzen = accelzen;
}

/**************************************************************************/
/*
        Gets the Acceleration Z-Axis Enable
*/
/**************************************************************************/
lisAccelZen_t LIS302DLTR::getAccelZen()
{
    return lis_accelzen;
}

/**************************************************************************/
/*
        Sets the Acceleration Y-Axis Enable
*/
/**************************************************************************/
void LIS302DLTR::setAccelYen(lisAccelYen_t accelyen)
{
    lis_accelyen = accelyen;
}

/**************************************************************************/
/*
        Gets the Acceleration Y-Axis Enable
*/
/**************************************************************************/
lisAccelYen_t LIS302DLTR::getAccelYen()
{
    return lis_accelyen;
}

/**************************************************************************/
/*
        Sets the Acceleration X-Axis Enable
*/
/**************************************************************************/
void LIS302DLTR::setAccelXen(lisAccelXen_t accelxen)
{
    lis_accelxen = accelxen;
}

/**************************************************************************/
/*
        Gets the Acceleration X-Axis Enable
*/
/**************************************************************************/
lisAccelXen_t LIS302DLTR::getAccelXen()
{
    return lis_accelxen;
}

/**************************************************************************/
/*
        Sets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
void LIS302DLTR::setAccelSPIWire(lisAccelSPIWire_t accelspiwire)
{
    lis_accelspiwire = accelspiwire;
}

/**************************************************************************/
/*
        Gets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
lisAccelSPIWire_t LIS302DLTR::getAccelSPIWire()
{
    return lis_accelspiwire;
}

/**************************************************************************/
/*
        Sets the Reboot Memory Content
*/
/**************************************************************************/
void LIS302DLTR::setAccelReboot(lisAccelReboot_t accelreboot)
{
    lis_accelreboot = accelreboot;
}

/**************************************************************************/
/*
        Gets the Reboot Memory Content
*/
/**************************************************************************/
lisAccelReboot_t LIS302DLTR::getAccelReboot()
{
    return lis_accelreboot;
}


/**************************************************************************/
/*
        Sets up the Accelerometer
*/
/**************************************************************************/
void LIS302DLTR::setUpAccelerometer(void)
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
    
    uint8_t config1 =   LIS302DLTR_REG_ACCEL_CTRL_REG1_PD_ACTIVE           |   // Active Mode
                        LIS302DLTR_REG_ACCEL_CTRL_REG1_STP_NORMAL          |   // Self Test P Normal
                        LIS302DLTR_REG_ACCEL_CTRL_REG1_STM_NORMAL          |   // Self Test M Normal
                        LIS302DLTR_REG_ACCEL_CTRL_REG1_AZEN_ENABLE         |   // Acceleration Z-Axis Enabled
                        LIS302DLTR_REG_ACCEL_CTRL_REG1_AYEN_ENABLE         |   // Acceleration Y-Axis Enabled
                        LIS302DLTR_REG_ACCEL_CTRL_REG1_AXEN_ENABLE;            // Acceleration X-Axis Enabled
    
    // Set the Acceleration Data Rate Configuration
    config1 |= lis_acceldatarate;
        
    // Write the configuration to the Accelerometer Control Register 1
    writeRegister(lis_i2cAddress, LIS302DLTR_REG_ACCEL_CTRL_REG1, config1);
    
    // Wait for the configuration to complete
    delay(lis_conversionDelay);
    
    // Set Up the Configuration for the Accelerometer Control Register 2
    /*
    // Set the SPI Serial Interface Mode Selection
    uint8_t config4 = lis_accelspiwire;
     
    // Set the Reboot Memory Content 
    config2 |= lis_accelreboot;
    */
    
    uint8_t config2 =   LIS302DLTR_REG_ACCEL_CTRL_REG2_SIM_4WIRE           |   // 4-Wire Interface
                        LIS302DLTR_REG_ACCEL_CTRL_REG2_BOOT_NORMAL;            // Reboot Normal Mode
        
    // Write the configuration to the Accelerometer Control Register 4
    writeRegister(lis_i2cAddress, LIS302DLTR_REG_ACCEL_CTRL_REG2, config2);
    
    // Wait for the configuration to complete
    delay(lis_conversionDelay);
}

/**************************************************************************/
/*
        Reads the results for the Accelerometer LIS302DLTR
*/
/**************************************************************************/
void LIS302DLTR::Measure_Accelerometer(void)
{
    // Read the Accelerometer
    uint8_t xAccel, yAccel, zAccel;
    
    // Read the Data
    // Reading the X-Axis Acceleration Data Register
    xAccel = readRegister(lis_i2cAddress, LIS302DLTR_REG_ACCEL_OUT_X);
    // Conversion of the result
    // 8-bit signed result for X-Axis Acceleration Data of LIS302DLTR
    lis_accelData.X = (int8_t)xAccel;
    
    // Reading the Y-Axis Acceleration Data Register
    yAccel = readRegister(lis_i2cAddress, LIS302DLTR_REG_ACCEL_OUT_Y);
    // Conversion of the result
    // 8-bit signed result for Y-Axis Acceleration Data of LIS302DLTR
    lis_accelData.Y = (int8_t)yAccel;
    
    // Reading the Z-Axis Acceleration Data Register
    zAccel = readRegister(lis_i2cAddress, LIS302DLTR_REG_ACCEL_OUT_Z);
    // Conversion of the result
    // 8-bit signed result for Z-Axis Acceleration Data of LIS302DLTR
    lis_accelData.Z = (int8_t)zAccel;
}
