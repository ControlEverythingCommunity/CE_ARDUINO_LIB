/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ADXL345
        This code is designed to work with the ADXL345_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelorometer?sku=ADXL345_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "ADXL345.h"

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
        Instantiates a new ADXL345 class with appropriate properties
        Accelerometer Address
*/
/**************************************************************************/
void ADXL345::getAddr_ADXL345(uint8_t i2cAddress)
{
    adxl_i2cAddress = i2cAddress;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool ADXL345::begin()
{
    Wire.begin();
    
    uint8_t devid = readRegister(adxl_i2cAddress, ADXL345_REG_ACCEL_DEVID);
    if (devid != ADXL345_ACCEL_DEV_ID)
        return false;
    
    // Set up the sensor for Accelerometer
    // setUpAccelerometer();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Output Data Rate Selection for the Accelerometer
*/
/**************************************************************************/
void ADXL345::setAccelDataRate(adxlAccelDataRate_t acceldatarate)
{
    adxl_acceldatarate = acceldatarate;
}

/**************************************************************************/
/*
        Gets the Output Data Rate Selection for the Accelerometer
*/
/**************************************************************************/
adxlAccelDataRate_t ADXL345::getAccelDataRate()
{
    return adxl_acceldatarate;
}


/**************************************************************************/
/*
        Sets the Acceleration Full-Scale Selection
*/
/**************************************************************************/
void ADXL345::setAccelRange(adxlAccelRange_t accelrange)
{
    adxl_accelrange = accelrange;
}

/**************************************************************************/
/*
        Gets the Acceleration Full-Scale Selection
*/
/**************************************************************************/
adxlAccelRange_t ADXL345::getAccelRange()
{
    return adxl_accelrange;
}

/**************************************************************************/
/*
        Sets up the Accelerometer
*/
/**************************************************************************/
void ADXL345::setUpAccelerometer(void)
{
    // Set Up the Configuration for the Accelerometer Bandwidth Rate Register
    uint8_t bw_rate =   ADXL345_REG_ACCEL_BW_RATE_LOW_POWER_NORMAL;   // Normal Operation
    
    bw_rate |= adxl_acceldatarate;      // Device Bandwidth and Output Data Rate
    
    // Write the configuration to the Accelerometer Bandwidth Rate Register
    writeRegister(adxl_i2cAddress, ADXL345_REG_ACCEL_BW_RATE, bw_rate);
    
    // Wait for the configuration to complete
    delay(adxl_conversionDelay);
    
    // Set Up the Configuration for the Accelerometer Power Control Register
    uint8_t power_ctl =   ADXL345_REG_ACCEL_POWER_CTL_LINK_CONCURRENT       |   // The Inactivity and Activity Functions are Concurrent
                        ADXL345_REG_ACCEL_POWER_CTL_AUTO_SLEEP_DISABLE      |   // Disables Automatic Switching to Sleep Mode
                        ADXL345_REG_ACCEL_POWER_CTL_MEASURE_NORMAL          |   // Normal Mode
                        ADXL345_REG_ACCEL_POWER_CTL_SLEEP_DISABLE           |   // Normal Mode of Operation
                        ADXL345_REG_ACCEL_POWER_CTL_WAKEUP_8_HZ;                // Frequency of Readings in Sleep Mode: 8Hz
    
    // Write the configuration to the Accelerometer Power Control Register
    writeRegister(adxl_i2cAddress, ADXL345_REG_ACCEL_POWER_CTL, power_ctl);
    
    // Wait for the configuration to complete
    delay(adxl_conversionDelay);
    
    // Set Up the Configuration for the Accelerometer Data Format Register
    uint8_t range = adxl_accelrange;   // Full Scale Range
    
    // Write the configuration to the Accelerometer Data Format Register
    writeRegister(adxl_i2cAddress, ADXL345_REG_ACCEL_DATA_FORMAT, range);
    
    // Wait for the configuration to complete
    delay(adxl_conversionDelay);

}

/**************************************************************************/
/*
        Reads the 3 axes of the Accelerometer
        The value is expressed in 13 bit as two’s complement
*/
/**************************************************************************/
void ADXL345::Measure_Accelerometer()
{
    // Read the Accelerometer
    uint8_t xAccelLo, xAccelHi, yAccelLo, yAccelHi, zAccelLo, zAccelHi;
    
    // Read the Data
    // Reading the Low X-Axis Acceleration Data Register
    xAccelLo = readRegister(adxl_i2cAddress, ADXL345_REG_ACCEL_DATAX0);
    // Reading the High X-Axis Acceleration Data Register
    xAccelHi = readRegister(adxl_i2cAddress, ADXL345_REG_ACCEL_DATAX1);
    // Conversion of the result
    // 13-bit signed result for X-Axis Acceleration Data of ADXL345
    adxl_accelData.X = (int16_t)((xAccelHi << 8) | xAccelLo);
    
    // Reading the Low Y-Axis Acceleration Data Register
    yAccelLo = readRegister(adxl_i2cAddress, ADXL345_REG_ACCEL_DATAY0);
    // Reading the High Y-Axis Acceleration Data Register
    yAccelHi = readRegister(adxl_i2cAddress, ADXL345_REG_ACCEL_DATAY1);
    // Conversion of the result
    // 13-bit signed result for Y-Axis Acceleration Data of ADXL345
    adxl_accelData.Y = (int16_t)((yAccelHi << 8) | yAccelLo);
    
    // Reading the Low Z-Axis Acceleration Data Register
    zAccelLo = readRegister(adxl_i2cAddress, ADXL345_REG_ACCEL_DATAZ0);
    // Reading the High Z-Axis Acceleration Data Register
    zAccelHi = readRegister(adxl_i2cAddress, ADXL345_REG_ACCEL_DATAZ1);
    // Conversion of the result
    // 13-bit signed result for Z-Axis Acceleration Data of ADXL345
    adxl_accelData.Z = (int16_t)((zAccelHi << 8) | zAccelLo);
}
