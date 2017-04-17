/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HTU21D
        This code is designed to work with the HTU21D_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/quick-response-humidity-and-temperature-sensor-2-rh-0-3-c
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "HTU21D.h"

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
        Instantiates a new HTU21D class with appropriate properties
*/
/**************************************************************************/
void HTU21D::getAddr_HTU21D(uint8_t i2cAddress)
{
    htu_i2cAddress = i2cAddress;
    htu_conversionDelay = HTU21D_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool HTU21D::begin()
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
void HTU21D::softReset(void)
{
    // Resets the HTU21D with the RESET command
    writeRegister8(htu_i2cAddress, HTU21D_CMD_SOFT_RESET);
}

/**************************************************************************/
/*
        Sets the Temperature Measurement Mode
 */
/**************************************************************************/
void HTU21D::setTempMode(htuTempMode_t tempmode)
{
    htu_tempmode = tempmode;
}

/**************************************************************************/
/*
        Gets the Temperature Measurement Mode
 */
/**************************************************************************/
htuTempMode_t HTU21D::getTempMode()
{
    return htu_tempmode;
}

/**************************************************************************/
/*
        Sets the Humidity Measurement Mode
*/
/**************************************************************************/
void HTU21D::setHumidityMode(htuHumidityMode_t humiditymode)
{
    htu_humiditymode = humiditymode;
}

/**************************************************************************/
/*
        Gets the Humidity Measurement Mode
*/
/**************************************************************************/
htuHumidityMode_t HTU21D::getHumidityMode()
{
    return htu_humiditymode;
}

/**************************************************************************/
/*
        Sets the Measurement Resolution
*/
/**************************************************************************/
void HTU21D::setResolution(htuResolution_t resolution)
{
    htu_resolution = resolution;
}

/**************************************************************************/
/*
        Gets the Measurement Resolution
*/
/**************************************************************************/
htuResolution_t HTU21D::getResolution()
{
    return htu_resolution;
}

/**************************************************************************/
/*
        Sets the VDD Status
*/
/**************************************************************************/
void HTU21D::setVoltage(htuVoltage_t voltage)
{
    htu_voltage = voltage;
}

/**************************************************************************/
/*
        Gets the VDD Status
*/
/**************************************************************************/
htuVoltage_t HTU21D::getVoltage()
{
    return htu_voltage;
}

/**************************************************************************/
/*
        Sets the Heater Status
*/
/**************************************************************************/
void HTU21D::setHeaterStatus(htuHeaterStatus_t heaterstatus)
{
    htu_heaterstatus = heaterstatus;
}

/**************************************************************************/
/*
        Gets the Heater Status
*/
/**************************************************************************/
htuHeaterStatus_t HTU21D::getHeaterStatus()
{
    return htu_heaterstatus;
}

/**************************************************************************/
/*
        Sets the OTP Reload
*/
/**************************************************************************/
void HTU21D::setOTPStatus(htuOTPStatus_t otpstatus)
{
    htu_otpstatus = otpstatus;
}

/**************************************************************************/
/*
        Gets the OTP Reload
*/
/**************************************************************************/
htuOTPStatus_t HTU21D::getOTPStatus()
{
    return htu_otpstatus;
}

/**************************************************************************/
/*
        Reads the Results by Sending Temperature Command
        Reads Latest Temperature Value
        Measuring the 16-bit Temperature Register
*/
/**************************************************************************/
float HTU21D::Measure_Temperature()
{
    // Set Measurement Resolution
    uint8_t tempControl = htu_resolution;
    
    // Set VDD (Voltage Status)
    tempControl |= htu_voltage;
    
    // Set On-Chip Heater Status
    tempControl |= htu_heaterstatus;
    
    // Set OTP Reload
    tempControl |= htu_otpstatus;

    // Sending Command to the User Register 1 for HTU21D
    writeRegister(htu_i2cAddress, HTU21D_CMD_WRITE_USER, tempControl);
    
    // Wait for the conversion to complete
    delay(htu_conversionDelay);
    
    // Set the Temperature Measurement Mode
    uint8_t tempMode = htu_tempmode;
    
    // Sending Command for Temperature Measurement Mode
    uint16_t rawTemperature;
    float temperature;
    writeRegister8(htu_i2cAddress, tempMode);
    delay(htu_conversionDelay);
    
    // Read the Conversion Results
    // 16-bit unsigned results for the HTU21D
    rawTemperature = readRegister(htu_i2cAddress);
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
float HTU21D::Measure_Humidity()
{
    // Set Measurement Resolution
    uint8_t tempControl = htu_resolution;
    
    // Set VDD (Voltage Status)
    tempControl |= htu_voltage;
    
    // Set On-Chip Heater Status
    tempControl |= htu_heaterstatus;
    
    // Sending command to the User Register 1 for HTU21D
    writeRegister(htu_i2cAddress, HTU21D_CMD_WRITE_USER, tempControl);
    
    // Wait for the conversion to complete
    delay(htu_conversionDelay);
    
    // Set the Humidity Measurement Mode
    uint8_t humidityMode = htu_humiditymode;
    
    // Sending command for Humidity Measurement Mode
    uint16_t rawHumidity;
    float humidity;
    writeRegister8(htu_i2cAddress, humidityMode);
    delay(htu_conversionDelay);
    
    // Read the conversion results
    // 16-bit unsigned results for the HTU21D
    rawHumidity = readRegister(htu_i2cAddress);
    humidity = (125.0 * rawHumidity)/65536.0 - 6;

    return (float)humidity;
}
