/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        STTS751
        This code is designed to work with the STTS751_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "STTS751.h"

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
    i2cwrite((uint8_t)value);
    Wire.endTransmission();
}

/**************************************************************************/
/*
        Reads 8-bits from the specified destination register
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
        Instantiates a new STTS751 class with appropriate properties
*/
/**************************************************************************/
void STTS751::getAddr_STTS751(uint8_t i2cAddress)
{
    stts_i2cAddress = i2cAddress;
    stts_conversionDelay = STTS751_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
boolean STTS751::begin()
{
    Wire.begin();
    
    if (readRegister(stts_i2cAddress, STTS751_REG_PRODUCT_ID)   != 0x00) return false;
    if (readRegister(stts_i2cAddress, STTS751_REG_MANUF_ID)     != 0x53) return false;
    if (readRegister(stts_i2cAddress, STTS751_REG_REVISION_NO)  != 0x01) return false;
    
    return true;
}

/**************************************************************************/
/*
        Sets the MASK1
        Asserts the EVENT pin
*/
/**************************************************************************/
void STTS751::setMask1(sttsMask1_t mask1)
{
    stts_mask1 = mask1;
}

/**************************************************************************/
/*
        Gets the MASK1
*/
/**************************************************************************/
sttsMask1_t STTS751::getMask1()
{
    return stts_mask1;
}

/**************************************************************************/
/*
        Sets the RUN/STOP
        Controls temperature conversions by the ADC
*/
/**************************************************************************/
void STTS751::setMode(sttsMode_t mode)
{
    stts_mode = mode;
}

/**************************************************************************/
/*
        Gets the RUN/STOP
*/
/**************************************************************************/
sttsMode_t STTS751::getMode()
{
    return stts_mode;
}

/**************************************************************************/
/*
        Sets the Conversion Resolution
*/
/**************************************************************************/
void STTS751::setTres(sttsTres_t tres)
{
    stts_tres = tres;
}

/**************************************************************************/
/*
        Gets the Coversion Resolution
*/
/**************************************************************************/
sttsTres_t STTS751::getTres()
{
    return stts_tres;
}

/**************************************************************************/
/*
        Sets the Conversion Rate/Conversions Per Second
*/
/**************************************************************************/
void STTS751::setConvRate(sttsConvRate_t convrate)
{
    stts_convrate = convrate;
}

/**************************************************************************/
/*
        Gets the Conversion Rate/Conversions Per Second
*/
/**************************************************************************/
sttsConvRate_t STTS751::getConvRate()
{
    return stts_convrate;
}

/**************************************************************************/
/*
        Reads the results, measuring the 16-bit temperature register
*/
/**************************************************************************/
int16_t STTS751::Measure_Temp()
{
    // Set MASK1
    uint8_t config = stts_mask1;

    // Set RUN/STOP
    config |= stts_mode;
    
    // Set Conversion Resolution
    config |= stts_tres;

    // Write config register to the stts
    writeRegister(stts_i2cAddress, STTS751_REG_CONFIG, config);

    // Wait for the conversion to complete
    delay(stts_conversionDelay);
    
    // Set the Conversion Rate
    uint8_t rate = stts_convrate;
    
    // Write conversion rate register to the stts
    writeRegister(stts_i2cAddress, STTS751_REG_CONV_RATE, rate);

    // Wait for the conversion to complete
    delay(stts_conversionDelay);
    
    // Read the conversion results
    // Reading temperature high byte data (MSB)
    uint8_t raw_MSB = readRegister(stts_i2cAddress, STTS751_REG_TEMP_HIGH);
    // Reading temperature low byte data (LSB)
    uint8_t raw_LSB = readRegister(stts_i2cAddress, STTS751_REG_TEMP_LOW);
    // 16-bit unsigned results for the STTS751
    uint16_t raw_temp = raw_MSB << 8 | raw_LSB;
    return (int16_t)raw_temp;
}
