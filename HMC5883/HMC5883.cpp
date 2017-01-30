/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HMC5883
        This code is designed to work with the HMC5883_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Compass?sku=HMC5883_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "HMC5883.h"

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
        Instantiates a new HMC5883 class with appropriate properties
*/
/**************************************************************************/
void HMC5883::getAddr_HMC5883(uint8_t i2cAddress)
{
    hmc_i2cAddress = i2cAddress;
    hmc_ConversionDelay = HMC5883_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool HMC5883::begin()
{
    Wire.begin();
    if (readRegister(hmc_i2cAddress, HMC5883_REG_MAG_IDENTITY_A) != 0x48) return false;
    if (readRegister(hmc_i2cAddress, HMC5883_REG_MAG_IDENTITY_B) != 0x34) return false;
    if (readRegister(hmc_i2cAddress, HMC5883_REG_MAG_IDENTITY_C) != 0x33) return false;
    
    setUpMagnetometer();
    
    return true;
}

/**************************************************************************/
/*
        Sets the number of samples averaged (1 to 8) per measurement output
*/
/**************************************************************************/
void HMC5883::setMASample(hmcMASample_t sample)
{
    hmc_masample = sample;
}
                    
/**************************************************************************/
/*
        Gets the Output Data Rate Selection for the Accelerometer
*/
/**************************************************************************/
hmcMASample_t HMC5883::getMASample()
{
    return hmc_masample;
}

/**************************************************************************/
/*
        Sets the Data Output Rate
        These bits set the rate at which data is written to all 3 data output registers
*/
/**************************************************************************/
void HMC5883::setDataRate(hmcDataRate_t datarate)
{
    hmc_datarate = datarate;
}

/**************************************************************************/
/*
        Gets the Data Output Rate
*/
/**************************************************************************/
hmcDataRate_t HMC5883::getDataRate()
{
    return hmc_datarate;
}

/**************************************************************************/
/*
        Sets the Measurement Configuration/Mode
        This defines the measurement flow of the device, specifically whether
        or not to incorporate an applied bias into the measurement
*/
/**************************************************************************/
void HMC5883::setMeasMode(hmcMeasMode_t measmode)
{
    hmc_measmode = measmode;
}

/**************************************************************************/
/*
        Gets the Measurement Configuration/Mode
*/
/**************************************************************************/
hmcMeasMode_t HMC5883::getMeasMode()
{
    return hmc_measmode;
}

/**************************************************************************/
/*
        Sets the Gain Configuration 
        This configures the gain for the device
        The gain configuration is common for all channels
*/
/**************************************************************************/
void HMC5883::setMagGain(hmcMagGain_t maggain)
{
    hmc_maggain = maggain;
}

/**************************************************************************/
/*
        Gets the Gain Configuration
*/
/**************************************************************************/
hmcMagGain_t HMC5883::getMagGain()
{
    return hmc_maggain;
}

/**************************************************************************/
/*
        Sets the Operating Mode of the Device
*/
/**************************************************************************/
void HMC5883::setOpMode(hmcOpMode_t opmode)
{
    hmc_opmode = opmode;
}

/**************************************************************************/
/*
        Gets the Operating Mode of the Device
*/
/**************************************************************************/
hmcOpMode_t HMC5883::getOpMode()
{
    return hmc_opmode;
}

/**************************************************************************/
/*
        Set up the Magnetometer
*/
/**************************************************************************/
void HMC5883::setUpMagnetometer(void)
{
    // Set up the Configuration Register A
    // Set the number of samples averaged (1 to 8) per measurement output
    uint8_t cra = hmc_masample;
    
    // Set the Data Output Rate
    cra |= hmc_datarate;
    
    // Set the Measurement Configuration
    cra |= hmc_measmode;
    
    // Write the configuration to Configuration Register A
    writeRegister(hmc_i2cAddress, HMC5883_REG_MAG_CONFIG_A, cra);
    
    // Wait for the configuration to complete
    delay(hmc_ConversionDelay);
    
    // Set up the Configuration Register B
    // Set the Gain Configuration
    uint8_t crb = hmc_maggain;
    
    // Write the configuration to Configuration Register B
    writeRegister(hmc_i2cAddress, HMC5883_REG_MAG_CONFIG_B, crb);
    
    // Wait for the configuration to complete
    delay(hmc_ConversionDelay);
    
    // Set up the Mode Register
    // Set the Operating Mode
    uint8_t mode = hmc_opmode;
    
    // Write the configuration to Mode Register
    writeRegister(hmc_i2cAddress, HMC5883_REG_MAG_MODE, mode);
    
    // Wait for the configuration to complete
    delay(hmc_ConversionDelay);
}

/**************************************************************************/
/*
        Reads the results for the magnetometer HMC5883 (Compass)
*/
/**************************************************************************/
void HMC5883::Measure_Magnetometer(void)
{
    // Read the Magnetometer
    uint8_t xMagHi, xMagLo, yMagHi, yMagLo, zMagHi, zMagLo;
    
    // Read the conversion results
    // Reading the MSB data of output X register
    xMagHi = readRegister(hmc_i2cAddress, HMC5883_REG_MAG_OUT_X_MSB);
    // Reading the LSB data of output X register
    xMagLo = readRegister(hmc_i2cAddress, HMC5883_REG_MAG_OUT_X_LSB);
    // Conversion of the result
    // 16-bit signed result for Channel X of HMC5883
    hmc_magdata.X = (int16_t)((xMagHi << 8) | xMagLo);
    
    // Reading the MSB data of output Y register
    yMagHi = readRegister(hmc_i2cAddress, HMC5883_REG_MAG_OUT_Y_MSB);
    // Reading the LSB data of output Y register
    yMagLo = readRegister(hmc_i2cAddress, HMC5883_REG_MAG_OUT_Y_LSB);
    // Conversion of the result
    // 16-bit signed result for Channel Y of HMC5883
    hmc_magdata.Y = (int16_t)((yMagHi << 8) | yMagLo);
    
    // Reading the MSB data of output Z register
    zMagHi = readRegister(hmc_i2cAddress, HMC5883_REG_MAG_OUT_Z_MSB);
    // Reading the LSB data of output Z register
    zMagLo = readRegister(hmc_i2cAddress, HMC5883_REG_MAG_OUT_Z_LSB);
    // Conversion of the result
    // 16-bit signed result for Channel Z of HMC5883
    hmc_magdata.Z = (int16_t)((zMagHi << 8) | zMagLo);
}
