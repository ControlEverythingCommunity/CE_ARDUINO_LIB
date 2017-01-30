/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ITG-3200
        This code is designed to work with the ITG-3200_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Gyro?sku=ITG-3200_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "ITG_3200.h"

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
        Writes 8-bits to the specified destination Register
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
        Reads 8-bits to the specified destination Register
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
        Instantiates a new ITG_3200 class with appropriate properties
*/
/**************************************************************************/
void ITG_3200::getAddr_ITG_3200(uint8_t i2cAddress)
{
    itg_i2cAddress = i2cAddress;
    itg_ConversionDelay = ITG_3200_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool ITG_3200::begin()
{
    Wire.begin();
    
    // Set up the sensor for the Gyroscope
    // setUpGyoscope();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Full-Scale Range of the Gyro Sensors
*/
/**************************************************************************/
void ITG_3200::setFSRange(itgFSRange_t fsrange)
{
    itg_fsrange = fsrange;
}
                    
/**************************************************************************/
/*
        Gets the Full-Scale Range of the Gyro Sensors
*/
/**************************************************************************/
itgFSRange_t ITG_3200::getFSRange()
{
    return itg_fsrange;
}

/**************************************************************************/
/*
        Sets the Digital Low Pass Filter Configuration
*/
/**************************************************************************/
void ITG_3200::setDLPFBandwidth(itgDLPFBandwidth_t dlpfbandwidth)
{
    itg_dlpfbandwidth = dlpfbandwidth;
}

/**************************************************************************/
/*
        Gets the Digital Low Pass Filter Configuration
*/
/**************************************************************************/
itgDLPFBandwidth_t ITG_3200::getDLPFBandwidth()
{
    return itg_dlpfbandwidth;
}

/**************************************************************************/
/*
        Sets the Logic Level for INT Output Pin
*/
/**************************************************************************/
void ITG_3200::setINTLogic(itgINTLogic_t intlogic)
{
    itg_intlogic = intlogic;
}

/**************************************************************************/
/*
        Gets the Logic Level for INT Output Pin
*/
/**************************************************************************/
itgINTLogic_t ITG_3200::getINTLogic()
{
    return itg_intlogic;
}

/**************************************************************************/
/*
        Sets the Drive Type for INT Output Pin
*/
/**************************************************************************/
void ITG_3200::setINTDrive(itgINTDrive_t intdrive)
{
    itg_intdrive = intdrive;
}

/**************************************************************************/
/*
        Gets the Drive Type for INT Output Pin
*/
/**************************************************************************/
itgINTDrive_t ITG_3200::getINTDrive()
{
    return itg_intdrive;
}

/**************************************************************************/
/*
        Sets the Latch Mode
*/
/**************************************************************************/
void ITG_3200::setLatchMode(itgLatchMode_t latchmode)
{
    itg_latchmode = latchmode;
}

/**************************************************************************/
/*
        Gets the Latch Mode
*/
/**************************************************************************/
itgLatchMode_t ITG_3200::getLatchMode()
{
    return itg_latchmode;
}

/**************************************************************************/
/*
        Sets the Latch Clear Method
*/
/**************************************************************************/
void ITG_3200::setLatchClear(itgLatchClear_t latchclear)
{
    itg_latchclear = latchclear;
}

/**************************************************************************/
/*
    Gets the Latch Clear Method
*/
/**************************************************************************/
itgLatchClear_t ITG_3200::getLatchClear()
{
    return itg_latchclear;
}

/**************************************************************************/
/*
        Sets the Status of Interrupt when Device is Ready
        (PLL ready after changing clock source)
*/
/**************************************************************************/
void ITG_3200::setINTDevice(itgINTDevice_t intdevice)
{
    itg_intdevice = intdevice;
}

/**************************************************************************/
/*
        Gets the Status of Interrupt when Device is Ready
        (PLL ready after changing clock source)
 */
/**************************************************************************/
itgINTDevice_t ITG_3200::getINTDevice()
{
    return itg_intdevice;
}

/**************************************************************************/
/*
        Sets the Status of Interrupt when Data is Ready
*/
/**************************************************************************/
void ITG_3200::setINTData(itgINTData_t intdata)
{
    itg_intdata = intdata;
}

/**************************************************************************/
/*
        Gets the Status of Interrupt when Data is Ready
*/
/**************************************************************************/
itgINTData_t ITG_3200::getINTData()
{
    return itg_intdata;
}

/**************************************************************************/
/*
        Sets the Status of PLL Ready
*/
/**************************************************************************/
void ITG_3200::setPLLReady(itgPLLReady_t pllready)
{
    itg_pllready = pllready;
}

/**************************************************************************/
/*
        Gets the Status of PLL Ready
*/
/**************************************************************************/
itgPLLReady_t ITG_3200::getPLLReady()
{
    return itg_pllready;
}

/**************************************************************************/
/*
        Sets the Status of Raw Data Ready
*/
/**************************************************************************/
void ITG_3200::setRawDataReady(itgRawDataReady_t rawdataready)
{
    itg_rawdataready = rawdataready;
}

/**************************************************************************/
/*
        Gets the Status of Raw Data Ready
*/
/**************************************************************************/
itgRawDataReady_t ITG_3200::getRawDataReady()
{
    return itg_rawdataready;
}

/**************************************************************************/
/*
        Sets the Status of Device Reset
        and Internal Registers to the Power-Up-Default Settings
*/
/**************************************************************************/
void ITG_3200::setDeviceReset(itgDeviceReset_t devicereset)
{
    itg_devicereset = devicereset;
}

/**************************************************************************/
/*
        Gets the Status of Device Reset
*/
/**************************************************************************/
itgDeviceReset_t ITG_3200::getDeviceReset()
{
    return itg_devicereset;
}

/**************************************************************************/
/*
        Sets the Low Power Sleep Mode
*/
/**************************************************************************/
void ITG_3200::setSleepMode(itgSleepMode_t sleepmode)
{
    itg_sleepmode = sleepmode;
}

/**************************************************************************/
/*
        Gets the Low Power Sleep Mode
*/
/**************************************************************************/
itgSleepMode_t ITG_3200::getSleepMode()
{
    return itg_sleepmode;
}

/**************************************************************************/
/*
        Sets the Gyro X Mode
*/
/**************************************************************************/
void ITG_3200::setGyroXMode(itgGyroXMode_t gyroxmode)
{
    itg_gyroxmode = gyroxmode;
}

/**************************************************************************/
/*
        Gets the Gyro X Mode
*/
/**************************************************************************/
itgGyroXMode_t ITG_3200::getGyroXMode()
{
    return itg_gyroxmode;
}

/**************************************************************************/
/*
        Sets the Gyro Y Mode
*/
/**************************************************************************/
void ITG_3200::setGyroYMode(itgGyroYMode_t gyroymode)
{
    itg_gyroymode = gyroymode;
}

/**************************************************************************/
/*
        Gets the Gyro Y Mode
*/
/**************************************************************************/
itgGyroYMode_t ITG_3200::getGyroYMode()
{
    return itg_gyroymode;
}

/**************************************************************************/
/*
        Sets the Gyro Z Mode
*/
/**************************************************************************/
void ITG_3200::setGyroZMode(itgGyroZMode_t gyrozmode)
{
    itg_gyrozmode = gyrozmode;
}

/**************************************************************************/
/*
        Gets the Gyro Z Mode
*/
/**************************************************************************/
itgGyroZMode_t ITG_3200::getGyroZMode()
{
    return itg_gyrozmode;
}

/**************************************************************************/
/*
        Sets the Clock Source
*/
/**************************************************************************/
void ITG_3200::setClockSource(itgClockSource_t clocksource)
{
    itg_clocksource = clocksource;
}

/**************************************************************************/
/*
        Gets the Clock Source
*/
/**************************************************************************/
itgClockSource_t ITG_3200::getClockSource()
{
    return itg_clocksource;
}

/**************************************************************************/
/*
        Set up the Gyroscope
*/
/**************************************************************************/
void ITG_3200::setUpGyoscope(void)
{
    // Set up the DLPF, Full Scale Register
    // Set the Full-Scale Range for the Gyro Sensors
    uint8_t dlpf = itg_fsrange;
    
    // Set the Digital Low Pass Filter Configuration
    dlpf |= itg_dlpfbandwidth;
    
    // Write the configuration to DLPF, Full Scale Register
    writeRegister(itg_i2cAddress, ITG_3200_REG_GYRO_DLPF_FS, dlpf);
    
    // Wait for the configuration to complete
    delay(itg_ConversionDelay);
    
    // Set up the Interrupt Configuration Register
    // Set the Logic Level for INT Output Pin
    uint8_t interrupt = itg_intlogic;
    
    // Set the Drive Type for INT Output Pin
    interrupt |= itg_intdrive;
    
    // Set the Latch Mode
    interrupt |= itg_latchmode;
    
    // Set the Latch Clear Method
    interrupt |= itg_latchclear;
    
    // Set the Interrupt Status when Device is Ready (PLL ready after changing clock source)
    interrupt |= itg_intdevice;
    
    // Set the Interrupt Status when Data is Available
    interrupt |= itg_intdata;
    
    // Write the configuration to Interrupt Configuration Register
    writeRegister(itg_i2cAddress, ITG_3200_REG_GYRO_INT_CFG, interrupt);
    
    // Wait for the configuration to complete
    delay(itg_ConversionDelay);
    
    // Set up the Interrupt Status Register
    // Set the PLL Ready Status
    uint8_t interrupt_status = itg_pllready;
    
    // Set the Raw Data Ready Status
    interrupt_status |= itg_rawdataready;
    
    // Write the configuration to Interrupt Status Register
    writeRegister(itg_i2cAddress, ITG_3200_REG_GYRO_INT_STATUS, interrupt_status);
    
    // Set up the Power Management Register
    // Set the Device Reset Status and Internal Registers to the Power-Up-Default Settings
    uint8_t power_manage = itg_devicereset;
    
    // Set the Low Power Sleep Mode Status
    power_manage |= itg_sleepmode;
    
    // Set the Gyro X Mode Status
    power_manage |= itg_gyroxmode;
    
    // Set the Gyro Y Mode Status
    power_manage |= itg_gyroymode;
    
    // Set the Gyro Z Mode Status
    power_manage |= itg_gyrozmode;
    
    // Set the Device Clock Source
    power_manage |= itg_clocksource;
    
    // Write the configuration to Interrupt Status Register
    writeRegister(itg_i2cAddress, ITG_3200_REG_GYRO_GYRO_PWR_MGM, power_manage);
    
    // Wait for the configuration to complete
    delay(itg_ConversionDelay);
}

/**************************************************************************/
/*
        Reads the results for the Gyro Sensor Data for the Gyoscope ITG_3200
*/
/**************************************************************************/
void ITG_3200::Measure_Gyroscope(void)
{
    // Read the Gyoscope for Gyro Data
    uint8_t xGyroHi, xGyroLo, yGyroHi, yGyroLo, zGyroHi, zGyroLo;
    
    // Read the conversion results
    // Reading the High data of Output X Register
    xGyroHi = readRegister(itg_i2cAddress, ITG_3200_REG_GYRO_GYRO_XOUT_H);
    // Reading the Low data of Output X Register
    xGyroLo = readRegister(itg_i2cAddress, ITG_3200_REG_GYRO_GYRO_XOUT_L);
    // Conversion of the result
    // 16-bit signed result for Channel X of ITG_3200
    itg_gyrodata.X = (int16_t)((xGyroHi << 8) | xGyroLo);
    
    // Reading the High data of Output Y Register
    yGyroHi = readRegister(itg_i2cAddress, ITG_3200_REG_GYRO_GYRO_YOUT_H);
    // Reading the Low data of Output Y Register
    yGyroLo = readRegister(itg_i2cAddress, ITG_3200_REG_GYRO_GYRO_YOUT_L);
    // Conversion of the result
    // 16-bit signed result for Channel Y of ITG_3200
    itg_gyrodata.Y = (int16_t)((yGyroHi << 8) | yGyroLo);
    
    // Reading the High data of Output Z Register
    zGyroHi = readRegister(itg_i2cAddress, ITG_3200_REG_GYRO_GYRO_ZOUT_H);
    // Reading the Low data of Output Z Register
    zGyroLo = readRegister(itg_i2cAddress, ITG_3200_REG_GYRO_GYRO_ZOUT_L);
    // Conversion of the result
    // 16-bit signed result for Channel Z of ITG_3200
    itg_gyrodata.Z = (int16_t)((zGyroHi << 8) | zGyroLo);
}

/**************************************************************************/
/*
        Reads the results for the Temperature Sensor Data for the Gyoscope ITG_3200
*/
/**************************************************************************/
void ITG_3200::Measure_Temperature(void)
{
    // Read the Gyoscope for DIE Temperature Data
    uint8_t TempHi, TempLo;
    int16_t rawTemp;
    
    // Read the conversion results
    // Reading the High data of Temperature Register
    TempHi = readRegister(itg_i2cAddress, ITG_3200_REG_GYRO_TEMP_OUT_H);
    // Reading the Low data of Temperature Register
    TempLo = readRegister(itg_i2cAddress, ITG_3200_REG_GYRO_TEMP_OUT_L);
    // Conversion of the result
    // 16-bit unsigned result for Channel X of ITG_3200
    rawTemp = (uint16_t)((TempHi << 8) | TempLo);
    // Temperature Offset
    rawTemp = -13200 - rawTemp ;
    // Convert the Offset and Add 35 degrees C to Compensate for the Offset
    itg_gyrodata.T = (float)((float)(rawTemp / 280.0) + 35);
}

