/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LSM303DLHC
        This code is designed to work with the LSM303DLHC_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "LSM303DLHC.h"

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
        Instantiates a new LSM303DLHC class with appropriate properties (Accelerometer Address)
*/
/**************************************************************************/
void LSM303DLHC::getAddrX_LSM303DLHC(uint8_t i2cAddress)
{
    lsm_i2cAddressX = i2cAddress;
    lsm_conversionDelay = lsm_conversionDelay;
}

/**************************************************************************/
/*
        Instantiates a new LSM303DLHC class with appropriate properties
        Accelerometer & Magnetometer Address
*/
/**************************************************************************/
void LSM303DLHC::getAddrM_LSM303DLHC(uint8_t i2cAddress)
{
    lsm_i2cAddressM = i2cAddress;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool LSM303DLHC::begin()
{
    Wire.begin();
    
    // Set up the sensor for Accelerometer and Magnetometer
    // setUpAccelerometer();
    // setUpMagnetometer();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Output Data Rate Selection for the Accelerometer
*/
/**************************************************************************/
void LSM303DLHC::setAccelDataRate(lsmAccelDataRate_t acceldatarate)
{
    lsm_acceldatarate = acceldatarate;
}

/**************************************************************************/
/*
        Gets the Output Data Rate Selection for the Accelerometer
*/
/**************************************************************************/
lsmAccelDataRate_t LSM303DLHC::getAccelDataRate()
{
    return lsm_acceldatarate;
}

/**************************************************************************/
/*
        Sets the Low Power Mode Enable
*/
/**************************************************************************/
void LSM303DLHC::setAccelMode(lsmAccelMode_t accelmode)
{
    lsm_accelmode = accelmode;
}

/**************************************************************************/
/*
        Gets the Low Power Mode Enable
*/
/**************************************************************************/
lsmAccelMode_t LSM303DLHC::getAccelMode()
{
    return lsm_accelmode;
}

/**************************************************************************/
/*
        Sets the Accelerometer Z-Axis Enable
*/
/**************************************************************************/
void LSM303DLHC::setAccelZen(lsmAccelZen_t accelzen)
{
    lsm_accelzen = accelzen;
}

/**************************************************************************/
/*
        Gets the Accelerometer Z-Axis Enable
*/
/**************************************************************************/
lsmAccelZen_t LSM303DLHC::getAccelZen()
{
    return lsm_accelzen;
}

/**************************************************************************/
/*
        Sets the Accelerometer Y-Axis Enable
*/
/**************************************************************************/
void LSM303DLHC::setAccelYen(lsmAccelYen_t accelyen)
{
    lsm_accelyen = accelyen;
}

/**************************************************************************/
/*
        Gets the Accelerometer Y-Axis Enable
*/
/**************************************************************************/
lsmAccelYen_t LSM303DLHC::getAccelYen()
{
    return lsm_accelyen;
}

/**************************************************************************/
/*
        Sets the Accelerometer X-Axis Enable
*/
/**************************************************************************/
void LSM303DLHC::setAccelXen(lsmAccelXen_t accelxen)
{
    lsm_accelxen = accelxen;
}

/**************************************************************************/
/*
        Gets the Accelerometer X-Axis Enable
*/
/**************************************************************************/
lsmAccelXen_t LSM303DLHC::getAccelXen()
{
    return lsm_accelxen;
}

/**************************************************************************/
/*
        Sets the Block Data Update
*/
/**************************************************************************/
void LSM303DLHC::setAccelBlockData(lsmAccelBlockData_t accelblockdata)
{
    lsm_accelblockdata = accelblockdata;
}

/**************************************************************************/
/*
        Gets the Block Data Update
*/
/**************************************************************************/
lsmAccelBlockData_t LSM303DLHC::getAccelBlockData()
{
    return lsm_accelblockdata;
}

/**************************************************************************/
/*
        Sets the Big/Little Endian Data Selection
*/
/**************************************************************************/
void LSM303DLHC::setAccelEndianData(lsmAccelEndianData_t accelendiandata)
{
    lsm_accelendiandata = accelendiandata;
}

/**************************************************************************/
/*
        Gets the Big/Little Endian Data Selection
*/
/**************************************************************************/
lsmAccelEndianData_t LSM303DLHC::getAccelEndianData()
{
    return lsm_accelendiandata;
}

/**************************************************************************/
/*
        Sets the Full-Scale Selection for the Accelerometer
*/
/**************************************************************************/
void LSM303DLHC::setAccelRange(lsmAccelRange_t accelrange)
{
    lsm_accelrange = accelrange;
}
                    
/**************************************************************************/
/*
        Gets the Full-Scale Selection for the Accelerometer
*/
/**************************************************************************/
lsmAccelRange_t LSM303DLHC::getAccelRange()
{
    return lsm_accelrange;
}

/**************************************************************************/
/*
        Sets the High-Resolution Output Mode
*/
/**************************************************************************/
void LSM303DLHC::setAccelResolution(lsmAccelResolution_t accelresolution)
{
    lsm_accelresolution = accelresolution;
}

/**************************************************************************/
/*
        Gets the High-Resolution Output Mode
*/
/**************************************************************************/
lsmAccelResolution_t LSM303DLHC::getAccelResolution()
{
    return lsm_accelresolution;
}

/**************************************************************************/
/*
        Sets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
void LSM303DLHC::setAccelSPIWire(lsmAccelSPIWire_t accelspiwire)
{
    lsm_accelspiwire = accelspiwire;
}

/**************************************************************************/
/*
        Gets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
lsmAccelSPIWire_t LSM303DLHC::getAccelSPIWire()
{
    return lsm_accelspiwire;
}

/**************************************************************************/
/*
        Sets the Temperature Sensor Enable
*/
/**************************************************************************/
void LSM303DLHC::setTempSensor(lsmTempSensor_t tempsensor)
{
    lsm_tempsensor = tempsensor;
}

/**************************************************************************/
/*
        Gets the Temperature Sensor Enable
*/
/**************************************************************************/
lsmTempSensor_t LSM303DLHC::getTempSensor()
{
    return lsm_tempsensor;
}

/**************************************************************************/
/*
        Sets the Magnetic Output Data Rate Selection
*/
/**************************************************************************/
void LSM303DLHC::setMagDataRate(lsmMagDataRate_t magdatarate)
{
    lsm_magdatarate = magdatarate;
}
                    
/**************************************************************************/
/*
        Gets the Magnetic Output Data Rate Selection
*/
/**************************************************************************/
lsmMagDataRate_t LSM303DLHC::getMagDataRate()
{
    return lsm_magdatarate;
}

/**************************************************************************/
/*
        Sets the Magnetic Gain Selection
*/
/**************************************************************************/
void LSM303DLHC::setMagGain(lsmMagGain_t maggain)
{
    lsm_maggain = maggain;
}
                    
/**************************************************************************/
/*
        Gets the Magnetic Gain Selection
*/
/**************************************************************************/
lsmMagGain_t LSM303DLHC::getMagGain()
{
    return lsm_maggain;
}

/**************************************************************************/
/*
        Sets the Magnetic Sensor Operating Mode Selection
*/
/**************************************************************************/
void LSM303DLHC::setMagMode(lsmMagMode_t magmode)
{
    lsm_magmode = magmode;
}

/**************************************************************************/
/*
        Gets the Magnetic Sensor Operating Mode Selection
*/
/**************************************************************************/
lsmMagMode_t LSM303DLHC::getMagMode()
{
    return lsm_magmode;
}

/**************************************************************************/
/*
        Sets up the Accelerometer
*/
/**************************************************************************/
void LSM303DLHC::setUpAccelerometer(void)
{
    // Set Up the Configuration for the Accelerometer Control Register 1
    /*
    // Set the Output Data Rate
    uint8_t config1 = lsm_acceldatarate;
    
    // Set the Power Down Mode Enable
    config1 |= lsm_accelmode;
    
    // Set the accelscpe Z-Axis Enable
    config1 |= lsm_accelzen;
    
    // Set the accelscpe Y-Axis Enable
    config1 |= lsm_accelyen;
    
    // Set the accelscpe X-Axis Enable
    config1 |= lsm_accelxen;
    */
    
    uint8_t config1 =   LSM303DLHC_REG_ACCEL_CTRL_REG1_LPEN_NORMAL      |   // Normal Mode
                        LSM303DLHC_REG_ACCEL_CTRL_REG1_AZEN_ENABLE      |   // Accelerometer Z-Axis Enabled
                        LSM303DLHC_REG_ACCEL_CTRL_REG1_AYEN_ENABLE      |   // Accelerometer Y-Axis Enabled
                        LSM303DLHC_REG_ACCEL_CTRL_REG1_AXEN_ENABLE;         // Accelerometer X-Axis Enabled
    
    config1 |= lsm_acceldatarate;       // Output Data Rate Selection
    
    // Write the configuration to the Accelerometer Control Register 1
    writeRegister(lsm_i2cAddressX, LSM303DLHC_REG_ACCEL_CTRL_REG1, config1);
    
    // Wait for the configuration to complete
    delay(lsm_conversionDelay);
    
    // Set Up the Configuration for the Accelerometer Control Register 4
    /*
    // Set the Block Data Update
    uint8_t config4 = lsm_accelblockdata;
    
    // Set the Big/Little Endian Data Selection
    config4 |= lsm_accelendiandata;
    
    // Set the High Resolution Enable
    config4 |= lsm_accelresolution;
    
    // Set the SPI Serial Interface Mode Selection
    config4 |= lsm_accelspiwire;
    */
    
    uint8_t config4 =   LSM303DLHC_REG_ACCEL_CTRL_REG4_BDU_CONTINUOUS   |   // Continuous Update
                        LSM303DLHC_REG_ACCEL_CTRL_REG4_BLE_LSB          |   // Data LSB @ Lower Address
                        LSM303DLHC_REG_ACCEL_CTRL_REG4_HS_ENABLE        |   // High Resolution Enable
                        LSM303DLHC_REG_ACCEL_CTRL_REG4_SIM_4WIRE;           // 4-Wire Interface
    
    config4 |= lsm_accelrange;      // Full-Scale Selection
    
    // Write the configuration to the Accelerometer Control Register 4
    writeRegister(lsm_i2cAddressX, LSM303DLHC_REG_ACCEL_CTRL_REG4, config4);
    
    // Wait for the configuration to complete
    delay(lsm_conversionDelay);
}

/**************************************************************************/
/*
        Sets up the Magnetometer
*/
/**************************************************************************/
void LSM303DLHC::setUpMagnetometer(void)
{
    // Set Up the Configuration for the Magnetometer Magnetic Field Sensing Register A
    /*
    // Set the Temperature Sensor Enable
    uint8_t cra = lsm_tempsensor;
    */
    
    uint8_t cra = LSM303DLHC_REG_MAG_CTRL_CRA_TEMP_DISABLED;   // Temperature Sensor Disabled
    
    cra |= lsm_magdatarate;     // Magnetic Output Data Rate Selection
    
    // Write the configuration to the Magnetometer Magnetic Field Sensing Register A
    writeRegister(lsm_i2cAddressM, LSM303DLHC_REG_MAG_CRA_REG, cra);
    
    // Wait for the configuration to complete
    delay(lsm_conversionDelay);
    
    // Set Up the Configuration for the Magnetometer Magnetic Field Sensing Register B
    // Set the Magnetic Full-Scale Selection
    uint8_t crb = lsm_maggain;
        
    // Write the configuration to the Magnetometer Magnetic Field Sensing Register B
    writeRegister(lsm_i2cAddressM, LSM303DLHC_REG_MAG_CRB_REG, crb);
    
    // Wait for the configuration to complete
    delay(lsm_conversionDelay);
    
    // Set Up the Configuration for the Magnetometer Magnetic Sensor Operating Mode Register
    /*
    // Set the Magnetic Sensor Operating Mode
    uint8_t mode = lsm_magmode;
    */
    
    uint8_t mode = LSM303DLHC_REG_MAG_CTRL_MODE_CONTINUOUS;     // Continuous-Conversion Mode
    
    // Write the configuration to the Accelerometer/Magnetometer Control Register 7
    writeRegister(lsm_i2cAddressM, LSM303DLHC_REG_MAG_MR_REG, mode);
    
    // Wait for the configuration to complete
    delay(lsm_conversionDelay);
}

/**************************************************************************/
/*
        Reads up the Sensor which comprises of an Accelerometer and Magnetometer
*/
/**************************************************************************/
void LSM303DLHC::Measure_Sensor(void)
{
    // Read all the Sensors
    // Accelerometer, Magnetometer Data
    Measure_Accelerometer();
    Measure_Magnetometer();
}

/**************************************************************************/
/*
        Reads the 3 axes of the Accelerometer
        The value is expressed in 16 bit as two’s complement
*/
/**************************************************************************/
void LSM303DLHC::Measure_Accelerometer()
{
    // Read the Accelerometer
    uint8_t xAccelLo, xAccelHi, yAccelLo, yAccelHi, zAccelLo, zAccelHi;
    
    // Read the Data
    // Reading the Low X-Axis Angular Rate Data Register
    xAccelLo = readRegister(lsm_i2cAddressX, LSM303DLHC_REG_ACCEL_OUT_X_L);
    // Reading the High X-Axis Angular Rate Data Register
    xAccelHi = readRegister(lsm_i2cAddressX, LSM303DLHC_REG_ACCEL_OUT_X_H);
    // Conversion of the result
    // 16-bit signed result for X-Axis Angular Rate Data of LSM303DLHC
    lsm_accelData.X = (int16_t)((xAccelHi << 8) | xAccelLo);
    
    // Reading the Low Y-Axis Angular Rate Data Register
    yAccelLo = readRegister(lsm_i2cAddressX, LSM303DLHC_REG_ACCEL_OUT_Y_L);
    // Reading the High Y-Axis Angular Rate Data Register
    yAccelHi = readRegister(lsm_i2cAddressX, LSM303DLHC_REG_ACCEL_OUT_Y_H);
    // Conversion of the result
    // 16-bit signed result for Y-Axis Angular Rate Data of LSM303DLHC
    lsm_accelData.Y = (int16_t)((yAccelHi << 8) | yAccelLo);
    
    // Reading the Low Z-Axis Angular Rate Data Register
    zAccelLo = readRegister(lsm_i2cAddressX, LSM303DLHC_REG_ACCEL_OUT_Z_L);
    // Reading the High Z-Axis Angular Rate Data Register
    zAccelHi = readRegister(lsm_i2cAddressX, LSM303DLHC_REG_ACCEL_OUT_Z_H);
    // Conversion of the result
    // 16-bit signed result for Z-Axis Angular Rate Data of LSM303DLHC
    lsm_accelData.Z = (int16_t)((zAccelHi << 8) | zAccelLo);
    
}

/**************************************************************************/
/*
        Reads the 3 axes of the Magnetometer
        The value is expressed in 16 bit as two’s complement
*/
/**************************************************************************/
void LSM303DLHC::Measure_Magnetometer()
{
    // Read the Magnetometer
    uint8_t xMagHi, xMagLo, yMagHi, yMagLo, zMagHi, zMagLo;
    
    // Read the Data
    // Reading the Low X-Axis Magnetic Data Register
    xMagHi = readRegister(lsm_i2cAddressM, LSM303DLHC_REG_MAG_OUT_X_H);
    // Reading the High X-Axis Magnetic Data Register
    xMagLo = readRegister(lsm_i2cAddressM, LSM303DLHC_REG_MAG_OUT_X_L);
    // Conversion of the result
    // 16-bit signed result for X-Axis Magnetic Data of LSM303DLHC
    lsm_magData.X = (int16_t)((xMagHi << 8) | xMagLo);
    
    // Reading the Low Y-Axis Magnetic Data Register
    yMagHi = readRegister(lsm_i2cAddressM, LSM303DLHC_REG_MAG_OUT_Y_H);
    // Reading the High Y-Axis Magnetic Data Register
    yMagLo = readRegister(lsm_i2cAddressM, LSM303DLHC_REG_MAG_OUT_Y_L);
    // Conversion of the result
    // 16-bit signed result for Y-Axis Magnetic Data of LSM303DLHC
    lsm_magData.Y = (int16_t)((yMagHi << 8) | yMagLo);
    
    // Reading the Low Z-Axis Magnetic Data Register
    zMagHi = readRegister(lsm_i2cAddressM, LSM303DLHC_REG_MAG_OUT_Z_H);
    // Reading the High Z-Axis Magnetic Data Register
    zMagLo = readRegister(lsm_i2cAddressM, LSM303DLHC_REG_MAG_OUT_Z_L);
    // Conversion of the result
    // 16-bit signed result for Z-Axis Magnetic Data of LSM303DLHC
    lsm_magData.Z = (int16_t)((zMagHi << 8) | zMagLo);
}
