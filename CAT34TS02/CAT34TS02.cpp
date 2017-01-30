/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        CAT34TS02
        This code is designed to work with the CAT34TS02_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "CAT34TS02.h"

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
        Instantiates a new CAT34TS02 class with appropriate properties
*/
/**************************************************************************/
void CAT34TS02::getAddr_CAT34TS02(uint8_t i2cAddress)
{
    cat_i2cAddress = i2cAddress;
    cat_conversionDelay = CAT34TS02_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
boolean CAT34TS02::begin()
{
    Wire.begin();
    
    if (readRegister(cat_i2cAddress, CAT34TS02_REG_POINTER_MANUFID) != 0x1B09) return false;
    if (readRegister(cat_i2cAddress, CAT34TS02_REG_POINTER_DEVID) != 0x0813) return false;
    if (readRegister(cat_i2cAddress, CAT34TS02_REG_POINTER_DEVID) != 0x0A00) return false;
    return true;
}

/**************************************************************************/
/*
        Sets the EVENT Output with TS Shut-Down
*/
/**************************************************************************/
void CAT34TS02::setEVSD(catEVSD_t evsd)
{
    cat_evsd = evsd;
}

/**************************************************************************/
/*
        Gets the EVENT Output with TS Shut-Down
*/
/**************************************************************************/
catEVSD_t CAT34TS02::getEVSD()
{
    return cat_evsd;
}

/**************************************************************************/
/*
        Sets the SMBus Time−Out Limit by TS
*/
/**************************************************************************/
void CAT34TS02::setSMBus(catSMBus_t smbus)
{
    cat_smbus = smbus;
}

/**************************************************************************/
/*
        Gets the SMBus Time−Out Limit by TS
*/
/**************************************************************************/
catSMBus_t CAT34TS02::getSMBus()
{
    return cat_smbus;
}

/**************************************************************************/
/*
        Sets the Conversion Resolution
*/
/**************************************************************************/
void CAT34TS02::setTRes(catTRes_t tres)
{
    cat_tres = tres;
}

/**************************************************************************/
/*
        Gets the Conversion Resolution
*/
/**************************************************************************/
catTRes_t CAT34TS02::getTRes()
{
    return cat_tres;
}

/**************************************************************************/
/*
        Sets the Temperature Range
*/
/**************************************************************************/
void CAT34TS02::setTRange(catTRange_t trange)
{
    cat_trange = trange;
}

/**************************************************************************/
/*
        Gets the Temperature Range
*/
/**************************************************************************/
catTRange_t CAT34TS02::getTRange()
{
    return cat_trange;
}

/**************************************************************************/
/*
        Sets the Active Range Class
*/
/**************************************************************************/
void CAT34TS02::setARange(catARange_t arange)
{
    cat_arange = arange;
}

/**************************************************************************/
/*
        Gets the Active Range Class
*/
/**************************************************************************/
catARange_t CAT34TS02::getARange()
{
    return cat_arange;
}

/**************************************************************************/
/*
        Sets the Temperature Event
*/
/**************************************************************************/
void CAT34TS02::setTEvent(catTEvent_t tevent)
{
    cat_tevent = tevent;
}

/**************************************************************************/
/*
        Gets the Temperature Event
*/
/**************************************************************************/
catTEvent_t CAT34TS02::getTEvent()
{
    return cat_tevent;
}

/**************************************************************************/
/*
        Sets the TUPPER and TLOWER Limit Hysteresis
*/
/**************************************************************************/
void CAT34TS02::setTHyst(catTHyst_t thyst)
{
    cat_thyst = thyst;
}

/**************************************************************************/
/*
        Gets the TUPPER and TLOWER Limit Hysteresis
*/
/**************************************************************************/
catTHyst_t CAT34TS02::getTHyst()
{
    return cat_thyst;
}

/**************************************************************************/
/*
        Sets the Thermal Sensor Shutdown Mode
*/
/**************************************************************************/
void CAT34TS02::setTShdn(catTShdn_t tshdn)
{
    cat_tshdn = tshdn;
}

/**************************************************************************/
/*
        Gets the Thermal Sensor Shutdown Mode
*/
/**************************************************************************/
catTShdn_t CAT34TS02::getTShdn()
{
    return cat_tshdn;
}

/**************************************************************************/
/*
        Sets the Critical Trip Output
*/
/**************************************************************************/
void CAT34TS02::setTCrit(catTCrit_t tcrit)
{
    cat_tcrit = tcrit;
}

/**************************************************************************/
/*
        Gets the Critical Trip Output
*/
/**************************************************************************/
catTCrit_t CAT34TS02::getTCrit()
{
    return cat_tcrit;
}

/**************************************************************************/
/*
        Sets the Alarm Trip Output
*/
/**************************************************************************/
void CAT34TS02::setTAlarm_t(catTAlarm_t talarm)
{
    cat_talarm = talarm;
}

/**************************************************************************/
/*
        Gets the Alarm Trip Output
*/
/**************************************************************************/
catTAlarm_t CAT34TS02::getTAlarm_t()
{
    return cat_talarm;
}

/**************************************************************************/
/*
        Sets the Interrupt Clear
*/
/**************************************************************************/
void CAT34TS02::setIntrClear(catIntrClear_t intrclear)
{
    cat_intrclear = intrclear;
}

/**************************************************************************/
/*
        Gets the Interrupt Clear
*/
/**************************************************************************/
catIntrClear_t CAT34TS02::getIntrClear()
{
    return cat_intrclear;
}

/**************************************************************************/
/*
        Sets the EVENT Output Status
*/
/**************************************************************************/
void CAT34TS02::setEventStat(catEventStat_t eventstat)
{
    cat_eventstat = eventstat;
}

/**************************************************************************/
/*
        Gets the EVENT Output Status
*/
/**************************************************************************/
catEventStat_t CAT34TS02::getEventStat()
{
    return cat_eventstat;
}

/**************************************************************************/
/*
        Sets the EVENT Output Control
*/
/**************************************************************************/
void CAT34TS02::setEventCtrl(catEventCtrl_t eventctrl)
{
    cat_eventctrl = eventctrl;
}

/**************************************************************************/
/*
        Gets the EVENT Output Control
*/
/**************************************************************************/
catEventCtrl_t CAT34TS02::getEventCtrl()
{
    return cat_eventctrl;
}

/**************************************************************************/
/*
        Sets the Event Condition Triggering
*/
/**************************************************************************/
void CAT34TS02::setEventCrit(catEventCrit_t eventcrit)
{
    cat_eventcrit = eventcrit;
}

/**************************************************************************/
/*
        Gets the Event Condition Triggering
*/
/**************************************************************************/
catEventCrit_t CAT34TS02::getEventCrit()
{
    return cat_eventcrit;
}

/**************************************************************************/
/*
        Sets the EVENT Output Polarity
*/
/**************************************************************************/
void CAT34TS02::setEventPol(catEventPol_t eventpol)
{
    cat_eventpol = eventpol;
}

/**************************************************************************/
/*
        Gets the EVENT Output Polarity
*/
/**************************************************************************/
catEventPol_t CAT34TS02::getEventPol()
{
    return cat_eventpol;
}

/**************************************************************************/
/*
        Sets the EVENT Output Mode
*/
/**************************************************************************/
void CAT34TS02::setEventMod(catEventMod_t eventmod)
{
    cat_eventmod = eventmod;
}

/**************************************************************************/
/*
        Gets the EVENT Output Mode
*/
/**************************************************************************/
catEventMod_t CAT34TS02::getEventMod()
{
    return cat_eventmod;
}

/**************************************************************************/
/*
        Reads the results, measuring the 16-bit temperature register
*/
/**************************************************************************/
int16_t CAT34TS02::Measure_Temp()
{
    // Start with default values
    uint16_t capable =  CAT34TS02_REG_CAPABL_EVSD_FROZEN    |   // A TS shut−down freezes the EVENT output
                        CAT34TS02_REG_CAPABL_TMOUT_LOW      |   // The TS implements SMBus time−out within the range 10 to 60 ms
                        CAT34TS02_REG_CAPABL_ACC_C          |   // +/- 2°C over the active range and +/- 3°C over the operating range (Class C)
                        CAT34TS02_REG_CAPABL_EVENT_CRIT;        // Critical Temperature Only

    // Set the Conversion Resolution
    capable |= cat_tres;
    
    // Set the Temperature Range
    capable |= cat_trange;
    
    // Write config register to the device
    writeRegister(cat_i2cAddress, CAT34TS02_REG_POINTER_CAPABLE, capable);
    
    // Wait for the conversion to complete
    delay(cat_conversionDelay);
    
    uint16_t config =   CAT34TS02_REG_CONFIG_TCRIT_UPDATE       |   // Critical Trip register can be Updated
                        CAT34TS02_REG_CONFIG_TALARM_UPDATE      |   // Alarm Trip register can be Updated
                        CAT34TS02_REG_CONFIG_INTRCLEAR_SELF     |   // Interrupt Clear Always reads as 0 (Self−Clearing)
                        CAT34TS02_REG_CONFIG_EVRNTSTS_NOASRT    |   // EVENT output pin is not being asserted
                        CAT34TS02_REG_CONFIG_EVENTCTRL_DISABLE  |   // EVENT Output Disabled
                        CAT34TS02_REG_CONFIG_TCRITONLY_ALL      |   // Event condition triggered by Alarm or Critical Temperature Limit crossing
                        CAT34TS02_REG_CONFIG_EVENTPOL_LOW       |   // EVENT Output Active Low
                        CAT34TS02_REG_CONFIG_EVENTMOD_COMP;         // EVENT Output Comparator Mode
    
    // Set TUPPER and TLOWER Limit Hysteresis
    config |= cat_thyst;
    
    // Set Thermal Sensor Shutdown Mode
    config |= cat_tshdn;

    // Write config register to the cat
    writeRegister(cat_i2cAddress, CAT34TS02_REG_POINTER_CONFIG, config);

    // Wait for the conversion to complete
    delay(cat_conversionDelay);

    // Read the conversion results
    // 16-bit signed results for the CAT34TS02
    uint16_t raw_temp = readRegister(cat_i2cAddress, CAT34TS02_REG_POINTER_TEMP);
    raw_temp &= 0x0FFF;
    return (int16_t)raw_temp;
}
