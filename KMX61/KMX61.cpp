/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        KMX61
        This code is designed to work with the KMX61_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "KMX61.h"

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
        Instantiates a new KMX61 class with appropriate properties
*/
/**************************************************************************/
void KMX61::getAddr_KMX61(uint8_t i2cAddress)
{
    kmx_i2cAddress = i2cAddress;
    kmx_conversionDelay = kmx_conversionDelay;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool KMX61::begin()
{
    Wire.begin();
    
    // Set up the sensor
    // setUpSensor()
    
    return true;
}

/**************************************************************************/
/*
        Sets the ASIC_AL to Control the Operating Mode of all of the Connected
        Devices based on the Activity State of the Device
*/
/**************************************************************************/
void KMX61::setActStandby(kmxActStandby_t actstandby)
{
    kmx_actstandby = actstandby;
}

/**************************************************************************/
/*
        Gets the ASIC_AL to Control the Operating Mode of all of the Connected 
        Devices based on the Activity State of the Device
*/
/**************************************************************************/
kmxActStandby_t KMX61::getActStandby()
{
    return kmx_actstandby;
}

/**************************************************************************/
/*
        Sets the Controls the Operating Mode of the ASIC_AL’s Magnetometer
*/
/**************************************************************************/
void KMX61::setMagStandby(kmxMagStandby_t magstandby)
{
    kmx_magstandby = magstandby;
}

/**************************************************************************/
/*
        Gets the Controls the Operating Mode of the ASIC_AL’s Magnetometer
*/
/**************************************************************************/
kmxMagStandby_t KMX61::getMagStandby()
{
    return kmx_magstandby;
}

/**************************************************************************/
/*
        Sets the Controls the Operating Mode of the ASIC_AL’s Accelerometer
*/
/**************************************************************************/
void KMX61::setAccelStandby(kmxAccelStandby_t accelstandby)
{
    kmx_accelstandby = accelstandby;
}

/**************************************************************************/
/*
        Gets the Controls the Operating Mode of the ASIC_AL’s Accelerometer
*/
/**************************************************************************/
kmxAccelStandby_t KMX61::getAccelStandby()
{
    return kmx_accelstandby;
}

/**************************************************************************/
/*
        Sets the Enables the Back to Sleep Engine
*/
/**************************************************************************/
void KMX61::setSleepEngine(kmxSleepEngine_t sleepengine)
{
    kmx_sleepengine = sleepengine;
}

/**************************************************************************/
/*
        Gets the Enables the Back to Sleep Engine
*/
/**************************************************************************/
kmxSleepEngine_t KMX61::getSleepEngine()
{
    return kmx_sleepengine;
}

/**************************************************************************/
/*
        Sets the Enables the Wake up Engine
*/
/**************************************************************************/
void KMX61::setWakeEngine(kmxWakeEngine_t wakeengine)
{
    kmx_wakeengine = wakeengine;
}

/**************************************************************************/
/*
        Gets the Enables the Wake up Engine
*/
/**************************************************************************/
kmxWakeEngine_t KMX61::getWakeEngine()
{
    return kmx_wakeengine;
}

/**************************************************************************/
/*
        Sets the Full-Scale Selection for the Accelerometer
*/
/**************************************************************************/
void KMX61::setAccelRange(kmxAccelRange_t accelrange)
{
    kmx_accelrange = accelrange;
}

/**************************************************************************/
/*
        Gets the Full-Scale Selection for the Accelerometer
*/
/**************************************************************************/
kmxAccelRange_t KMX61::getAccelRange()
{
    return kmx_accelrange;
}

/**************************************************************************/
/*
        Sets the Magnetic Output Data Rate Selection
*/
/**************************************************************************/
void KMX61::setMagDataRate(kmxMagDataRate_t magdatarate)
{
    kmx_magdatarate = magdatarate;
}

/**************************************************************************/
/*
        Gets the Magnetic Output Data Rate Selection
*/
/**************************************************************************/
kmxMagDataRate_t KMX61::getMagDataRate()
{
    return kmx_magdatarate;
}

/**************************************************************************/
/*
        Sets the Output Data Rate Selection for the Accelerometer
*/
/**************************************************************************/
void KMX61::setAccelDataRate(kmxAccelDataRate_t acceldatarate)
{
    kmx_acceldatarate = acceldatarate;
}

/**************************************************************************/
/*
        Gets the Output Data Rate Selection for the Accelerometer
*/
/**************************************************************************/
kmxAccelDataRate_t KMX61::getAccelDataRate()
{
    return kmx_acceldatarate;
}

/**************************************************************************/
/*
        Sets up the Sensor
*/
/**************************************************************************/
void KMX61::setUpSensor(void)
{
    // Set Up the Configuration for the Stand-by Status Register
    /*
     // Set the ASIC_AL to Control the Operating Mode of all of the Connected Devices based on the Activity State of the Device
     uint8_t standby = kmx_actstandby;
     
     // Set the Controls the Operating Mode of the ASIC_AL’s Magnetometer
     standby |= kmx_magstandby;
     
     // Set the Controls the Operating Mode of the ASIC_AL’s Accelerometer
     standby |= kmx_accelstandby;
    */
    
    uint8_t standby =   KMX61_REG_STBY_REG_ACT_STBY_0      |    // Feature Disabled
                        KMX61_REG_STBY_REG_MAG_STBY_0      |    // Operating Mode
                        KMX61_REG_STBY_REG_ACCEL_STBY_0;        // Operating Mode
    
    // Write the configuration to the Stand-by Status Register
    writeRegister(kmx_i2cAddress, KMX61_REG_STBY_REG, standby);
    
    // Wait for the configuration to complete
    delay(kmx_conversionDelay);
    
    // Set Up the Configuration for the Control Register 1
    /*
     // Set the Enables the Back to Sleep Engine
     uint8_t config1 = kmx_sleepengine;
    
     // Set the Enables the Wake up Engine
     config1 |= kmx_wakeengine;
    
     // Set the Acceleration Range of the Accelerometer Outputs
     config1 |= kmx_accelrange;
    */
    
    uint8_t config1 =   KMX61_REG_CNTL1_BTSE_DISABLE      |   // Disable Back to Sleep Engine
                        KMX61_REG_CNTL1_WUFE_DISABLE;         // Disable Wake up Engine
    
    config1 |= kmx_accelrange;       // Acceleration Range of the Accelerometer Outputs
    
    // Write the configuration to the Control Register 1
    writeRegister(kmx_i2cAddress, KMX61_REG_BUF_CTRL1, 0x10 | config1);
    
    // Wait for the configuration to complete
    delay(kmx_conversionDelay);
    
    // Set Up the Configuration for the Output Data Control Register
    
    // Set the Rate at Which Data Samples from the Magnetometer will be Updated
    uint8_t datacontrol = kmx_magdatarate;
    
    // Set the Rate at Which Data Samples from the Accelerometer will be Updated
    datacontrol |= kmx_acceldatarate;
    
    // Write the configuration to the Output Data Control Register
    writeRegister(kmx_i2cAddress, KMX61_REG_ODCNTL, datacontrol);
    
    // Wait for the configuration to complete
    delay(kmx_conversionDelay);
    
}

/**************************************************************************/
/*
        Reads up the Sensor which comprises of an Accelerometer and Magnetometer
*/
/**************************************************************************/
void KMX61::Measure_Sensor(void)
{
    // Read all the Sensors
    // Accelerometer, Magnetometer Data
    Measure_Accelerometer();
    Measure_Magnetometer();
}

/**************************************************************************/
/*
        Reads the 3 axes of the Accelerometer
        The value is expressed in 12 bit as two’s complement
*/
/**************************************************************************/
void KMX61::Measure_Accelerometer()
{
    // Read the Accelerometer
    uint8_t xAccelLo, xAccelHi, yAccelLo, yAccelHi, zAccelLo, zAccelHi;
    
    // Read the Data
    // Reading the Low X-Axis Angular Rate Data Register
    xAccelLo = readRegister(kmx_i2cAddress, KMX61_REG_ACCEL_XOUT_L);
    // Reading the High X-Axis Angular Rate Data Register
    xAccelHi = readRegister(kmx_i2cAddress, KMX61_REG_ACCEL_XOUT_H);
    // Conversion of the result
    // 12-bit signed result for X-Axis Angular Rate Data of KMX61
    kmx_accelData.X = (int16_t)((xAccelHi << 8) | (xAccelLo & 0xF0));
    kmx_accelData.X = kmx_accelData.X >> 4;
    
    // Reading the Low Y-Axis Angular Rate Data Register
    yAccelLo = readRegister(kmx_i2cAddress, KMX61_REG_ACCEL_YOUT_L);
    // Reading the High Y-Axis Angular Rate Data Register
    yAccelHi = readRegister(kmx_i2cAddress, KMX61_REG_ACCEL_YOUT_H);
    // Conversion of the result
    // 12-bit signed result for Y-Axis Angular Rate Data of KMX61
    kmx_accelData.Y = (int16_t)((yAccelHi << 8) | (yAccelLo & 0xF0));
    kmx_accelData.Y = kmx_accelData.Y >> 4;
    
    // Reading the Low Z-Axis Angular Rate Data Register
    zAccelLo = readRegister(kmx_i2cAddress, KMX61_REG_ACCEL_ZOUT_L);
    // Reading the High Z-Axis Angular Rate Data Register
    zAccelHi = readRegister(kmx_i2cAddress, KMX61_REG_ACCEL_ZOUT_H);
    // Conversion of the result
    // 12-bit signed result for Z-Axis Angular Rate Data of KMX61
    kmx_accelData.Z = (int16_t)((zAccelHi << 8) | (zAccelLo & 0xF0));
    kmx_accelData.Z = kmx_accelData.Z >> 4;
    
}

/**************************************************************************/
/*
        Reads the 3 axes of the Magnetometer
        The value is expressed in 14 bit as two’s complement
*/
/**************************************************************************/
void KMX61::Measure_Magnetometer()
{
    // Read the Magnetometer
    uint8_t xMagHi, xMagLo, yMagHi, yMagLo, zMagHi, zMagLo;
    
    // Read the Data
    // Reading the High X-Axis Magnetic Data Register
    xMagHi = readRegister(kmx_i2cAddress, KMX61_REG_MAG_XOUT_L);
    // Reading the Low X-Axis Magnetic Data Register
    xMagLo = readRegister(kmx_i2cAddress, KMX61_REG_MAG_XOUT_H);
    // Conversion of the result
    // 16-bit signed result for X-Axis Magnetic Data of KMX61
    kmx_magData.X = (int16_t)((xMagHi << 8) | (xMagLo & 0xFC));
    kmx_magData.X = kmx_magData.X >> 2;
    
    // Reading the High Y-Axis Magnetic Data Register
    yMagHi = readRegister(kmx_i2cAddress, KMX61_REG_MAG_YOUT_L);
    // Reading the Low Y-Axis Magnetic Data Register
    yMagLo = readRegister(kmx_i2cAddress, KMX61_REG_MAG_YOUT_H);
    // Conversion of the result
    // 16-bit signed result for Y-Axis Magnetic Data of KMX61
    kmx_magData.Y = (int16_t)((yMagHi << 8) | (yMagLo & 0xFC));
    kmx_magData.Y = kmx_magData.Y >> 2;
    
    // Reading the High Z-Axis Magnetic Data Register
    zMagHi = readRegister(kmx_i2cAddress, KMX61_REG_MAG_ZOUT_L);
    // Reading the Low Z-Axis Magnetic Data Register
    zMagLo = readRegister(kmx_i2cAddress, KMX61_REG_MAG_ZOUT_H);
    // Conversion of the result
    // 16-bit signed result for Z-Axis Magnetic Data of KMX61
    kmx_magData.Z = (int16_t)((zMagHi << 8) | (zMagLo & 0xFC));
    kmx_magData.Z = kmx_magData.Z >> 2;
}
