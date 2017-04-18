/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        PCA9536_WDBZ
        This code is designed to work with the PCA9536_WDBZ_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/water-detect-sensor-with-buzzer
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "PCA9536_WDBZ.h"

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
        Instantiates a new PCA9536_WDBZ class with appropriate properties
*/
/**************************************************************************/
void PCA9536_WDBZ::getAddr_PCA9536_WDBZ(uint8_t i2cAddress)
{
    pca_i2cAddress = i2cAddress;
    pca_conversionDelay = PCA9536_WDBZ_CONVERSIONDELAY;
    
}

/**************************************************************************/
/*
        Constructor for Class PCA9536_WDBZ
*/
/**************************************************************************/
PCA9536_WDBZ::PCA9536_WDBZ()
{
    
}

/**************************************************************************/
/*
        Destructor for Class PCA9536_WDBZ
*/
/**************************************************************************/
PCA9536_WDBZ::~PCA9536_WDBZ()
{
    
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
uint8_t PCA9536_WDBZ::begin()
{
    Wire.begin();
}
/**************************************************************************/
/*
        Resets up the Device
*/
/**************************************************************************/
void PCA9536_WDBZ::Reset()
{
    setPinMode(PCA9536_WDBZ_MODE_INPUT);
    setPinState(PCA9536_WDBZ_STATE_HIGH);
    setPinPolarity(PCA9536_WDBZ_STATE_NON_INVERTED);
    writeRegister8(pca_i2cAddress, PCA9536_WDBZ_REG_INPUT);
    delay(pca_conversionDelay);
}

/**************************************************************************/
/*
        Gets the Pin Mode
        Output: 0, Input: 1
*/
/**************************************************************************/
uint8_t PCA9536_WDBZ::getPinMode(pcaPin_t pin)
{
    return getPin(pin, PCA9536_WDBZ_REG_CONFIG);
}

/**************************************************************************/
/*
        Gets the Pin State
        Low: 0, High: 1
*/
/**************************************************************************/
uint8_t PCA9536_WDBZ::getPinState(pcaPin_t pin)
{
    return getPin(pin, getPinMode(pin) ? PCA9536_WDBZ_REG_INPUT : PCA9536_WDBZ_REG_OUTPUT);
}

/**************************************************************************/
/*
        Gets the Pin Polarity (Input Pins Only)
        Non-Inverted:0, Inverted: 1
*/
/**************************************************************************/
uint8_t PCA9536_WDBZ::getPinPolarity(pcaPin_t pin)
{
    return getPin(pin, PCA9536_WDBZ_REG_POLARITY);
}

/**************************************************************************/
/*
        Sets the Pin Mode (IO0 / IO1 / IO2 / IO3)
        Mode: Input / Output
*/
/**************************************************************************/
void PCA9536_WDBZ::setPinMode(pcaPin_t pin, pcaMode_t newMode)
{
    setPin(pin, PCA9536_WDBZ_REG_CONFIG, newMode);
}

/**************************************************************************/
/*
        Sets the Pin Mode (All Pins)
        Mode: Input / Output
*/
/**************************************************************************/
void PCA9536_WDBZ::setPinMode(pcaMode_t newMode)
{
    setCommand(PCA9536_WDBZ_REG_CONFIG, newMode ? PCA9536_WDBZ_ALL_INPUT : PCA9536_WDBZ_ALL_OUTPUT);
}

/**************************************************************************/
/*
        Sets the Pin State (Output Pins Only: IO0 / IO1 / IO2 / IO3)
        State: Low / High
*/
/**************************************************************************/
void PCA9536_WDBZ::setPinState(pcaPin_t pin, pcaState_t newState)
{
    setPin(pin, PCA9536_WDBZ_REG_OUTPUT, newState);
}

/**************************************************************************/
/*
        Sets the Pin State (All Pins)
        State: Low / High
*/
/**************************************************************************/
void PCA9536_WDBZ::setPinState(pcaState_t newState)
{
    setCommand(PCA9536_WDBZ_REG_OUTPUT, newState ? PCA9536_WDBZ_ALL_HIGH : PCA9536_WDBZ_ALL_LOW);
}

/**************************************************************************/
/*
        Sets the Pin Polarity (Input Pins Only: IO0 / IO1 / IO2 / IO3)
        Polarity: Non-Inverted / Inverted
*/
/**************************************************************************/
void PCA9536_WDBZ::setPinPolarity(pcaPin_t pin, pcaPolarity_t newPolarity)
{
    setPin(pin, PCA9536_WDBZ_REG_POLARITY, newPolarity);
}

/**************************************************************************/
/*
        Sets the Pin Polarity (All Pins, Input Pins Only)
        Polarity: Non-Inverted / Inverted
*/
/**************************************************************************/
void PCA9536_WDBZ::setPinPolarity(pcaPolarity_t newPolarity)
{
    uint8_t polarityVals, polarityMask, polarityNew;
    polarityVals = getCommand(PCA9536_WDBZ_REG_POLARITY);
    polarityMask = getCommand(PCA9536_WDBZ_REG_CONFIG);
    polarityNew  = newPolarity ? PCA9536_WDBZ_ALL_INVERTED : PCA9536_WDBZ_ALL_NON_INVERTED;
    setCommand(PCA9536_WDBZ_REG_POLARITY, (polarityVals & ~polarityMask) | (polarityNew & polarityMask));
}

/**************************************************************************/
/*
        Gets the Register Data
*/
/**************************************************************************/
uint8_t PCA9536_WDBZ::getCommand(pcaCommand_t command)
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
uint8_t PCA9536_WDBZ::getPin(pcaPin_t pin, pcaCommand_t command)
{
    return bitRead(getCommand(command), pin);
}

/**************************************************************************/
/*
        Sets the Register Data
*/
/**************************************************************************/
void PCA9536_WDBZ::setCommand(pcaCommand_t command, uint8_t newSetting)
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
void PCA9536_WDBZ::setPin(pcaPin_t pin, pcaCommand_t command, uint8_t newSetting)
{
    uint8_t newReg = getCommand(command);
    bitWrite(newReg, pin, newSetting);
    setCommand(command, newReg);
}
