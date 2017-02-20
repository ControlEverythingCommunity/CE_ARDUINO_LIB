/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LPS25HB
        This code is designed to work with the LPS25HB_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Barometer?sku=LPS25HB_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "LPS25HB.h"

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
        Instantiates a new LPS25HB class with appropriate properties (Barometer Address)
*/
/**************************************************************************/
void LPS25HB::getAddr_LPS25HB(uint8_t i2cAddress)
{
    lps_i2cAddress = i2cAddress;
    lps_conversionDelay = lps_conversionDelay;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool LPS25HB::begin()
{
    Wire.begin();
    
    uint8_t Pdevid = readRegister(lps_i2cAddress, LPS25HB_REG_BARO_WHO_AM_I);
    if (Pdevid != LPS25HB_BARO_DEV_ID)
        return false;
    
    // Set up the sensor for Barometer
    // setUpBarometer();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Temperature Resolution Configuration
*/
/**************************************************************************/
void LPS25HB::setTempResolution(lpsTempResolution_t tempresolution)
{
    lps_tempresolution = tempresolution;
}

/**************************************************************************/
/*
        Gets the Temperature Resolution Configuration
*/
/**************************************************************************/
lpsTempResolution_t LPS25HB::getTempResolution()
{
    return lps_tempresolution;
}

/**************************************************************************/
/*
        Sets the Pressure Resolution Configuration
*/
/**************************************************************************/
void LPS25HB::setPresResolution(lpsPresResolution_t presresolution)
{
    lps_presresolution = presresolution;
}

/**************************************************************************/
/*
        Gets the Pressure Resolution Configuration
*/
/**************************************************************************/
lpsPresResolution_t LPS25HB::getPresResolution()
{
    return lps_presresolution;
}

/**************************************************************************/
/*
        Sets the Power-Down Mode Control
*/
/**************************************************************************/
void LPS25HB::setBaroMode(lpsBaroMode_t baromode)
{
    lps_baromode = baromode;
}

/**************************************************************************/
/*
        Gets the Power-Down Mode Control
*/
/**************************************************************************/
lpsBaroMode_t LPS25HB::getBaroMode()
{
    return lps_baromode;
}

/**************************************************************************/
/*
        Sets the Output Data Rate Configuration for the Barometer
*/
/**************************************************************************/
void LPS25HB::setBaroDataRate(lpsBaroDataRate_t barodatarate)
{
    lps_barodatarate = barodatarate;
}

/**************************************************************************/
/*
        Gets the Output Data Rate Configuration for the Barometer
*/
/**************************************************************************/
lpsBaroDataRate_t LPS25HB::getBaroDataRate()
{
    return lps_barodatarate;
}

/**************************************************************************/
/*
        Sets the Interrupt Generation Enable
*/
/**************************************************************************/
void LPS25HB::setBaroInterrupt(lpsBaroInterrupt_t barointerrupt)
{
    lps_barointerrupt = barointerrupt;
}

/**************************************************************************/
/*
        Gets the Interrupt Generation Enable
*/
/**************************************************************************/
lpsBaroInterrupt_t LPS25HB::getBaroInterrupt()
{
    return lps_barointerrupt;
}

/**************************************************************************/
/*
        Sets the Block Data Update
*/
/**************************************************************************/
void LPS25HB::setBaroBlockData(lpsBaroBlockData_t baroblockdata)
{
    lps_baroblockdata = baroblockdata;
}

/**************************************************************************/
/*
        Gets the Block Data Update
*/
/**************************************************************************/
lpsBaroBlockData_t LPS25HB::getBaroBlockData()
{
    return lps_baroblockdata;
}

/**************************************************************************/
/*
        Sets the Big/Little Endian Data Selection
*/
/**************************************************************************/
void LPS25HB::setBaroAutozero(lpsBaroAutozero_t baroautozero)
{
    lps_baroautozero = baroautozero;
}

/**************************************************************************/
/*
        Gets the Big/Little Endian Data Selection
*/
/**************************************************************************/
lpsBaroAutozero_t LPS25HB::getBaroAutozero()
{
    return lps_baroautozero;
}

/**************************************************************************/
/*
        Sets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
void LPS25HB::setBaroSPIWire(lpsBaroSPIWire_t barospiwire)
{
    lps_barospiwire = barospiwire;
}

/**************************************************************************/
/*
        Gets the SPI Serial Interface Mode Selection
*/
/**************************************************************************/
lpsBaroSPIWire_t LPS25HB::getBaroSPIWire()
{
    return lps_barospiwire;
}

/**************************************************************************/
/*
        Sets up the Barometer
*/
/**************************************************************************/
void LPS25HB::setUpBarometer(void)
{
    // Set Up the Configuration for the Pressure and Temperature Resolution Register
    /*
    // Set the Temperature Resolution Configuration
    uint8_t resolution = lps_tempresolution;
    
    // Set the Pressure Resolution Configuration
    resolution |= lps_presresolution;
    */
    
    uint8_t resolution =    LPS25HB_REG_BARO_RES_CONF_TEMP_8    |   // Nr. Internal Average: 8
                            LPS25HB_REG_BARO_RES_CONF_PRES_8;       // Nr. Internal Average: 8
    
    // Write the configuration to the Pressure and Temperature Resolution Register
    writeRegister(lps_i2cAddress, LPS25HB_REG_BARO_RES_CONFIG, resolution);
    
    // Wait for the configuration to complete
    delay(lps_conversionDelay);
    
    // Set Up the Configuration for the Barometer Control Register 1
    /*
    // Se the Power-Down Mode Control Mode
    uint8_t config1 = lps_baromode;
     
    // Set the  Interrupt Generation Enable
    config1 |= lps_barointerrupt;
     
    // Set the Block Data Update
    config1 |= lps_baroblockdata;
    
    // Set the Reset Autozero Function
    config1 |= lps_baroautozero;
    
    // Set the SPI Serial Interface Mode Selection
    config1 |= lps_barospiwire;
    */
    
    uint8_t config1 =   LPS25HB_REG_BARO_CTRL_REG1_PD_ACTIVE            |   // Active Mode
                        LPS25HB_REG_BARO_CTRL_REG1_DIFF_EN_DISABLE      |   // Interrupt Generation Disabled
                        LPS25HB_REG_BARO_CTRL_REG1_BDU_CONTINUOUS       |   // Continuous Update
                        LPS25HB_REG_BARO_CTRL_REG1_RESET_AZ_NORMAL      |   // Normal Mode
                        LPS25HB_REG_BARO_CTRL_REG1_SIM_4WIRE;               // 4-Wire Interface
    
    config1 |= lps_barodatarate;    // Output Data Rate Bit Configurations
    
    // Write the configuration to the Barometer Control Register 1
    writeRegister(lps_i2cAddress, LPS25HB_REG_BARO_CTRL_REG1, config1);
    
    // Wait for the configuration to complete
    delay(lps_conversionDelay);
}
/**************************************************************************/
/*
        Reads up the Sensor which comprises of an Barometer
*/
/**************************************************************************/
void LPS25HB::Measure_Sensor(void)
{
    // Read all the Sensors
    // Barometer and Temperature Data
    Measure_Barometer();
    Measure_Temperature();
}

/**************************************************************************/
/*
        Reads the Barometer Output Data
        The value is expressed in 24-bit as two’s complement
*/
/**************************************************************************/
void LPS25HB::Measure_Barometer()
{
    // Read the Barometer
    uint32_t pbaroXLo, pbaroLo, pbaroHi;
    
    // Read the Data
    // Reading the Low Pressure Data Register
    pbaroXLo = readRegister(lps_i2cAddress, LPS25HB_REG_BARO_PRESS_OUT_XL);
    // Reading the Middle Pressure Data Register
    pbaroLo = readRegister(lps_i2cAddress, LPS25HB_REG_BARO_PRESS_OUT_L);
    // Reading the High Pressure Data Register
    pbaroHi = readRegister(lps_i2cAddress, LPS25HB_REG_BARO_PRESS_OUT_H);
    // Conversion of the result
    // 24-bit signed result for Pressure Data of LPS25HB
    lps_baroData.P = (float)((((pbaroHi << 16) | (pbaroLo << 8) | pbaroXLo)) / 4096.0);
    
}

/**************************************************************************/
/*
        Reads the Temperature Output Data
        The value is expressed in 16-bit as two’s complement
*/
/**************************************************************************/
void LPS25HB::Measure_Temperature()
{
    // Read the Barometer
    uint8_t ptempLo, ptempHi;
    
    // Read the Data
    // Reading the Low Temperature Data Register
    ptempLo = readRegister(lps_i2cAddress, LPS25HB_REG_BARO_TEMP_OUT_L);
    // Reading the High Temperature Data Register
    ptempHi = readRegister(lps_i2cAddress, LPS25HB_REG_BARO_TEMP_OUT_H);
    // Conversion of the result
    // 16-bit signed result for Temperature Data of LPS25HB
    lps_baroData.T = (int16_t)(((ptempHi << 8) | ptempLo)) / 480.0 + 42.5;
    
}

