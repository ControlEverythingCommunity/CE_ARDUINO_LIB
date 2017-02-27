/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        VCNL4010
        This code is designed to work with the VCNL4010_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Light?sku=VCNL4010_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "VCNL4010.h"

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
        Reads 16-bits from the specified destination register
*/
/**************************************************************************/
static uint16_t readRegister16(uint8_t i2cAddress, uint8_t reg)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)2);
    return (uint16_t)((i2cread() << 8) | i2cread());
}

/**************************************************************************/
/*
        Instantiates a new VCNL4010 class with appropriate properties
*/
/**************************************************************************/
void VCNL4010::getAddr_VCNL4010(uint8_t i2cAddress)
{
    vcnl_i2cAddress = i2cAddress;
    vcnl_conversionDelay = VCNL4010_CONVERSIONDELAY;
    
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool VCNL4010::begin()
{
    Wire.begin();
    
    uint8_t devid = readRegister(vcnl_i2cAddress, VCNL4010_REG_ALSPROX_PRODUCT_ID);
    if (devid != VCNL4010_ALSPROX_PRODUCT_ID)
        return false;
    
    // Set the LEDCurrent, Proximity and Ambient Light Sensor and Interrupt Control Register
    setLEDCurrent(20);
    // Measure_ALS();
    // Measure_Proximity();
    
    writeRegister(vcnl_i2cAddress, VCNL4010_REG_ALSPROX_INT_CTRL, 0x08);
    return true;
}

/**************************************************************************/
/*
        Sets the Proximity IR Test Signal Frequency
*/
/**************************************************************************/
void VCNL4010::setFrequency(vcnlFrequency_t frequency)
{
    uint8_t HZ = readRegister(vcnl_i2cAddress, VCNL4010_REG_ALSPROX_PROX_MOD);
    HZ &= ~0x18;
    HZ |= frequency;
    writeRegister(vcnl_i2cAddress, VCNL4010_REG_ALSPROX_PROX_MOD, HZ);
    
    // Wait for the configuration to complete
    delay(vcnl_conversionDelay);
}


/**************************************************************************/
/*
        Sets the LED current value for proximity measurement.
*/
/**************************************************************************/
void VCNL4010::setLEDCurrent(uint8_t current)
{
    // IR LED current = Value (dec.) x 10 mA
    // Valid Range = 0 to 20d. e.g. 0 = 0 mA , 1 = 10 mA, …., 20 = 200 mA (2 = 20 mA = DEFAULT)
    // LED Current is limited to 200 mA for values higher as 20d
    if (current > 20)
    {
        current = 20;
    }
    writeRegister(vcnl_i2cAddress, VCNL4010_REG_ALSPROX_IR_LED_CURRENT, current);
    
    // Wait for the configuration to complete
    delay(vcnl_conversionDelay);
}

/**************************************************************************/
/*
        Sets Up and Read the Data for the the Ambient Light Sensor
*/
/**************************************************************************/
uint16_t VCNL4010::Measure_ALS()
{
    // Check the Information About the Interrupt Status for Ambient Light
    uint8_t interrupt = readRegister(vcnl_i2cAddress, VCNL4010_REG_ALSPROX_INTR_STATUS);
    interrupt &= ~0x40;
    writeRegister(vcnl_i2cAddress, VCNL4010_REG_ALSPROX_INTR_STATUS, interrupt);
    
    // Set Up the Configuration for the Command Register
    uint8_t command = VCNL4010_REG_ALSPROX_CMD_ALS_OD_START;    // Starts a Single On-Demand Measurement for Ambient Light
    
    // Write the configuration to the Command Register
    writeRegister(vcnl_i2cAddress, VCNL4010_REG_ALSPROX_COMMAND, command);
    
    // Wait for the configuration to complete
    delay(vcnl_conversionDelay);
    
    while (1)
    {
        // Start the Proximity Measurements
        uint8_t ALS = readRegister(vcnl_i2cAddress, VCNL4010_REG_ALSPROX_COMMAND);
        if (ALS & VCNL4010_REG_ALSPROX_CMD_ALS_DATA_RDY_SELECT)
        {
            return readRegister16(vcnl_i2cAddress, VCNL4010_REG_ALSPROX_ALSDATAH);
        }
    }
    
    // Wait for the reading to complete
    delay(vcnl_conversionDelay);
}

/**************************************************************************/
/*
        Sets Up and Reads the Data for the Proximity Sensor
*/
/**************************************************************************/
uint16_t VCNL4010::Measure_Proximity(void)
{
    // Check the Information About the Interrupt Status for Proximity
    uint8_t interrupt = readRegister(vcnl_i2cAddress, VCNL4010_REG_ALSPROX_INTR_STATUS);
    interrupt &= ~0x80;
    writeRegister(vcnl_i2cAddress, VCNL4010_REG_ALSPROX_INTR_STATUS, interrupt);
    
    // Set Up the Configuration for the Command Register
    uint8_t command = VCNL4010_REG_ALSPROX_CMD_PROX_OD_START;   // Starts a Single On-Demand Measurement for Proximity
    
    // Write the configuration to the Command Register
    writeRegister(vcnl_i2cAddress, VCNL4010_REG_ALSPROX_COMMAND, command);
    
    // Wait for the configuration to complete
    delay(vcnl_conversionDelay);
    
    while (1)
    {
        // Start the Proximity Measurements
        uint8_t proximity = readRegister(vcnl_i2cAddress, VCNL4010_REG_ALSPROX_COMMAND);
        if (proximity & VCNL4010_REG_ALSPROX_CMD_PROX_DATA_RDY_SELECT)
        {
            return readRegister16(vcnl_i2cAddress, VCNL4010_REG_ALSPROX_PDATAH);
        }
    }
    
    // Wait for the reading to complete
    delay(vcnl_conversionDelay);
}
