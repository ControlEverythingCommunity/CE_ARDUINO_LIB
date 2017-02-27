/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        BME280
        This code is designed to work with the BME280_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelerometer?sku=BME280_I2CS_A01#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "BME280.h"

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
int16_t BME280::readRegisterS16(uint8_t i2cAddress, uint8_t reg)
{
    return (int16_t)readRegister16(i2cAddress, reg);
}

/**************************************************************************/
/*
        Reads 16-bits Little Endian from the specified destination register
*/
/**************************************************************************/
uint16_t BME280::readRegister16LE(uint8_t i2cAddress, uint8_t reg)
{
    uint16_t data = readRegister16(i2cAddress, reg);
    return (data >> 8) | (data << 8);
}

/**************************************************************************/
/*
        Reads Signed 16-bits Little Endian from the specified destination register
*/
/**************************************************************************/
int16_t BME280::readRegisterS16LE(uint8_t i2cAddress, uint8_t reg)
{
    return (int16_t)readRegister16LE(i2cAddress, reg);
}

/**************************************************************************/
/*
        Instantiates a new BME280 class with appropriate properties
*/
/**************************************************************************/
void BME280::getAddr_BME280(uint8_t i2cAddress)
{
    bme_i2cAddress = i2cAddress;
    bme_conversionDelay = BME280_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool BME280::begin()
{
    Wire.begin();
    
    uint8_t chipid= readRegister8(bme_i2cAddress, BME280_CHIP_ID_REG);
    if (chipid != BME280_CHIP_ID)
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
void BME280::Reset()
{
    writeRegister8(bme_i2cAddress, BME280_SOFTRESET_REG, 0xB6);
    delay(bme_conversionDelay);
}

/**************************************************************************/
/*
        Sets the Humidity Oversampling
*/
/**************************************************************************/
void BME280::setHumiditySampling(bmeHumiditySampling_t humiditysampling)
{
    bme_humiditysampling = humiditysampling;
}

/**************************************************************************/
/*
        Gets the Humidity Oversampling
 */
/**************************************************************************/
bmeHumiditySampling_t BME280::getHumiditySampling()
{
    return bme_humiditysampling;
}

/**************************************************************************/
/*
        Sets the Pressure Oversampling
 */
/**************************************************************************/
void BME280::setPressureSampling(bmePressureSampling_t pressuresampling)
{
    bme_pressuresampling = pressuresampling;
}

/**************************************************************************/
/*
        Gets the Pressure Oversampling
 */
/**************************************************************************/
bmePressureSampling_t BME280::getPressureSampling()
{
    return bme_pressuresampling;
}

/**************************************************************************/
/*
        Sets the Temperature Oversampling
*/
/**************************************************************************/
void BME280::setTemperatureSampling(bmeTemperatureSampling_t temperaturesampling)
{
    bme_temperaturesampling = temperaturesampling;
}

/**************************************************************************/
/*
        Gets the Temperature Oversampling
*/
/**************************************************************************/
bmeTemperatureSampling_t BME280::getTemperatureSampling()
{
    return bme_temperaturesampling;
}

/**************************************************************************/
/*
        Sets the Mode Status
*/
/**************************************************************************/
void BME280::setSensorMode(bmeSensorMode_t sensormode)
{
    bme_sensormode = sensormode;
}

/**************************************************************************/
/*
        Gets the Mode Status
*/
/**************************************************************************/
bmeSensorMode_t BME280::getSensorMode()
{
    return bme_sensormode;
}

/**************************************************************************/
/*
        Sets the Controls Inactive Duration tstandby in Normal Mode
*/
/**************************************************************************/
void BME280::setStandbyDuration(bmeStandbyDuration_t standbyduration)
{
    bme_standbyduration = standbyduration;
}

/**************************************************************************/
/*
        Gets the Controls Inactive Duration tstandby in Normal Mode
*/
/**************************************************************************/
bmeStandbyDuration_t BME280::getStandbyDuration()
{
    return bme_standbyduration;
}

/**************************************************************************/
/*
        Sets the Controls Inactive Duration tstandby in Normal Mode
*/
/**************************************************************************/
void BME280::setSensorFilter(bmeSensorFilter_t sensorfilter)
{
    bme_sensorfilter = sensorfilter;
}

/**************************************************************************/
/*
        Gets the Controls Inactive Duration tstandby in Normal Mode
*/
/**************************************************************************/
bmeSensorFilter_t BME280::getSensorFilter()
{
    return bme_sensorfilter;
}

/**************************************************************************/
/*
        Sets the SPI Wire Mode
*/
/**************************************************************************/
void BME280::setSPIWire(bmeSPIWire_t spiwire)
{
    bme_spiwire = spiwire;
}

/**************************************************************************/
/*
        Gets the SPI Wire Mode
*/
/**************************************************************************/
bmeSPIWire_t BME280::getSPIWire()
{
    return bme_spiwire;
}

/**************************************************************************/
/*
        Setup the Sensor and Prepare for General Usage
*/
/**************************************************************************/
void BME280::setUpSampling()
{
    // Set Up the Configuration for the Humidity Control Register
    /*
     // Set the Humidity Oversampling
     uint8_t humid_ctrl = bme_humiditysampling;
    */
    
    uint8_t humid_ctrl = BME280_CTRL_HUMIDITY_REG_OVERSAMPLING_X1; // Oversampling X1
    
    // Write the configuration to the Humidity Control Register
    writeRegister8(bme_i2cAddress, BME280_CTRL_HUMIDITY_REG, humid_ctrl);
    
    // Wait for the configuration to complete
    delay(bme_conversionDelay);
    
    // Set Up the Configuration for the Measurement Control Register
    /*
     // Set the Pressure Oversampling
     uint8_t meas_ctrl = bme_pressuresampling;
     
     // Set the Temperature Oversampling
     meas_ctrl |= bme_temperaturesampling;
     
     // Set the Mode Status
     meas_ctrl |= bme_sensormode;
    */
    
    uint8_t meas_ctrl =     BME280_CTRL_MEAS_REG_PRES_OVERSAMPLING_X1   |   // Oversampling X1
                            BME280_CTRL_MEAS_REG_TEMP_OVERSAMPLING_X1   |   // Oversampling X1
                            BME280_CTRL_MEAS_REG_MODE_NORMAL;               // Normal Mode
    
    // Write the configuration to the Measurement Control Register
    writeRegister8(bme_i2cAddress, BME280_CTRL_MEAS_REG, meas_ctrl);
    
    // Wait for the configuration to complete
    delay(bme_conversionDelay);
    
    // Set Up the Configuration for the Configuration Control Register
    /*
     // Set the Controls Inactive Duration tstandby in Normal Mode
     uint8_t config = bme_standbyduration;
     
     // Set the Controls the Time Constant of the IIR Filter
     config |= bme_sensorfilter;
     
     // Set the SPI Wire Mode
     config |= bme_spiwire
    */
    
    uint8_t config =    BME280_CONFIG_REG_STANDBY_DURATION_1000         |   // tstandby [ms]: 1000
                        BME280_CONFIG_REG_FILTER_COEFF_FILTER_OFF       |   // Filter Coefficient: Filter Off
                        BME280_CONFIG_REG_SPI3W_EN_DISABLE;                 // 4-Wire SPI Interface
    
    // Write the configuration to the Configuration Control Register
    writeRegister8(bme_i2cAddress, BME280_CONFIG_REG, config);
    
    // Wait for the configuration to complete
    delay(bme_conversionDelay);
}

/**************************************************************************/
/*
        Reads up the Device which comprises of an Pressure,
        Temperature and Humidity Sensor
*/
/**************************************************************************/
void BME280::Measure_Sensor(void)
{
    // Read all the Sensors
    // Pressure, Temperature, Humidity Data
    Measure_Pressure();
    Measure_Temperature();
    Measure_Humidity();
}

/**************************************************************************/
/*
        Reads Calibration Data (Coefficients that are Factory Calibrated) stored in PROM
*/
/**************************************************************************/
void BME280::readCoefficients(void)
{
    bme_calibdata.dig_T1 = readRegister16LE(bme_i2cAddress, BME280_DIG_T1_LSB_REG);
    bme_calibdata.dig_T2 = readRegisterS16LE(bme_i2cAddress, BME280_DIG_T2_LSB_REG);
    bme_calibdata.dig_T3 = readRegisterS16LE(bme_i2cAddress, BME280_DIG_T3_LSB_REG);
    
    bme_calibdata.dig_P1 = readRegister16LE(bme_i2cAddress, BME280_DIG_P1_LSB_REG);
    bme_calibdata.dig_P2 = readRegisterS16LE(bme_i2cAddress, BME280_DIG_P2_LSB_REG);
    bme_calibdata.dig_P3 = readRegisterS16LE(bme_i2cAddress, BME280_DIG_P3_LSB_REG);
    bme_calibdata.dig_P4 = readRegisterS16LE(bme_i2cAddress, BME280_DIG_P4_LSB_REG);
    bme_calibdata.dig_P5 = readRegisterS16LE(bme_i2cAddress, BME280_DIG_P5_LSB_REG);
    bme_calibdata.dig_P6 = readRegisterS16LE(bme_i2cAddress, BME280_DIG_P6_LSB_REG);
    bme_calibdata.dig_P7 = readRegisterS16LE(bme_i2cAddress, BME280_DIG_P7_LSB_REG);
    bme_calibdata.dig_P8 = readRegisterS16LE(bme_i2cAddress, BME280_DIG_P8_LSB_REG);
    bme_calibdata.dig_P9 = readRegisterS16LE(bme_i2cAddress, BME280_DIG_P9_LSB_REG);
    
    bme_calibdata.dig_H1 = readRegister8(bme_i2cAddress, BME280_DIG_H1_REG);
    bme_calibdata.dig_H2 = readRegisterS16LE(bme_i2cAddress, BME280_DIG_H2_LSB_REG);
    bme_calibdata.dig_H3 = readRegister8(bme_i2cAddress, BME280_DIG_H3_REG);
    bme_calibdata.dig_H4 = (readRegister8(bme_i2cAddress, BME280_DIG_H4_LSB_REG) << 4) | (readRegister8(bme_i2cAddress, BME280_DIG_H4_MSB_REG) & 0xF);
    bme_calibdata.dig_H5 = (readRegister8(bme_i2cAddress, BME280_DIG_H5_REG+1) << 4) | (readRegister8(bme_i2cAddress, BME280_DIG_H5_REG) >> 4);
    bme_calibdata.dig_H6 = (int8_t)readRegister8(bme_i2cAddress, BME280_DIG_H6_REG);
    
}

/**************************************************************************/
/*
        Reads 24-bits from the Destination Register
        Reads the results for Digital Temperarure Value (D2)
*/
/**************************************************************************/
void BME280::Measure_Temperature(void)
{
    int32_t var1, var2;
    
    int32_t adc_T = readRegister24(bme_i2cAddress, BME280_TEMPERATURE_MSB_REG);
  
    adc_T >>= 4;
    
    var1 = ((((adc_T>>3) - ((int32_t)bme_calibdata.dig_T1 <<1))) * ((int32_t)bme_calibdata.dig_T2)) >> 11;
    
    var2 = (((((adc_T>>4) - ((int32_t)bme_calibdata.dig_T1)) * ((adc_T>>4) - ((int32_t)bme_calibdata.dig_T1))) >> 12) * ((int32_t)bme_calibdata.dig_T3)) >> 14;
    
    t_fine = var1 + var2;
    
    float T = (t_fine * 5 + 128) >> 8;
    bme_sensorData.T = (float)(T/100.0);
}

/**************************************************************************/
/*
        Reads 24-bits from the Destination Register
        Reads the Results for Digital Pressure Value
*/
/**************************************************************************/
void BME280::Measure_Pressure(void)
{
    int64_t var1, var2, p;
    
    Measure_Temperature();
    
    int32_t adc_P = readRegister24(bme_i2cAddress, BME280_PRESSURE_MSB_REG);
    
    adc_P >>= 4;
    
    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)bme_calibdata.dig_P6;
    var2 = var2 + ((var1*(int64_t)bme_calibdata.dig_P5)<<17);
    var2 = var2 + (((int64_t)bme_calibdata.dig_P4)<<35);
    var1 = ((var1 * var1 * (int64_t)bme_calibdata.dig_P3)>>8) +
    ((var1 * (int64_t)bme_calibdata.dig_P2)<<12);
    var1 = (((((int64_t)1)<<47)+var1))*((int64_t)bme_calibdata.dig_P1)>>33;

    p = 1048576 - adc_P;
    p = (((p<<31) - var2)*3125) / var1;
    var1 = (((int64_t)bme_calibdata.dig_P9) * (p>>13) * (p>>13)) >> 25;
    var2 = (((int64_t)bme_calibdata.dig_P8) * p) >> 19;
    
    p = ((p + var1 + var2) >> 8) + (((int64_t)bme_calibdata.dig_P7)<<4);
    bme_sensorData.P = (float)(p/256.0);
}

/**************************************************************************/
/*
        Reads 16-bits from the Destination Register
        Reads the Results for Digital Pressure Value
*/
/**************************************************************************/
void BME280::Measure_Humidity(void)
{
    Measure_Temperature();
    
    int32_t adc_H = readRegister16(bme_i2cAddress, BME280_HUMIDITY_MSB_REG);
    
    int32_t v_x1_u32r;
    
    v_x1_u32r = (t_fine - ((int32_t)76800));
    
    v_x1_u32r = (((((adc_H << 14) - (((int32_t)bme_calibdata.dig_H4) << 20) -
                    (((int32_t)bme_calibdata.dig_H5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *
                 (((((((v_x1_u32r * ((int32_t)bme_calibdata.dig_H6)) >> 10) *
                      (((v_x1_u32r * ((int32_t)bme_calibdata.dig_H3)) >> 11) + ((int32_t)32768))) >> 10) +
                    ((int32_t)2097152)) * ((int32_t)bme_calibdata.dig_H2) + 8192) >> 14));
    
    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *
                               ((int32_t)bme_calibdata.dig_H1)) >> 4));
    
    v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
    v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
    float h = (v_x1_u32r>>12);
    bme_sensorData.H = (float)(h/1024.0);
}

