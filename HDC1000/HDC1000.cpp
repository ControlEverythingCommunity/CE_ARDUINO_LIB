/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HDC1000
        This code is designed to work with the HDC1000_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=HDC1000_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "HDC1000.h"

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
        Writes 16-bits to the specified destination register
*/
/**************************************************************************/
static void writeRegister(uint8_t i2cAddress, uint8_t reg, uint16_t value)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    i2cwrite((uint8_t)(value>>8));
    i2cwrite((uint8_t)(value & 0xFF));
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
        Instantiates a new HDC1000 class with appropriate properties
*/
/**************************************************************************/
void HDC1000::getAddr_HDC1000(uint8_t i2cAddress)
{
    hdc_i2cAddress = i2cAddress;
    hdc_conversionDelay = HDC1000_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool HDC1000::begin()
{
    Wire.begin();
    
    uint8_t manufid = readRegister(hdc_i2cAddress, HDC1000_REG_MANUF_ID);
    if (manufid != HDC1000_MANUF_ID)
        return false;
    
    uint8_t devid = readRegister(hdc_i2cAddress, HDC1000_REG_DEVICE_ID);
    if (devid != HDC1000_DEV_ID)
        return false;
    
    // Set the Configuration for the HDC1000
    // setConfig();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Software Reset Mode
 */
/**************************************************************************/
void HDC1000::setReset(hdcReset_t reset)
{
    hdc_reset = reset;
}

/**************************************************************************/
/*
        Gets the Software Reset Mode
 */
/**************************************************************************/
hdcReset_t HDC1000::getReset()
{
    return hdc_reset;
}

/**************************************************************************/
/*
        Sets the Heater Status
*/
/**************************************************************************/
void HDC1000::setHeaterStatus(hdcHeaterStatus_t heaterstatus)
{
    hdc_heaterstatus = heaterstatus;
}
    
/**************************************************************************/
/*
        Gets the Heater Status
*/
/**************************************************************************/
hdcHeaterStatus_t HDC1000::getHeaterStatus()
{
    return hdc_heaterstatus;
}

/**************************************************************************/
/*
        Sets the Mode of Acquisition
*/
/**************************************************************************/
void HDC1000::setMode(hdcMode_t mode)
{
    hdc_mode = mode;
}

/**************************************************************************/
/*
        Gets the Mode of Acquisition
*/
/**************************************************************************/
hdcMode_t HDC1000::getMode()
{
    return hdc_mode;
}

/**************************************************************************/
/*
        Sets the Battery Status
*/
/**************************************************************************/
void HDC1000::setVoltage(hdcVoltage_t voltage)
{
    hdc_voltage = voltage;
}
    
/**************************************************************************/
/*
        Gets the Battery Status
*/
/**************************************************************************/
hdcVoltage_t HDC1000::getVoltage()
{
    return hdc_voltage;
}
    
/**************************************************************************/
/*
        Sets the Temperature Measurement Resolution
*/
/**************************************************************************/
void HDC1000::setTempResolution(hdcTempResolution_t tempresolution)
{
    hdc_tempresolution = tempresolution;
}

/**************************************************************************/
/*
        Gets the Temperature Measurement Resolution
*/
/**************************************************************************/
hdcTempResolution_t HDC1000::getTempResolution()
{
    return hdc_tempresolution;
}

/**************************************************************************/
/*
        Sets the Humidity Measurement Resolution
*/
/**************************************************************************/
void HDC1000::setHumResolution(hdcHumResolution_t humresolution)
{
    hdc_humresolution = humresolution;
}

/**************************************************************************/
/*
        Gets the Humidity Measurement Resolution
*/
/**************************************************************************/
hdcHumResolution_t HDC1000::getHumResolution()
{
    return hdc_humresolution;
}

/**************************************************************************/
/*
        Sets the Configuration for Device Functionality and Returns Status
*/
/**************************************************************************/
void HDC1000::setConfig()
{
    // Set Software Reset
    uint16_t config = hdc_reset;
    
    // Set Heater Status
    config |= hdc_heaterstatus;

    // Set Mode of Acquisition
    config |= hdc_mode;
    
    // Set Battery Voltage
    config |= hdc_voltage;
     
    // Set Temperature Measurement Resolution
    config |= hdc_tempresolution;
     
    // Set Humidity Measurement Resolution
    config |= hdc_humresolution;
    
    // Write the configuration to the Configuration Register
    writeRegister(hdc_i2cAddress, HDC1000_REG_CONFIG, config);
    
    // Wait for the conversion to complete
    delay(hdc_conversionDelay);
    
}

/**************************************************************************/
/*
        Reads the results by sending temperature command
        Reads latest Temperature Value
        Measuring the 16-bit temperature register
 */
/**************************************************************************/
void HDC1000::Measure_Temperature()
{
    // Read the conversion results
    // 16-bit unsigned results for the HDC1000
    uint16_t rawTemperature;
    rawTemperature = readRegister(hdc_i2cAddress, HDC1000_REG_TEMP);
    hdc_sensorData.T = (float)((165.0 * rawTemperature) / 65536 - 40.0);
    
}

/**************************************************************************/
/*
        Reads the results by sending humidity command
        Reads latest Humidity Value
        Measuring the 16-bit humidity register
 */
/**************************************************************************/
void HDC1000::Measure_Humidity()
{
    // Read the conversion results
    // 16-bit unsigned results for the HDC1000
    uint16_t rawHumidity;
    rawHumidity = readRegister(hdc_i2cAddress, HDC1000_REG_HUMIDITY);
    hdc_sensorData.H = (float)((100.0 * rawHumidity) / 65536.0);
    
}
