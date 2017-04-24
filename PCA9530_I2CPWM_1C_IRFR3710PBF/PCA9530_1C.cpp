/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        PCA9530_1C
        This code is designed to work with the PCA9530_I2CPWM_1C_IRFR3710PBF I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "PCA9530_1C.h"

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
        Instantiates a new PCA9530_1C class with appropriate properties
*/
/**************************************************************************/
void PCA9530_1C::getAddr_PCA9530_1C(uint8_t i2cAddress)
{
    pca_i2cAddress = i2cAddress;
    pca_conversionDelay = PCA9530_1C_CONVERSIONDELAY;
    
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool PCA9530_1C::begin()
{
    Wire.begin();
    
    return true;
}

/**************************************************************************/
/*
        Sets the PSC0 used to Program the Period of the PWM Output
        The Period of BLINK0 = (PSC0 + 1) / 152
*/
/**************************************************************************/
void PCA9530_1C::setFreqPSC0(uint8_t period)
{
    writeRegister(pca_i2cAddress, PCA9530_1C_REG_PSC0, period);
    delay(pca_conversionDelay);
}

/**************************************************************************/
/*
        Sets the PWM0 to Determine the Duty Cycle of BLINK0
        The Duty Cycle of BLINK0 = PWM0 / 256
*/
/**************************************************************************/
void PCA9530_1C::setPWM0(uint8_t dutycycle)
{
    writeRegister(pca_i2cAddress, PCA9530_1C_REG_PWM0, dutycycle);
    delay(pca_conversionDelay);
}

/**************************************************************************/
/*
        Sets the PSC1 used to Program the Period of the PWM Output
        The Period of BLINK1 = (PSC1 + 1) / 152
*/
/**************************************************************************/
void PCA9530_1C::setFreqPSC1(uint8_t period)
{
    writeRegister(pca_i2cAddress, PCA9530_1C_REG_PSC1, period);
    delay(pca_conversionDelay);
}

/**************************************************************************/
/*
        Sets the PWM1 to Determine the Duty Cycle of BLINK1
        The Duty Cycle of BLINK1 = PWM1 / 256
 */
/**************************************************************************/
void PCA9530_1C::setPWM1(uint8_t dutycycle)
{
    writeRegister(pca_i2cAddress, PCA9530_1C_REG_PWM1, dutycycle);
    delay(pca_conversionDelay);
}

/**************************************************************************/
/*
        Sets the LS0 LED Select Registers to Determine the Source of the LED Data
        For the particular LED output user have to shift configuration according to the given below data
        LS0-LED selector
        7:6		1111*		Reserved
        3:2		00*			LED1 Selected
        1:0		00*			LED0 Selected
*/
/**************************************************************************/
void PCA9530_1C::setLEDSelector(uint8_t LED1, uint8_t LED0)
{
    writeRegister(pca_i2cAddress, PCA9530_1C_REG_LS0, LED1 << 2 | LED0);
    delay(pca_conversionDelay);
}
