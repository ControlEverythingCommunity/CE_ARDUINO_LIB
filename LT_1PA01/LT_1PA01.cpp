/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        LT-1PA01
        This code is designed to work with the LT-1PA01_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "LT_1PA01.h"

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
        Instantiates a new LT_1PA01 class with appropriate properties
*/
/**************************************************************************/
void LT_1PA01::getAddr_LT_1PA01(uint8_t i2cAddress)
{
    lt_i2cAddress = i2cAddress;
    lt_conversionDelay = LT_1PA01_CONVERSIONDELAY;
    
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool LT_1PA01::begin()
{
    Wire.begin();
    
    uint8_t devid = readRegister(lt_i2cAddress, LT_1PA01_REG_ALSPROX_DEVID);
    if (devid != LT_1PA01_ALSPROX_DEV_ID)
        return false;
    
    // Set up for Proximity/Ambient Light Sensor
    // setUpALSProximity();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Proximity Enable/Disable Register
*/
/**************************************************************************/
void LT_1PA01::setProximityEnable(ltProximityEnable_t proximityenable)
{
    lt_proximityenable = proximityenable;
}

/**************************************************************************/
/*
        Gets the Proximity Enable/Disable Register
*/
/**************************************************************************/
ltProximityEnable_t LT_1PA01::getProximityEnable()
{
    return lt_proximityenable;
}

/**************************************************************************/
/*
        Sets the Proximity Sleep Time Select
*/
/**************************************************************************/
void LT_1PA01::setProximitySleep(ltProximitySleep_t proximitysleep)
{
    lt_proximitysleep = proximitysleep;
}

/**************************************************************************/
/*
        Gets the Proximity Sleep Time Select
*/
/**************************************************************************/
ltProximitySleep_t LT_1PA01::getProximitySleep()
{
    return lt_proximitysleep;
}

/**************************************************************************/
/*
        Sets the IRDR (VCSEL) Current
*/
/**************************************************************************/
void LT_1PA01::setIRDRCurrent(ltIRDRCurrent_t irdrcurrent)
{
    lt_irdrcurrent = irdrcurrent;
}

/**************************************************************************/
/*
        Gets the IRDR (VCSEL) Current
*/
/**************************************************************************/
ltIRDRCurrent_t LT_1PA01::getIRDRCurrent()
{
    return lt_irdrcurrent;
}

/**************************************************************************/
/*
        Sets the Interrupt Algorithm
*/
/**************************************************************************/
void LT_1PA01::setInterrupt(ltInterrupt_t interrupt)
{
    lt_interrupt = interrupt;
}

/**************************************************************************/
/*
        Gets the Interrupt Algorithm
*/
/**************************************************************************/
ltInterrupt_t LT_1PA01::getInterrupt()
{
    return lt_interrupt;
}

/**************************************************************************/
/*
        Sets the Proximity Offset Compensation
*/
/**************************************************************************/
void LT_1PA01::setProximityOffset(ltProximityOffset_t proximityoffset)
{
    lt_proximityoffset = proximityoffset;
}

/**************************************************************************/
/*
        Gets the Proximity Offset Compensation
*/
/**************************************************************************/
ltProximityOffset_t LT_1PA01::getProximityOffset()
{
    return lt_proximityoffset;
}

/**************************************************************************/
/*
        Sets the Ambient Light Sensing Enable/Disable
*/
/**************************************************************************/
void LT_1PA01::setALSEnable(ltALSEnable_t alsenable)
{
    lt_alsenable = alsenable;
}

/**************************************************************************/
/*
        Gets the Ambient Light Sensing Enable/Disable
*/
/**************************************************************************/
ltALSEnable_t LT_1PA01::getALSEnable()
{
    return lt_alsenable;
}

/**************************************************************************/
/*
        Sets the Ambient Light Sensor Range Select
*/
/**************************************************************************/
void LT_1PA01::setALSRange(ltALSRange_t alsrange)
{
    lt_alsrange = alsrange;
}
                    
/**************************************************************************/
/*
        Gets the Ambient Light Sensor Range Select
*/
/**************************************************************************/
ltALSRange_t LT_1PA01::getALSRange()
{
    return lt_alsrange;
}

/**************************************************************************/
/*
        Sets up the Proximity/Ambient Light Sensor
*/
/**************************************************************************/
void LT_1PA01::setUpALSProximity(void)
{
    // Set Up the Configuration for the Proximity Configuration Register
    /*
     // Set the Proximity Enable/Disable
     uint8_t config0 = lt_proximityenable;
    
     // Set the Proximity Sleep Time Select
     config0 |= lt_proximitysleep;
    
     // Set the IRDR (VCSEL) Current
     config0 |= lt_irdrcurrent;
    */
    
    uint8_t config0 =   LT_1PA01_REG_ALSPROX_CONFIG0_PROX_EN_ENABLE     |   // Proximity Enable
                        LT_1PA01_REG_ALSPROX_CONFIG0_PROX_SLP_800MS     |   // Proximity Sleep Time: 800ms
                        LT_1PA01_REG_ALSPROX_CONFIG0_IRDR_DRV_3_6MA;        // IRDR (VCSEL) Current: 3.6 mA
    
    // Write the Configuration for the Proximity Configuration Register
    writeRegister(lt_i2cAddress, LT_1PA01_REG_ALSPROX_CONFIG0, config0);
    
    // Wait for the configuration to complete
    delay(lt_conversionDelay);
    
    // Set Up the Configuration for the Proximity/ALS Configuration Register
    /*
     // Set the Interrupt Algorithm
     uint8_t config1 = lt_interrupt;
     
     // Set the Proximity Offset Compensation
     config1 |= lt_proximityoffset;
     
     // Set the Ambient Light Sensing Enable/Disable
     config1 |= lt_alsenable;
     
     // Set the Ambient Light Sensor Range Select
     config1 |= lt_alsrange;
    */
    
    uint8_t config1 =   LT_1PA01_REG_ALSPROX_CONFIG1_INT_ALG_HYST           |   // Hysteresis Window
                        LT_1PA01_REG_ALSPROX_CONFIG1_PROX_OFFSET_DISABLE    |   // Proximity Offset Compensation Disable
                        LT_1PA01_REG_ALSPROX_CONFIG1_ALS_EN_ENABLE          |   // Ambient Light Sensing Enable
                        LT_1PA01_REG_ALSPROX_CONFIG1_ALS_RANGE_2000LX;          // 2000 Lux
    
    // Write the Configuration for the Proximity/ALS Configuration Register
    writeRegister(lt_i2cAddress, LT_1PA01_REG_ALSPROX_CONFIG1, config1);
    
    // Wait for the configuration to complete
    delay(lt_conversionDelay);
    
}
/**************************************************************************/
/*
        Reads up the Proximity/Ambient Light Sensor
*/
/**************************************************************************/
void LT_1PA01::Measure_Sensor(void)
{
    // Read all the Sensors
    // Proximity/ALS Data
    Measure_Proximity();
    Measure_ALS();
}

/**************************************************************************/
/*
        Reads the Proximity Sensor
*/
/**************************************************************************/
void LT_1PA01::Measure_Proximity()
{
    uint8_t Pdata;
    
    // Read the Data
    // Reading the Proximity Data Register
    Pdata = readRegister(lt_i2cAddress, LT_1PA01_REG_ALSPROX_PROX_DATA);
    
    // Conversion of the result
    // 8-bit unsigned result for Proximity Data of LT_1PA01
    lt_alsproxData.P = (uint8_t)(Pdata);
}

/**************************************************************************/
/*
        Reads the Digital Ambient Light Sensor
*/
/**************************************************************************/
void LT_1PA01::Measure_ALS()
{
    uint8_t ALSdataLo, ALSdataHi;
    
    // Read the Data
    // Reading the ALS Data High Register
    ALSdataHi = readRegister(lt_i2cAddress, LT_1PA01_REG_ALSPROX_ALS_DATA_HB);
    // Reading the ALS Data Low Register
    ALSdataLo = readRegister(lt_i2cAddress, LT_1PA01_REG_ALSPROX_ALS_DATA_LB);

    // Conversion of the result
    // 12-bit unsigned result for ALS Data of LT_1PA01
    lt_alsproxData.L = (uint16_t)((ALSdataHi << 8) | ALSdataLo);
}
