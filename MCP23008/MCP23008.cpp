/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MCP23008
        This code is designed to work with the MCP23008_I2CR8G5LE_10A_D I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/8-channel-relay-controller-i2c#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "MCP23008.h"

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
static uint8_t writeRegister(uint8_t i2cAddress, uint8_t reg, uint8_t value)
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
    return (int8_t)(i2cread());
}

/**************************************************************************/
/*
        Instantiates a new MCP23008 class with appropriate properties
*/
/**************************************************************************/
void MCP23008::getAddr_MCP23008(uint8_t i2cAddress)
{
    mcp_i2cAddress = i2cAddress;
    mcp_conversionDelay = MCP23008_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void MCP23008::begin(void)
{
    Wire.begin();
}

/**************************************************************************/
/*
        Set the Controls the direction of the data I/O
*/
/**************************************************************************/
void MCP23008::pinMode(uint8_t pin, uint8_t mode)
{
    uint8_t iodir;
    
    // 8 Bits/Pin
    if (pin > 7)
    {
        //return;
    }
    
    // Reads the most recent result of the I/O Direction Register
    iodir = readRegister(mcp_i2cAddress, MCP23008_REG_IODIR);
    
    // Set the Pin and Mode of the Corresponding Pin
    if (mode == INPUT)
    {
        iodir |= 1 << pin;
    }
    else if (mode == OUTPUT)
    {
        iodir &= ~(1 << pin);
    }
    
    // Write the new configuration to the I/O Direction (IODIR) Register
    writeRegister(mcp_i2cAddress, MCP23008_REG_IODIR, iodir);
    
    // Wait for the configuration to complete
    delay(mcp_conversionDelay);
    
}

/**************************************************************************/
/*
        Set the Controls of Pull-Up Resistors for the Port Pins
*/
/**************************************************************************/
void MCP23008::pullUpRegister(uint8_t pin, uint8_t mode)
{
    uint8_t gppu;
    
    // 8 Bits/Pins
    if (pin > 7)
    {
        //return;
    }
    
    // Reads the most recent result of the Pull-Up Resistor Configuration (GPPU) Register
    gppu = readRegister(mcp_i2cAddress, MCP23008_REG_GPPU);
    
    // Set the Pin and Mode of the Corresponding Pin
    if (mode == HIGH)
    {
        gppu |= 1 << pin;
    }
    else if (mode == LOW)
    {
        gppu &= ~(1 << pin);
    }
    
    // Write the new configuration to the Pull-Up Resistor Configuration (GPPU) Register
    writeRegister(mcp_i2cAddress, MCP23008_REG_GPPU, gppu);
    
    // Wait for the configuration to complete
    delay(mcp_conversionDelay);
}

/**************************************************************************/
/*
        Gets the status of the Port (GPIO) Register
*/
/**************************************************************************/
uint8_t MCP23008::readGPIO(void)
{
    // Reads the most recent result of the Port (GPIO) Register
    return readRegister(mcp_i2cAddress, MCP23008_REG_GPIO);
}

/**************************************************************************/
/*
        Sets the status of the Port (GPIO) Register
*/
/**************************************************************************/
void MCP23008::writeGPIO(uint8_t gpio)
{
    // Write the new configuration to the Port (GPIO) Register
    writeRegister(mcp_i2cAddress, MCP23008_REG_GPIO, gpio);
}

/**************************************************************************/
/*
        Gets the Value of the Port (GPIO) Register
*/
/**************************************************************************/
uint8_t MCP23008::digitalRead(uint8_t pin)
{
    // 8 Bits/Pins
    if (pin > 7)
    {
        //return;
    }
    
    // Reads the Current Value of the Port (GPIO) Register
    return (readGPIO() >> pin) & 0x1;
}

/**************************************************************************/
/*
        Sets the Value of the Port (GPIO) Register
*/
/**************************************************************************/
void MCP23008::digitalWrite(uint8_t pin, uint8_t mode)
{
    uint8_t gpio;
    
    // 8 Bits/Pins
    if (pin > 7)
    {
        //return;
    }
    
    // Reads the Current Value of the Port (GPIO) Register
    gpio = readGPIO();
    
    // Set the Pin and Mode of the Corresponding Pin
    if (mode == HIGH)
    {
        gpio |= 1 << pin;
    }
    else if (mode == LOW)
    {
        gpio &= ~(1 << pin);
    }
    
    
    // Write the new configuration to the Port (GPIO) Register
    writeGPIO(gpio);
    
    // Wait for the configuration to complete
    delay(mcp_conversionDelay);
}
