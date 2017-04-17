/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        PCA9531
        This code is designed to work with the PCA9531_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/2-channel-8-bit-pwm-with-8-outputs-gpio-i2c-led-dimmer
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "PCA9531.h"

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
        Instantiates a new PCA9531 class with appropriate properties
*/
/**************************************************************************/
void PCA9531::getAddr_PCA9531(uint8_t i2cAddress)
{
    pca_i2cAddress = i2cAddress;
    pca_conversionDelay = PCA9531_CONVERSIONDELAY;
    
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool PCA9531::begin()
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
void PCA9531::setFreqPSC0(uint8_t period)
{
    writeRegister(pca_i2cAddress, PCA9531_REG_PSC0, period);
    delay(pca_conversionDelay);
}

/**************************************************************************/
/*
        Sets the PWM0 to Determine the Duty Cycle of BLINK0
        The Duty Cycle of BLINK0 = PWM0 / 256
*/
/**************************************************************************/
void PCA9531::setPWM0(uint8_t dutycycle)
{
    writeRegister(pca_i2cAddress, PCA9531_REG_PWM0, dutycycle);
    delay(pca_conversionDelay);
}

/**************************************************************************/
/*
        Sets the PSC1 used to Program the Period of the PWM Output
        The Period of BLINK1 = (PSC1 + 1) / 152
*/
/**************************************************************************/
void PCA9531::setFreqPSC1(uint8_t period)
{
    writeRegister(pca_i2cAddress, PCA9531_REG_PSC1, period);
    delay(pca_conversionDelay);
}

/**************************************************************************/
/*
        Sets the PWM1 to Determine the Duty Cycle of BLINK1
        The Duty Cycle of BLINK1 = PWM1 / 256
 */
/**************************************************************************/
void PCA9531::setPWM1(uint8_t dutycycle)
{
    writeRegister(pca_i2cAddress, PCA9531_REG_PWM1, dutycycle);
    delay(pca_conversionDelay);
}

/**************************************************************************/
/*
        Sets the LS0 LED Select Registers to Determine the Source of the LED Data
        For the particular LED output user have to shift configuration according to the given below data
        LS0-LED0 to LED3 selector
        7:6		00*			LED3 Selected
        5:4		00*			LED2 Selected
        3:2		00*			LED1 Selected
        1:0		00*			LED0 Selected
*/
/**************************************************************************/
void PCA9531::setLEDSelector0(uint8_t LED3, uint8_t LED2, uint8_t LED1, uint8_t LED0)
{
    writeRegister(pca_i2cAddress, PCA9531_REG_LS0, LED3 << 6 | LED2 << 4 | LED1 << 2 | LED0);
    delay(pca_conversionDelay);
}

/**************************************************************************/
/*
        Sets the LS1 LED Select Registers to Determine the Source of the LED Data
        Sets the LS1 LED Select Registers to Determine the Source of the LED Data
        For the particular LED output user have to shift configuration according to the given below data
        LS1-LED4 to LED7 selector
        7:6		00*			LED7 Selected
        5:4		00*			LED6 Selected
        3:2		00*			LED5 Selected
        1:0		00*			LED4 Selected
*/
/**************************************************************************/
void PCA9531::setLEDSelector1(uint8_t LED7, uint8_t LED6, uint8_t LED5, uint8_t LED4)
{
    writeRegister(pca_i2cAddress, PCA9531_REG_LS0, LED7 << 6 | LED6 << 4 | LED5 << 2 | LED4);
    delay(pca_conversionDelay);
}
