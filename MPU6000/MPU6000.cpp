/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MPU6000
        This code is designed to work with the MPU6000_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/3-axis-12-bit-8-bit-digital-accelerometer
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "MPU6000.h"

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
        Instantiates a new MPU6000 class with appropriate properties
        Accelerometer Address
*/
/**************************************************************************/
void MPU6000::getAddr_MPU6000(uint8_t i2cAddress)
{
    mpu_i2cAddress = i2cAddress;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool MPU6000::begin()
{
    Wire.begin();
    
    uint8_t devid = readRegister(mpu_i2cAddress, MPU6000_REG_ACCELGYRO_WHO_AM_I);
    if (devid != MPU6000_DEFAULT_DEV_ID)
        return false;
    
    // Set up the sensor for Accelerometer and Gyroscope
    // setUpSensor();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Full Scale Range of the Gyroscope Outputs
*/
/**************************************************************************/
void MPU6000::setGyroRange(mpuGyroRange_t gyrorange)
{
    mpu_gyrorange = gyrorange;
}

/**************************************************************************/
/*
        Gets the Full Scale Range of the Gyroscope Outputs
*/
/**************************************************************************/
mpuGyroRange_t MPU6000::getGyroRange()
{
    return mpu_gyrorange;
}

/**************************************************************************/
/*
        Sets the Full Scale Range of the Accelerometer Outputs
*/
/**************************************************************************/
void MPU6000::setAccelRange(mpuAccelRange_t accelrange)
{
    mpu_accelrange = accelrange;
}

/**************************************************************************/
/*
        Gets the Full Scale Range of the Accelerometer Outputs
*/
/**************************************************************************/
mpuAccelRange_t MPU6000::getAccelRange()
{
    return mpu_accelrange;
}

/**************************************************************************/
/*
        Sets the Reset of All Internal Registers
*/
/**************************************************************************/
void MPU6000::setDeviceReset(mpuDeviceReset_t devicereset)
{
    mpu_devicereset = devicereset;
}

/**************************************************************************/
/*
        Gets the Reset of All Internal Registers
*/
/**************************************************************************/
mpuDeviceReset_t MPU6000::getDeviceReset()
{
    return mpu_devicereset;
}

/**************************************************************************/
/*
        Sets the Sleep Mode
*/
/**************************************************************************/
void MPU6000::setSleepMode(mpuSleepMode_t sleepmode)
{
    mpu_sleepmode = sleepmode;
}

/**************************************************************************/
/*
        Gets the Sleep Mode
*/
/**************************************************************************/
mpuSleepMode_t MPU6000::getSleepMode()
{
    return mpu_sleepmode;
}

/**************************************************************************/
/*
        Sets the Cycle Between Sleep Mode and Waking Up to Take Sample Data
*/
/**************************************************************************/
void MPU6000::setCycleMode(mpuCycleMode_t cyclemode)
{
    mpu_cyclemode = cyclemode;
}

/**************************************************************************/
/*
        Gets the Cycle Between Sleep Mode and Waking Up to Take Sample Data
*/
/**************************************************************************/
mpuCycleMode_t MPU6000::getCycleMode()
{
    return mpu_cyclemode;
}

/**************************************************************************/
/*
        Sets the Temperature Sensor
*/
/**************************************************************************/
void MPU6000::setTempMode(mpuTempMode_t tempmode)
{
    mpu_tempmode = tempmode;
}

/**************************************************************************/
/*
        Gets the Temperature Sensor
*/
/**************************************************************************/
mpuTempMode_t MPU6000::getTempMode()
{
    return mpu_tempmode;
}

/**************************************************************************/
/*
        Sets the Specifies the Clock Source of the Device
*/
/**************************************************************************/
void MPU6000::setClockSource(mpuClockSource_t clocksource)
{
    mpu_clocksource = clocksource;
}

/**************************************************************************/
/*
        Gets the Specifies the Clock Source of the Device
*/
/**************************************************************************/
mpuClockSource_t MPU6000::getClockSource()
{
    return mpu_clocksource;
}

/**************************************************************************/
/*
        Sets up the Sensor for Accelerometer and Gyroscope
*/
/**************************************************************************/
void MPU6000::setUpSensor(void)
{
    // Set Up the Configuration for the Gyroscope Configuration Register
    uint8_t gyro_config =   MPU6000_REG_ACCELGYRO_GYRO_CONFIG_XG_ST_DISABLE     |   // Disable X-Axis Gyroscope to Perform Self Test
                            MPU6000_REG_ACCELGYRO_GYRO_CONFIG_YG_ST_DISABLE     |   // Disable Y-Axis Gyroscope to Perform Self Test
                            MPU6000_REG_ACCELGYRO_GYRO_CONFIG_ZG_ST_DISABLE;        // Disable Z-Axis Gyroscope to Perform Self Test
    
    // Full Scale Range of the Gyroscope Outputs
    gyro_config |= mpu_gyrorange;
    
    // Write the configuration to the Accelerometer Configuration Register
    writeRegister(mpu_i2cAddress, MPU6000_REG_ACCELGYRO_GYRO_CONFIG, gyro_config);
    
    // Wait for the configuration to complete
    delay(mpu_conversionDelay);
    
    // Set Up the Configuration for the Accelerometer Configuration Register
    uint8_t accel_config =   MPU6000_REG_ACCELGYRO_ACCEL_CONFIG_XA_ST_DISABLE   |   // Disable X-Axis Accelerometer to Perform Self Test
                            MPU6000_REG_ACCELGYRO_ACCEL_CONFIG_YA_ST_DISABLE    |   // Disable Y-Axis Accelerometer to Perform Self Test
                            MPU6000_REG_ACCELGYRO_ACCEL_CONFIG_ZA_ST_DISABLE;       // Disable Z-Axis Accelerometer to Perform Self Test
    
    // Full Scale Range of the Accelerometer Outputs
    accel_config |= mpu_accelrange;
    
    // Write the configuration to the Accelerometer Configuration Register
    writeRegister(mpu_i2cAddress, MPU6000_REG_ACCELGYRO_ACCEL_CONFIG, accel_config);
    
    // Wait for the configuration to complete
    delay(mpu_conversionDelay);
    
    // Set Up the Configuration for the Power Management Register 1
    uint8_t pwr_mgmt1 =  MPU6000_REG_ACCELGYRO_PWR_MGMT_1_DEVICE_RESET_DISABLE      |   // Disable the Reset of All Internal Registers
                        MPU6000_REG_ACCELGYRO_PWR_MGMT_1_SLEEP_DISABLE              |   // Disables Sleep Mode
                        MPU6000_REG_ACCELGYRO_PWR_MGMT_1_CYCLE_DISABLE              |   // Disables Cycle Mode
                        MPU6000_REG_ACCELGYRO_PWR_MGMT_1_TEMP_DIS_ENABLE;               // Enables Temperature Sensor
    
    // Set the Specifies the Clock Source of the Device
    pwr_mgmt1 |= mpu_clocksource;
    
    // Write the configuration to the Power Management Register 1
    writeRegister(mpu_i2cAddress, MPU6000_REG_ACCELGYRO_PWR_MGMT_1, pwr_mgmt1);
    
    // Wait for the configuration to complete
    delay(mpu_conversionDelay);

}

/**************************************************************************/
/*
        Reads up the Sensor which comprises of an Accelerometer, and Gyroscope
*/
/**************************************************************************/
void MPU6000::Measure_Sensor(void)
{
    // Read all the Sensors
    // Gyroscope, Accelerometer and Temperature Data
    Measure_Accelerometer();
    Measure_Gyroscope();
    Measure_Temperature();
}

/**************************************************************************/
/*
        Reads the 3 axes of the Accelerometer
        The value is expressed in 16 bit as two’s complement
*/
/**************************************************************************/
void MPU6000::Measure_Accelerometer()
{
    // Read the Accelerometer
    uint8_t xAccelHi, xAccelLo, yAccelHi, yAccelLo, zAccelHi, zAccelLo;
    
    // Read the Data
    // Reading the High X-Axis Acceleration Data Register
    xAccelHi = readRegister(mpu_i2cAddress, MPU6000_REG_ACCELGYRO_ACCEL_XOUT_H);
    // Reading the Low X-Axis Acceleration Data Register
    xAccelLo = readRegister(mpu_i2cAddress, MPU6000_REG_ACCELGYRO_ACCEL_XOUT_L);
    // Conversion of the result
    // 16-bit signed result for X-Axis Acceleration Data of MPU6000
    mpu_accelData.X = (int16_t)((xAccelHi << 8) | xAccelLo);
    
    // Reading the High Y-Axis Acceleration Data Register
    yAccelHi = readRegister(mpu_i2cAddress, MPU6000_REG_ACCELGYRO_ACCEL_YOUT_H);
    // Reading the Low Y-Axis Acceleration Data Register
    yAccelLo = readRegister(mpu_i2cAddress, MPU6000_REG_ACCELGYRO_ACCEL_YOUT_L);
    // Conversion of the result
    // 16-bit signed result for Y-Axis Acceleration Data of MPU6000
    mpu_accelData.Y = (int16_t)((yAccelHi << 8) | yAccelLo);
    
    // Reading the High Z-Axis Acceleration Data Register
    zAccelHi = readRegister(mpu_i2cAddress, MPU6000_REG_ACCELGYRO_ACCEL_ZOUT_H);
    // Reading the Low Z-Axis Acceleration Data Register
    zAccelLo = readRegister(mpu_i2cAddress, MPU6000_REG_ACCELGYRO_ACCEL_ZOUT_L);
    // Conversion of the result
    // 16-bit signed result for Z-Axis Acceleration Data of MPU6000
    mpu_accelData.Z = (int16_t)((zAccelHi << 8) | zAccelLo);
}

/**************************************************************************/
/*
        Reads the 3 axes of the Gyroscope
        The value is expressed in 16 bit as two’s complement
*/
/**************************************************************************/
void MPU6000::Measure_Gyroscope()
{
    // Read the Gyroscope
    uint8_t xGyroHi, xGyroLo, yGyroHi, yGyroLo, zGyroHi, zGyroLo;
    
    // Read the Data
    // Reading the High X-Axis Angular Rate Data Register
    xGyroHi = readRegister(mpu_i2cAddress, MPU6000_REG_ACCELGYRO_GYRO_XOUT_H);
    // Reading the Low X-Axis Angular Rate Data Register
    xGyroLo = readRegister(mpu_i2cAddress, MPU6000_REG_ACCELGYRO_GYRO_XOUT_L);
    // Conversion of the result
    // 16-bit signed result for X-Axis Angular Rate Data of MPU6000
    mpu_gyroData.X = (int16_t)((xGyroHi << 8) | xGyroLo);
    
    // Reading the High Y-Axis Angular Rate Data Register
    yGyroHi = readRegister(mpu_i2cAddress, MPU6000_REG_ACCELGYRO_GYRO_YOUT_H);
    // Reading the Low Y-Axis Angular Rate Data Register
    yGyroLo = readRegister(mpu_i2cAddress, MPU6000_REG_ACCELGYRO_GYRO_YOUT_L);
    // Conversion of the result
    // 16-bit signed result for Y-Axis Angular Rate Data of MPU6000
    mpu_gyroData.Y = (int16_t)((yGyroHi << 8) | yGyroLo);
    
    // Reading the High Z-Axis Angular Rate Data Register
    zGyroHi = readRegister(mpu_i2cAddress, MPU6000_REG_ACCELGYRO_GYRO_ZOUT_H);
    // Reading the Low Z-Axis Angular Rate Data Register
    zGyroLo = readRegister(mpu_i2cAddress, MPU6000_REG_ACCELGYRO_GYRO_ZOUT_L);
    // Conversion of the result
    // 16-bit signed result for Z-Axis Angular Rate Data of MPU6000
    mpu_gyroData.Z = (int16_t)((zGyroHi << 8) | zGyroLo);
    
}

/**************************************************************************/
/*
        Reads the Temperature Sensor
*/
/**************************************************************************/
void MPU6000::Measure_Temperature()
{
    // Read the Temperature Sensor
    uint8_t xTempHi, xTempLo;
    int16_t rawTemp;
    
    // Read the Data
    // Reading the High Temperature Output Data Register
    xTempHi = readRegister(mpu_i2cAddress, MPU6000_REG_ACCELGYRO_TEMP_OUT_H);
    // Reading the Low Temperature Output Data Register
    xTempLo = readRegister(mpu_i2cAddress, MPU6000_REG_ACCELGYRO_TEMP_OUT_L);
    // Conversion of the result
    // 16-bit signed result for X-Axis Magnetic Data of MPU6000
    rawTemp = (int16_t)((xTempHi << 8) | xTempLo);
    mpu_tempData.T = (float)(rawTemp / 340.0 + 36.53);
}
