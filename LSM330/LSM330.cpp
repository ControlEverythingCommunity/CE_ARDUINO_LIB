/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LSM330
        This code is designed to work with the LSM330_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelorometer?sku=LSM330_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "LSM330.h"

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
        Instantiates a new LSM330 class with appropriate properties (Gyroscope Address)
*/
/**************************************************************************/
void LSM330::getAddrX_LSM330(uint8_t i2cAddress)
{
    lsm_i2cAddressX = i2cAddress;
    lsm_conversionDelay = lsm_conversionDelay;
}

/**************************************************************************/
/*
        Instantiates a new LSM330 class with appropriate properties
        Accelerometer Address
*/
/**************************************************************************/
void LSM330::getAddrG_LSM330(uint8_t i2cAddress)
{
    lsm_i2cAddressG = i2cAddress;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool LSM330::begin()
{
    Wire.begin();
    
    uint8_t Xdevid = readRegister(lsm_i2cAddressX, LSM330_REG_ACCEL_WHO_AM_I);
    if (Xdevid != LSM330_ACCEL_DEV_ID)
        return false;
    
    uint8_t Gdevid = readRegister(lsm_i2cAddressG, LSM330_REG_GYRO_WHO_AM_I);
    if (Gdevid != LSM330_GYRO_DEV_ID)
        return false;
    
    // Set up the sensor for Accelerometer and Gyroscope
    // setUpAccelerometer();
    // setUpGyroscope();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Output Data Rate Selection for the Accelerometer
*/
/**************************************************************************/
void LSM330::setAccelDataRate(lsmAccelDataRate_t acceldatarate)
{
    lsm_acceldatarate = acceldatarate;
}

/**************************************************************************/
/*
        Gets the Output Data Rate Selection for the Accelerometer
*/
/**************************************************************************/
lsmAccelDataRate_t LSM330::getAccelDataRate()
{
    return lsm_acceldatarate;
}

/**************************************************************************/
/*
        Sets the Block Data Update for Acceleration Data
*/
/**************************************************************************/
void LSM330::setAccelBlockData(lsmAccelBlockData_t accelblockdata)
{
    lsm_accelblockdata = accelblockdata;
}

/**************************************************************************/
/*
        Gets the Block Data Update for Acceleration Data
*/
/**************************************************************************/
lsmAccelBlockData_t LSM330::getAccelBlockData()
{
    return lsm_accelblockdata;
}

/**************************************************************************/
/*
 Sets the Acceleration Z-Axis Enable
 */
/**************************************************************************/
void LSM330::setAccelZen(lsmAccelZen_t accelzen)
{
    lsm_accelzen = accelzen;
}

/**************************************************************************/
/*
 Gets the Acceleration Z-Axis Enable
 */
/**************************************************************************/
lsmAccelZen_t LSM330::getAccelZen()
{
    return lsm_accelzen;
}

/**************************************************************************/
/*
 Sets the Acceleration Y-Axis Enable
 */
/**************************************************************************/
void LSM330::setAccelYen(lsmAccelYen_t accelyen)
{
    lsm_accelyen = accelyen;
}

/**************************************************************************/
/*
 Gets the Acceleration Y-Axis Enable
 */
/**************************************************************************/
lsmAccelYen_t LSM330::getAccelYen()
{
    return lsm_accelyen;
}

/**************************************************************************/
/*
 Sets the Acceleration X-Axis Enable
 */
/**************************************************************************/
void LSM330::setAccelXen(lsmAccelXen_t accelxen)
{
    lsm_accelxen = accelxen;
}

/**************************************************************************/
/*
 Gets the Acceleration X-Axis Enable
 */
/**************************************************************************/
lsmAccelXen_t LSM330::getAccelXen()
{
    return lsm_accelxen;
}

/**************************************************************************/
/*
 Sets the Acceleration Anti-Alias Filter Bandwidth
 */
/**************************************************************************/
void LSM330::setAccelBandwidth(lsmAccelBandwidth_t accelbandwidth)
{
    lsm_accelbandwidth = accelbandwidth;
}

/**************************************************************************/
/*
 Gets the Acceleration Anti-Alias Filter Bandwidth
 */
/**************************************************************************/
lsmAccelBandwidth_t LSM330::getAccelBandwidth()
{
    return lsm_accelbandwidth;
}

/**************************************************************************/
/*
 Sets the Acceleration Full-Scale Selection
 */
/**************************************************************************/
void LSM330::setAccelRange(lsmAccelRange_t accelrange)
{
    lsm_accelrange = accelrange;
}

/**************************************************************************/
/*
 Gets the Acceleration Full-Scale Selection
 */
/**************************************************************************/
lsmAccelRange_t LSM330::getAccelRange()
{
    return lsm_accelrange;
}

/**************************************************************************/
/*
        Sets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
void LSM330::setAccelSPIWire(lsmAccelSPIWire_t accelspiwire)
{
    lsm_accelspiwire = accelspiwire;
}

/**************************************************************************/
/*
        Gets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
lsmAccelSPIWire_t LSM330::getAccelSPIWire()
{
    return lsm_accelspiwire;
}

/**************************************************************************/
/*
        Sets the Output Data Rate Selection and Bandwidth Selection
        for the Gyroscope
*/
/**************************************************************************/
void LSM330::setGyroDataRate(lsmGyroDataRate_t gyrodatarate)
{
    lsm_gyrodatarate = gyrodatarate;
}

/**************************************************************************/
/*
        Gets the Output Data Rate Selection and Bandwidth Selection
        for the Gyroscope
*/
/**************************************************************************/
lsmGyroDataRate_t LSM330::getGyroDataRate()
{
    return lsm_gyrodatarate;
}

/**************************************************************************/
/*
        Sets the Power-Down Mode Enable
*/
/**************************************************************************/
void LSM330::setGyroMode(lsmGyroMode_t gyromode)
{
    lsm_gyromode = gyromode;
}

/**************************************************************************/
/*
        Gets the Power-Down Mode Enable
*/
/**************************************************************************/
lsmGyroMode_t LSM330::getGyroMode()
{
    return lsm_gyromode;
}

/**************************************************************************/
/*
        Sets the Gyroscope Z-Axis Enable
*/
/**************************************************************************/
void LSM330::setGyroZen(lsmGyroZen_t gyrozen)
{
    lsm_gyrozen = gyrozen;
}

/**************************************************************************/
/*
        Gets the Gyroscope Z-Axis Enable
*/
/**************************************************************************/
lsmGyroZen_t LSM330::getGyroZen()
{
    return lsm_gyrozen;
}

/**************************************************************************/
/*
        Sets the Gyroscope Y-Axis Enable
*/
/**************************************************************************/
void LSM330::setGyroYen(lsmGyroYen_t gyroyen)
{
    lsm_gyroyen = gyroyen;
}

/**************************************************************************/
/*
        Gets the Gyroscope Y-Axis Enable
*/
/**************************************************************************/
lsmGyroYen_t LSM330::getGyroYen()
{
    return lsm_gyroyen;
}

/**************************************************************************/
/*
        Sets the Gyroscope X-Axis Enable
*/
/**************************************************************************/
void LSM330::setGyroXen(lsmGyroXen_t gyroxen)
{
    lsm_gyroxen = gyroxen;
}

/**************************************************************************/
/*
        Gets the Gyroscope X-Axis Enable
*/
/**************************************************************************/
lsmGyroXen_t LSM330::getGyroXen()
{
    return lsm_gyroxen;
}

/**************************************************************************/
/*
        Sets the Block Data Update
*/
/**************************************************************************/
void LSM330::setGyroBlockData(lsmGyroBlockData_t gyroblockdata)
{
    lsm_gyroblockdata = gyroblockdata;
}

/**************************************************************************/
/*
        Gets the Block Data Update
*/
/**************************************************************************/
lsmGyroBlockData_t LSM330::getGyroBlockData()
{
    return lsm_gyroblockdata;
}

/**************************************************************************/
/*
        Sets the Big/Little Endian Data Selection
*/
/**************************************************************************/
void LSM330::setGyroEndianData(lsmGyroEndianData_t gyroendiandata)
{
    lsm_gyroendiandata = gyroendiandata;
}

/**************************************************************************/
/*
        Gets the Big/Little Endian Data Selection
*/
/**************************************************************************/
lsmGyroEndianData_t LSM330::getGyroEndianData()
{
    return lsm_gyroendiandata;
}

/**************************************************************************/
/*
        Sets the Full-Scale Selection for the Gyroscope
*/
/**************************************************************************/
void LSM330::setGyroScale(lsmGyroScale_t gyroscale)
{
    lsm_gyroscale = gyroscale;
}
                    
/**************************************************************************/
/*
        Gets the Full-Scale Selection for the Gyroscope
*/
/**************************************************************************/
lsmGyroScale_t LSM330::getGyroScale()
{
    return lsm_gyroscale;
}

/**************************************************************************/
/*
        Sets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
void LSM330::setGyroSPIWire(lsmGyroSPIWire_t gyrospiwire)
{
    lsm_gyrospiwire = gyrospiwire;
}

/**************************************************************************/
/*
        Gets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
lsmGyroSPIWire_t LSM330::getGyroSPIWire()
{
    return lsm_gyrospiwire;
}

/**************************************************************************/
/*
        Sets up the Accelerometer
*/
/**************************************************************************/
void LSM330::setUpAccelerometer(void)
{
    // Set Up the Configuration for the Accelerometer Control Register 5
    /*
     // Set the Acceleration Output Data Rate
     uint8_t config5 = lsm_acceldatarate;
     
     // Set the Block Data Update for Acceleration Data
     config5 |= lsm_accelblockdata;
     
     // Set the Acceleration Z-Axis Enable
     config5 |= lsm_accelzen;
     
     // Set the Acceleration Y-Axis Enable
     config5 |= lsm_accelyen;
     
     // Set the Acceleration X-Axis Enable
     config5 |= lsm_accelxen;
     */
    
    uint8_t config5 =   LSM330_REG_ACCEL_CTRL_REG5_BDU_CONTINUOUS   |   // Continuous Update
                        LSM330_REG_ACCEL_CTRL_REG5_AZEN_ENABLE      |   // Acceleration Z-Axis Enabled
                        LSM330_REG_ACCEL_CTRL_REG5_AYEN_ENABLE      |   // Acceleration Y-Axis Enabled
                        LSM330_REG_ACCEL_CTRL_REG5_AXEN_ENABLE;         // Acceleration X-Axis Enabled
    
    config5 |= lsm_acceldatarate;      // Acceleration Data Rate Configuration
    
    // Write the configuration to the Accelerometer/netometer Control Register 1
    writeRegister(lsm_i2cAddressX, LSM330_REG_ACCEL_CTRL_REG5, config5);
    
    // Wait for the configuration to complete
    delay(lsm_conversionDelay);
    
    // Set Up the Configuration for the Accelerometer Control Register 6
    /*
     // Set the Accelerometer Anti-Alias Filter Bandwidth
     uint8_t config6 = lsm_accelbandwidth;
     
     // Set the SPI Serial Interface Mode Selection
     config6 |= lsm_accelspiwire;
     */
    
    uint8_t config6 =   LSM330_REG_ACCEL_CTRL_REG6_ABW_800          |   // 800 Hz, Accelerometer Anti-Alias Filter Bandwidth
                        LSM330_REG_ACCEL_CTRL_REG6_SIM_4WIRE;           // 4-Wire Interface
    
    config6 |= lsm_accelrange;     // Acceleration Full-Scale Selection
    
    // Write the configuration to the Accelerometer/netometer Control Register 2
    writeRegister(lsm_i2cAddressX, LSM330_REG_ACCEL_CTRL_REG6, config6);
    
    // Wait for the configuration to complete
    delay(lsm_conversionDelay);
}


/**************************************************************************/
/*
        Sets up the Gyroscope
*/
/**************************************************************************/
void LSM330::setUpGyroscope(void)
{
    // Set Up the Configuration for the Gyroscope Control Register 1
    /*
    // Set the Output Data Rate
    uint8_t config1 = lsm_gyrodatarate;
    
    // Set the Power Down Mode Enable
    config1 |= lsm_gyromode;
    
    // Set the Gyroscpe Z-Axis Enable
    config1 |= lsm_gyrozen;
    
    // Set the Gyroscpe Y-Axis Enable
    config1 |= lsm_gyroyen;
    
    // Set the Gyroscpe X-Axis Enable
    config1 |= lsm_gyroxen;
    */
    
    uint8_t config1 =   LSM330_REG_GYRO_CTRL_REG1_PD_NORMAL         |   // Normal Mode
                        LSM330_REG_GYRO_CTRL_REG1_ZEN_ENABLE        |   // Gyroscope Z-Axis Enabled
                        LSM330_REG_GYRO_CTRL_REG1_YEN_ENABLE        |   // Gyroscope Y-Axis Enabled
                        LSM330_REG_GYRO_CTRL_REG1_XEN_ENABLE;           // Gyroscope X-Axis Enabled
    
    config1 |= lsm_gyrodatarate;       // Output Data Rate and Bandwidth Selection
    
    // Write the configuration to the Gyroscope Control Register 1
    writeRegister(lsm_i2cAddressG, LSM330_REG_GYRO_CTRL_REG1, config1);
    
    // Wait for the configuration to complete
    delay(lsm_conversionDelay);
    
    // Set Up the Configuration for the Gyroscope Control Register 4
    /*
    // Set the Block Data Update
    uint8_t config4 = lsm_gyroblockdata;
    
    // Set the Big/Little Endian Data Selection
    config4 |= lsm_gyroendiandata;
    
    // Set the Full Scale Selection
    config4 |= lsm_gyroscale;
    
    // Set the SPI Serial Interface Mode Selection
    config4 |= lsm_gyrospiwire;
    */
    
    uint8_t config4 =   LSM330_REG_GYRO_CTRL_REG4_BDU_CONTINUOUS    |   // Continuous Update
                        LSM330_REG_GYRO_CTRL_REG4_BLE_LSB           |   // Data LSB @ Lower Address
                        LSM330_REG_GYRO_CTRL_REG4_SIM_3WIRE_DISABLE;    // 3-Wire Interface Disabled
    
    config4 |= lsm_gyroscale;      // Full-Scale Selection
    
    // Write the configuration to the Gyroscope Control Register 4
    writeRegister(lsm_i2cAddressG, LSM330_REG_GYRO_CTRL_REG4, config4);
    
    // Wait for the configuration to complete
    delay(lsm_conversionDelay);
}
/**************************************************************************/
/*
        Reads up the Sensor which comprises of an Accelerometer and Gyroscope
*/
/**************************************************************************/
void LSM330::Measure_Sensor(void)
{
    // Read all the Sensors
    // Accelerometer, Gyroscope Data
    Measure_Accelerometer();
    Measure_Gyroscope();
}

/**************************************************************************/
/*
        Reads the 3 axes of the Accelerometer
        The value is expressed in 16 bit as two’s complement
*/
/**************************************************************************/
void LSM330::Measure_Accelerometer()
{
    // Read the Accelerometer
    uint8_t xAccelLo, xAccelHi, yAccelLo, yAccelHi, zAccelLo, zAccelHi;
    
    // Read the Data
    // Reading the Low X-Axis Acceleration Data Register
    xAccelLo = readRegister(lsm_i2cAddressX, LSM330_REG_ACCEL_OUT_X_L);
    // Reading the High X-Axis Acceleration Data Register
    xAccelHi = readRegister(lsm_i2cAddressX, LSM330_REG_ACCEL_OUT_X_H);
    // Conversion of the result
    // 16-bit signed result for X-Axis Acceleration Data of LSM330
    lsm_accelData.X = (int16_t)((xAccelHi << 8) | xAccelLo);
    
    // Reading the Low Y-Axis Acceleration Data Register
    yAccelLo = readRegister(lsm_i2cAddressX, LSM330_REG_ACCEL_OUT_Y_L);
    // Reading the High Y-Axis Acceleration Data Register
    yAccelHi = readRegister(lsm_i2cAddressX, LSM330_REG_ACCEL_OUT_Y_H);
    // Conversion of the result
    // 16-bit signed result for Y-Axis Acceleration Data of LSM330
    lsm_accelData.Y = (int16_t)((yAccelHi << 8) | yAccelLo);
    
    // Reading the Low Z-Axis Acceleration Data Register
    zAccelLo = readRegister(lsm_i2cAddressX, LSM330_REG_ACCEL_OUT_Z_L);
    // Reading the High Z-Axis Acceleration Data Register
    zAccelHi = readRegister(lsm_i2cAddressX, LSM330_REG_ACCEL_OUT_Z_H);
    // Conversion of the result
    // 16-bit signed result for Z-Axis Acceleration Data of LSM330
    lsm_accelData.Z = (int16_t)((zAccelHi << 8) | zAccelLo);
}

/**************************************************************************/
/*
        Reads the 3 axes of the Gyroscope
        The value is expressed in 16 bit as two’s complement
*/
/**************************************************************************/
void LSM330::Measure_Gyroscope()
{
    // Read the Gyroscope
    uint8_t xGyroLo, xGyroHi, yGyroLo, yGyroHi, zGyroLo, zGyroHi;
    
    // Read the Data
    // Reading the Low X-Axis Angular Rate Data Register
    xGyroLo = readRegister(lsm_i2cAddressG, LSM330_REG_GYRO_OUT_X_L);
    // Reading the High X-Axis Angular Rate Data Register
    xGyroHi = readRegister(lsm_i2cAddressG, LSM330_REG_GYRO_OUT_X_H);
    // Conversion of the result
    // 16-bit signed result for X-Axis Angular Rate Data of LSM330
    lsm_gyroData.X = (int16_t)((xGyroHi << 8) | xGyroLo);
    
    // Reading the Low Y-Axis Angular Rate Data Register
    yGyroLo = readRegister(lsm_i2cAddressG, LSM330_REG_GYRO_OUT_Y_L);
    // Reading the High Y-Axis Angular Rate Data Register
    yGyroHi = readRegister(lsm_i2cAddressG, LSM330_REG_GYRO_OUT_Y_H);
    // Conversion of the result
    // 16-bit signed result for Y-Axis Angular Rate Data of LSM330
    lsm_gyroData.Y = (int16_t)((yGyroHi << 8) | yGyroLo);
    
    // Reading the Low Z-Axis Angular Rate Data Register
    zGyroLo = readRegister(lsm_i2cAddressG, LSM330_REG_GYRO_OUT_Z_L);
    // Reading the High Z-Axis Angular Rate Data Register
    zGyroHi = readRegister(lsm_i2cAddressG, LSM330_REG_GYRO_OUT_Z_H);
    // Conversion of the result
    // 16-bit signed result for Z-Axis Angular Rate Data of LSM330
    lsm_gyroData.Z = (int16_t)((zGyroHi << 8) | zGyroLo);
    
}
