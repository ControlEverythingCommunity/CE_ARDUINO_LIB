/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        L3G4200D
        This code is designed to work with the L3G4200D_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Gyro?sku=L3G4200D_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "L3G4200D.h"

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
        Instantiates a new L3G4200D class with appropriate properties (Gyroscope Address)
*/
/**************************************************************************/
void L3G4200D::getAddr_L3G4200D(uint8_t i2cAddress)
{
    l3g_i2cAddress = i2cAddress;
    l3g_conversionDelay = l3g_conversionDelay;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool L3G4200D::begin()
{
    Wire.begin();
    
    uint8_t Gdevid = readRegister(l3g_i2cAddress, L3G4200D_REG_GYRO_WHO_AM_I);
    if (Gdevid != L3G4200D_GYRO_DEV_ID)
        return false;
    
    // Set up the sensor for Gyroscope
    // setUpGyroscope();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Output Data Rate Selection and Bandwidth Selection
        for the Gyroscope
*/
/**************************************************************************/
void L3G4200D::setGyroDataRate(l3gGyroDataRate_t gyrodatarate)
{
    l3g_gyrodatarate = gyrodatarate;
}

/**************************************************************************/
/*
        Gets the Output Data Rate Selection and Bandwidth Selection
        for the Gyroscope
*/
/**************************************************************************/
l3gGyroDataRate_t L3G4200D::getGyroDataRate()
{
    return l3g_gyrodatarate;
}

/**************************************************************************/
/*
        Sets the Power-Down Mode Enable
*/
/**************************************************************************/
void L3G4200D::setGyroMode(l3gGyroMode_t gyromode)
{
    l3g_gyromode = gyromode;
}

/**************************************************************************/
/*
        Gets the Power-Down Mode Enable
*/
/**************************************************************************/
l3gGyroMode_t L3G4200D::getGyroMode()
{
    return l3g_gyromode;
}

/**************************************************************************/
/*
        Sets the Gyroscope Z-Axis Enable
*/
/**************************************************************************/
void L3G4200D::setGyroZen(l3gGyroZen_t gyrozen)
{
    l3g_gyrozen = gyrozen;
}

/**************************************************************************/
/*
        Gets the Gyroscope Z-Axis Enable
*/
/**************************************************************************/
l3gGyroZen_t L3G4200D::getGyroZen()
{
    return l3g_gyrozen;
}

/**************************************************************************/
/*
        Sets the Gyroscope Y-Axis Enable
*/
/**************************************************************************/
void L3G4200D::setGyroYen(l3gGyroYen_t gyroyen)
{
    l3g_gyroyen = gyroyen;
}

/**************************************************************************/
/*
        Gets the Gyroscope Y-Axis Enable
*/
/**************************************************************************/
l3gGyroYen_t L3G4200D::getGyroYen()
{
    return l3g_gyroyen;
}

/**************************************************************************/
/*
        Sets the Gyroscope X-Axis Enable
*/
/**************************************************************************/
void L3G4200D::setGyroXen(l3gGyroXen_t gyroxen)
{
    l3g_gyroxen = gyroxen;
}

/**************************************************************************/
/*
        Gets the Gyroscope X-Axis Enable
*/
/**************************************************************************/
l3gGyroXen_t L3G4200D::getGyroXen()
{
    return l3g_gyroxen;
}

/**************************************************************************/
/*
        Sets the Block Data Update
*/
/**************************************************************************/
void L3G4200D::setGyroBlockData(l3gGyroBlockData_t gyroblockdata)
{
    l3g_gyroblockdata = gyroblockdata;
}

/**************************************************************************/
/*
        Gets the Block Data Update
*/
/**************************************************************************/
l3gGyroBlockData_t L3G4200D::getGyroBlockData()
{
    return l3g_gyroblockdata;
}

/**************************************************************************/
/*
        Sets the Big/Little Endian Data Selection
*/
/**************************************************************************/
void L3G4200D::setGyroEndianData(l3gGyroEndianData_t gyroendiandata)
{
    l3g_gyroendiandata = gyroendiandata;
}

/**************************************************************************/
/*
        Gets the Big/Little Endian Data Selection
*/
/**************************************************************************/
l3gGyroEndianData_t L3G4200D::getGyroEndianData()
{
    return l3g_gyroendiandata;
}

/**************************************************************************/
/*
        Sets the Full-Scale Selection for the Gyroscope
*/
/**************************************************************************/
void L3G4200D::setGyroScale(l3gGyroScale_t gyroscale)
{
    l3g_gyroscale = gyroscale;
}
                    
/**************************************************************************/
/*
        Gets the Full-Scale Selection for the Gyroscope
*/
/**************************************************************************/
l3gGyroScale_t L3G4200D::getGyroScale()
{
    return l3g_gyroscale;
}

/**************************************************************************/
/*
        Sets the Self-Test Enable
*/
/**************************************************************************/
void L3G4200D::setGyroSelfTest(l3gGyroSelfTest_t gyroselftest)
{
    l3g_gyroselftest = gyroselftest;
}

/**************************************************************************/
/*
        Gets the Self-Test Enable
*/
/**************************************************************************/
l3gGyroSelfTest_t L3G4200D::getGyroSelfTest()
{
    return l3g_gyroselftest;
}

/**************************************************************************/
/*
        Sets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
void L3G4200D::setGyroSPIWire(l3gGyroSPIWire_t gyrospiwire)
{
    l3g_gyrospiwire = gyrospiwire;
}

/**************************************************************************/
/*
        Gets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
l3gGyroSPIWire_t L3G4200D::getGyroSPIWire()
{
    return l3g_gyrospiwire;
}

/**************************************************************************/
/*
        Sets up the Gyroscope
*/
/**************************************************************************/
void L3G4200D::setUpGyroscope(void)
{
    // Set Up the Configuration for the Gyroscope Control Register 1
    /*
    // Set the Output Data Rate
    uint8_t config1 = l3g_gyrodatarate;
    
    // Set the Power Down Mode Enable
    config1 |= l3g_gyromode;
    
    // Set the Gyroscpe Z-Axis Enable
    config1 |= l3g_gyrozen;
    
    // Set the Gyroscpe Y-Axis Enable
    config1 |= l3g_gyroyen;
    
    // Set the Gyroscpe X-Axis Enable
    config1 |= l3g_gyroxen;
    */
    
    uint8_t config1 =   L3G4200D_REG_GYRO_CTRL_REG1_PD_NORMAL           |   // Normal Mode
                        L3G4200D_REG_GYRO_CTRL_REG1_ZEN_ENABLE          |   // Gyroscope Z-Axis Enabled
                        L3G4200D_REG_GYRO_CTRL_REG1_YEN_ENABLE          |   // Gyroscope Y-Axis Enabled
                        L3G4200D_REG_GYRO_CTRL_REG1_XEN_ENABLE;             // Gyroscope X-Axis Enabled
    
    config1 |= l3g_gyrodatarate;       // Output Data Rate and Bandwidth Selection
    
    // Write the configuration to the Gyroscope Control Register 1
    writeRegister(l3g_i2cAddress, L3G4200D_REG_GYRO_CTRL_REG1, config1);
    
    // Wait for the configuration to complete
    delay(l3g_conversionDelay);
    
    // Set Up the Configuration for the Gyroscope Control Register 4
    /*
    // Set the Block Data Update
    uint8_t config4 = l3g_gyroblockdata;
    
    // Set the Big/Little Endian Data Selection
    config4 |= l3g_gyroendiandata;
    
    // Set the Self-Test Enable
    config4 |= l3g_gyroselftest;
    
    // Set the SPI Serial Interface Mode Selection
    config4 |= l3g_gyrospiwire;
    */
    
    uint8_t config4 =   L3G4200D_REG_GYRO_CTRL_REG4_BDU_CONTINUOUS      |   // Continuous Update
                        L3G4200D_REG_GYRO_CTRL_REG4_BLE_LSB             |   // Data LSB @ Lower Address
                        L3G4200D_REG_GYRO_CTRL_REG4_ST_NORMAL           |   // Normal Mode
                        L3G4200D_REG_GYRO_CTRL_REG4_SIM_4WIRE;              // 4-Wire Interface
    
    config4 |= l3g_gyroscale;      // Full-Scale Selection
    
    // Write the configuration to the Gyroscope Control Register 4
    writeRegister(l3g_i2cAddress, L3G4200D_REG_GYRO_CTRL_REG4, config4);
    
    // Wait for the configuration to complete
    delay(l3g_conversionDelay);
}
/**************************************************************************/
/*
        Reads up the Sensor which comprises of an Gyroscope
*/
/**************************************************************************/
void L3G4200D::Measure_Sensor(void)
{
    // Read all the Sensors
    // Gyroscope Data
    Measure_Gyroscope();
}

/**************************************************************************/
/*
        Reads the 3 axes of the Gyroscope
        The value is expressed in 16 bit as two’s complement
*/
/**************************************************************************/
void L3G4200D::Measure_Gyroscope()
{
    // Read the Gyroscope
    uint8_t xGyroLo, xGyroHi, yGyroLo, yGyroHi, zGyroLo, zGyroHi;
    
    // Read the Data
    // Reading the Low X-Axis Angular Rate Data Register
    xGyroLo = readRegister(l3g_i2cAddress, L3G4200D_REG_GYRO_OUT_X_L);
    // Reading the High X-Axis Angular Rate Data Register
    xGyroHi = readRegister(l3g_i2cAddress, L3G4200D_REG_GYRO_OUT_X_H);
    // Conversion of the result
    // 16-bit signed result for X-Axis Angular Rate Data of L3G4200D
    l3g_gyroData.X = (int16_t)((xGyroHi << 8) | xGyroLo);
    
    // Reading the Low Y-Axis Angular Rate Data Register
    yGyroLo = readRegister(l3g_i2cAddress, L3G4200D_REG_GYRO_OUT_Y_L);
    // Reading the High Y-Axis Angular Rate Data Register
    yGyroHi = readRegister(l3g_i2cAddress, L3G4200D_REG_GYRO_OUT_Y_H);
    // Conversion of the result
    // 16-bit signed result for Y-Axis Angular Rate Data of L3G4200D
    l3g_gyroData.Y = (int16_t)((yGyroHi << 8) | yGyroLo);
    
    // Reading the Low Z-Axis Angular Rate Data Register
    zGyroLo = readRegister(l3g_i2cAddress, L3G4200D_REG_GYRO_OUT_Z_L);
    // Reading the High Z-Axis Angular Rate Data Register
    zGyroHi = readRegister(l3g_i2cAddress, L3G4200D_REG_GYRO_OUT_Z_H);
    // Conversion of the result
    // 16-bit signed result for Z-Axis Angular Rate Data of L3G4200D
    l3g_gyroData.Z = (int16_t)((zGyroHi << 8) | zGyroLo);
    
}
