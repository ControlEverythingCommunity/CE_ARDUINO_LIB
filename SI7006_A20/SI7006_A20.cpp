/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        SI7006-A20
        This code is designed to work with the SI7006-A20_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=SI7006-A20_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "SI7006_A20.h"

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
static void writeRegister8(uint8_t i2cAddress, uint8_t reg)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    Wire.endTransmission();
    
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
        Reads 16-bits from the destination register
*/
/**************************************************************************/
static uint16_t readRegister16(uint8_t i2cAddress)
{
    Wire.beginTransmission(i2cAddress);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)2);
    return (uint16_t)((i2cread() << 8) | i2cread());
}

/**************************************************************************/
/*
        Reads 16-bits from the specified destination register
*/
/**************************************************************************/
static uint16_t readRegister(uint8_t i2cAddress, uint8_t reg)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)2);
    return (uint16_t)((i2cread() << 8) | i2cread());
}

/**************************************************************************/
/*
        Instantiates a new SI7006_A20 class with appropriate properties
*/
/**************************************************************************/
void SI7006_A20::getAddr_SI7006_A20(uint8_t i2cAddress)
{
    si_i2cAddress = i2cAddress;
    si_conversionDelay = SI7006_A20_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void SI7006_A20::begin()
{
    Wire.begin();
}

/**************************************************************************/
/*
        Resets up the Hardware
*/
/**************************************************************************/
void SI7006_A20::Reset(void)
{
    // Resets the SI7006_A20 with the RESET command
    writeRegister8(si_i2cAddress, SI7006_A20_CMD_RESET);
}

/**************************************************************************/
/*
        Sets the Temperature Measurement Mode
 */
/**************************************************************************/
void SI7006_A20::setTempMode(siTempMode_t tempmode)
{
    si_tempmode = tempmode;
}

/**************************************************************************/
/*
        Gets the Temperature Measurement Mode
 */
/**************************************************************************/
siTempMode_t SI7006_A20::getTempMode()
{
    return si_tempmode;
}

/**************************************************************************/
/*
        Sets the Humidity Measurement Mode
*/
/**************************************************************************/
void SI7006_A20::setHumidityMode(siHumidityMode_t humiditymode)
{
    si_humiditymode = humiditymode;
}

/**************************************************************************/
/*
        Gets the Humidity Measurement Mode
*/
/**************************************************************************/
siHumidityMode_t SI7006_A20::getHumidityMode()
{
    return si_humiditymode;
}

/**************************************************************************/
/*
        Sets the Measurement Resolution
*/
/**************************************************************************/
void SI7006_A20::setResolution(siResolution_t resolution)
{
    si_resolution = resolution;
}

/**************************************************************************/
/*
        Gets the Measurement Resolution
*/
/**************************************************************************/
siResolution_t SI7006_A20::getResolution()
{
    return si_resolution;
}

/**************************************************************************/
/*
        Sets the VDD Status
*/
/**************************************************************************/
void SI7006_A20::setVoltage(siVoltage_t voltage)
{
    si_voltage = voltage;
}

/**************************************************************************/
/*
        Gets the VDD Status
*/
/**************************************************************************/
siVoltage_t SI7006_A20::getVoltage()
{
    return si_voltage;
}

/**************************************************************************/
/*
        Sets the Heater Status
*/
/**************************************************************************/
void SI7006_A20::setHeaterStatus(siHeaterStatus_t heaterstatus)
{
    si_heaterstatus = heaterstatus;
}

/**************************************************************************/
/*
        Gets the Heater Status
*/
/**************************************************************************/
siHeaterStatus_t SI7006_A20::getHeaterStatus()
{
    return si_heaterstatus;
}

/**************************************************************************/
/*
        Sets the Heater Current
*/
/**************************************************************************/
void SI7006_A20::setHeaterCurrent(siHeaterCurrent_t heatercurrent)
{
    si_heatercurrent = heatercurrent;
}

/**************************************************************************/
/*
        Gets the Heater Current
*/
/**************************************************************************/
siHeaterCurrent_t SI7006_A20::getHeaterCurrent()
{
    return si_heatercurrent;
}

/**************************************************************************/
/*
        Reads the results by sending temperature command
        Reads latest Temperature Value
        Measuring the 16-bit temperature register
*/
/**************************************************************************/
float SI7006_A20::Measure_Temperature()
{
    // Set Measurement Resolution
    uint8_t tempControl = si_resolution;
    
    // Set VDD (Voltage Status)
    tempControl |= si_voltage;
    
    // Set On-Chip Heater Status
    tempControl |= si_heaterstatus;

    // Sending command to the User register 1 for SI7006_A20
    writeRegister(si_i2cAddress, SI7006_A20_CMD_WRITE_USER_1, tempControl);

    // Wait for the conversion to complete
    delay(si_conversionDelay);
    
    // Set the Heater Current
    uint8_t heaterControl = si_heatercurrent;
    
    // Sending command to the Heater Control Register 3 for SI7006_A20
    writeRegister(si_i2cAddress, SI7006_A20_CMD_READ_HEATER_CTRL, heaterControl);
    
    // Wait for the conversion to complete
    delay(si_conversionDelay);
    
    // Set the Temperature Measurement Mode
    uint8_t tempMode = si_tempmode;
    
    // Sending command for Temperature Measurement Mode
    // Read the conversion results
    // 16-bit unsigned results for the SI7006_A20
    uint16_t rawTemperature;
    float temperature;
    writeRegister8(si_i2cAddress, tempMode);
    delay(si_conversionDelay);
    rawTemperature = readRegister16(si_i2cAddress);
    temperature = (172.72 * rawTemperature)/65536.0 - 46.85;
    
    return (float)temperature;
}

/**************************************************************************/
/*
        Reads the results by sending temperature command
        Reads the Temperature Value from Previous RH Measurement
        Measuring the 16-bit temperature register
 */
/**************************************************************************/
float SI7006_A20::Measure_OldTemperature()
{
    // Set Measurement Resolution
    uint8_t tempControl = si_resolution;
    
    // Set VDD (Voltage Status)
    tempControl |= si_voltage;
    
    // Set On-Chip Heater Status
    tempControl |= si_heaterstatus;
    
    // Sending command to the User register 1 for SI7006_A20
    writeRegister(si_i2cAddress, SI7006_A20_CMD_WRITE_USER_1, tempControl);
    
    // Wait for the conversion to complete
    delay(si_conversionDelay);
    
    // Set the Heater Current
    uint8_t heaterControl = si_heatercurrent;
    
    // Sending command to the Heater Control Register 3 for SI7006_A20
    writeRegister(si_i2cAddress, SI7006_A20_CMD_READ_HEATER_CTRL, heaterControl);
    
    // Wait for the conversion to complete
    delay(si_conversionDelay);
    
    // Sending command for Temperature Value from Previous RH Measurement
    // Read the conversion results
    // 16-bit unsigned results for the SI7006_A20
    uint16_t rawTemperature;
    float temperature;
    writeRegister8(si_i2cAddress, SI7006_A20_CMD_TEMP_PREV_RH);
    delay(si_conversionDelay);
    rawTemperature = readRegister16(si_i2cAddress);
    temperature = (172.72 * rawTemperature)/65536 - 46.85;
    
    return (float)temperature;
}

/**************************************************************************/
/*
        Reads the results by sending humidity command
        Reads latest Humidity Value
        Measuring the 16-bit humidity register
 */
/**************************************************************************/
float SI7006_A20::Measure_Humidity()
{
    // Set Measurement Resolution
    uint8_t tempControl = si_resolution;
    
    // Set VDD (Voltage Status)
    tempControl |= si_voltage;
    
    // Set On-Chip Heater Status
    tempControl |= si_heaterstatus;
    
    // Sending command to the User register 1 for SI7006_A20
    writeRegister(si_i2cAddress, SI7006_A20_CMD_WRITE_USER_1, tempControl);
    
    // Wait for the conversion to complete
    delay(si_conversionDelay);
    
    // Set the Heater Current
    uint8_t heaterControl = si_heatercurrent;
    
    // Sending command to the Heater Control Register 3 for SI7006_A20
    writeRegister(si_i2cAddress, SI7006_A20_CMD_READ_HEATER_CTRL, heaterControl);
    
    // Wait for the conversion to complete
    delay(si_conversionDelay);
    
    // Set the Humidity Measurement Mode
    uint8_t humidityMode = si_humiditymode;
    
    // Sending command for Humidity Measurement Mode
    // Read the conversion results
    // 16-bit unsigned results for the SI7006_A20
    uint16_t rawHumidity;
    float humidity;
    writeRegister8(si_i2cAddress, humidityMode);
    delay(si_conversionDelay);
    rawHumidity = readRegister16(si_i2cAddress);
    humidity = (125.0 * rawHumidity)/65536.0 - 6;

    return (float)humidity;
}
