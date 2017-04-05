/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TMP007
        This code is designed to work with the TMP007_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=TMP007_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "TMP007.h"

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
    Wire.endTransmission();
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
    return (int16_t)((i2cread() << 8) | i2cread());
}

/**************************************************************************/
/*
        Instantiates a new TMP007 class with appropriate properties
*/
/**************************************************************************/
void TMP007::getAddr_TMP007(uint8_t i2cAddress)
{
    tmp_i2cAddress = i2cAddress;
    tmp_conversionDelay = TMP007_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool TMP007::begin()
{
    Wire.begin();
    
    uint8_t devid = readRegister(tmp_i2cAddress, TMP007_REG_DEV_ID);
    if (devid != TMP007_DEV_ID)
        return false;
    
    // Set up the Sensor
    setConfiguration();
    return true;
}

/**************************************************************************/
/*
        Sets the Software Reset Bit
        Writing 1 to this bit generates a system reset that is the same as
        power on reset. It will reset all registers to default values including
        configuration register. This bit self-clears. 
        Any conversion in progress is terminated.
*/
/**************************************************************************/
void TMP007::setReset(tmpReset_t reset)
{
    tmp_reset = reset;
}

/**************************************************************************/
/*
        Gets the Software Reset Bit
*/
/**************************************************************************/
tmpReset_t TMP007::getReset()
{
    return tmp_reset;
}

/**************************************************************************/
/*
        Sets the Conversion Mode Select
*/
/**************************************************************************/
void TMP007::setPowerMode(tmpPowerMode_t powermode)
{
    tmp_powermode = powermode;
}

/**************************************************************************/
/*
        Gets the Conversion Mode Select
*/
/**************************************************************************/
tmpPowerMode_t TMP007::getPowerMode()
{
    return tmp_powermode;
}

/**************************************************************************/
/*
        Sets the Conversion Rate/Averaging Mode
        Controls the Number of conversions used to generate the 
        value in the VSensor and TDIE registers
*/
/**************************************************************************/
void TMP007::setConvRate(tmpConvRate_t convrate)
{
    tmp_convrate = convrate;
}

/**************************************************************************/
/*
        Gets the Conversion Rate/Averaging Mode
*/
/**************************************************************************/
tmpConvRate_t TMP007::getConvRate()
{
    return tmp_convrate;
}

/**************************************************************************/
/*
        Sets the Alert Pin Enable
        Makes ALERT pin controlled by the alert flag bit. The ALERT pin 
        is active low. The ALRTEN bit is mirrored in the status mask and 
        enable register. Writing to the ALRTEN bit in the status mask and 
        enable register also sets this bit, and vice versa.
*/
/**************************************************************************/
void TMP007::setAlert(tmpAlert_t alert)
{
    tmp_alert = alert;
}

/**************************************************************************/
/*
        Gets the Alert Pin Enable
*/
/**************************************************************************/
tmpAlert_t TMP007::getAlert()
{
    return tmp_alert;
}

/**************************************************************************/
/*
        Sets the Cumulative Alert Flag
        This flag is the logical OR of all enabled flags, and is cleared 
        when the status register is read in INT mode or at the end of a
        conversion when all enabled flags are 0 in COMP mode.
*/
/**************************************************************************/
void TMP007::setAlertFlag(tmpAlertFlag_t alertflag)
{
    tmp_alertflag = alertflag;
}

/**************************************************************************/
/*
        Gets the Cumulative Alert Flag
*/
/**************************************************************************/
tmpAlertFlag_t TMP007::getAlertFlag()
{
    return tmp_alertflag;
}

/**************************************************************************/
/*
        Sets the Transient Correction Enable
        Setting this bit turns on the transient correction enabling sensor 
        voltage and object temperature output filtering
*/
/**************************************************************************/
void TMP007::setTransient(tmpTransient_t transient)
{
    tmp_transient = transient;
}

/**************************************************************************/
/*
        Gets the Transient Correction Enable
*/
/**************************************************************************/
tmpTransient_t TMP007::getTransient()
{
    return tmp_transient;
}

/**************************************************************************/
/*
        Sets the Status Cumulative Alert Flag
        This flag is the logical OR of all enabled flags, and is cleared 
        when the status register is read in INT mode or at the end of a
        conversion when all enabled flags are 0 in COMP mode.
*/
/**************************************************************************/
void TMP007::setStatusAlertFlag(tmpStatusAlertFlag_t statusalertflag)
{
    tmp_statusalertflag = statusalertflag;
}

/**************************************************************************/
/*
        Gets the Status Cumulative Alert Flag
*/
/**************************************************************************/
tmpStatusAlertFlag_t TMP007::getStatusAlertFlag()
{
    return tmp_statusalertflag;
}
/**************************************************************************/
/*
        Sets the Status Conversion Ready Flag
        The conversion ready flag is provided to help coordinate one-shot 
        conversions for temperature measurements
*/
/**************************************************************************/
void TMP007::setStatusConversion(tmpStatusConversion_t statusconversion)
{
    tmp_statusconversion = statusconversion;
}

/**************************************************************************/
/*
        Gets the Status Conversion Ready Flag
*/
/**************************************************************************/
tmpStatusConversion_t TMP007::getStatusConversion()
{
    return tmp_statusconversion;
}

/**************************************************************************/
/*
        Sets the Configuration for the Temperature Sensor
*/
/**************************************************************************/
void TMP007::setConfiguration()
{
    // Set Up the Configuration for the Configuration Register
    /*
     // Set the Software Reset
     uint16_t config = tmp_reset;
     
     // Set the Conversion Mode
     config |= tmp_powermode;
     
     // Set the Conversion Rate/Averaging Mode
     config |= tmp_convrate;
     
     // Set the Alert Pin Enable 
     config |= tmp_alert;
     
     // Set the Cumulative Alert Flag
     config |= tmp_alertflag;
     
     // Set the Transient Correction Enable
     config |= tmp_transient;
     
     // Set the INT/COMP Mode
     config |= tmp_intcomp;
    */
    
    // Start with default values
    uint16_t config =   TMP007_REG_CONFIG_RST_DISABLED          |   // System Reset Disabled
                        TMP007_REG_CONFIG_MOD_ON                |   // Conversion ON
                        TMP007_REG_CONFIG_CR_16                 |   // No. of Avgs. per Conversion: 16, Total Conversion Time (s): 4.01
                        TMP007_REG_CONFIG_ALRTEN_ENABLE         |   // Alert Pin Enable
                        TMP007_REG_CONFIG_ALRTF_ENABLE          |   // Cumulative Alert Flag Enable
                        TMP007_REG_CONFIG_TC_ENABLE             |   // Transient Correction Enable
                        TMP007_REG_CONFIG_INT_COMP_INTERRUPT;       // Interrupt Mode

    // Write the configuration to the Configuration Register
    writeRegister(tmp_i2cAddress, TMP007_REG_CONFIG, config);

    // Wait for the conversion to complete
    delay(tmp_conversionDelay);

    // Set Up the Configuration for the Status Register
    /*
     // Set the Cumulative Alert Flag Enable
     uint16_t status = tmp_statusalertflag;
     
     // Set the Conversion Ready Flag Enable
     status |= tmp_statusconversion;
    */
    
    // Start with default values
    uint16_t status =   TMP007_REG_STATUS_ALRTF_ENABLE      |   // Cumulative Alert Flag Enable
                        TMP007_REG_STATUS_CRTF_ENABLE;          // Conversion Ready Flag Enable
    
    // Write the configuration to the Status Register
    writeRegister(tmp_i2cAddress, TMP007_REG_STATUS, status);
    
    // Wait for the conversion to complete
    delay(tmp_conversionDelay);
}

/**************************************************************************/
/*
        Reads the Digitized IR Sensor Voltage
        The value is expressed in 16 bit as two’s complement
*/
/**************************************************************************/
float TMP007::Measure_Voltage(void)
{
    // Read the conversion results
    // 16-bit unsigned results for the TMP007
    int16_t voltage = readRegister(tmp_i2cAddress, TMP007_REG_VSENSOR_VOLTAGE);
    
    float VSensor = voltage * 156.25;
    VSensor /= 1000.0;
    return VSensor;
}

/**************************************************************************/
/*
        Reads the TDIE Local Temperature Sensor
        The value is expressed in 16 bit as two’s complement
*/
/**************************************************************************/
float TMP007::Measure_DieTemp(void)
{
    // Read the conversion results
    // 16-bit unsigned results for the TMP007
    int16_t dietemp = readRegister(tmp_i2cAddress, TMP007_REG_TDIE_LOCAL_TEMP);
    dietemp >>= 2;
    
    float Tdie = dietemp * 0.03125;
    return Tdie;
}

/**************************************************************************/
/*
        Reads the TOBJECT Local Temperature Sensor
        The value is expressed in 16 bit as two’s complement
*/
/**************************************************************************/
float TMP007::Measure_ObjTemp(void)
{
    // Read the conversion results
    // 16-bit unsigned results for the TMP007
    int16_t objtemp = readRegister(tmp_i2cAddress, TMP007_REG_TOBJECT_TEMP);
    objtemp >>= 2;
    
    float Tobj = objtemp * 0.03125;
    return Tobj;
}
