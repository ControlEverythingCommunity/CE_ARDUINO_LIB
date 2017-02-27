/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MAG3110
        This code is designed to work with the MAG3110_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "MAG3110.h"

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
        Instantiates a new MAG3110 class with appropriate properties
        Magnetometer Address
*/
/**************************************************************************/
void MAG3110::getAddr_MAG3110(uint8_t i2cAddress)
{
    mag_i2cAddress = i2cAddress;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool MAG3110::begin()
{
    Wire.begin();
    
    uint8_t devid = readRegister(mag_i2cAddress, MAG3110_REG_MAG_WHO_AM_I);
    if (devid != MAG3110_MAG_DEV_ID)
        return false;
    
    // Set up the sensor for Magnetometer
    // setUpMagnetometer();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Over-Sampling Ratio and Data Rate for the Magnetometer
*/
/**************************************************************************/
void MAG3110::setMagDataRate(magDataRate_t datarate)
{
    mag_datarate = datarate;
}

/**************************************************************************/
/*
        Gets the Over-Sampling Ratio and Data Rate for the Magnetometer
*/
/**************************************************************************/
magDataRate_t MAG3110::getMagDataRate()
{
    return mag_datarate;
}

/**************************************************************************/
/*
        Sets the Fast Read Selection
*/
/**************************************************************************/
void MAG3110::setRead(magRead_t read)
{
    mag_read = read;
}

/**************************************************************************/
/*
        Gets the Fast Read Selection
*/
/**************************************************************************/
magRead_t MAG3110::getRead()
{
    return mag_read;
}

/**************************************************************************/
/*
        Sets the Trigger Immediate Measurement
*/
/**************************************************************************/
void MAG3110::setTrigger(magTrigger_t trigger)
{
    mag_trigger = trigger;
}

/**************************************************************************/
/*
        Gets the Trigger Immediate Measurement
*/
/**************************************************************************/
magTrigger_t MAG3110::getTrigger()
{
    return mag_trigger;
}

/**************************************************************************/
/*
        Sets the Operating Mode Selection
*/
/**************************************************************************/
void MAG3110::setMagMode(magMode_t mode)
{
    mag_mode = mode;
}

/**************************************************************************/
/*
        Gets the Operating Mode Selection
*/
/**************************************************************************/
magMode_t MAG3110::getMagMode()
{
    return mag_mode;
}

/**************************************************************************/
/*
        Sets up the Magnetometer
*/
/**************************************************************************/
void MAG3110::setUpMagnetometer(void)
{
    // Set Up the Configuration for the Magnetometer Control Register 1
    /*
    // Set the Over-Sampling Ratio and Data Rate
    uint8_t config1 = mag_datarate;
    
    // Set the Fast Read Selection
    config1 |= mag_read;
    
    // Set the Trigger Immediate Measurement
    config1 |= mag_trigger;
    
    // Set the Operating Mode Selection
    config1 |= mag_mode;
    */
    
    uint8_t config1 =   MAG3110_REG_MAG_CTRL_REG1_MDROS_80HZ        |   // Output Rate (Hz): 80.00, Over Sample Ratio: 16, ADC Rate (Hz): 1280, Current Type (microA): 900.0, Noise Type (microT rms): 0.4
                        MAG3110_REG_MAG_CTRL_REG1_FR_FULL           |   // The Full 16-Bit Values are Read
                        MAG3110_REG_MAG_CTRL_REG1_TM_NORMAL         |   // Normal Operation Based on AC Condition
                        MAG3110_REG_MAG_CTRL_REG1_AC_ACTIVE;            // ACTIVE Mode
    
    // Write the configuration to the Magnetometer Control Register 1
    writeRegister(mag_i2cAddress, MAG3110_REG_MAG_CTRL_REG1, config1);
    
    // Wait for the configuration to complete
    delay(mag_conversionDelay);
}

/**************************************************************************/
/*
        Reads the 3 axes of the Magnetometer
        The value is expressed in 16 bit as two’s complement
*/
/**************************************************************************/
void MAG3110::Measure_Magnetometer()
{
    // Read the Magnetometer
    uint8_t xMagHi, xMagLo, yMagHi, yMagLo, zMagHi, zMagLo;
    
    // Read the Data
    // Reading the High X-Axis Angular Rate Data Register
    xMagHi = readRegister(mag_i2cAddress, MAG3110_REG_MAG_OUT_X_MSB);
    // Reading the Low X-Axis Angular Rate Data Register
    xMagLo = readRegister(mag_i2cAddress, MAG3110_REG_MAG_OUT_X_LSB);
    // Conversion of the result
    // 16-bit signed result for X-Axis Angular Rate Data of MAG3110
    mag_Data.X = (int16_t)((xMagHi << 8) | xMagLo);
    
    // Reading the High Y-Axis Angular Rate Data Register
    yMagHi = readRegister(mag_i2cAddress, MAG3110_REG_MAG_OUT_Y_MSB);
    // Reading the Low Y-Axis Angular Rate Data Register
    yMagLo = readRegister(mag_i2cAddress, MAG3110_REG_MAG_OUT_Y_LSB);
    // Conversion of the result
    // 16-bit signed result for Y-Axis Angular Rate Data of MAG3110
    mag_Data.Y = (int16_t)((yMagHi << 8) | yMagLo);
    
    // Reading the High Z-Axis Angular Rate Data Register
    zMagHi = readRegister(mag_i2cAddress, MAG3110_REG_MAG_OUT_Z_MSB);
    // Reading the Low Z-Axis Angular Rate Data Register
    zMagLo = readRegister(mag_i2cAddress, MAG3110_REG_MAG_OUT_Z_LSB);
    // Conversion of the result
    // 16-bit signed result for Z-Axis Angular Rate Data of MAG3110
    mag_Data.Z = (int16_t)((zMagHi << 8) | zMagLo);
    
}
