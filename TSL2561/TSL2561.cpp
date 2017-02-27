/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TSL2561
        This code is designed to work with the TSL2561_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Light?sku=TSL2561_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "TSL2561.h"

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
        Reads 16-bits from the specified destination register
*/
/**************************************************************************/
static uint16_t readRegister(uint8_t i2cAddress, uint8_t reg)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)2);
    return (uint16_t)(i2cread() | (i2cread() << 8));
}

/**************************************************************************/
/*
        Instantiates a new TSL2561 class with appropriate properties
*/
/**************************************************************************/
void TSL2561::getAddr_TSL2561(uint8_t i2cAddress)
{
    tsl_i2cAddress = i2cAddress;
    tsl_conversionDelay = TSL2561_CONVERSIONDELAY;
    
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool TSL2561::begin()
{
    Wire.begin();
    
    // Set up for Light-to-Digital Converter
    setUpALS();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Select Command Register
*/
/**************************************************************************/
void TSL2561::setCommand(tslCommand_t command)
{
    tsl_command = command;
}

/**************************************************************************/
/*
        Gets the Select Command Register
*/
/**************************************************************************/
tslCommand_t TSL2561::getCommand()
{
    return tsl_command;
}

/**************************************************************************/
/*
        Sets the Power ON
*/
/**************************************************************************/
void TSL2561::setPowerEnable(tslPowerEnable_t powerenable)
{
    tsl_powerenable = powerenable;
}

/**************************************************************************/
/*
        Gets the Power ON
*/
/**************************************************************************/
tslPowerEnable_t TSL2561::getPowerEnable()
{
    return tsl_powerenable;
}

/**************************************************************************/
/*
        Sets the Gain Between Low Gain and High Gain Modes
*/
/**************************************************************************/
void TSL2561::setGain(tslGain_t gain)
{
    tsl_gain = gain;
}

/**************************************************************************/
/*
        Gets the Gain Between Low Gain and High Gain Modes
*/
/**************************************************************************/
tslGain_t TSL2561::getGain()
{
    return tsl_gain;
}

/**************************************************************************/
/*
        Sets the Integrate Time
*/
/**************************************************************************/
void TSL2561::setIntegTime(tslIntegTime_t integtime)
{
    tsl_integtime = integtime;
}

/**************************************************************************/
/*
        Sets the INTR Control Select
        This Field Determines Mode of Interrupt Logic
*/
/**************************************************************************/
void TSL2561::setInterruptControl(tslInterruptControl_t interruptcontrol)
{
    tsl_interruptcontrol = interruptcontrol;
}

/**************************************************************************/
/*
        Gets the INTR Control Select
        This Field Determines Mode of Interrupt Logic
*/
/**************************************************************************/
tslInterruptControl_t TSL2561::getInterruptControl()
{
    return tsl_interruptcontrol;
}

/**************************************************************************/
/*
        Sets the Interrupt Persistence
        Controls Rate of Interrupts to the Host Processor
*/
/**************************************************************************/
void TSL2561::setInterruptPersistence(tslInterruptPersistence_t interruptpersistence)
{
    tsl_interruptpersistence = interruptpersistence;
}

/**************************************************************************/
/*
        Gets the Interrupt Persistence
        Controls Rate of Interrupts to the Host Processor
*/
/**************************************************************************/
tslInterruptPersistence_t TSL2561::getInterruptPersistence()
{
    return tsl_interruptpersistence;
}

/**************************************************************************/
/*
        Sets up the Light-to-Digital Converter
*/
/**************************************************************************/
void TSL2561::setUpALS(void)
{
    // Set Up the Configuration for the Light-to-Digital Converter Timing Register
    // Set the Gain Between Low Gain and High Gain Modes
    uint8_t timing = tsl_gain;
    
    // Set the Integrate Time
    timing |= tsl_integtime;
    
    // Write the configuration to the Light-to-Digital Converter Timing Register
    writeRegister(tsl_i2cAddress, TSL2561_REG_ALS_TIMING | COMMAND_SELECT, timing);
    
    // Wait for the configuration to complete
    delay(tsl_conversionDelay);
    
    // Set Up the Configuration for the Light-to-Digital Converter Interrupt Control Register
    /*
     // Set the INTR Control Select
     uint8_t intr_control = tsl_interruptcontrol;
     
     // Set the Interrupt Persistence
     intr_control |= tsl_interruptpersistence;
    */
    
    uint8_t intr_control =  TSL2561_REG_ALS_INTERRUPT_INTR_DISABLE        |   // Interrupt Output Disabled
                            TSL2561_REG_ALS_INTERRUPT_PERSIST_EVERY;          // Every ADC Cycle Generates Interrupt
    
    // Write the configuration to the Light-to-Digital Converter Interrupt Control Register
    writeRegister(tsl_i2cAddress, TSL2561_REG_ALS_INTERRUPT | COMMAND_SELECT, intr_control);
    
    // Wait for the configuration to complete
    delay(tsl_conversionDelay);
    
    // Set Up the Configuration for the Light-to-Digital Converter Control Register
    /*
     // Set the Power ON
     uint8_t control = tsl_powerenable;
    */
    
    uint8_t control = TSL2561_REG_ALS_CONTROL_POWER_ENABLE;     // Powers On the Device

    // Write the configuration to the Light-to-Digital Converter Control Register
    writeRegister(tsl_i2cAddress, TSL2561_REG_ALS_CONTROL | COMMAND_SELECT, control);
    
    // Wait for the configuration to complete
    delay(tsl_conversionDelay);
}

/**************************************************************************/
/*
        Calculate the Approximate Illuminance (lux) 
        Given the Raw Channel Values of the TSL2561
*/
/**************************************************************************/
uint32_t TSL2561::Measure_ALS(uint16_t ch0, uint16_t ch1)
{
    unsigned long chScale;
    unsigned long channel1;
    unsigned long channel0;
    
    switch (tsl_integtime)
    {
        case INTEGRATIONTIME_13_7MS:
            chScale = TSL2561_LUX_CHSCALE_TINT0;
            break;
        case INTEGRATIONTIME_101MS:
            chScale = TSL2561_LUX_CHSCALE_TINT1;
            break;
        default: // Assume No Scaling
            chScale = (1 << TSL2561_LUX_CHSCALE);
            break;
    }
    
    // Scale if Gain is NOT 16X
    if (!tsl_gain)
    {
       chScale = chScale << 4;
    }
    
    // Scale the Channel Values
    channel0 = (ch0 * chScale) >> TSL2561_LUX_CHSCALE;
    channel1 = (ch1 * chScale) >> TSL2561_LUX_CHSCALE;
    
    // Find the Ratio of the Channel Values (Channel1/Channel0)
    // Protect Against Divide by Zero
    unsigned long ratio1 = 0;
    if (channel0 != 0) ratio1 = (channel1 << (TSL2561_LUX_RATIOSCALE+1)) / channel0;
    
    // Round the Ratio Value
    unsigned long ratio = (ratio1 + 1) >> 1;
    
    // is Ratio <= eachBreak ?
    unsigned int b, m;
    
    #ifdef TSL2561_PACKAGE_CS
        if ((ratio >= 0) && (ratio <= TSL2561_LUX_K1C))
        {
            b=TSL2561_LUX_B1C; m=TSL2561_LUX_M1C;
        }
    else if (ratio <= TSL2561_LUX_K2C)
        {
            b=TSL2561_LUX_B2C; m=TSL2561_LUX_M2C;
        }
    else if (ratio <= TSL2561_LUX_K3C)
        {
            b=TSL2561_LUX_B3C; m=TSL2561_LUX_M3C;
        }
    else if (ratio <= TSL2561_LUX_K4C)
        {
            b=TSL2561_LUX_B4C; m=TSL2561_LUX_M4C;
        }
    else if (ratio <= TSL2561_LUX_K5C)
        {
            b=TSL2561_LUX_B5C; m=TSL2561_LUX_M5C;
        }
    else if (ratio <= TSL2561_LUX_K6C)
        {
            b=TSL2561_LUX_B6C; m=TSL2561_LUX_M6C;
        }
    else if (ratio <= TSL2561_LUX_K7C)
        {
            b=TSL2561_LUX_B7C; m=TSL2561_LUX_M7C;
        }
    else if (ratio > TSL2561_LUX_K8C)
        {
            b=TSL2561_LUX_B8C; m=TSL2561_LUX_M8C;
        }
    #else
    if ((ratio >= 0) && (ratio <= TSL2561_LUX_K1T))
        {
            b=TSL2561_LUX_B1T; m=TSL2561_LUX_M1T;
        }
    else if (ratio <= TSL2561_LUX_K2T)
        {
            b=TSL2561_LUX_B2T; m=TSL2561_LUX_M2T;
        }
    else if (ratio <= TSL2561_LUX_K3T)
        {
            b=TSL2561_LUX_B3T; m=TSL2561_LUX_M3T;
        }
    else if (ratio <= TSL2561_LUX_K4T)
        {
            b=TSL2561_LUX_B4T; m=TSL2561_LUX_M4T;
        }
    else if (ratio <= TSL2561_LUX_K5T)
        {
            b=TSL2561_LUX_B5T; m=TSL2561_LUX_M5T;
        }
    else if (ratio <= TSL2561_LUX_K6T)
        {
            b=TSL2561_LUX_B6T; m=TSL2561_LUX_M6T;
        }
    else if (ratio <= TSL2561_LUX_K7T)
        {
            b=TSL2561_LUX_B7T; m=TSL2561_LUX_M7T;
        }
    else if (ratio > TSL2561_LUX_K8T)
        {
            b=TSL2561_LUX_B8T; m=TSL2561_LUX_M8T;
        }
    #endif
    
    unsigned long temp;
    temp = ((channel0 * b) - (channel1 * m));
    
    // Do Not Allow Negative lux Value
    if (temp < 0) temp = 0;
    
    // Round lsb (2^(LUX_SCALE-1))
    temp += (1 << (TSL2561_LUX_LUXSCALE-1));
    
    // Strip off Fractional Portion
    uint32_t lux = temp >> TSL2561_LUX_LUXSCALE;
    
    return lux;
}

/**************************************************************************/
/*
        Reads the Light Sensor Data
        Consists of Full Spectrum (Visible + Infrared) and Infrared
*/
/**************************************************************************/
void TSL2561::getLuminosity (void)
{
    if (!false)
    {
        begin();
    }
    
    // Wait x ms for ADC to complete
    switch (tsl_integtime)
    {
        case INTEGRATIONTIME_13_7MS:
            delay(14);
            break;
        case INTEGRATIONTIME_101MS:
            delay(101);
            break;
        default:
            delay(402);
            break;
    }
    
    // Reading the Channel 0 Data Register
    // 16-bit unsigned result for Full Spectrum (Visible + Infrared) Data of TSL2561
    tsl_alsData.F = readRegister(tsl_i2cAddress, COMMAND_SELECT | COMMAND_WORD | TSL2561_REG_ALS_C0DATAL);
    
    // Reading the Channel 1 Data Register
    // 16-bit unsigned result for Infrared Data of TSL2561
     tsl_alsData.IR = readRegister(tsl_i2cAddress, COMMAND_SELECT | COMMAND_WORD | TSL2561_REG_ALS_C1DATAL);

    // Conversion of the Visible Result
    // 16-bit unsigned result for Visible Data of TSL2561
    tsl_alsData.V = tsl_alsData.F - tsl_alsData.IR;
}
