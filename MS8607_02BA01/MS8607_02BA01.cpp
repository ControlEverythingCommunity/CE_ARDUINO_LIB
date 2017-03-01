/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MS8607-02BA01
        This code is designed to work with the MS8607-02BA01_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Accelerometer?sku=MS8607-02BA01_I2CS_A01#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "MS8607_02BA01.h"

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
        Writes 8-bits to the Destination Register
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
        Writes 8-bits to the specified Destination Register
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
        Reads 16-bits from the Destination Register
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
        Reads 16-bits from the specified Destination Register
*/
/**************************************************************************/
static uint16_t readRegister16(uint8_t i2cAddress, uint8_t reg)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)2);
    return (int16_t)((i2cread() << 8) | i2cread());
}

/**************************************************************************/
/*
    Reads 24-bits from the specified Destination Register
*/
/**************************************************************************/
static uint32_t readRegister24(uint8_t i2cAddress, uint8_t reg)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)3);
    return (uint32_t)((int32_t)i2cread() << 16) | ((int32_t)i2cread() << 8) | i2cread();
}

/**************************************************************************/
/*
        Instantiates a new MS8607-02BA01 class with appropriate properties
        Pressure and Temperature
*/
/**************************************************************************/
void MS8607_02BA01::getAddrPT_MS8607_02BA01(uint8_t i2cAddress)
{
    ms_i2cAddressPT = i2cAddress;
    ms_conversionDelay = MS8607_02BA01_CONVERSIONDELAY;
}


