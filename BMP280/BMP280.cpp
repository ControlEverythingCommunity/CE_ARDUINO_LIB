/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        BMP280
        This code is designed to work with the BMP280_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelerometer?sku=BMP280_I2CS_A01#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "BMP280.h"

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
static void writeRegister8(uint8_t i2cAddress, uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    i2cwrite((uint8_t)(value));
    Wire.endTransmission();
}

/**************************************************************************/
/*
        Reads 8-bits from the specified destination register
*/
/**************************************************************************/
static uint8_t readRegister8(uint8_t i2cAddress, uint8_t reg)
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
    Reads 24-bits from the specified destination register
*/
/**************************************************************************/
static uint32_t readRegister24(uint8_t i2cAddress, uint8_t reg)
{
    uint32_t value;
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)3);
    value = i2cread();
    value <<= 8;
    value |= i2cread();
    value <<= 8;
    value |= i2cread();
    return value;
}

/**************************************************************************/
/*
        Reads Signed 16-bits from the specified destination register
*/
/**************************************************************************/
int16_t BMP280::readRegisterS16(uint8_t i2cAddress, uint8_t reg)
{
    return (int16_t)readRegister16(i2cAddress, reg);
}

/**************************************************************************/
/*
        Reads 16-bits Little Endian from the specified destination register
*/
/**************************************************************************/
uint16_t BMP280::readRegister16LE(uint8_t i2cAddress, uint8_t reg)
{
    uint16_t data = readRegister16(i2cAddress, reg);
    return (data >> 8) | (data << 8);
}

/**************************************************************************/
/*
        Reads Signed 16-bits Little Endian from the specified destination register
*/
/**************************************************************************/
int16_t BMP280::readRegisterS16LE(uint8_t i2cAddress, uint8_t reg)
{
    return (int16_t)readRegister16LE(i2cAddress, reg);
}

