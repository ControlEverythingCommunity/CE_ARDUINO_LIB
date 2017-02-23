/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HP206C
        This code is designed to work with the HP206C_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/barometer-and-altimeter-300-1200-mbar?variant=25687698891#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "HP206C.h"

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
        Writes 8-bits to the destination register
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
    Reads 24-bits from the specified destination register
*/
/**************************************************************************/
static uint32_t readRegister(uint8_t i2cAddress, uint8_t reg)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)3);
    return (uint32_t)((int32_t)i2cread() << 16) | ((int32_t)i2cread() << 8) | i2cread();
}

/**************************************************************************/
/*
        Instantiates a new HP206C class with appropriate properties
*/
/**************************************************************************/
void HP206C::getAddr_HP206C(uint8_t i2cAddress)
{
    hp_i2cAddress = i2cAddress;
    hp_conversionDelay = HP206C_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool HP206C::begin()
{
    Wire.begin();
    
    // Reset();
    // delay(hp_conversionDelay);
    
    return true;
}

/**************************************************************************/
/*
        Sets the Soft Reset Command
        The Sevice will Immediately be Reset No Matter What it is Working On
*/
/**************************************************************************/
void HP206C::Reset()
{
    writeRegister(hp_i2cAddress, HP206C_CMD_SOFT_RESET);
    delay(hp_conversionDelay);
}

/**************************************************************************/
/*
        Sets the OSR Command Value to select Decimation Rate of the Internal Digital Filter
*/
/**************************************************************************/
void HP206C::setOSR(hpOSR_t osr)
{
    hp_osr = osr;
}

/**************************************************************************/
/*
        Gets the OSR Command Value to select Decimation Rate of the Internal Digital Filter
 */
/**************************************************************************/
hpOSR_t HP206C::getOSR()
{
    return hp_osr;
}

/**************************************************************************/
/*
        Reads up the Device comprising of a Pressure, Altitude & Temperature Sensor
*/
/**************************************************************************/
void HP206C::Measure_Sensor(void)
{
    // Read all the Sensors
    Measure_Pressure();
    Measure_Altitude();
    Measure_Temperature();
}

/**************************************************************************/
/*
        Reads 20-bits from the destination register
        Reads the results for Digital Pressure Value
*/
/**************************************************************************/
void HP206C::Measure_Pressure()
{
    // Set Up the Configuration for the Pressure Sensor
    uint8_t command =   HP206C_CMD_CONVERT           |  // Convert the Sensor Output to the Digital Values
                        HP206C_CMD_CHNL_PRESTEMP;       // Sensor Pressure and Temperature Channel
    
    command |= hp_osr;       // OSR
    
    // Write the configuration to the Pressure Sensor
    writeRegister(hp_i2cAddress, command);
    
    // Wait for the configuration to complete
    delay(hp_conversionDelay);
    
    // Reads the pressure value
    uint32_t pressure = readRegister(hp_i2cAddress, HP206C_CMD_READ_P);
    hp_sensorData.P = pressure /100.0;
}

/**************************************************************************/
/*
        Reads 20-bits from the destination register
        Reads the results for Digital Altitude Value
*/
/**************************************************************************/
void HP206C::Measure_Altitude()
{
    // Set Up the Configuration for the Altitude Sensor
    uint8_t command =   HP206C_CMD_CONVERT           |  // Convert the Sensor Output to the Digital Values
                        HP206C_CMD_CHNL_PRESTEMP;       // Sensor Pressure and Temperature Channel
    
    command |= hp_osr;       // OSR
    
    // Write the configuration to the Altitude Sensor
    writeRegister(hp_i2cAddress, command);
    
    // Wait for the configuration to complete
    delay(hp_conversionDelay);
    
    // Reads the Altitude value
    uint32_t altitude = readRegister(hp_i2cAddress, HP206C_CMD_READ_A);
    hp_sensorData.A = altitude /100.0;
}

/**************************************************************************/
/*
        Reads 20-bits from the destination register
        Reads the results for Digital Temperature Value
*/
/**************************************************************************/
void HP206C::Measure_Temperature()
{
    // Set Up the Configuration for the Temperature Sensor
    uint8_t command =   HP206C_CMD_CONVERT           |  // Convert the Sensor Output to the Digital Values
                        HP206C_CMD_CHNL_PRESTEMP;       // Sensor Pressure and Temperature Channel
    
    command |= hp_osr;       // OSR
    
    // Write the configuration to the Temperature Sensor
    writeRegister(hp_i2cAddress, command);
    
    // Wait for the configuration to complete
    delay(hp_conversionDelay);
    
    // Reads the Temperature value
    uint32_t temperature = readRegister(hp_i2cAddress, HP206C_CMD_READ_T);
    hp_sensorData.T = temperature /100.0;
}
