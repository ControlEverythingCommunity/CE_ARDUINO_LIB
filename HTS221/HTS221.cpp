/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HTS221
        This code is designed to work with the HTS221_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Sensor?sku=HTS221_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "HTS221.h"

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
        Instantiates a new HTS221 class with appropriate properties
*/
/**************************************************************************/
void HTS221::getAddr_HTS221(uint8_t i2cAddress)
{
    hts_i2cAddress = i2cAddress;
    hts_conversionDelay = hts_conversionDelay;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool HTS221::begin()
{
    Wire.begin();
    
    uint8_t Pdevid = readRegister(hts_i2cAddress, HTS221_REG_HUMTEMP_WHO_AM_I);
    if (Pdevid != HTS221_HUMTEMP_DEV_ID)
        return false;
    
    // Set up the sensor for Relative Humidity and Temperature
    // setUpSensor();
    
    // Read the Calibration Coefficients
    readCoefficients();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Temperature Average Configuration
*/
/**************************************************************************/
void HTS221::setTempAverage(htsTempAverage_t tempaverage)
{
    hts_tempaverage = tempaverage;
}

/**************************************************************************/
/*
        Gets the Temperature Average Configuration
*/
/**************************************************************************/
htsTempAverage_t HTS221::getTempAverage()
{
    return hts_tempaverage;
}

/**************************************************************************/
/*
        Sets the Humidity Average Configuration
*/
/**************************************************************************/
void HTS221::setPresAverage(htsPresAverage_t presaverage)
{
    hts_presaverage = presaverage;
}

/**************************************************************************/
/*
        Gets the Humidity Average Configuration
*/
/**************************************************************************/
htsPresAverage_t HTS221::getPresAverage()
{
    return hts_presaverage;
}

/**************************************************************************/
/*
        Sets the Power-Down Mode Control
*/
/**************************************************************************/
void HTS221::setMode(htsMode_t mode)
{
    hts_mode = mode;
}

/**************************************************************************/
/*
        Gets the Power-Down Mode Control
*/
/**************************************************************************/
htsMode_t HTS221::getMode()
{
    return hts_mode;
}

/**************************************************************************/
/*
        Sets the Block Data Update
*/
/**************************************************************************/
void HTS221::setBlockData(htsBlockData_t blockdata)
{
    hts_blockdata = blockdata;
}

/**************************************************************************/
/*
        Gets the Block Data Update
*/
/**************************************************************************/
htsBlockData_t HTS221::getBlockData()
{
    return hts_blockdata;
}

/**************************************************************************/
/*
        Sets the Output Data Rate Configuration for the Humidity and Temperature Sensor
*/
/**************************************************************************/
void HTS221::setDataRate(htsDataRate_t datarate)
{
    hts_datarate = datarate;
}

/**************************************************************************/
/*
        Gets the Output Data Rate Configuration for the Humidity and Temperature Sensor
*/
/**************************************************************************/
htsDataRate_t HTS221::getDataRate()
{
    return hts_datarate;
}

/**************************************************************************/
/*
        Sets the Reboot Memory Content
*/
/**************************************************************************/
void HTS221::setBoot(htsBoot_t boot)
{
    hts_boot = boot;
}

/**************************************************************************/
/*
        Gets the Reboot Memory Content
*/
/**************************************************************************/
htsBoot_t HTS221::getBoot()
{
    return hts_boot;
}

/**************************************************************************/
/*
        Sets the Heater
*/
/**************************************************************************/
void HTS221::setHeater(htsHeater_t heater)
{
    hts_heater = heater;
}

/**************************************************************************/
/*
        Gets the Heater
*/
/**************************************************************************/
htsHeater_t HTS221::getHeater()
{
    return hts_heater;
}

/**************************************************************************/
/*
        Sets the One-Shot Enable
*/
/**************************************************************************/
void HTS221::setOneShot(htsOneShot_t oneshot)
{
    hts_oneshot = oneshot;
}

/**************************************************************************/
/*
        Gets the One-Shot Enable
*/
/**************************************************************************/
htsOneShot_t HTS221::getOneShot()
{
    return hts_oneshot;
}

/**************************************************************************/
/*
        Sets up the Relative Humidity and Temperature Sensor
*/
/**************************************************************************/
void HTS221::setUpSensor(void)
{
    // Set Up the Configuration for the Relative Humidity and Temperature Resolution Mode Register
    /*
     // Set the Temperature Average Configuration
     uint8_t average = hts_tempaverage;
    
     // Set the Relative Humidity Average Configuration
     average |= hts_presaverage;
    */
    
    uint8_t average =   HTS221_REG_HUMTEMP_RES_CONF_AVGT_2    |   // Temperature (AVGT): 2
                        HTS221_REG_HUMTEMP_RES_CONF_AVGH_4;       // Humidity (AVGH): 4
    
    // Write the configuration to the Relative Humidity and Temperature Resolution Mode Register
    writeRegister(hts_i2cAddress, HTS221_REG_HUMTEMP_RES_CONF, average);
    
    // Wait for the configuration to complete
    delay(hts_conversionDelay);
    
    // Set Up the Configuration for the Relative Humidity and Temperature Sensor Control Register 1
    /*
     // Se the Power-Down Mode Control Mode
     uint8_t config1 = hts_mode;
     
     // Set the Block Data Update
     config1 |= hts_blockdata;
     
     // Set the Block Data Rate
     config1 |= hts_datarate;
    */
    
    uint8_t config1 =   HTS221_REG_HUMTEMP_CTRL_REG1_PD_ACTIVE          |   // Active Mode
                        HTS221_REG_HUMTEMP_CTRL_REG1_BDU_CONTINUOUS;        // Continuous Update
    
    config1 |= hts_datarate;    // Output Data Rate Bit Configurations
    
    // Write the configuration to the Relative Humidity and Temperature Sensor Control Register 1
    writeRegister(hts_i2cAddress, HTS221_REG_HUMTEMP_CTRL_REG1, config1);
    
    // Wait for the configuration to complete
    delay(hts_conversionDelay);
    
    // Set Up the Configuration for the Relative Humidity and Temperature Sensor Control Register 2
    /*
     // Se the  Reboot Memory Content
     uint8_t config2 = hts_boot;
     
     // Set the Heater
     config2 |= hts_heater;
     
     // Set the One-Shot Enable
     config2 |= hts_oneshot;
     */
    
    uint8_t config2 =   HTS221_REG_HUMTEMP_CTRL_REG2_BOOT_NORMAL            |   // Normal Mode
                        HTS221_REG_HUMTEMP_CTRL_REG2_HEATER_DISABLE         |   // Disable Heater
                        HTS221_REG_HUMTEMP_CTRL_REG2_ONE_SHOT_WAITING;          // Waiting for Start of Conversion
    
    // Write the configuration to the Relative Humidity and Temperature Sensor Control Register 2
    writeRegister(hts_i2cAddress, HTS221_REG_HUMTEMP_CTRL_REG2, config2);
    
    // Wait for the configuration to complete
    delay(hts_conversionDelay);
}

/**************************************************************************/
/*
        Reads Calibration Data (Coefficients that are Factory Calibrated)
*/
/**************************************************************************/
void HTS221::readCoefficients(void)
{
    uint8_t T0_degC, T1_degC, H0_T0_OUT_L, H0_T0_OUT_H, H1_T0_OUT_L, H1_T0_OUT_H, T0_OUT_L, T0_OUT_H, T1_OUT_L, T1_OUT_H;
    
    hts_calibdata.H0_rH_x2 = readRegister(hts_i2cAddress, HTS221_REG_HUMTEMP_H0_rH_x2);
    hts_calibdata.H1_rH_x2 = readRegister(hts_i2cAddress, HTS221_REG_HUMTEMP_H1_rH_x2);
    
    T0_degC = readRegister(hts_i2cAddress, HTS221_REG_HUMTEMP_T0_degC_x8);
    T1_degC = readRegister(hts_i2cAddress, HTS221_REG_HUMTEMP_T1_degC_x8);
    
    hts_calibdata.T1_T0_msb = readRegister(hts_i2cAddress, HTS221_REG_HUMTEMP_T1_T0_msb);
    hts_calibdata.T0_degC_x8 = (((uint16_t)(hts_calibdata.T1_T0_msb & 0x03)) << 8) | ((uint16_t)T0_degC);
    hts_calibdata.T1_degC_x8 = (((uint16_t)(hts_calibdata.T1_T0_msb & 0x0C)) << 6) | ((uint16_t)T1_degC);
    
    H0_T0_OUT_L = readRegister(hts_i2cAddress, HTS221_REG_HUMTEMP_H0_T0_OUT_L);
    H0_T0_OUT_H = readRegister(hts_i2cAddress, HTS221_REG_HUMTEMP_H0_T0_OUT_H);
    hts_calibdata.H0_T0_OUT = (int16_t)(H0_T0_OUT_H << 8 | H0_T0_OUT_L);
    
    H1_T0_OUT_L = readRegister(hts_i2cAddress, HTS221_REG_HUMTEMP_H1_T0_OUT_L);
    H1_T0_OUT_H = readRegister(hts_i2cAddress, HTS221_REG_HUMTEMP_H1_T0_OUT_H);
    hts_calibdata.H1_T0_OUT = (int16_t)(H1_T0_OUT_H << 8 | H1_T0_OUT_L);
    
    T0_OUT_L = readRegister(hts_i2cAddress, HTS221_REG_HUMTEMP_T0_OUT_L);
    T0_OUT_H = readRegister(hts_i2cAddress, HTS221_REG_HUMTEMP_T0_OUT_H);
    hts_calibdata.T0_OUT = (int16_t)(T0_OUT_H << 8 | T0_OUT_L);
    
    T1_OUT_L = readRegister(hts_i2cAddress, HTS221_REG_HUMTEMP_T1_OUT_L);
    T1_OUT_H = readRegister(hts_i2cAddress, HTS221_REG_HUMTEMP_T1_OUT_H);
    hts_calibdata.T1_OUT = (int16_t)(T1_OUT_H << 8 | T1_OUT_L);
    
}

/**************************************************************************/
/*
        Reads up the Sensor which comprises of a Relative Humidity and Temperature
*/
/**************************************************************************/
void HTS221::Measure_Sensor(void)
{
    // Read all the Sensors
    // Relative Humidity and Temperature Data
    Measure_Humidity();
    Measure_Temperature();
}

/**************************************************************************/
/*
        Reads the Relative Humidity Output Data
        The value is expressed in 16-bit as two’s complement
*/
/**************************************************************************/
void HTS221::Measure_Humidity()
{
    // Read the Sensor
    uint8_t HumLo, HumHi;
    int16_t hum;
    float humidity;
    
    // Read the Data
    // 16-bit signed result for Relative Humidity Data of HTS221
    HumLo = readRegister(hts_i2cAddress, HTS221_REG_HUMTEMP_HUMIDITY_OUT_L);
    HumHi = readRegister(hts_i2cAddress, HTS221_REG_HUMTEMP_HUMIDITY_OUT_H);
    hum = (int16_t)((uint16_t)HumHi << 8 | HumLo);
    
    // Conversion of the Relative Humidity Data
    float tg_rh = (hts_calibdata.H1_rH_x2 - hts_calibdata.H0_rH_x2) / 2.0 / (hts_calibdata.H1_T0_OUT - hts_calibdata.H0_T0_OUT);
    humidity = tg_rh * (hum - hts_calibdata.H0_T0_OUT) + hts_calibdata.H0_rH_x2  / 2.0;
    hts_sensorData.H = (float)(humidity);
    
}

/**************************************************************************/
/*
        Reads the Temperature Output Data
        The value is expressed in 16-bit as two’s complement
*/
/**************************************************************************/
void HTS221::Measure_Temperature()
{
    // Read the Sensor
    uint8_t TempLo, TempHi;
    int16_t temp;
    float temperature;
    
    // Read the Data
    // 16-bit signed result for Temperature Data of HTS221
    TempLo = readRegister(hts_i2cAddress, HTS221_REG_HUMTEMP_TEMP_OUT_L);
    TempHi = readRegister(hts_i2cAddress, HTS221_REG_HUMTEMP_TEMP_OUT_H);
    temp = (int16_t)((uint16_t)TempHi << 8 | TempLo);

    // Conversion of the Temperature Data
    float tg_t = (hts_calibdata.T1_degC_x8 - hts_calibdata.T0_degC_x8) / 8.0 / (hts_calibdata.T1_OUT - hts_calibdata.T0_OUT);
    temperature = tg_t * (temp - hts_calibdata.T0_OUT) + hts_calibdata.T0_degC_x8 / 8.0;
    
    hts_sensorData.T = (float)(temperature);
    
}

