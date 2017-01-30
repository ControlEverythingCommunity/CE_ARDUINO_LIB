/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LSM9DS0
        This code is designed to work with the LSM9DS0_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelorometer?sku=LSM9DS0_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "LSM9DS0.h"

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
        Instantiates a new LSM9DS0 class with appropriate properties (Gyroscope Address)
*/
/**************************************************************************/
void LSM9DS0::getAddrG_LSM9DS0(uint8_t i2cAddress)
{
    lsm_i2cAddressG = i2cAddress;
    lsm_conversionDelay = lsm_conversionDelay;
}

/**************************************************************************/
/*
        Instantiates a new LSM9DS0 class with appropriate properties
        Accelerometer & Magnetometer Address
*/
/**************************************************************************/
void LSM9DS0::getAddrXM_LSM9DS0(uint8_t i2cAddress)
{
    lsm_i2cAddressXM = i2cAddress;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool LSM9DS0::begin()
{
    Wire.begin();
    
    uint8_t XMdevid = readRegister(lsm_i2cAddressXM, LSM9DS0_REG_MAG_WHO_AM_I);
    if (XMdevid != LSM9DS0_ACCELMAG_DEV_ID)
        return false;
    
    uint8_t Gdevid = readRegister(lsm_i2cAddressG, LSM9DS0_REG_GYRO_WHO_AM_I);
    if (Gdevid != LSM9DS0_GYRO_DEV_ID)
        return false;
    
    // Set up the sensor for Gyroscope, Accelerometer, and Magnetometer
    // setUpGyroscope();
    // setUpAccelerometer();
    // setUpMagnetometer();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Output Data Rate Selection and Bandwidth Selection
        for the Gyroscope
*/
/**************************************************************************/
void LSM9DS0::setGyroDataRate(lsmGyroDataRate_t gyrodatarate)
{
    lsm_gyrodatarate = gyrodatarate;
}

/**************************************************************************/
/*
        Gets the Output Data Rate Selection and Bandwidth Selection
        for the Gyroscope
*/
/**************************************************************************/
lsmGyroDataRate_t LSM9DS0::getGyroDataRate()
{
    return lsm_gyrodatarate;
}

/**************************************************************************/
/*
        Sets the Power-Down Mode Enable
*/
/**************************************************************************/
void LSM9DS0::setGyroMode(lsmGyroMode_t gyromode)
{
    lsm_gyromode = gyromode;
}

/**************************************************************************/
/*
        Gets the Power-Down Mode Enable
*/
/**************************************************************************/
lsmGyroMode_t LSM9DS0::getGyroMode()
{
    return lsm_gyromode;
}

/**************************************************************************/
/*
        Sets the Gyroscope Z-Axis Enable
*/
/**************************************************************************/
void LSM9DS0::setGyroZen(lsmGyroZen_t gyrozen)
{
    lsm_gyrozen = gyrozen;
}

/**************************************************************************/
/*
        Gets the Gyroscope Z-Axis Enable
*/
/**************************************************************************/
lsmGyroZen_t LSM9DS0::getGyroZen()
{
    return lsm_gyrozen;
}

/**************************************************************************/
/*
        Sets the Gyroscope Y-Axis Enable
*/
/**************************************************************************/
void LSM9DS0::setGyroYen(lsmGyroYen_t gyroyen)
{
    lsm_gyroyen = gyroyen;
}

/**************************************************************************/
/*
        Gets the Gyroscope Y-Axis Enable
*/
/**************************************************************************/
lsmGyroYen_t LSM9DS0::getGyroYen()
{
    return lsm_gyroyen;
}

/**************************************************************************/
/*
        Sets the Gyroscope X-Axis Enable
*/
/**************************************************************************/
void LSM9DS0::setGyroXen(lsmGyroXen_t gyroxen)
{
    lsm_gyroxen = gyroxen;
}

/**************************************************************************/
/*
        Gets the Gyroscope X-Axis Enable
*/
/**************************************************************************/
lsmGyroXen_t LSM9DS0::getGyroXen()
{
    return lsm_gyroxen;
}

/**************************************************************************/
/*
        Sets the Block Data Update
*/
/**************************************************************************/
void LSM9DS0::setGyroBlockData(lsmGyroBlockData_t gyroblockdata)
{
    lsm_gyroblockdata = gyroblockdata;
}

/**************************************************************************/
/*
        Gets the Block Data Update
*/
/**************************************************************************/
lsmGyroBlockData_t LSM9DS0::getGyroBlockData()
{
    return lsm_gyroblockdata;
}

/**************************************************************************/
/*
        Sets the Big/Little Endian Data Selection
*/
/**************************************************************************/
void LSM9DS0::setGyroEndianData(lsmGyroEndianData_t gyroendiandata)
{
    lsm_gyroendiandata = gyroendiandata;
}

/**************************************************************************/
/*
        Gets the Big/Little Endian Data Selection
*/
/**************************************************************************/
lsmGyroEndianData_t LSM9DS0::getGyroEndianData()
{
    return lsm_gyroendiandata;
}

/**************************************************************************/
/*
        Sets the Full-Scale Selection for the Gyroscope
*/
/**************************************************************************/
void LSM9DS0::setGyroScale(lsmGyroScale_t gyroscale)
{
    lsm_gyroscale = gyroscale;
}
                    
/**************************************************************************/
/*
        Gets the Full-Scale Selection for the Gyroscope
*/
/**************************************************************************/
lsmGyroScale_t LSM9DS0::getGyroScale()
{
    return lsm_gyroscale;
}

/**************************************************************************/
/*
        Sets the Self-Test Enable
*/
/**************************************************************************/
void LSM9DS0::setGyroSelfTest(lsmGyroSelfTest_t gyroselftest)
{
    lsm_gyroselftest = gyroselftest;
}

/**************************************************************************/
/*
        Gets the Self-Test Enable
*/
/**************************************************************************/
lsmGyroSelfTest_t LSM9DS0::getGyroSelfTest()
{
    return lsm_gyroselftest;
}

/**************************************************************************/
/*
        Sets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
void LSM9DS0::setGyroSPIWire(lsmGyroSPIWire_t gyrospiwire)
{
    lsm_gyrospiwire = gyrospiwire;
}

/**************************************************************************/
/*
        Gets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
lsmGyroSPIWire_t LSM9DS0::getGyroSPIWire()
{
    return lsm_gyrospiwire;
}

/**************************************************************************/
/*
        Sets the Output Data Rate Selection for the Accelerometer
*/
/**************************************************************************/
void LSM9DS0::setAccelDataRate(lsmAccelDataRate_t acceldatarate)
{
    lsm_acceldatarate = acceldatarate;
}
                    
/**************************************************************************/
/*
        Gets the Output Data Rate Selection for the Accelerometer
*/
/**************************************************************************/
lsmAccelDataRate_t LSM9DS0::getAccelDataRate()
{
    return lsm_acceldatarate;
}

/**************************************************************************/
/*
        Sets the Block Data Update for Acceleration and Magnetic Data
*/
/**************************************************************************/
void LSM9DS0::setAccelMagBlockData(lsmAccelMagBlockData_t accelmagblockdata)
{
    lsm_accelmagblockdata = accelmagblockdata;
}

/**************************************************************************/
/*
        Gets the Block Data Update for Acceleration and Magnetic Data
*/
/**************************************************************************/
lsmAccelMagBlockData_t LSM9DS0::getAccelMagBlockData()
{
    return lsm_accelmagblockdata;
}

/**************************************************************************/
/*
        Sets the Acceleration Z-Axis Enable
*/
/**************************************************************************/
void LSM9DS0::setAccelZen(lsmAccelZen_t accelzen)
{
    lsm_accelzen = accelzen;
}

/**************************************************************************/
/*
        Gets the Acceleration Z-Axis Enable
*/
/**************************************************************************/
lsmAccelZen_t LSM9DS0::getAccelZen()
{
    return lsm_accelzen;
}

/**************************************************************************/
/*
        Sets the Acceleration Y-Axis Enable
*/
/**************************************************************************/
void LSM9DS0::setAccelYen(lsmAccelYen_t accelyen)
{
    lsm_accelyen = accelyen;
}

/**************************************************************************/
/*
        Gets the Acceleration Y-Axis Enable
*/
/**************************************************************************/
lsmAccelYen_t LSM9DS0::getAccelYen()
{
    return lsm_accelyen;
}

/**************************************************************************/
/*
        Sets the Acceleration X-Axis Enable
*/
/**************************************************************************/
void LSM9DS0::setAccelXen(lsmAccelXen_t accelxen)
{
    lsm_accelxen = accelxen;
}

/**************************************************************************/
/*
        Gets the Acceleration X-Axis Enable
*/
/**************************************************************************/
lsmAccelXen_t LSM9DS0::getAccelXen()
{
    return lsm_accelxen;
}

/**************************************************************************/
/*
        Sets the Acceleration Anti-Alias Filter Bandwidth
*/
/**************************************************************************/
void LSM9DS0::setAccelBandwidth(lsmAccelBandwidth_t accelbandwidth)
{
    lsm_accelbandwidth = accelbandwidth;
}

/**************************************************************************/
/*
        Gets the Acceleration Anti-Alias Filter Bandwidth
*/
/**************************************************************************/
lsmAccelBandwidth_t LSM9DS0::getAccelBandwidth()
{
    return lsm_accelbandwidth;
}

/**************************************************************************/
/*
        Sets the Acceleration Full-Scale Selection
*/
/**************************************************************************/
void LSM9DS0::setAccelRange(lsmAccelRange_t accelrange)
{
    lsm_accelrange = accelrange;
}
                    
/**************************************************************************/
/*
        Gets the Acceleration Full-Scale Selection
*/
/**************************************************************************/
lsmAccelRange_t LSM9DS0::getAccelRange()
{
    return lsm_accelrange;
}

/**************************************************************************/
/*
        Sets the Self-Test Mode Configuration
*/
/**************************************************************************/
void LSM9DS0::setAccelSelfTest(lsmAccelSelfTest_t accelselftest)
{
    lsm_accelselftest = accelselftest;
}

/**************************************************************************/
/*
        Gets the Self-Test Mode Configuration
*/
/**************************************************************************/
lsmAccelSelfTest_t LSM9DS0::getAccelSelfTest()
{
    return lsm_accelselftest;
}

/**************************************************************************/
/*
        Sets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
void LSM9DS0::setAccelMagSPIWire(lsmAccelMagSPIWire_t accelmagspiwire)
{
    lsm_accelmagspiwire = accelmagspiwire;
}

/**************************************************************************/
/*
        Gets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
lsmAccelMagSPIWire_t LSM9DS0::getAccelMagSPIWire()
{
    return lsm_accelmagspiwire;
}

/**************************************************************************/
/*
        Sets the Temperature Sensor Enable
*/
/**************************************************************************/
void LSM9DS0::setTempSensor(lsmTempSensor_t tempsensor)
{
    lsm_tempsensor = tempsensor;
}

/**************************************************************************/
/*
        Gets the Temperature Sensor Enable
*/
/**************************************************************************/
lsmTempSensor_t LSM9DS0::getTempSensor()
{
    return lsm_tempsensor;
}

/**************************************************************************/
/*
        Sets the Magnetic Resolution Selection
*/
/**************************************************************************/
void LSM9DS0::setMagResolution(lsmMagResolution_t magresolution)
{
    lsm_magresolution = magresolution;
}

/**************************************************************************/
/*
        Gets the Magnetic Resolution Selection
*/
/**************************************************************************/
lsmMagResolution_t LSM9DS0::getMagResolution()
{
    return lsm_magresolution;
}

/**************************************************************************/
/*
        Sets the Magnetic Data Rate Selection
*/
/**************************************************************************/
void LSM9DS0::setMagDataRate(lsmMagDataRate_t magdatarate)
{
    lsm_magdatarate = magdatarate;
}
                    
/**************************************************************************/
/*
        Gets the Magnetic Data Rate Selection
*/
/**************************************************************************/
lsmMagDataRate_t LSM9DS0::getMagDataRate()
{
    return lsm_magdatarate;
}

/**************************************************************************/
/*
        Sets the Latch Interrupt Request on INT2_SRC Register,
        With INT2_SRC Register Cleared by Reading INT2_SRC Itself
*/
/**************************************************************************/
void LSM9DS0::setMagIntr2Latch(lsmMagIntr2Latch_t magintr2latch)
{
    lsm_magintr2latch = magintr2latch;
}

/**************************************************************************/
/*
        Gets the Sets the Latch Interrupt Request on INT2_SRC Register,
        With INT2_SRC Register Cleared by Reading INT2_SRC Itself
*/
/**************************************************************************/
lsmMagIntr2Latch_t LSM9DS0::getMagIntr2Latch()
{
    return lsm_magintr2latch;
}

/**************************************************************************/
/*
        Sets the Latch Interrupt Request on INT1_SRC Register,
        With INT1_SRC Register Cleared by Reading INT1_SRC Itself
*/
/**************************************************************************/
void LSM9DS0::setMagIntr1Latch(lsmMagIntr1Latch_t magintr1latch)
{
    lsm_magintr1latch = magintr1latch;
}

/**************************************************************************/
/*
        Gets the Sets the Latch Interrupt Request on INT1_SRC Register,
        With INT1_SRC Register Cleared by Reading INT1_SRC Itself
*/
/**************************************************************************/
lsmMagIntr1Latch_t LSM9DS0::getMagIntr1Latch()
{
    return lsm_magintr1latch;
}

/**************************************************************************/
/*
        Sets the Magnetic Full-Scale Selection
*/
/**************************************************************************/
void LSM9DS0::setMagGain(lsmMagGain_t maggain)
{
    lsm_maggain = maggain;
}
                    
/**************************************************************************/
/*
        Gets the Magnetic Full-Scale Selection
*/
/**************************************************************************/
lsmMagGain_t LSM9DS0::getMagGain()
{
    return lsm_maggain;
}

/**************************************************************************/
/*
        Sets the Magnetic Sensor Mode Selection
*/
/**************************************************************************/
void LSM9DS0::setMagMode(lsmMagMode_t magmode)
{
    lsm_magmode = magmode;
}

/**************************************************************************/
/*
        Gets the Magnetic Sensor Mode Selection
*/
/**************************************************************************/
lsmMagMode_t LSM9DS0::getMagMode()
{
    return lsm_magmode;
}

/**************************************************************************/
/*
        Sets up the Gyroscope
*/
/**************************************************************************/
void LSM9DS0::setUpGyroscope(void)
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
    
    uint8_t config1 =   LSM9DS0_REG_GYRO_CTRL_REG1_PD_NORMAL    |   // Normal Mode
                        LSM9DS0_REG_GYRO_CTRL_REG1_ZEN_ENABLE   |   // Gyroscope Z-Axis Enabled
                        LSM9DS0_REG_GYRO_CTRL_REG1_YEN_ENABLE   |   // Gyroscope Y-Axis Enabled
                        LSM9DS0_REG_GYRO_CTRL_REG1_XEN_ENABLE;      // Gyroscope X-Axis Enabled
    
    config1 |= lsm_gyrodatarate;       // Output Data Rate and Bandwidth Selection
    
    // Write the configuration to the Gyroscope Control Register 1
    writeRegister(lsm_i2cAddressG, LSM9DS0_REG_GYRO_CTRL_REG1, config1);
    
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
    
    // Set the Self-Test Enable
    config4 |= lsm_gyroselftest;
    
    // Set the SPI Serial Interface Mode Selection
    config4 |= lsm_gyrospiwire;
    */
    
    uint8_t config4 =   LSM9DS0_REG_GYRO_CTRL_REG4_BDU_CONTINUOUS   |   // Continuous Update
                        LSM9DS0_REG_GYRO_CTRL_REG4_BLE_LSB          |   // Data LSB @ Lower Address
                        LSM9DS0_REG_GYRO_CTRL_REG4_ST_NORMAL        |   // Normal Mode
                        LSM9DS0_REG_GYRO_CTRL_REG4_SIM_4WIRE;           // 4-Wire Interface
    
    config4 |= lsm_gyroscale;      // Full-Scale Selection
    
    // Write the configuration to the Gyroscope Control Register 4
    writeRegister(lsm_i2cAddressG, LSM9DS0_REG_GYRO_CTRL_REG4, config4);
    
    // Wait for the configuration to complete
    delay(lsm_conversionDelay);
}

/**************************************************************************/
/*
        Sets up the Accelerometer
*/
/**************************************************************************/
void LSM9DS0::setUpAccelerometer(void)
{
    // Set Up the Configuration for the Accelerometer/Magnetometer Control Register 1
    /*
    // Set the Acceleration Output Data Rate
    uint8_t config1 = lsm_acceldatarate;
    
    // Set the Block Data Update for Acceleration and Magnetic Data
    config1 |= lsm_accelmagblockdata;
    
    // Set the Acceleration Z-Axis Enable
    config1 |= lsm_accelzen;
    
    // Set the Acceleration Y-Axis Enable
    config1 |= lsm_accelyen;
    
    // Set the Acceleration X-Axis Enable
    config1 |= lsm_accelxen;
    */
    
    uint8_t config1 =   LSM9DS0_REG_ACCELMAG_CTRL_REG1_BDU_CONTINUOUS   |   // Continuous Update
                        LSM9DS0_REG_ACCELMAG_CTRL_REG1_AZEN_ENABLE      |   // Acceleration Z-Axis Enabled
                        LSM9DS0_REG_ACCELMAG_CTRL_REG1_AYEN_ENABLE      |   // Acceleration Y-Axis Enabled
                        LSM9DS0_REG_ACCELMAG_CTRL_REG1_AXEN_ENABLE;         // Acceleration X-Axis Enabled
    
    config1 |= lsm_acceldatarate;      // Acceleration Data Rate Configuration
    
    // Write the configuration to the Accelerometer/Magnetometer Control Register 1
    writeRegister(lsm_i2cAddressXM, LSM9DS0_REG_ACCELMAG_CTRL_REG1, config1);
    
    // Wait for the configuration to complete
    delay(lsm_conversionDelay);
    
    // Set Up the Configuration for the Accelerometer/Magnetometer Control Register 2
    /*
    // Set the Accelerometer Anti-Alias Filter Bandwidth
    uint8_t config2 = lsm_accelbandwidth;
    
    // Set the Acceleration Full-Scale Selection
    config2 |= lsm_accelrange;
    
    // Set the Acceleration Self-Test Enable
    config2 |= lsm_sccelselftest;
    
    // Set the SPI Serial Interface Mode Selection
    config2 |= lsm_accelmagspiwire;
    */
    
    uint8_t config2 =   LSM9DS0_REG_ACCELMAG_CTRL_REG2_ABW_773      |   // 773 Hz, Accelerometer Anti-Alias Filter Bandwidth
                        LSM9DS0_REG_ACCELMAG_CTRL_REG2_AST_NORMAL   |   // Normal Mode
                        LSM9DS0_REG_ACCELMAG_CTRL_REG2_SIM_4WIRE;       // 4-Wire Interface
    
    config2 |= lsm_accelrange;     // Acceleration Full-Scale Selection
    
    // Write the configuration to the Accelerometer/Magnetometer Control Register 2
    writeRegister(lsm_i2cAddressXM, LSM9DS0_REG_ACCELMAG_CTRL_REG2, config2);
        
    // Wait for the configuration to complete
    delay(lsm_conversionDelay);
}

/**************************************************************************/
/*
        Sets up the Magnetometer
*/
/**************************************************************************/
void LSM9DS0::setUpMagnetometer(void)
{
    // Set Up the Configuration for the Accelerometer/Magnetometer Control Register 5
    /*
    // Set the Temperature Sensor Enable
    uint8_t config5 = lsm_tempsensor;
    
    // Set the Magnetic Resolution Selection
    config5 |= lsm_magresolution;
    
    // Set the Magnetic Data Rate Selection
    config5 |= lsm_magdatarate;
    
    // Set the Latch Interrupt Request on INT2_SRC Register
    config5 |= lsm_magintr2latch;
    
    // Set the Latch Interrupt Request on INT1_SRC Register
    config5 |= lsm_magintr1latch;
    */
    
    uint8_t config5 =   LSM9DS0_REG_ACCELMAG_CTRL_REG5_TEMP_ENABLED     |   // Temperature Sensor Enabled
                        LSM9DS0_REG_ACCELMAG_CTRL_REG5_M_RES_HIGH;          // Magnetic Resolution High
    
    config5 |= lsm_magdatarate;     // Magnetic Data Rate Selection
    
    // Write the configuration to the Accelerometer/Magnetometer Control Register 5
    writeRegister(lsm_i2cAddressXM, LSM9DS0_REG_ACCELMAG_CTRL_REG5, config5);
    
    // Wait for the configuration to complete
    delay(lsm_conversionDelay);
    
    // Set Up the Configuration for the Accelerometer/Magnetometer Control Register 6
    // Set the Magnetic Full-Scale Selection
    uint8_t config6 = lsm_maggain;
        
    // Write the configuration to the Accelerometer/Magnetometer Control Register 6
    writeRegister(lsm_i2cAddressXM, LSM9DS0_REG_ACCELMAG_CTRL_REG6, config6);
    
    // Wait for the configuration to complete
    delay(lsm_conversionDelay);
    
    // Set Up the Configuration for the Accelerometer/Magnetometer Control Register 7
    
    uint8_t config7 =   LSM9DS0_REG_ACCELMAG_CTRL_REG7_AHPM_NORMAL  |   // Normal Mode
                        LSM9DS0_REG_ACCELMAG_CTRL_REG7_AFDS_BYPASS  |   // Internal Filter Bypassed
                        LSM9DS0_REG_ACCELMAG_CTRL_REG7_MLP_MODR     |   // Magnetic Data Rate is Configured by MODR Bits
                        LSM9DS0_REG_ACCELMAG_CTRL_REG7_MD_CONTINUOUS;   // Continuous-Conversion Mode
    
    // Set the Magnetic Sensor Mode
    config7 |= lsm_magmode;
    
    // Write the configuration to the Accelerometer/Magnetometer Control Register 7
    writeRegister(lsm_i2cAddressXM, LSM9DS0_REG_ACCELMAG_CTRL_REG7, config7);
    
    // Wait for the configuration to complete
    delay(lsm_conversionDelay);
}

/**************************************************************************/
/*
        Reads up the Sensor which comprises of an Accelerometer,
        Magnetometer and Gyroscope
*/
/**************************************************************************/
void LSM9DS0::Measure_Sensor(void)
{
    // Read all the Sensors
    // Gyroscope, Accelerometer, Magnetometer and Temperature Data
    Measure_Gyroscope();
    Measure_Accelerometer();
    Measure_Magnetometer();
    Measure_Temperature();
}

/**************************************************************************/
/*
        Reads the 3 axes of the Gyroscope
        The value is expressed in 16 bit as two’s complement
*/
/**************************************************************************/
void LSM9DS0::Measure_Gyroscope()
{
    // Read the Gyroscope
    uint8_t xGyroLo, xGyroHi, yGyroLo, yGyroHi, zGyroLo, zGyroHi;
    
    // Read the Data
    // Reading the Low X-Axis Angular Rate Data Register
    xGyroLo = readRegister(lsm_i2cAddressG, LSM9DS0_REG_GYRO_OUT_X_L);
    // Reading the High X-Axis Angular Rate Data Register
    xGyroHi = readRegister(lsm_i2cAddressG, LSM9DS0_REG_GYRO_OUT_X_H);
    // Conversion of the result
    // 16-bit signed result for X-Axis Angular Rate Data of LSM9DS0
    lsm_gyroData.X = (int16_t)((xGyroHi << 8) | xGyroLo);
    
    // Reading the Low Y-Axis Angular Rate Data Register
    yGyroLo = readRegister(lsm_i2cAddressG, LSM9DS0_REG_GYRO_OUT_Y_L);
    // Reading the High Y-Axis Angular Rate Data Register
    yGyroHi = readRegister(lsm_i2cAddressG, LSM9DS0_REG_GYRO_OUT_Y_H);
    // Conversion of the result
    // 16-bit signed result for Y-Axis Angular Rate Data of LSM9DS0
    lsm_gyroData.Y = (int16_t)((yGyroHi << 8) | yGyroLo);
    
    // Reading the Low Z-Axis Angular Rate Data Register
    zGyroLo = readRegister(lsm_i2cAddressG, LSM9DS0_REG_GYRO_OUT_Z_L);
    // Reading the High Z-Axis Angular Rate Data Register
    zGyroHi = readRegister(lsm_i2cAddressG, LSM9DS0_REG_GYRO_OUT_Z_H);
    // Conversion of the result
    // 16-bit signed result for Z-Axis Angular Rate Data of LSM9DS0
    lsm_gyroData.Z = (int16_t)((zGyroHi << 8) | zGyroLo);
    
}
/**************************************************************************/
/*
        Reads the 3 axes of the Accelerometer
        The value is expressed in 16 bit as two’s complement
*/
/**************************************************************************/
void LSM9DS0::Measure_Accelerometer()
{
    // Read the Accelerometer
    uint8_t xAccelLo, xAccelHi, yAccelLo, yAccelHi, zAccelLo, zAccelHi;
    
    // Read the Data
    // Reading the Low X-Axis Acceleration Data Register
    xAccelLo = readRegister(lsm_i2cAddressXM, LSM9DS0_REG_ACCEL_OUT_X_L);
    // Reading the High X-Axis Acceleration Data Register
    xAccelHi = readRegister(lsm_i2cAddressXM, LSM9DS0_REG_ACCEL_OUT_X_H);
    // Conversion of the result
    // 16-bit signed result for X-Axis Acceleration Data of LSM9DS0
    lsm_accelData.X = (int16_t)((xAccelHi << 8) | xAccelLo);
    
    // Reading the Low Y-Axis Acceleration Data Register
    yAccelLo = readRegister(lsm_i2cAddressXM, LSM9DS0_REG_ACCEL_OUT_Y_L);
    // Reading the High Y-Axis Acceleration Data Register
    yAccelHi = readRegister(lsm_i2cAddressXM, LSM9DS0_REG_ACCEL_OUT_Y_H);
    // Conversion of the result
    // 16-bit signed result for Y-Axis Acceleration Data of LSM9DS0
    lsm_accelData.Y = (int16_t)((yAccelHi << 8) | yAccelLo);
    
    // Reading the Low Z-Axis Acceleration Data Register
    zAccelLo = readRegister(lsm_i2cAddressXM, LSM9DS0_REG_ACCEL_OUT_Z_L);
    // Reading the High Z-Axis Acceleration Data Register
    zAccelHi = readRegister(lsm_i2cAddressXM, LSM9DS0_REG_ACCEL_OUT_Z_H);
    // Conversion of the result
    // 16-bit signed result for Z-Axis Acceleration Data of LSM9DS0
    lsm_accelData.Z = (int16_t)((zAccelHi << 8) | zAccelLo);
}

/**************************************************************************/
/*
        Reads the 3 axes of the Magnetometer
        The value is expressed in 16 bit as two’s complement
*/
/**************************************************************************/
void LSM9DS0::Measure_Magnetometer()
{
    // Read the Magnetometer
    uint8_t xMagLo, xMagHi, yMagLo, yMagHi, zMagLo, zMagHi;
    
    // Read the Data
    // Reading the Low X-Axis Magnetic Data Register
    xMagLo = readRegister(lsm_i2cAddressXM, LSM9DS0_REG_MAG_OUT_X_L);
    // Reading the High X-Axis Magnetic Data Register
    xMagHi = readRegister(lsm_i2cAddressXM, LSM9DS0_REG_MAG_OUT_X_H);
    // Conversion of the result
    // 16-bit signed result for X-Axis Magnetic Data of LSM9DS0
    lsm_magData.X = (int16_t)((xMagHi << 8) | xMagLo);
    
    // Reading the Low Y-Axis Magnetic Data Register
    yMagLo = readRegister(lsm_i2cAddressXM, LSM9DS0_REG_MAG_OUT_Y_L);
    // Reading the High Y-Axis Magnetic Data Register
    yMagHi = readRegister(lsm_i2cAddressXM, LSM9DS0_REG_MAG_OUT_Y_H);
    // Conversion of the result
    // 16-bit signed result for Y-Axis Magnetic Data of LSM9DS0
    lsm_magData.Y = (int16_t)((yMagHi << 8) | yMagLo);
    
    // Reading the Low Z-Axis Magnetic Data Register
    zMagLo = readRegister(lsm_i2cAddressXM, LSM9DS0_REG_MAG_OUT_Z_L);
    // Reading the High Z-Axis Magnetic Data Register
    zMagHi = readRegister(lsm_i2cAddressXM, LSM9DS0_REG_MAG_OUT_Z_H);
    // Conversion of the result
    // 16-bit signed result for Z-Axis Magnetic Data of LSM9DS0
    lsm_magData.Z = (int16_t)((zMagHi << 8) | zMagLo);
}

/**************************************************************************/
/*
        Reads the Temperature Sensor
*/
/**************************************************************************/
void LSM9DS0::Measure_Temperature()
{
    // Read the Temperature Sensor
    uint8_t xTempLo, xTempHi;
    int16_t rawTemp;
    
    // Read the Data
    // Reading the Low Temperature Output Data Register
    xTempLo = readRegister(lsm_i2cAddressXM, LSM9DS0_REG_TEMP_OUT_L);
    // Reading the High Temperature Output Data Register
    xTempHi = readRegister(lsm_i2cAddressXM, LSM9DS0_REG_TEMP_OUT_H);
    // Conversion of the result
    // 16-bit signed result for X-Axis Magnetic Data of LSM9DS0
    rawTemp = (int16_t)((xTempHi << 8) | xTempLo);
    lsm_tempData.T = 21.0 + (float)rawTemp / 8.0;
}