/**************************************************************************/
/*
        Instantiates a new MS8607-02BA01 class with appropriate properties
        Relative Humidity
*/
/**************************************************************************/
void MS8607_02BA01::getAddrRH_MS8607_02BA01(uint8_t i2cAddress)
{
    ms_i2cAddressRH = i2cAddress;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool MS8607_02BA01::begin()
{
    Wire.begin();
    Reset();
    delay(ms_conversionDelay);
    Initialize();
    return true;
}

/**************************************************************************/
/*
        Sets the Reset Command
        The Reset sequence shall be sent once after power-on to make sure 
        that the calibration PROM gets loaded into the internal Register
*/
/**************************************************************************/
void MS8607_02BA01::Reset()
{
    // Reset the Pressure and Temperature Sensor with the RESET Command
    writeRegister8(ms_i2cAddressPT, MS8607_02BA01_CMD_RESET);
    delay(ms_conversionDelay);
    
    // Resets the Relative Humidty Sensor with the RESET command
    writeRegister8(ms_i2cAddressRH, MS8607_02BA01_CMD_RESET);
}

/**************************************************************************/
/*
        Sets the OSR command value (typ=D1) to intiate the pressure conversion
*/
/**************************************************************************/
void MS8607_02BA01::setinitiatePressure(msD1OSR_t d1osr)
{
    ms_d1osr = d1osr;
}

/**************************************************************************/
/*
        Gets the OSR command value (typ=D1) to intiate the pressure conversion
 */
/**************************************************************************/
msD1OSR_t MS8607_02BA01::getinitiatePressure()
{
    return ms_d1osr;
}

/**************************************************************************/
/*
        Sets the OSR command value (typ=D2) to intiate the temperature conversion
 */
/**************************************************************************/
void MS8607_02BA01::setinitiateTemperature(msD2OSR_t d2osr)
{
    ms_d2osr = d2osr;
}

/**************************************************************************/
/*
        Gets the OSR command value (typ=D2) to intiate the temperature conversion
 */
/**************************************************************************/
msD2OSR_t MS8607_02BA01::getinitiateTemperature()
{
    return ms_d2osr;
}

/**************************************************************************/
/*
        Sets the Humidity Measurement Mode
*/
/**************************************************************************/
void MS8607_02BA01::setHumidityMode(msHumidityMode_t humiditymode)
{
    ms_humiditymode = humiditymode;
}

/**************************************************************************/
/*
        Gets the Humidity Measurement Mode
*/
/**************************************************************************/
msHumidityMode_t MS8607_02BA01::getHumidityMode()
{
    return ms_humiditymode;
}

/**************************************************************************/
/*
        Sets the Measurement Resolution
*/
/**************************************************************************/
void MS8607_02BA01::setResolution(msResolution_t resolution)
{
    ms_resolution = resolution;
}

/**************************************************************************/
/*
        Gets the Measurement Resolution
*/
/**************************************************************************/
msResolution_t MS8607_02BA01::getResolution()
{
    return ms_resolution;
}

/**************************************************************************/
/*
        Sets the VDD Status
*/
/**************************************************************************/
void MS8607_02BA01::setVoltage(msVoltage_t voltage)
{
    ms_voltage = voltage;
}

/**************************************************************************/
/*
        Gets the VDD Status
*/
/**************************************************************************/
msVoltage_t MS8607_02BA01::getVoltage()
{
    return ms_voltage;
}

/**************************************************************************/
/*
        Sets the Heater Status
*/
/**************************************************************************/
void MS8607_02BA01::setHeaterStatus(msHeaterStatus_t heaterstatus)
{
    ms_heaterstatus = heaterstatus;
}

/**************************************************************************/
/*
        Gets the Heater Status
*/
/**************************************************************************/
msHeaterStatus_t MS8607_02BA01::getHeaterStatus()
{
    return ms_heaterstatus;
}

/**************************************************************************/
/*
        Measures up the Device which Comprises of an Pressure,
        Temperature and Humidity Sensor
*/
/**************************************************************************/
void MS8607_02BA01::Measure_Sensor(void)
{
    // Read all the Sensors
    // Pressure, Temperature, Humidity Data
    Measure_Pressure();
    delay(ms_conversionDelay);
    Measure_Temperature();
    delay(ms_conversionDelay);
    Measure_Humidity();
}


/**************************************************************************/
/*
        Power On and Prepare for General Usage
        Reads Calibration Data (Coefficients that are Factory Calibrated) Stored in PROM
*
/**************************************************************************/
void MS8607_02BA01::Initialize()
{
    // Reading 6 Calibration Data Values
    // Read cofficients values stored in EPROM of the device
    // Pressure sensitivity, SENST1
    // Pressure offset, OFFT1
    // Temperature coefficient of pressure sensitivity, TCS
    // Temperature coefficient of pressure offset, TCO
    // Reference temperature, TREF
    // Temperature coefficient of the temperature, TEMPSENS
    
    for (uint8_t offset = 0; offset < 6; offset++)
    {
        C[offset] = readRegister16(ms_i2cAddressPT, MS8607_02BA01_CMD_C1 + (offset * 2));
    }
}

/**************************************************************************/
/*
        Reads 24-bits from the Destination Register
        Reads the Results for Digital Pressure Value (D1)
*/
/**************************************************************************/
uint32_t MS8607_02BA01::readPressure()
{
    uint8_t pressure_osr = ms_d1osr;
    // Initiate the process of pressure measurement
    writeRegister8(ms_i2cAddressPT, pressure_osr);
    // Waiting for pressure data ready
    delay(ms_conversionDelay);
    
    // Reads the pressure value
    return readRegister24(ms_i2cAddressPT, MS8607_02BA01_CMD_ADC);
}

/**************************************************************************/
/*
        Reads 24-bits from the Destination Register
        Reads the Results for Digital Temperarure Value (D2)
*/
/**************************************************************************/
uint32_t MS8607_02BA01::readTemperature()
{
    uint8_t temperature_osr = ms_d2osr;
    // Initiate the process of temperarure measurement
    writeRegister8(ms_i2cAddressPT, temperature_osr);
    // Waiting for temperarure data ready
    delay(ms_conversionDelay);
    
    // Reads the temperarure value
   return readRegister24(ms_i2cAddressPT, MS8607_02BA01_CMD_ADC);

}

/**************************************************************************/
/* 
        Calculate Pressure Calculations and Perform Compensation
        More Info About these Calculations is Available in the Datasheet
*/
/**************************************************************************/
void MS8607_02BA01::Measure_Pressure(void)
{
    uint32_t D1 = readPressure();
    
    uint32_t D2 = readTemperature();
    
    // Difference between actual and reference temperature
    int32_t dT = D2 - (uint32_t)C[4] * pow(2, 8);

    // Actual temperature (-40…85°C with 0.01°C resolution)
    int32_t TEMP = 2000 + ((int64_t) dT * C[5]) / pow(2, 23);
    // Offset at actual temperature
    int64_t OFF = (int64_t)C[1] * pow(2, 17) + (int64_t)C[3] * dT / pow(2, 6);
    // Sensitivity at actual temperature
    int64_t SENS = (int64_t)C[0] * pow(2, 16) + (int64_t)C[2] * dT / pow(2, 7);
    
    // Second Order Temperature Compensation
        
    if (TEMP >= 2000)
    {
        // High temperature
        OFF2 = 0;
        SENS2 = 0;
    }
    else if (TEMP < 2000)
    {
        // Low Temperature
        OFF2 = 61 * (pow(TEMP - 2000, 2)) / 16;
        SENS2 = 29 * (pow(TEMP - 2000, 2)) / 16;
    }
    else if (TEMP < -1500)
    {
        // Very Low Temperature
        OFF2 = OFF2 + 17 * (pow(TEMP + 1500, 2));
        SENS2 = SENS2 + 9 * (pow(TEMP + 1500, 2));
    }
    
    // Calculate pressure
    OFF = OFF - OFF2;
    SENS = SENS - SENS2;
    
    // Final calculations
    // Temperature Compensated Pressure (10…1200mbar with 0.01mbar resolution)
    uint32_t PRES = ((D1 * SENS) / pow(2, 21) - OFF) / pow(2, 15) / 100.0;
    ms_sensorData.P = (float)PRES;
}

/**************************************************************************/
/*
        Calculate Temperature Calculations and Perform Compensation
        More Info About these Calculations is Available in the Datasheet
*/
/**************************************************************************/
void MS8607_02BA01::Measure_Temperature(void)
{
    uint32_t D2 = readTemperature();
    
    // Difference between actual and reference temperature
    int32_t dT = D2 - (uint32_t)C[4] * pow(2, 8);
    
    // Actual temperature (-40…85°C with 0.01°C resolution)
    int32_t TEMP = 2000 + ((int64_t) dT * C[5]) / pow(2, 23);
    

    if (TEMP >= 2000)
    {
        // High temperature
        T2 = 0;
    }
    else if (TEMP < 2000)
    {
        // Low temperature
        T2 = 3 * (pow(dT, 2)) / pow(2, 33);
    }
    
    TEMP = TEMP - T2;
    
    // Compensated Temperature
    TEMP = TEMP / 100.0;
    ms_sensorData.T = (float)TEMP;
}

/**************************************************************************/
/*
        Reads the Results by Sending Humidity Command
        Reads Latest Humidity Value
        Measuring the 16-bit Humidity Register
        Calculate Relative Humidity Calculations and Perform Compensation
        More Info About these Calculations is Available in the Datasheet
*/
/**************************************************************************/
void MS8607_02BA01::Measure_Humidity()
{
    // Set Measurement Resolution
    uint8_t tempControl = ms_resolution;
    
    // Set VDD (Voltage Status)
    tempControl |= ms_voltage;
    
    // Set On-Chip Heater Status
    tempControl |= ms_heaterstatus;
    
    // Sending command to the User Register 1 for MS8607_02BA01
    writeRegister(ms_i2cAddressRH, MS8607_02BA01_CMD_WRITE_USER, tempControl);
    
    // Wait for the conversion to complete
    delay(ms_conversionDelay);
    
    // Set the Humidity Measurement Mode
    uint8_t humidityMode = ms_humiditymode;
    
    // Sending command for Humidity Measurement Mode
    // Read the conversion Results
    // 16-bit unsigned Results for the MS8607_02BA01
    writeRegister8(ms_i2cAddressRH, humidityMode);
    delay(ms_conversionDelay);
    // Digital Relative Humidity Value
    uint8_t data[3];
    uint8_t crc;
    Wire.requestFrom((uint8_t)ms_i2cAddressRH, (uint8_t)3);
    for (uint8_t i = 0; i < 3; i++)
    {
        data[i] = i2cread();
    }
    
    uint16_t D3 = (data[0] << 8) | data[1];
    // crc = data[2];

    // Final Calculations
    // Actual Relative Humidity (-6 %RH…118%RH with 0.01 %RH resolution)
    float RH = (-6 + 125.0 * D3 / 65536.0);
    
    // Result of Compensated Humidity
    // RHcompensated = RH + ( 20 - TEMP) * Tcoeff
    float RH_Comp = RH + (20 - ms_sensorData.T) * (float(- 0.18));
    ms_sensorData.H = (float)RH_Comp;
    
}
