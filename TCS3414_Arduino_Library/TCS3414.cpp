/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        TCS3414
        This code is designed to work with the TCS3414_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Color?sku=TCS3414_I2CS#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "TCS3414.h"

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
        Instantiates a new TCS3414 class with appropriate properties
*/
/**************************************************************************/
void TCS3414::getAddr_TCS3414(uint8_t i2cAddress)
{
    tcs_i2cAddress = i2cAddress;
    tcs_conversionDelay = TCS3414_CONVERSIONDELAY;
    
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool TCS3414::begin()
{
    Wire.begin();
    
    uint8_t devid = readRegister(tcs_i2cAddress, TCS3414_REG_COLOR_ID);
    if (devid != TCS3414_COLOR_DEV_ID)
        return false;
    
    // Set up for Digital Color Sensor
    setUpColor();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Select Command Register
*/
/**************************************************************************/
void TCS3414::setCommand(tcsCommand_t command)
{
    tcs_command = command;
}

/**************************************************************************/
/*
        Gets the Select Command Register
*/
/**************************************************************************/
tcsCommand_t TCS3414::getCommand()
{
    return tcs_command;
}

/**************************************************************************/
/*
        Sets the Four-Channel RGBC ADC Enable to Begin Integration
*/
/**************************************************************************/
void TCS3414::setColorEnable(tcsColorEnable_t colorenable)
{
    tcs_colorenable = colorenable;
}

/**************************************************************************/
/*
        Gets the Four-Channel RGBC ADC Enable to Begin Integration
*/
/**************************************************************************/
tcsColorEnable_t TCS3414::getColorEnable()
{
    return tcs_colorenable;
}

/**************************************************************************/
/*
        Sets the Power ON
*/
/**************************************************************************/
void TCS3414::setPowerEnable(tcsPowerEnable_t powerenable)
{
    tcs_powerenable = powerenable;
}

/**************************************************************************/
/*
        Gets the Power ON
*/
/**************************************************************************/
tcsPowerEnable_t TCS3414::getPowerEnable()
{
    return tcs_powerenable;
}

/**************************************************************************/
/*
        Sets the Sync Pin Edge
*/
/**************************************************************************/
void TCS3414::setSyncEdge(tcsSyncEdge_t syncedge)
{
    tcs_syncedge = syncedge;
}

/**************************************************************************/
/*
        Gets the Sync Pin Edge
*/
/**************************************************************************/
tcsSyncEdge_t TCS3414::getSyncEdge()
{
    return tcs_syncedge;
}

/**************************************************************************/
/*
        Sets the Preset Integration Time, Manual Integration (Via Serial Bus),
        or External Synchronization (SYNC IN) Modes
*/
/**************************************************************************/
void TCS3414::setIntegMode(tcsIntegMode_t integmode)
{
    tcs_integmode = integmode;
}

/**************************************************************************/
/*
        Gets the Preset Integration Time, Manual Integration (Via Serial Bus),
        or External Synchronization (SYNC IN) Modes
*/
/**************************************************************************/
tcsIntegMode_t TCS3414::getIntegMode()
{
    return tcs_integmode;
}

/**************************************************************************/
/*
        Sets the NOMINAL INTEGRATION TIME
*/
/**************************************************************************/
void TCS3414::setIntegParamTime(tcsIntegParamTime_t integparamtime)
{
    tcs_integparamtime = integparamtime;
}

/**************************************************************************/
/*
        Gets the NOMINAL INTEGRATION TIME
*/
/**************************************************************************/
tcsIntegParamTime_t TCS3414::getIntegParamTime()
{
    return tcs_integparamtime;
}

/**************************************************************************/
/*
        Sets the SYNC IN PULSE COUNT
*/
/**************************************************************************/
void TCS3414::setIntegParamPulse(tcsIntegParamPulse_t integparampulse)
{
    tcs_integparampulse = integparampulse;
}

/**************************************************************************/
/*
        Gets the SYNC IN PULSE COUNT
*/
/**************************************************************************/
tcsIntegParamPulse_t TCS3414::getIntegParamPulse()
{
    return tcs_integparampulse;
}

/**************************************************************************/
/*
        Sets the Stop ADC Integration on Interrupt
*/
/**************************************************************************/
void TCS3414::setInterruptStop(tcsInterruptStop_t interruptstop)
{
    tcs_interruptstop = interruptstop;
}

/**************************************************************************/
/*
        Gets the Stop ADC Integration on Interrupt
*/
/**************************************************************************/
tcsInterruptStop_t TCS3414::getInterruptStop()
{
    return tcs_interruptstop;
}

/**************************************************************************/
/*
        Sets the INTR Control Select
        This Field Determines Mode of Interrupt Logic
*/
/**************************************************************************/
void TCS3414::setInterruptControl(tcsInterruptControl_t interruptcontrol)
{
    tcs_interruptcontrol = interruptcontrol;
}

/**************************************************************************/
/*
        Gets the INTR Control Select
        This Field Determines Mode of Interrupt Logic
*/
/**************************************************************************/
tcsInterruptControl_t TCS3414::getInterruptControl()
{
    return tcs_interruptcontrol;
}

/**************************************************************************/
/*
        Sets the Interrupt Persistence
        Controls Rate of Interrupts to the Host Processor
*/
/**************************************************************************/
void TCS3414::setInterruptPersistence(tcsInterruptPersistence_t interruptpersistence)
{
    tcs_interruptpersistence = interruptpersistence;
}

/**************************************************************************/
/*
        Gets the Interrupt Persistence
        Controls Rate of Interrupts to the Host Processor
*/
/**************************************************************************/
tcsInterruptPersistence_t TCS3414::getInterruptPersistence()
{
    return tcs_interruptpersistence;
}

/**************************************************************************/
/*
        Sets the Interrupt Source
        Selects which ADC Channel to Use to Generate an Interrupt
*/
/**************************************************************************/
void TCS3414::setInterruptSource(tcsInterruptSource_t interruptsource)
{
    tcs_interruptsource = interruptsource;
}

/**************************************************************************/
/*
        Gets the Interrupt Source
        Selects which ADC Channel to Use to Generate an Interrupt
*/
/**************************************************************************/
tcsInterruptSource_t TCS3414::getInterruptSource()
{
    return tcs_interruptsource;
}

/**************************************************************************/
/*
        Sets the Analog Gain Control.
        This Field Switches the Common Analog Gain of the Four ADC Channels
*/
/**************************************************************************/
void TCS3414::setAnalogGain(tcsAnalogGain_t analoggain)
{
    tcs_analoggain = analoggain;
}

/**************************************************************************/
/*
        Gets the Analog Gain Control
        This Field Switches the Common Analog Gain of the Four ADC Channels
*/
/**************************************************************************/
tcsAnalogGain_t TCS3414::getAnalogGain()
{
    return tcs_analoggain;
}

/**************************************************************************/
/*
        Sets the Field that Controls a 6-bit Digital Prescaler and Divider
        The Prescaler Reduces the Sensitivity of each ADC Integrator
*/
/**************************************************************************/
void TCS3414::setPrescaler(tcsPrescaler_t prescaler)
{
    tcs_prescaler = prescaler;
}

/**************************************************************************/
/*
        Gets the Field that Controls a 6-bit Digital Prescaler and Divider
        The Prescaler Reduces the Sensitivity of each ADC Integrator
*/
/**************************************************************************/
tcsPrescaler_t TCS3414::getPrescaler()
{
    return tcs_prescaler;
}

/**************************************************************************/
/*
        Sets up the Digital Color Sensor
*/
/**************************************************************************/
void TCS3414::setUpColor(void)
{
    // Set Up the Configuration for the Digital Color Sensor Timing Register
    /*
     // Set the Sync Pin Edge
     uint8_t timing = tcs_syncedge;
    
     // Set the SYNC IN PULSE COUNT
     timing |= tcs_integparampulse;
    */
    
    uint8_t timing =    TCS3414_REG_COLOR_TIMING_SYNC_EDGE_LOW          |   // If SYNC_EDGE is Low, the Falling Edge of the Sync Pin is Used to Stop an Integration Cycle when INTEG_MODE is 11
                        TCS3414_REG_COLOR_TIMING_PARAM_PULSE_COUNT_1;       // SYNC IN PULSE COUNT: 1
    
    // Set the Preset Integration Time
    timing |= tcs_integmode;
    
    // Set the NOMINAL INTEGRATION TIME
    timing |= tcs_integparamtime;
    
    // Write the configuration to the Digital Color Sensor Timing Register
    writeRegister(tcs_i2cAddress, TCS3414_REG_COLOR_TIMING | COMMAND_WRITE | COMMAND_TRANC_BYTE, timing);
    
    // Wait for the configuration to complete
    delay(tcs_conversionDelay);
    
    // Set Up the Configuration for the Digital Color Sensor Interrupt Control Register
    /*
     // Set the Stop ADC Integration on Interrupt
     uint8_t intr_control = tcs_interruptstop;
     
     // Set the INTR Control Select
     intr_control |= tcs_interruptcontrol;
     
     // Set the INTR Control Select
     intr_control |= tcs_interruptpersistence;
    */
    
    uint8_t intr_control =   TCS3414_REG_COLOR_INTERRUPT_INTR_STOP_LOW      |   // When High, ADC Integration will Stop Once an Interrupt is Asserted
                            TCS3414_REG_COLOR_INTERRUPT_INTR_DISABLE        |   // Interrupt Output Disabled
                            TCS3414_REG_COLOR_INTERRUPT_PERSIST_EVERY;          // Every ADC Cycle Generates Interrupt
    
    // Write the configuration to the Digital Color Sensor Interrupt Control Register
    writeRegister(tcs_i2cAddress, TCS3414_REG_COLOR_INTERRUPT | COMMAND_WRITE | COMMAND_TRANC_BYTE, intr_control);
    
    // Wait for the configuration to complete
    delay(tcs_conversionDelay);
    
    // Set Up the Configuration for the Digital Color Sensor Interrupt Source Register
    /*
     // Set the Interrupt Source
     uint8_t intr_source = tcs_interruptsource;
     */
    
    uint8_t intr_source = TCS3414_REG_COLOR_INT_SOURCE_CLEAR;       // Clear Channel
    
    // Write the configuration to the Digital Color Sensor Interrupt Source Register
    writeRegister(tcs_i2cAddress, TCS3414_REG_COLOR_INT_SOURCE | COMMAND_WRITE | COMMAND_TRANC_BYTE, intr_source);
    
    // Wait for the configuration to complete
    delay(tcs_conversionDelay);
    
    // Set Up the Configuration for the Digital Color Sensor ADC Gain Control Register
    // Set the Analog Gain Control
    uint8_t gain_control = tcs_analoggain;
    
    // Set the 6-bit Digital Prescaler and Divider
    gain_control |= tcs_prescaler;
    
    // Write the configuration to the Digital Color Sensor ADC Gain Control Register
    writeRegister(tcs_i2cAddress, TCS3414_REG_COLOR_GAIN | COMMAND_WRITE | COMMAND_TRANC_BYTE, gain_control);
    
    // Wait for the configuration to complete
    delay(tcs_conversionDelay);
    
    // Set Up the Configuration for the Digital Color Sensor Control Register
    /*
     // Set the Four-Channel RGBC ADC Enable to Begin Integration
     uint8_t control = tcs_colorenable;
     
     // Set the Power ON
     control |= tcs_powerenable;
     */
    
    uint8_t control =   TCS3414_REG_COLOR_CONTROL_ADC_EN_ENABLE     |       // Activates the RGBC ADC
                        TCS3414_REG_COLOR_CONTROL_POWER_ENABLE;             // Powers On the Device
    
    // Write the configuration to the Digital Color Sensor Control Register
    writeRegister(tcs_i2cAddress, TCS3414_REG_COLOR_CONTROL | COMMAND_WRITE | COMMAND_TRANC_BYTE, control);
    
    // Wait for the configuration to complete
    delay(tcs_conversionDelay);
}

/**************************************************************************/
/*
        Reads the Digital Color Sensor
*/
/**************************************************************************/
void TCS3414::Measure_Color()
{
    uint8_t GdataLo, GdataHi, RdataLo, RdataHi, BdataLo, BdataHi, CdataLo, CdataHi;
    uint16_t GData, RData, BData, CData;
    
    // Reading the Green Data Low Register
    GdataLo = readRegister(tcs_i2cAddress, TCS3414_REG_COLOR_GDATAL | COMMAND_WRITE | COMMAND_TRANC_BYTE);
    // Reading the Green Data High Register
    GdataHi = readRegister(tcs_i2cAddress, TCS3414_REG_COLOR_GDATAH | COMMAND_WRITE | COMMAND_TRANC_BYTE);
    // Conversion of the result
    // 16-bit unsigned result for Green Data of TCS3414
    GData = (uint16_t)((GdataHi << 8) | GdataLo);
    
    // Reading the Red Data Low Register
    RdataLo = readRegister(tcs_i2cAddress, TCS3414_REG_COLOR_RDATAL | COMMAND_WRITE | COMMAND_TRANC_BYTE);
    // Reading the Red Data High Register
    RdataHi = readRegister(tcs_i2cAddress, TCS3414_REG_COLOR_RDATAH | COMMAND_WRITE | COMMAND_TRANC_BYTE);
    // Conversion of the result
    // 16-bit unsigned result for Red Data of TCS3414
    RData = (uint16_t)((RdataHi << 8) | RdataLo);
    
    // Reading the Blue Data Low Register
    BdataLo = readRegister(tcs_i2cAddress, TCS3414_REG_COLOR_BDATAL | COMMAND_WRITE | COMMAND_TRANC_BYTE);
    // Reading the Blue Data High Register
    BdataHi = readRegister(tcs_i2cAddress, TCS3414_REG_COLOR_BDATAH | COMMAND_WRITE | COMMAND_TRANC_BYTE);
    // Conversion of the result
    // 16-bit unsigned result for Blue Data of TCS3414
    BData = (uint16_t)((BdataHi << 8) | BdataLo);
    
    // Read the Data
    // Reading the Clear Data Low Register
    CdataLo = readRegister(tcs_i2cAddress, TCS3414_REG_COLOR_CDATAL | COMMAND_WRITE | COMMAND_TRANC_BYTE);
    // Reading the Clear Data High Register
    CdataHi = readRegister(tcs_i2cAddress, TCS3414_REG_COLOR_CDATAH | COMMAND_WRITE | COMMAND_TRANC_BYTE);
    // Conversion of the result
    // 16-bit unsigned result for Clear Data of TCS3414
    CData = (uint16_t)((CdataHi << 8) | CdataLo);
    
    tcs_colorData.G = GData;
    tcs_colorData.R = RData;
    tcs_colorData.B = BData;
    tcs_colorData.C = CData;
}