/**************************************************************************/
/*
        Instantiates a new BMP280 class with appropriate properties
*/
/**************************************************************************/
void BMP280::getAddr_BMP280(uint8_t i2cAddress)
{
    bmp_i2cAddress = i2cAddress;
    bmp_conversionDelay = BMP280_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool BMP280::begin()
{
    Wire.begin();
    
    uint8_t chipid= readRegister8(bmp_i2cAddress, BMP280_CHIP_ID_REG);
    if (chipid != BMP280_CHIP_ID)
        return false;
    
    // Reset the Device Using Soft-Reset
    // Reset();
    
    // Set up the sensor for Pressure, Temperature and Humidity
    // setUpSampling();
    
    // Read Trimming Parameters
    readCoefficients();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Soft Reset Command
        The Device is Reset using the Complete Power-ON-Reset Procedure
*/
/**************************************************************************/
void BMP280::Reset()
{
    writeRegister8(bmp_i2cAddress, BMP280_SOFTRESET_REG, 0xB6);
    delay(bmp_conversionDelay);
}

/**************************************************************************/
/*
        Sets the Pressure Oversampling
 */
/**************************************************************************/
void BMP280::setPressureSampling(bmpPressureSampling_t pressuresampling)
{
    bmp_pressuresampling = pressuresampling;
}

/**************************************************************************/
/*
        Gets the Pressure Oversampling
 */
/**************************************************************************/
bmpPressureSampling_t BMP280::getPressureSampling()
{
    return bmp_pressuresampling;
}

/**************************************************************************/
/*
        Sets the Temperature Oversampling
*/
/**************************************************************************/
void BMP280::setTemperatureSampling(bmpTemperatureSampling_t temperaturesampling)
{
    bmp_temperaturesampling = temperaturesampling;
}

/**************************************************************************/
/*
        Gets the Temperature Oversampling
*/
/**************************************************************************/
bmpTemperatureSampling_t BMP280::getTemperatureSampling()
{
    return bmp_temperaturesampling;
}

/**************************************************************************/
/*
        Sets the Mode Status
*/
/**************************************************************************/
void BMP280::setSensorMode(bmpSensorMode_t sensormode)
{
    bmp_sensormode = sensormode;
}

/**************************************************************************/
/*
        Gets the Mode Status
*/
/**************************************************************************/
bmpSensorMode_t BMP280::getSensorMode()
{
    return bmp_sensormode;
}

/**************************************************************************/
/*
        Sets the Controls Inactive Duration tstandby in Normal Mode
*/
/**************************************************************************/
void BMP280::setStandbyDuration(bmpStandbyDuration_t standbyduration)
{
    bmp_standbyduration = standbyduration;
}

/**************************************************************************/
/*
        Gets the Controls Inactive Duration tstandby in Normal Mode
*/
/**************************************************************************/
bmpStandbyDuration_t BMP280::getStandbyDuration()
{
    return bmp_standbyduration;
}

/**************************************************************************/
/*
        Sets the Controls Inactive Duration tstandby in Normal Mode
*/
/**************************************************************************/
void BMP280::setSensorFilter(bmpSensorFilter_t sensorfilter)
{
    bmp_sensorfilter = sensorfilter;
}

/**************************************************************************/
/*
        Gets the Controls Inactive Duration tstandby in Normal Mode
*/
/**************************************************************************/
bmpSensorFilter_t BMP280::getSensorFilter()
{
    return bmp_sensorfilter;
}

/**************************************************************************/
/*
        Sets the SPI Wire Mode
*/
/**************************************************************************/
void BMP280::setSPIWire(bmpSPIWire_t spiwire)
{
    bmp_spiwire = spiwire;
}

/**************************************************************************/
/*
        Gets the SPI Wire Mode
*/
/**************************************************************************/
bmpSPIWire_t BMP280::getSPIWire()
{
    return bmp_spiwire;
}

/**************************************************************************/
/*
        Setup the Sensor and Prepare for General Usage
*/
/**************************************************************************/
void BMP280::setUpSampling()
{
    // Set Up the Configuration for the Measurement Control Register
    /*
     // Set the Pressure Oversampling
     uint8_t meas_ctrl = bmp_pressuresampling;
     
     // Set the Temperature Oversampling
     meas_ctrl |= bmp_temperaturesampling;
     
     // Set the Mode Status
     meas_ctrl |= bmp_sensormode;
    */
    
    uint8_t meas_ctrl =     BMP280_CTRL_MEAS_REG_PRES_OVERSAMPLING_X1   |   // Oversampling X1
                            BMP280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_X1   |   // Oversampling X1
                            BMP280_CTRL_MEAS_REG_MODE_NORMAL;               // Normal Mode
    
    // Write the configuration to the Measurement Control Register
    writeRegister8(bmp_i2cAddress, BMP280_CTRL_MEAS_REG, meas_ctrl);
    
    // Wait for the configuration to complete
    delay(bmp_conversionDelay);
    
    // Set Up the Configuration for the Configuration Control Register
    /*
     // Set the Controls Inactive Duration tstandby in Normal Mode
     uint8_t config = bmp_standbyduration;
     
     // Set the Controls the Time Constant of the IIR Filter
     config |= bmp_sensorfilter;
     
     // Set the SPI Wire Mode
     config |= bmp_spiwire
    */
    
    uint8_t config =    BMP280_CONFIG_REG_STANDBY_DURATION_1000         |   // tstandby [ms]: 1000
                        BMP280_CONFIG_REG_FILTER_COEFF_FILTER_OFF       |   // Filter Coefficient: Filter Off
                        BMP280_CONFIG_REG_SPI3W_EN_DISABLE;                 // 4-Wire SPI Interface
    
    // Write the configuration to the Configuration Control Register
    writeRegister8(bmp_i2cAddress, BMP280_CONFIG_REG, config);
    
    // Wait for the configuration to complete
    delay(bmp_conversionDelay);
}

/**************************************************************************/
/*
        Reads up the Device which comprises of an Pressure,
        Temperature and Humidity Sensor
*/
/**************************************************************************/
void BMP280::Measure_Sensor(void)
{
    // Read all the Sensors
    // Pressure, Temperature Data
    Measure_Pressure();
    Measure_Temperature();
}

/**************************************************************************/
/*
        Reads Calibration Data (Coefficients that are Factory Calibrated) stored in PROM
*/
/**************************************************************************/
void BMP280::readCoefficients(void)
{
    bmp_calibdata.dig_T1 = readRegister16LE(bmp_i2cAddress, BMP280_DIG_T1_LSB_REG);
    bmp_calibdata.dig_T2 = readRegisterS16LE(bmp_i2cAddress, BMP280_DIG_T2_LSB_REG);
    bmp_calibdata.dig_T3 = readRegisterS16LE(bmp_i2cAddress, BMP280_DIG_T3_LSB_REG);
    
    bmp_calibdata.dig_P1 = readRegister16LE(bmp_i2cAddress, BMP280_DIG_P1_LSB_REG);
    bmp_calibdata.dig_P2 = readRegisterS16LE(bmp_i2cAddress, BMP280_DIG_P2_LSB_REG);
    bmp_calibdata.dig_P3 = readRegisterS16LE(bmp_i2cAddress, BMP280_DIG_P3_LSB_REG);
    bmp_calibdata.dig_P4 = readRegisterS16LE(bmp_i2cAddress, BMP280_DIG_P4_LSB_REG);
    bmp_calibdata.dig_P5 = readRegisterS16LE(bmp_i2cAddress, BMP280_DIG_P5_LSB_REG);
    bmp_calibdata.dig_P6 = readRegisterS16LE(bmp_i2cAddress, BMP280_DIG_P6_LSB_REG);
    bmp_calibdata.dig_P7 = readRegisterS16LE(bmp_i2cAddress, BMP280_DIG_P7_LSB_REG);
    bmp_calibdata.dig_P8 = readRegisterS16LE(bmp_i2cAddress, BMP280_DIG_P8_LSB_REG);
    bmp_calibdata.dig_P9 = readRegisterS16LE(bmp_i2cAddress, BMP280_DIG_P9_LSB_REG);
    
}

/**************************************************************************/
/*
        Reads 24-bits from the Destination Register
        Reads the results for Digital Temperarure Value (D2)
*/
/**************************************************************************/
void BMP280::Measure_Temperature(void)
{
    int32_t var1, var2;
    
    int32_t adc_T = readRegister24(bmp_i2cAddress, BMP280_TEMPERATURE_MSB_REG);
  
    adc_T >>= 4;
    
    var1 = ((((adc_T>>3) - ((int32_t)bmp_calibdata.dig_T1 <<1))) * ((int32_t)bmp_calibdata.dig_T2)) >> 11;
    
    var2 = (((((adc_T>>4) - ((int32_t)bmp_calibdata.dig_T1)) * ((adc_T>>4) - ((int32_t)bmp_calibdata.dig_T1))) >> 12) * ((int32_t)bmp_calibdata.dig_T3)) >> 14;
    
    t_fine = var1 + var2;
    
    float T = (t_fine * 5 + 128) >> 8;
    bmp_sensorData.T = (float)(T/100.0);
}

/**************************************************************************/
/*
        Reads 24-bits from the Destination Register
        Reads the Results for Digital Pressure Value
*/
/**************************************************************************/
void BMP280::Measure_Pressure(void)
{
    int64_t var1, var2, p;
    
    Measure_Temperature();
    
    int32_t adc_P = readRegister24(bmp_i2cAddress, BMP280_PRESSURE_MSB_REG);
    
    adc_P >>= 4;
    
    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)bmp_calibdata.dig_P6;
    var2 = var2 + ((var1*(int64_t)bmp_calibdata.dig_P5)<<17);
    var2 = var2 + (((int64_t)bmp_calibdata.dig_P4)<<35);
    var1 = ((var1 * var1 * (int64_t)bmp_calibdata.dig_P3)>>8) +
    ((var1 * (int64_t)bmp_calibdata.dig_P2)<<12);
    var1 = (((((int64_t)1)<<47)+var1))*((int64_t)bmp_calibdata.dig_P1)>>33;

    p = 1048576 - adc_P;
    p = (((p<<31) - var2)*3125) / var1;
    var1 = (((int64_t)bmp_calibdata.dig_P9) * (p>>13) * (p>>13)) >> 25;
    var2 = (((int64_t)bmp_calibdata.dig_P8) * p) >> 19;
    
    p = ((p + var1 + var2) >> 8) + (((int64_t)bmp_calibdata.dig_P7)<<4);
    bmp_sensorData.P = (float)(p/256.0);
}
