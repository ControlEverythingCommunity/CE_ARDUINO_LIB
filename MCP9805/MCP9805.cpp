/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MCP9805
        This code is designed to work with the MCP9805_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=MCP9805_I2CS#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "MCP9805.h"

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
static void writeRegister8(uint8_t i2cAddress, uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    i2cwrite((uint8_t)(value));
    Wire.endTransmission();
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
        Reads 16-bits to the specified destination register
*/
/**************************************************************************/
static uint16_t readRegister(uint8_t i2cAddress, uint8_t reg)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)2);
    return (int16_t)((i2cread()<< 8) | i2cread());
}

/**************************************************************************/
/*
        Instantiates a new MCP9805 class with appropriate properties
*/
/**************************************************************************/
void MCP9805::getAddr_MCP9805(uint8_t i2cAddress)
{
    mcp_i2cAddress = i2cAddress;
    mcp_conversionDelay = MCP9805_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
boolean MCP9805::begin()
{
    Wire.begin();
    
    if (readRegister(mcp_i2cAddress, MCP9805_REG_POINTER_MANUFID) != 0x0054) return false;
    if (readRegister(mcp_i2cAddress, MCP9805_REG_POINTER_DEVID) != 0x0400) return false;
    
    return true;
}

/**************************************************************************/
/*
        Sets the TUPPER and TLOWER Limit Hysteresis
        This can not be altered when either of the Lock bits are set.
        This can be programmed in Shutdown mode.
*/
/**************************************************************************/
void MCP9805::setTHyst(mcpTHyst_t thyst)
{
    mcp_thyst = thyst;
}

/**************************************************************************/
/*
        Gets the TUPPER and TLOWER Limit Hysteresis
*/
/**************************************************************************/
mcpTHyst_t MCP9805::getTHyst()
{
    return mcp_thyst;
}

/**************************************************************************/
/*
        Sets the Shutdown Mode
*/
/**************************************************************************/
void MCP9805::setTShdn(mcpTShdn_t tshdn)
{
    mcp_tshdn = tshdn;
}

/**************************************************************************/
/*
        Gets the Shutdown Mode
*/
/**************************************************************************/
mcpTShdn_t MCP9805::getTShdn()
{
  return mcp_tshdn;
}

/**************************************************************************/
/*
        Sets the TCRIT Lock
*/
/**************************************************************************/
void MCP9805::setTCrit(mcpTCrit_t tcrit)
{
    mcp_tcrit = tcrit;
}

/**************************************************************************/
/*
        Gets the TCRIT Lock
*/
/**************************************************************************/
mcpTCrit_t MCP9805::getTCrit()
{
    return mcp_tcrit;
}

/**************************************************************************/
/*
        Sets the TUPPER and TLOWER Window Lock
*/
/**************************************************************************/
void MCP9805::setTWin(mcpTWin_t twin)
{
    mcp_twin = twin;
}

/**************************************************************************/
/*
        Gets the TUPPER and TLOWER Window Lock
*/
/**************************************************************************/
mcpTWin_t MCP9805::getTWin()
{
    return mcp_twin;
}

/**************************************************************************/
/*
        Sets the Interrupt Clear bit
*/
/**************************************************************************/
void MCP9805::setInctl(mcpInctl_t inctl)
{
    mcp_inctl = inctl;
}

/**************************************************************************/
/*
        Gets the Interrupt Clear bit
*/
/**************************************************************************/
mcpInctl_t MCP9805::getInctl()
{
    return mcp_inctl;
}

/**************************************************************************/
/*
        Sets the Alert Output Status
*/
/**************************************************************************/
void MCP9805::setAlertStat(mcpAlertStat_t alertstat)
{
    mcp_alertstat = alertstat;
}

/**************************************************************************/
/*
        Gets the Alert Output Status
*/
/**************************************************************************/
mcpAlertStat_t MCP9805::getAlertStat()
{
    return mcp_alertstat;
}

/**************************************************************************/
/*
        Sets the Alert Output Control
*/
/**************************************************************************/
void MCP9805::setAlertCtrl(mcpAlertCtrl_t alertctrl)
{
    mcp_alertctrl = alertctrl;
}

/**************************************************************************/
/*
        Gets the Alert Output Control
*/
/**************************************************************************/
mcpAlertCtrl_t MCP9805::getAlertCtrl()
{
    return mcp_alertctrl;
}

/**************************************************************************/
/*
        Sets the Alert Output Select
*/
/**************************************************************************/
void MCP9805::setAlertSlct(mcpAlertSlct_t alertslct)
{
    mcp_alertslct = alertslct;
}

/**************************************************************************/
/*
        Gets the Alert Output Select
*/
/**************************************************************************/
mcpAlertSlct_t MCP9805::getAlertSlct()
{
    return mcp_alertslct;
}

/**************************************************************************/
/*
        Sets the Alert Output Polarity
*/
/**************************************************************************/
void MCP9805::setAlertPol(mcpAlertPol_t alertpol)
{
    mcp_alertpol = alertpol;
}

/**************************************************************************/
/*
        Gets the Alert Output Polarity
*/
/**************************************************************************/
mcpAlertPol_t MCP9805::getAlertPol()
{
    return mcp_alertpol;
}

/**************************************************************************/
/*
        Sets the Alert Output Mode
*/
/**************************************************************************/
void MCP9805::setAlertMod(mcpAlertMod_t alertmod)
{
    mcp_alertmod = alertmod;
}

/**************************************************************************/
/*
        Gets the Alert Output Mode
*/
/**************************************************************************/
mcpAlertMod_t MCP9805::getAlertMod()
{
    return mcp_alertmod;
}

/**************************************************************************/
/*
        Sets the Resolution
*/
/**************************************************************************/
void MCP9805::setRes(mcpRes_t res)
{
    mcp_res = res;
}

/**************************************************************************/
/*
        Gets the Resolution
*/
/**************************************************************************/
mcpRes_t MCP9805::getRes()
{
    return mcp_res;
}

/**************************************************************************/
/*
        Reads the results, measuring the 16-bit temperature register
*/
/**************************************************************************/
int16_t MCP9805::Measure_Temp()
{
  
    // Start with default values
    uint16_t config =   MCP9805_REG_CONFIG_TCRIT_UNLOCK     |    // Unlocked. TCRIT register can be written (power-up default)
                        MCP9805_REG_CONFIG_TWIN_UNLOCK      |       // Unlocked; TUPPER and TLOWER registers can be written (power-up default)
                        MCP9805_REG_CONFIG_INTCL_NOEFF      |        // No effect (power-up default)
                        MCP9805_REG_CONFIG_ALERTSTAT_NOASRT |   // Alert output is not asserted by the device (power-up default)
                        MCP9805_REG_CONFIG_ALERTCNT_DISABLE |   // Disabled (power-up default)
                        MCP9805_REG_CONFIG_ALERTSEL_ALL     |       // Alert output for TUPPER, TLOWER and TCRIT (power-up default)
                        MCP9805_REG_CONFIG_ALERTPOL_LOW     |       // Active-low (power-up default; pull-up resistor required)
                        MCP9805_REG_CONFIG_ALERTMOD_COMP;       // Comparator output (power-up default)
    
    // Set TUPPER and TLOWER Limit Hysteresis
    config |= mcp_thyst;
    
    // Set Shutdown Mode
    config |= mcp_tshdn;

    // Write config register to the MCP
    writeRegister(mcp_i2cAddress, MCP9805_REG_POINTER_CONFIG, config);
    
    uint8_t resolution = mcp_res;
    // Write resolution register to the MCP
    writeRegister8(mcp_i2cAddress, MCP9805_REG_POINTER_RES, resolution);

    // Wait for the conversion to complete
    delay(mcp_conversionDelay);

    // Read the conversion results
    // 16-bit unsigned results for the MCP9805
    uint16_t raw_temp = readRegister(mcp_i2cAddress, MCP9805_REG_POINTER_TEMP);
    raw_temp &= 0x1FFF;
    return (int16_t)raw_temp;
}
