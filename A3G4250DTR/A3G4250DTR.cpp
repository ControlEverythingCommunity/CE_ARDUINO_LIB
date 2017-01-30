/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        A3G4250DTR
        This code is designed to work with the A3G4250DTR_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Gyro?sku=A3G4250DTR_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "A3G4250DTR.h"

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
        Instantiates a new A3G4250DTR class with appropriate properties (Gyroscope Address)
*/
/**************************************************************************/
void A3G4250DTR::getAddr_A3G4250DTR(uint8_t i2cAddress)
{
    a3g_i2cAddress = i2cAddress;
    a3g_conversionDelay = a3g_conversionDelay;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool A3G4250DTR::begin()
{
    Wire.begin();
    
    uint8_t Gdevid = readRegister(a3g_i2cAddress, A3G4250DTR_REG_GYRO_WHO_AM_I);
    if (Gdevid != A3G4250DTR_GYRO_DEV_ID)
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
void A3G4250DTR::setGyroDataRate(a3gGyroDataRate_t gyrodatarate)
{
    a3g_gyrodatarate = gyrodatarate;
}

/**************************************************************************/
/*
        Gets the Output Data Rate Selection and Bandwidth Selection
        for the Gyroscope
*/
/**************************************************************************/
a3gGyroDataRate_t A3G4250DTR::getGyroDataRate()
{
    return a3g_gyrodatarate;
}

/**************************************************************************/
/*
        Sets the Power-Down Mode Enable
*/
/**************************************************************************/
void A3G4250DTR::setGyroMode(a3gGyroMode_t gyromode)
{
    a3g_gyromode = gyromode;
}

/**************************************************************************/
/*
        Gets the Power-Down Mode Enable
*/
/**************************************************************************/
a3gGyroMode_t A3G4250DTR::getGyroMode()
{
    return a3g_gyromode;
}

/**************************************************************************/
/*
        Sets the Gyroscope Z-Axis Enable
*/
/**************************************************************************/
void A3G4250DTR::setGyroZen(a3gGyroZen_t gyrozen)
{
    a3g_gyrozen = gyrozen;
}

/**************************************************************************/
/*
        Gets the Gyroscope Z-Axis Enable
*/
/**************************************************************************/
a3gGyroZen_t A3G4250DTR::getGyroZen()
{
    return a3g_gyrozen;
}

/**************************************************************************/
/*
        Sets the Gyroscope Y-Axis Enable
*/
/**************************************************************************/
void A3G4250DTR::setGyroYen(a3gGyroYen_t gyroyen)
{
    a3g_gyroyen = gyroyen;
}

/**************************************************************************/
/*
        Gets the Gyroscope Y-Axis Enable
*/
/**************************************************************************/
a3gGyroYen_t A3G4250DTR::getGyroYen()
{
    return a3g_gyroyen;
}

/**************************************************************************/
/*
        Sets the Gyroscope X-Axis Enable
*/
/**************************************************************************/
void A3G4250DTR::setGyroXen(a3gGyroXen_t gyroxen)
{
    a3g_gyroxen = gyroxen;
}

/**************************************************************************/
/*
        Gets the Gyroscope X-Axis Enable
*/
/**************************************************************************/
a3gGyroXen_t A3G4250DTR::getGyroXen()
{
    return a3g_gyroxen;
}

/**************************************************************************/
/*
        Sets the Big/Little Endian Data Selection
*/
/**************************************************************************/
void A3G4250DTR::setGyroEndianData(a3gGyroEndianData_t gyroendiandata)
{
    a3g_gyroendiandata = gyroendiandata;
}

/**************************************************************************/
/*
        Gets the Big/Little Endian Data Selection
*/
/**************************************************************************/
a3gGyroEndianData_t A3G4250DTR::getGyroEndianData()
{
    return a3g_gyroendiandata;
}

/**************************************************************************/
/*
        Sets the Self-Test Enable
*/
/**************************************************************************/
void A3G4250DTR::setGyroSelfTest(a3gGyroSelfTest_t gyroselftest)
{
    a3g_gyroselftest = gyroselftest;
}

/**************************************************************************/
/*
        Gets the Self-Test Enable
*/
/**************************************************************************/
a3gGyroSelfTest_t A3G4250DTR::getGyroSelfTest()
{
    return a3g_gyroselftest;
}

/**************************************************************************/
/*
        Sets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
void A3G4250DTR::setGyroSPIWire(a3gGyroSPIWire_t gyrospiwire)
{
    a3g_gyrospiwire = gyrospiwire;
}

/**************************************************************************/
/*
        Gets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
a3gGyroSPIWire_t A3G4250DTR::getGyroSPIWire()
{
    return a3g_gyrospiwire;
}

/**************************************************************************/
/*
        Sets up the Gyroscope
*/
/**************************************************************************/
void A3G4250DTR::setUpGyroscope(void)
{
    // Set Up the Configuration for the Gyroscope Control Register 1
    /*
    // Set the Output Data Rate
    uint8_t config1 = a3g_gyrodatarate;
    
    // Set the Power Down Mode Enable
    config1 |= a3g_gyromode;
    
    // Set the Gyroscpe Z-Axis Enable
    config1 |= a3g_gyrozen;
    
    // Set the Gyroscpe Y-Axis Enable
    config1 |= a3g_gyroyen;
    
    // Set the Gyroscpe X-Axis Enable
    config1 |= a3g_gyroxen;
    */
    
    uint8_t config1 =   A3G4250DTR_REG_GYRO_CTRL_REG1_PD_NORMAL           |   // Normal Mode
                        A3G4250DTR_REG_GYRO_CTRL_REG1_ZEN_ENABLE          |   // Gyroscope Z-Axis Enabled
                        A3G4250DTR_REG_GYRO_CTRL_REG1_YEN_ENABLE          |   // Gyroscope Y-Axis Enabled
                        A3G4250DTR_REG_GYRO_CTRL_REG1_XEN_ENABLE;             // Gyroscope X-Axis Enabled
    
    config1 |= a3g_gyrodatarate;       // Output Data Rate and Bandwidth Selection
    
    // Write the configuration to the Gyroscope Control Register 1
    writeRegister(a3g_i2cAddress, A3G4250DTR_REG_GYRO_CTRL_REG1, config1);
    
    // Wait for the configuration to complete
    delay(a3g_conversionDelay);
    
    // Set Up the Configuration for the Gyroscope Control Register 4
    /*
    // Set the Big/Little Endian Data Selection
    uint8_t config4 = a3g_gyroendiandata;
    
    // Set the Self-Test Enable
    config4 |= a3g_gyroselftest;
    
    // Set the SPI Serial Interface Mode Selection
    config4 |= a3g_gyrospiwire;
    */
    
    uint8_t config4 =   A3G4250DTR_REG_GYRO_CTRL_REG4_BLE_LSB             |   // Data LSB @ Lower Address
                        A3G4250DTR_REG_GYRO_CTRL_REG4_ST_NORMAL           |   // Normal Mode
                        A3G4250DTR_REG_GYRO_CTRL_REG4_SIM_4WIRE;              // 4-Wire Interface
    
    // Full-Scale Selection
    // ±245 dps full scale
    
    // Write the configuration to the Gyroscope Control Register 4
    writeRegister(a3g_i2cAddress, A3G4250DTR_REG_GYRO_CTRL_REG4, config4);
    
    // Wait for the configuration to complete
    delay(a3g_conversionDelay);
}
/**************************************************************************/
/*
        Reads up the Sensor which comprises of an Gyroscope
*/
/**************************************************************************/
void A3G4250DTR::Measure_Sensor(void)
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
void A3G4250DTR::Measure_Gyroscope()
{
    // Read the Gyroscope
    uint8_t xGyroLo, xGyroHi, yGyroLo, yGyroHi, zGyroLo, zGyroHi;
    
    // Read the Data
    // Reading the Low X-Axis Angular Rate Data Register
    xGyroLo = readRegister(a3g_i2cAddress, A3G4250DTR_REG_GYRO_OUT_X_L);
    // Reading the High X-Axis Angular Rate Data Register
    xGyroHi = readRegister(a3g_i2cAddress, A3G4250DTR_REG_GYRO_OUT_X_H);
    // Conversion of the result
    // 16-bit signed result for X-Axis Angular Rate Data of A3G4250DTR
    a3g_gyroData.X = (int16_t)((xGyroHi << 8) | xGyroLo);
    
    // Reading the Low Y-Axis Angular Rate Data Register
    yGyroLo = readRegister(a3g_i2cAddress, A3G4250DTR_REG_GYRO_OUT_Y_L);
    // Reading the High Y-Axis Angular Rate Data Register
    yGyroHi = readRegister(a3g_i2cAddress, A3G4250DTR_REG_GYRO_OUT_Y_H);
    // Conversion of the result
    // 16-bit signed result for Y-Axis Angular Rate Data of A3G4250DTR
    a3g_gyroData.Y = (int16_t)((yGyroHi << 8) | yGyroLo);
    
    // Reading the Low Z-Axis Angular Rate Data Register
    zGyroLo = readRegister(a3g_i2cAddress, A3G4250DTR_REG_GYRO_OUT_Z_L);
    // Reading the High Z-Axis Angular Rate Data Register
    zGyroHi = readRegister(a3g_i2cAddress, A3G4250DTR_REG_GYRO_OUT_Z_H);
    // Conversion of the result
    // 16-bit signed result for Z-Axis Angular Rate Data of A3G4250DTR
    a3g_gyroData.Z = (int16_t)((zGyroHi << 8) | zGyroLo);
    
}
