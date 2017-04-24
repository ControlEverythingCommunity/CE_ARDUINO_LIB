/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        PCA9537_IO2
        This code is designed to work with the PCA9537_I2CIO202_IRFR3710 I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "PCA9537_IO2.h"

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
        Writes 8-bits to the destination register
*/
/**************************************************************************/
static void writeRegister8(uint8_t i2cAddress, uint8_t reg)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    Wire.endTransmission();
}

/**************************************************************************/
/*
        Reads 8-bits to the specified destination register
*/
/**************************************************************************/
static uint8_t readRegister(uint8_t i2cAddress)
{
    Wire.beginTransmission(i2cAddress);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)1);
    return (uint8_t)(i2cread());
}

/**************************************************************************/
/*
        Instantiates a new PCA9537_IO2 class with appropriate properties
*/
/**************************************************************************/
void PCA9537_IO2::getAddr_PCA9537_IO2(uint8_t i2cAddress)
{
    pca_i2cAddress = i2cAddress;
    pca_conversionDelay = PCA9537_IO2_CONVERSIONDELAY;
    
}

/**************************************************************************/
/*
        Constructor for Class PCA9537_IO2
*/
/**************************************************************************/
PCA9537_IO2::PCA9537_IO2()
{
    
}

/**************************************************************************/
/*
        Destructor for Class PCA9537_IO2
*/
/**************************************************************************/
PCA9537_IO2::~PCA9537_IO2()
{
    
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
uint8_t PCA9537_IO2::begin()
{
    Wire.begin();
}
/**************************************************************************/
/*
        Resets up the Device
*/
/**************************************************************************/
void PCA9537_IO2::Reset()
{
    setPinMode(PCA9537_IO2_MODE_INPUT);
    setPinState(PCA9537_IO2_STATE_HIGH);
    setPinPolarity(PCA9537_IO2_STATE_NON_INVERTED);
    writeRegister8(pca_i2cAddress, PCA9537_IO2_REG_INPUT);
    delay(pca_conversionDelay);
}

/**************************************************************************/
/*
        Gets the Pin Mode
        Output: 0, Input: 1
*/
/**************************************************************************/
uint8_t PCA9537_IO2::getPinMode(pcaPin_t pin)
{
    return getPin(pin, PCA9537_IO2_REG_CONFIG);
}

/**************************************************************************/
/*
        Gets the Pin State
        Low: 0, High: 1
*/
/**************************************************************************/
uint8_t PCA9537_IO2::getPinState(pcaPin_t pin)
{
    return getPin(pin, getPinMode(pin) ? PCA9537_IO2_REG_INPUT : PCA9537_IO2_REG_OUTPUT);
}

/**************************************************************************/
/*
        Gets the Pin Polarity (Input Pins Only)
        Non-Inverted:0, Inverted: 1
*/
/**************************************************************************/
uint8_t PCA9537_IO2::getPinPolarity(pcaPin_t pin)
{
    return getPin(pin, PCA9537_IO2_REG_POLARITY);
}

/**************************************************************************/
/*
        Sets the Pin Mode (IO0 / IO1)
        Mode: Input / Output
*/
/**************************************************************************/
void PCA9537_IO2::setPinMode(pcaPin_t pin, pcaMode_t newMode)
{
    setPin(pin, PCA9537_IO2_REG_CONFIG, newMode);
}

/**************************************************************************/
/*
        Sets the Pin Mode (All Pins)
        Mode: Input / Output
*/
/**************************************************************************/
void PCA9537_IO2::setPinMode(pcaMode_t newMode)
{
    setCommand(PCA9537_IO2_REG_CONFIG, newMode ? PCA9537_IO2_ALL_INPUT : PCA9537_IO2_ALL_OUTPUT);
}

/**************************************************************************/
/*
        Sets the Pin State (Output Pins Only: IO0 / IO1)
        State: Low / High
*/
/**************************************************************************/
void PCA9537_IO2::setPinState(pcaPin_t pin, pcaState_t newState)
{
    setPin(pin, PCA9537_IO2_REG_OUTPUT, newState);
}

/**************************************************************************/
/*
        Sets the Pin State (All Pins)
        State: Low / High
*/
/**************************************************************************/
void PCA9537_IO2::setPinState(pcaState_t newState)
{
    setCommand(PCA9537_IO2_REG_OUTPUT, newState ? PCA9537_IO2_ALL_HIGH : PCA9537_IO2_ALL_LOW);
}

/**************************************************************************/
/*
        Sets the Pin Polarity (Input Pins Only: IO0 / IO1)
        Polarity: Non-Inverted / Inverted
*/
/**************************************************************************/
void PCA9537_IO2::setPinPolarity(pcaPin_t pin, pcaPolarity_t newPolarity)
{
    setPin(pin, PCA9537_IO2_REG_POLARITY, newPolarity);
}

/**************************************************************************/
/*
        Sets the Pin Polarity (All Pins, Input Pins Only)
        Polarity: Non-Inverted / Inverted
*/
/**************************************************************************/
void PCA9537_IO2::setPinPolarity(pcaPolarity_t newPolarity)
{
    uint8_t polarityVals, polarityMask, polarityNew;
    polarityVals = getCommand(PCA9537_IO2_REG_POLARITY);
    polarityMask = getCommand(PCA9537_IO2_REG_CONFIG);
    polarityNew  = newPolarity ? PCA9537_IO2_ALL_INVERTED : PCA9537_IO2_ALL_NON_INVERTED;
    setCommand(PCA9537_IO2_REG_POLARITY, (polarityVals & ~polarityMask) | (polarityNew & polarityMask));
}

/**************************************************************************/
/*
        Gets the Register Data
*/
/**************************************************************************/
uint8_t PCA9537_IO2::getCommand(pcaCommand_t command)
{
    uint8_t regData = 0;
    // Set the Command Mode for the Pin
    writeRegister8(pca_i2cAddress, command);
    
    // Read the Result for the Pin
    regData = (uint8_t)readRegister(pca_i2cAddress);
    return regData;
}

/**************************************************************************/
/*
        Gets the Pin Data
*/
/**************************************************************************/
uint8_t PCA9537_IO2::getPin(pcaPin_t pin, pcaCommand_t command)
{
    return bitRead(getCommand(command), pin);
}

/**************************************************************************/
/*
        Sets the Register Data
*/
/**************************************************************************/
void PCA9537_IO2::setCommand(pcaCommand_t command, uint8_t newSetting)
{
    if (command > 0)
    {
        writeRegister(pca_i2cAddress, (uint8_t)command, (uint8_t)newSetting );
        delay(pca_conversionDelay);
    }
}
/**************************************************************************/
/*
        Sets to Pin Data
*/
/**************************************************************************/
void PCA9537_IO2::setPin(pcaPin_t pin, pcaCommand_t command, uint8_t newSetting)
{
    uint8_t newReg = getCommand(command);
    bitWrite(newReg, pin, newSetting);
    setCommand(command, newReg);
}
