/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        SHT25
        This code is designed to work with the SHT25_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=SHT25_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "SHT25.h"

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
        Writes 8-bits to the destination Register
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
        Writes 8-bits to the specified destination Register
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
        Reads 16-bits from the specified destination Register
*/
/**************************************************************************/
static uint16_t readRegister(uint8_t i2cAddress)
{
    Wire.beginTransmission(i2cAddress);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)2);
    return (uint16_t)((i2cread() << 8) | i2cread());
}

/**************************************************************************/
/*
        Instantiates a new SHT25 class with appropriate properties
*/
/**************************************************************************/
void SHT25::getAddr_SHT25(uint8_t i2cAddress)
{
    si_i2cAddress = i2cAddress;
    si_conversionDelay = SHT25_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool SHT25::begin()
{
    Wire.begin();
    
    // Soft Reset up the Sensor for Temperature and Humidity Measurement
    softReset();
    return true;
    
}

/**************************************************************************/
/*
        Resets up the Hardware
*/
/**************************************************************************/
void SHT25::softReset(void)
{
    // Resets the SHT25 with the RESET command
    writeRegister8(si_i2cAddress, SHT25_CMD_SOFT_RESET);
}

/**************************************************************************/
/*
        Sets the Temperature Measurement Mode
 */
/**************************************************************************/
void SHT25::setTempMode(siTempMode_t tempmode)
{
    si_tempmode = tempmode;
}

/**************************************************************************/
/*
        Gets the Temperature Measurement Mode
 */
/**************************************************************************/
siTempMode_t SHT25::getTempMode()
{
    return si_tempmode;
}

/**************************************************************************/
/*
        Sets the Humidity Measurement Mode
*/
/**************************************************************************/
void SHT25::setHumidityMode(siHumidityMode_t humiditymode)
{
    si_humiditymode = humiditymode;
}

/**************************************************************************/
/*
        Gets the Humidity Measurement Mode
*/
/**************************************************************************/
siHumidityMode_t SHT25::getHumidityMode()
{
    return si_humiditymode;
}

/**************************************************************************/
/*
        Sets the Measurement Resolution
*/
/**************************************************************************/
void SHT25::setResolution(siResolution_t resolution)
{
    si_resolution = resolution;
}

/**************************************************************************/
/*
        Gets the Measurement Resolution
*/
/**************************************************************************/
siResolution_t SHT25::getResolution()
{
    return si_resolution;
}

/**************************************************************************/
/*
        Sets the VDD Status
*/
/**************************************************************************/
void SHT25::setVoltage(siVoltage_t voltage)
{
    si_voltage = voltage;
}

/**************************************************************************/
/*
        Gets the VDD Status
*/
/**************************************************************************/
siVoltage_t SHT25::getVoltage()
{
    return si_voltage;
}

/**************************************************************************/
/*
        Sets the Heater Status
*/
/**************************************************************************/
void SHT25::setHeaterStatus(siHeaterStatus_t heaterstatus)
{
    si_heaterstatus = heaterstatus;
}

/**************************************************************************/
/*
        Gets the Heater Status
*/
/**************************************************************************/
siHeaterStatus_t SHT25::getHeaterStatus()
{
    return si_heaterstatus;
}

/**************************************************************************/
/*
        Sets the OTP Reload
*/
/**************************************************************************/
void SHT25::setOTPStatus(siOTPStatus_t otpstatus)
{
    si_otpstatus = otpstatus;
}

/**************************************************************************/
/*
        Gets the OTP Reload
*/
/**************************************************************************/
siOTPStatus_t SHT25::getOTPStatus()
{
    return si_otpstatus;
}

/**************************************************************************/
/*
        Reads the Results by Sending Temperature Command
        Reads Latest Temperature Value
        Measuring the 16-bit Temperature Register
*/
/**************************************************************************/
float SHT25::Measure_Temperature()
{
    // Set Measurement Resolution
    uint8_t tempControl = si_resolution;
    
    // Set VDD (Voltage Status)
    tempControl |= si_voltage;
    
    // Set On-Chip Heater Status
    tempControl |= si_heaterstatus;
    
    // Set OTP Reload
    tempControl |= si_otpstatus;

    // Sending Command to the User Register 1 for SHT25
    writeRegister(si_i2cAddress, SHT25_CMD_WRITE_USER, tempControl);
    
    // Wait for the conversion to complete
    delay(si_conversionDelay);
    
    // Set the Temperature Measurement Mode
    uint8_t tempMode = si_tempmode;
    
    // Sending Command for Temperature Measurement Mode
    uint16_t rawTemperature;
    float temperature;
    writeRegister8(si_i2cAddress, tempMode);
    delay(si_conversionDelay);
    
    // Read the Conversion Results
    // 16-bit unsigned results for the SHT25
    rawTemperature = readRegister(si_i2cAddress);
    temperature = (172.72 * rawTemperature)/65536.0 - 46.85;
    
    return (float)temperature;
}

/**************************************************************************/
/*
        Reads the Results by Sending Humidity Command
        Reads Latest Humidity Value
        Measuring the 16-bit Humidity Register
 */
/**************************************************************************/
float SHT25::Measure_Humidity()
{
    // Set Measurement Resolution
    uint8_t tempControl = si_resolution;
    
    // Set VDD (Voltage Status)
    tempControl |= si_voltage;
    
    // Set On-Chip Heater Status
    tempControl |= si_heaterstatus;
    
    // Sending command to the User Register 1 for SHT25
    writeRegister(si_i2cAddress, SHT25_CMD_WRITE_USER, tempControl);
    
    // Wait for the conversion to complete
    delay(si_conversionDelay);
    
    // Set the Humidity Measurement Mode
    uint8_t humidityMode = si_humiditymode;
    
    // Sending command for Humidity Measurement Mode
    uint16_t rawHumidity;
    float humidity;
    writeRegister8(si_i2cAddress, humidityMode);
    delay(si_conversionDelay);
    
    // Read the conversion results
    // 16-bit unsigned results for the SHT25
    rawHumidity = readRegister(si_i2cAddress);
    humidity = (125.0 * rawHumidity)/65536.0 - 6;

    return (float)humidity;
}
