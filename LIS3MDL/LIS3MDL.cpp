/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LIS3MDL
        This code is designed to work with the LIS3MDL_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Magnetic-Sensor?sku=LIS3MDL_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "LIS3MDL.h"

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
        Instantiates a new LIS3MDL class with appropriate properties
*/
/**************************************************************************/
void LIS3MDL::getAddr_LIS3MDL(uint8_t i2cAddress)
{
    lis_i2cAddress = i2cAddress;
    lis_conversionDelay = LIS3MDL_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool LIS3MDL::begin()
{
    Wire.begin();
    
    uint8_t devid = readRegister(lis_i2cAddress, LIS3MDL_REG_MAG_WHO_AM_I);
    if (devid != LIS3MDL_MAG_DEV_ID)
        return false;
    
    // Set up the sensor for Magnetometer
    // setUpMagnetometer();
    
    return true;

}

/**************************************************************************/
/*
        Sets the Temperature Sensor Enable
*/
/**************************************************************************/
void LIS3MDL::setTempSensor(lisTempSensor_t tempsensor)
{
    lis_tempsensor = tempsensor;
}
                    
/**************************************************************************/
/*
        Gets the Temperature Sensor Enable
*/
/**************************************************************************/
lisTempSensor_t LIS3MDL::getTempSensor()
{
    return lis_tempsensor;
}

/**************************************************************************/
/*
        Sets the X and Y Axes Operative Mode Selection
*/
/**************************************************************************/
void LIS3MDL::setMagXYOpMode(lisMagXYOpMode_t magxyopmode)
{
    lis_magxyopmode = magxyopmode;
}

/**************************************************************************/
/*
        Gets the X and Y Axes Operative Mode Selection
*/
/**************************************************************************/
lisMagXYOpMode_t LIS3MDL::getMagXYOpMode()
{
    return lis_magxyopmode;
}

/**************************************************************************/
/*
        Sets the Output Data Rate Selection
*/
/**************************************************************************/
void LIS3MDL::setMagDataRate(lisMagDataRate_t magdatarate)
{
    lis_magdatarate = magdatarate;
}

/**************************************************************************/
/*
        Gets the Output Data Rate Selection
*/
/**************************************************************************/
lisMagDataRate_t LIS3MDL::getMagDataRate()
{
    return lis_magdatarate;
}

/**************************************************************************/
/*
        Sets the FAST_ODR Enables Data Rates Higher than 80 Hz
*/
/**************************************************************************/
void LIS3MDL::setMagFastOddr(lisMagFastOddr_t magfastoddr)
{
    lis_magfastoddr = magfastoddr;
}

/**************************************************************************/
/*
        Gets the FAST_ODR Enables Data Rates Higher than 80 Hz
*/
/**************************************************************************/
lisMagFastOddr_t LIS3MDL::getMagFastOddr()
{
    return lis_magfastoddr;
}

/**************************************************************************/
/*
        Sets the Self-Test Mode Configuration
*/
/**************************************************************************/
void LIS3MDL::setMagSelfTest(lisMagSelfTest_t magselftest)
{
    lis_magselftest = magselftest;
}

/**************************************************************************/
/*
        Gets the Self-Test Mode Configuration
*/
/**************************************************************************/
lisMagSelfTest_t LIS3MDL::getMagSelfTest()
{
    return lis_magselftest;
}

/**************************************************************************/
/*
        Sets the Full-Scale Selection
*/
/**************************************************************************/
void LIS3MDL::setMagScale(lisMagScale_t magscale)
{
    lis_magscale = magscale;
}

/**************************************************************************/
/*
        Gets the Full-Scale Selection
*/
/**************************************************************************/
lisMagScale_t LIS3MDL::getMagScale()
{
    return lis_magscale;
}

/**************************************************************************/
/*
        Sets the Reboot Memory Content
*/
/**************************************************************************/
void LIS3MDL::setMagReboot(lisMagReboot_t magreboot)
{
    lis_magreboot = magreboot;
}

/**************************************************************************/
/*
        Gets the Reboot Memory Content
*/
/**************************************************************************/
lisMagReboot_t LIS3MDL::getMagReboot()
{
    return lis_magreboot;
}

/**************************************************************************/
/*
        Sets the Configuration Registers and User Register Reset Function
*/
/**************************************************************************/
void LIS3MDL::setMagReset(lisMagReset_t magreset)
{
    lis_magreset = magreset;
}

/**************************************************************************/
/*
        Gets the Configuration Registers and User Register Reset Function
*/
/**************************************************************************/
lisMagReset_t LIS3MDL::getMagReset()
{
    return lis_magreset;
}

/**************************************************************************/
/*
        Sets the Low-Power Mode Configuration
*/
/**************************************************************************/
void LIS3MDL::setMagLowPower(lisMagLowPower_t maglowpower)
{
    lis_maglowpower = maglowpower;
}

/**************************************************************************/
/*
        Gets the Low-Power Mode Configuration
*/
/**************************************************************************/
lisMagLowPower_t LIS3MDL::getMagLowPower()
{
    return lis_maglowpower;
}

/**************************************************************************/
/*
        Sets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
void LIS3MDL::setMagSPIWire(lisMagSPIWire_t magspiwire)
{
    lis_magspiwire = magspiwire;
}

/**************************************************************************/
/*
        Gets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
lisMagSPIWire_t LIS3MDL::getMagSPIWire()
{
    return lis_magspiwire;
}

/**************************************************************************/
/*
        Sets the Operating Mode Selection
*/
/**************************************************************************/
void LIS3MDL::setMagMode(lisMagMode_t magmode)
{
    lis_magmode = magmode;
}

/**************************************************************************/
/*
        Gets the Operating Mode Selection
*/
/**************************************************************************/
lisMagMode_t LIS3MDL::getMagMode()
{
    return lis_magmode;
}

/**************************************************************************/
/*
        Sets the Z Axis Operative Mode Selection
*/
/**************************************************************************/
void LIS3MDL::setMagZOpMode(lisMagZOpMode_t magzopmode)
{
    lis_magzopmode = magzopmode;
}

/**************************************************************************/
/*
        Gets the Z Axis Operative Mode Selection
*/
/**************************************************************************/
lisMagZOpMode_t LIS3MDL::getMagZOpMode()
{
    return lis_magzopmode;
}

/**************************************************************************/
/*
        Sets the Big/Little Endian Data Selection
*/
/**************************************************************************/
void LIS3MDL::setMagEndianData(lisMagEndianData_t magendiandata)
{
    lis_magendiandata = magendiandata;
}

/**************************************************************************/
/*
        Gets the Big/Little Endian Data Selection
*/
/**************************************************************************/
lisMagEndianData_t LIS3MDL::getMagEndianData()
{
    return lis_magendiandata;
}

/**************************************************************************/
/*
        Sets the FAST READ Allows Reading the High Part of DATA OUT 
        Only in Order to Increase Reading Efficiency
*/
/**************************************************************************/
void LIS3MDL::setMagFastRead(lisMagFastRead_t magfastread)
{
    lis_magfastread = magfastread;
}

/**************************************************************************/
/*
        Gets the FAST READ Allows Reading the High Part of DATA OUT 
        Only in Order to Increase Reading Efficiency
*/
/**************************************************************************/
lisMagFastRead_t LIS3MDL::getMagFastRead()
{
    return lis_magfastread;
}

/**************************************************************************/
/*
        Sets the Block Data Update for Magnetic Data
*/
/**************************************************************************/
void LIS3MDL::setMagBlockData(lisMagBlockData_t magblockdata)
{
    lis_magblockdata = magblockdata;
}

/**************************************************************************/
/*
        Gets the Block Data Update for Magnetic Data
*/
/**************************************************************************/
lisMagBlockData_t LIS3MDL::getMagBlockData()
{
    return lis_magblockdata;
}


/**************************************************************************/
/*
        Sets up the Magnetometer
*/
/**************************************************************************/
void LIS3MDL::setUpMagnetometer(void)
{
    // Set Up the Configuration for the Magnetometer Control Register 1
    /*
    // Set the Temperature Sensor Enable
    uint8_t config1 = lis_tempsensor;
     
    // Set the X and Y Axes Operative Mode Selection
    config1 |= lis_magxyopmode;
     
    // Set the FAST_ODR Enables Data Rates Higher than 80 Hz
    config1 |= lis_magfastoddr;
     
    // Set the Self-Test Enable
    config1 |= lis_magselftest;
    */
    
    uint8_t config1 =   LIS3MDL_REG_MAG_CTRL_TEMP_EN_ENABLED            |   // Temperature Sensor Enabled
                        LIS3MDL_REG_MAG_CTRL_REG1_OM_ULTRA_HIGH         |   // Ultra-High-Performance Mode,  ODR [Hz]: 155, FAST_ODR: 1
                        LIS3MDL_REG_MAG_CTRL_REG1_FAST_ODR_ENABLED      |   // FAST_ODR Enabled
                        LIS3MDL_REG_MAG_CTRL_REG1_ST_DISABLED;              // Self Test Disabled
    
    // Set the Output Data Rate Selection
    config1 |= lis_magdatarate;
        
    // Write the configuration to the Magnetometer Control Register 1
    writeRegister(lis_i2cAddress, LIS3MDL_REG_MAG_CTRL_REG1, config1);
    
    // Wait for the configuration to complete
    delay(lis_conversionDelay);
    
    // Set Up the Configuration for the Magnetometer Control Register 2
    /*
    // Set the Reboot Memory Content
    uint8_t config2 = lis_magreboot;
     
    // Set the Configuration Registers and User Register Reset Function
    config2 |= lis_magreset;
    */
    
    uint8_t config2 =   LIS3MDL_REG_MAG_CTRL_REG2_REBOOT_NORMAL     |   // Normal Mode
                        LIS3MDL_REG_MAG_CTRL_REG2_SOFT_RST_DEFAULT;     // Default Value
    
    // Set the Full-Scale Selection
    config2 |= lis_magscale;
    
    // Write the configuration to the Magnetometer Control Register 2
    writeRegister(lis_i2cAddress, LIS3MDL_REG_MAG_CTRL_REG2, config2);
    
    // Wait for the configuration to complete
    delay(lis_conversionDelay);
    
    // Set Up the Configuration for the Magnetometer Control Register 3
    /*
     // Set the Low-Power Mode Configuration
     uint8_t config3 = lis_maglowpower;
     
     // Set the SPI Serial Interface Mode Selection
     config3 |= lis_magspiwire;
     
     // Set the Operating Mode Selection
     config3 |= lis_magmode;
    */
    
    uint8_t config3 =   LIS3MDL_REG_MAG_CTRL_REG3_LP_0              |   // The Magnetic Data rate is Configured by the DO Bits
                        LIS3MDL_REG_MAG_CTRL_REG3_SIM_4WIRE         |   // 4-Wire Interface
                        LIS3MDL_REG_MAG_CTRL_REG3_MD_CONTINUOUS;        // Continuous-Conversion Mode
    
    // Write the configuration to the Magnetometer Control Register 3
    writeRegister(lis_i2cAddress, LIS3MDL_REG_MAG_CTRL_REG3, config3);
    
    // Wait for the configuration to complete
    delay(lis_conversionDelay);
    
    // Set Up the Configuration for the Magnetometer Control Register 4
    /*
     // Set the Z-Axis Operative Mode Selection
     uint8_t config4 = lis_magzopmode;
     
     // Set the Big/Little Endian Data Selection
     config4 |= lis_magendiandata;
     */
    
    uint8_t config4 =   LIS3MDL_REG_MAG_CTRL_REG4_OMZ_ULTRA_HIGH    |   // Ultra-High-Performance Mode
                        LIS3MDL_REG_MAG_CTRL_REG4_BLE_LSB;              // Data LSB at Lower Address
    
    // Write the configuration to the Magnetometer Control Register 4
    writeRegister(lis_i2cAddress, LIS3MDL_REG_MAG_CTRL_REG4, config4);
    
    // Wait for the configuration to complete
    delay(lis_conversionDelay);
    
    // Set Up the Configuration for the Magnetometer Control Register 5
    /*
     // Set the FAST READ Allows Reading the High Part of DATA OUT Only in Order to Increase Reading Efficiency
     uint8_t config5 = lis_magfastread;
     
     // Set the Block Data Update for Magnetic Data
     config5 |= lis_magblockdata;
     */
    
    uint8_t config5 =   LIS3MDL_REG_MAG_CTRL_REG5_FAST_READ_ENABLED     |   // FAST_READ Enabled
                        LIS3MDL_REG_MAG_CTRL_REG5_BDU_CONTINUOUS;           // Continuous Update Mode
    
    // Write the configuration to the Magnetometer Control Register 5
    writeRegister(lis_i2cAddress, LIS3MDL_REG_MAG_CTRL_REG5, config5);
    
    // Wait for the configuration to complete
    delay(lis_conversionDelay);
    
    
}

/**************************************************************************/
/*
        Reads the 3 axes of the Magnetometer
        The value is expressed in 16 bit as two’s complement
*/
/**************************************************************************/
void LIS3MDL::Measure_Magnetometer()
{
    // Read the Magnetometer
    uint8_t xMagLo, xMagHi, yMagLo, yMagHi, zMagLo, zMagHi;
    
    // Read the Data
    // Reading the Low X-Axis Magnetic Data Register
    xMagLo = readRegister(lis_i2cAddress, LIS3MDL_REG_MAG_OUT_X_L);
    // Reading the High X-Axis Magnetic Data Register
    xMagHi = readRegister(lis_i2cAddress, LIS3MDL_REG_MAG_OUT_X_H);
    // Conversion of the result
    // 16-bit signed result for X-Axis Magnetic Data of LIS3MDL
    lis_magData.X = (int16_t)((xMagHi << 8) | xMagLo);
    
    // Reading the Low Y-Axis Magnetic Data Register
    yMagLo = readRegister(lis_i2cAddress, LIS3MDL_REG_MAG_OUT_Y_L);
    // Reading the High Y-Axis Magnetic Data Register
    yMagHi = readRegister(lis_i2cAddress, LIS3MDL_REG_MAG_OUT_Y_H);
    // Conversion of the result
    // 16-bit signed result for Y-Axis Magnetic Data of LIS3MDL
    lis_magData.Y = (int16_t)((yMagHi << 8) | yMagLo);
    
    // Reading the Low Z-Axis Magnetic Data Register
    zMagLo = readRegister(lis_i2cAddress, LIS3MDL_REG_MAG_OUT_Z_L);
    // Reading the High Z-Axis Magnetic Data Register
    zMagHi = readRegister(lis_i2cAddress, LIS3MDL_REG_MAG_OUT_Z_H);
    // Conversion of the result
    // 16-bit signed result for Z-Axis Magnetic Data of LIS3MDL
    lis_magData.Z = (int16_t)((zMagHi << 8) | zMagLo);
}
