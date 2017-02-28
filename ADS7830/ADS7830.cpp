/**************************************************************************/
/*
 Distributed with a free-will license.
 Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
 ADS7830
 This code is designed to work with the ADS7830_I2CADC I2C Mini Module available from ControlEverything.com.
 https://www.controleverything.com/content/Analog-Digital-Converters?sku=ADS7830_I2CADC#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "ADS7830.h"

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
        Writes 8-bits to the register
*/
/**************************************************************************/
static void writeRegister(uint8_t i2cAddress, uint8_t reg)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    Wire.endTransmission();
}

/**************************************************************************/
/*
        Reads 8-bits from the register
*/
/**************************************************************************/
static uint8_t readRegister(uint8_t i2cAddress)
{
    Wire.beginTransmission(i2cAddress);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)1);
    return (int8_t)(i2cread());
}

/**************************************************************************/
/*
        Instantiates a new ADS7830 class with appropriate properties
*/
/**************************************************************************/
void ADS7830::getAddr_ADS7830(uint8_t i2cAddress)
{
    ads_i2cAddress = i2cAddress;
    ads_conversionDelay = ADS7830_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void ADS7830::begin() {
  Wire.begin();
}

/**************************************************************************/
/*
        Sets the Single-Ended/Differential Inputs
*/
/**************************************************************************/
void ADS7830::setSDMode(adsSDMode_t sdmode)
{
    ads_sdmode = sdmode;
}

/**************************************************************************/
/*
        Gets the Single-Ended/Differential Inputs
*/
/**************************************************************************/
adsSDMode_t ADS7830::getSDMode()
{
    return ads_sdmode;
}

/**************************************************************************/
/*
        Sets the Power-Down Mode
*/
/**************************************************************************/
void ADS7830::setPDMode(adsPDMode_t pdmode)
{
    ads_pdmode = pdmode;
}

/**************************************************************************/
/*
        Gets the Power-Down Mode
*/
/**************************************************************************/
adsPDMode_t ADS7830::getPDMode()
{
    return ads_pdmode;
}

/**************************************************************************/
/*
        Reads the conversion results, measuring the voltage
        for a single-ended ADC reading from the specified channel
        Negative voltages cannot be applied to this circuit because the
        ADS7830 can only accept positive voltages in this mode
*/
/**************************************************************************/
uint8_t ADS7830::Measure_SingleEnded(uint8_t channel)
{
    if (channel > 7)
    {
        return 0;
    }
    uint8_t config = 0;
    // Set Single-Ended/Differential Inputs
    config |= ads_sdmode;
    
    // Set Power-Down Selection
    config |= ads_pdmode;

    // Set single-ended input channel
    switch (channel)
    {
        case (0):
            config |= ADS7830_REG_COMMAND_CH_SINGLE_0;
            break;
        case (1):
            config |= ADS7830_REG_COMMAND_CH_SINGLE_1;
            break;
        case (2):
            config |= ADS7830_REG_COMMAND_CH_SINGLE_2;
            break;
        case (3):
            config |= ADS7830_REG_COMMAND_CH_SINGLE_3;
            break;
        case (4):
            config |= ADS7830_REG_COMMAND_CH_SINGLE_4;
            break;
        case (5):
            config |= ADS7830_REG_COMMAND_CH_SINGLE_5;
            break;
        case (6):
            config |= ADS7830_REG_COMMAND_CH_SINGLE_6;
            break;
        case (7):
            config |= ADS7830_REG_COMMAND_CH_SINGLE_7;
            break;
    }

    // Write config register to the ADC
    writeRegister(ads_i2cAddress, config);

    // Wait for the conversion to complete
    delay(ads_conversionDelay);

    // Read the conversion results
    // 8-bit unsigned results for the ADS7830
    return readRegister(ads_i2cAddress);
}

/**************************************************************************/
/* 
        Reads the conversion results, measuring the voltage
        difference between the P (CH#) and N (CH#) input
        Generates a signed value since the difference can be either
        positive or negative
*/
/**************************************************************************/
int8_t ADS7830::Measure_Differential(uint8_t channel)
{
    uint8_t config = 0;
    
    // Set Single-Ended/Differential Inputs
    config |= ads_sdmode;
    
    // Set Power-Down Selection
    config |= ads_pdmode;
    
    // Set Differential input channel
    switch (channel)
    {
        case (01):
            config |= ADS7830_REG_COMMAND_CH_DIFF_0_1;  // CH0 = P, CH1 = N
            break;
        case (10):
            config |= ADS7830_REG_COMMAND_CH_DIFF_1_0;  // CH1 = P, CH0 = N
            break;
        case (23):
            config |= ADS7830_REG_COMMAND_CH_DIFF_2_3;  // CH2 = P, CH3 = N
            break;
        case (32):
            config |= ADS7830_REG_COMMAND_CH_DIFF_3_2;  // CH3 = P, CH2 = N
            break;
        case (45):
            config |= ADS7830_REG_COMMAND_CH_DIFF_4_5;  // CH4 = P, CH5 = N
            break;
        case (54):
            config |= ADS7830_REG_COMMAND_CH_DIFF_5_4;  // CH5 = P, CH4 = N
            break;
        case (67):
            config |= ADS7830_REG_COMMAND_CH_DIFF_6_7;  // CH6 = P, CH7 = N
            break;
        case (76):
            config |= ADS7830_REG_COMMAND_CH_DIFF_7_6;  // CH7 = P, CH6 = N
            break;
    }

    // Write config register to the ADC
    writeRegister(ads_i2cAddress, config);

    // Wait for the conversion to complete
    delay(ads_conversionDelay);

    // Read the conversion results
    uint8_t raw_adc = readRegister(ads_i2cAddress);
    return (int8_t)raw_adc;
}
