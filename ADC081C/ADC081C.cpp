/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ADC081C
        This code is designed to work with the ADC081C_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Analog-Digital-Converters?sku=ADC081C_I2CADC#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "ADC081C.h"

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
static uint8_t writeRegister8(uint8_t i2cAddress, uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    i2cwrite((uint8_t)value);
    Wire.endTransmission();
}

/**************************************************************************/
/*
        Writes 16-bits to the specified destination register
*/
/**************************************************************************/
static uint16_t writeRegister(uint8_t i2cAddress, uint8_t reg, uint16_t value)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)reg);
    i2cwrite((uint8_t)(value>>8));
    i2cwrite((uint8_t)(value & 0xFF));
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
    return (int16_t)(i2cread());
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
        Instantiates a new ADC081C class with appropriate properties
*/
/**************************************************************************/
void ADC081C::getAddr_ADC081C(uint8_t i2cAddress)
{
    adc_i2cAddress = i2cAddress;
    adc_conversionDelay = ADC081C_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void ADC081C::begin()
{
    Wire.begin();
}

/**************************************************************************/
/*
        Get the current status of the alert flag
        If the flag is set, the low or high alert indicators are set as appropriate, 
        and you can access these values with alertLowTriggered() or alertHighTriggered().
*/
/**************************************************************************/
bool ADC081C::getAlertStatus()
{
    // high order bit is the alert flag, mask off the rest
    bool alert = (readRegister(adc_i2cAddress, ADC081C_REG_POINTER_CONVERSION) & 0x8000) ? true : false;
    
    if (alert)
    {
        // Read the alert low and high values
        // And set the appropriate member variables
        uint8_t status = readRegister8(adc_i2cAddress, ADC081C_REG_POINTER_ALERT_STATUS);
        // Under Range Alert Flag
        if (status & 0x01)
            adc_alertLow = true;
        else
            adc_alertLow = false;
        
        // Over Range Alert Flag
        if (status & 0x02)
            adc_alertHigh = true;
        else
            adc_alertHigh = false;
    }
    
    return alert;
}

/**************************************************************************/
/*
        Get the current status of the Under Range Alert/adc_alertLow
        getAlertStatus() must be called to update this value
*/
/**************************************************************************/
bool ADC081C::alertLowTriggered()
{
    return adc_alertLow;
}

/**************************************************************************/
/*
        Get the current status of the Over Range Alert/adc_alertHigh
        getAlertStatus() must be called to update this value
*/
/**************************************************************************/
bool ADC081C::alertHighTriggered()
{
    return adc_alertHigh;
}

/**************************************************************************/
/*
        Clears the alert low and alert high flags. 
        This also clears the last stored alert values.
*/
/**************************************************************************/
void ADC081C::clearAlertStatus()
{
    // Zero out both the low and high alert flags
    uint8_t status = 0x03;
    writeRegister8(adc_i2cAddress, ADC081C_REG_POINTER_ALERT_STATUS, status);
    
    adc_alertHigh = false;
    adc_alertLow = false;
}

/**************************************************************************/
/*
        Sets the Cycle Time
        This configures Automatic Conversion Mode
        When these bits are set to zeros, the automatic conversion
        mode is disabled. This is the case at power-up.
        When these bits are set to a non-zero value, 
        the ADC will begin operating in automatic conversion mode.
        Different values provide various conversion intervals.
*/
/**************************************************************************/
void ADC081C::setCycleTime(adccycleTime_t cycletime)
{
    adc_cycletime = cycletime;
}

/**************************************************************************/
/*
        Gets the Automatic Conversion Mode
*/
/**************************************************************************/
adccycleTime_t ADC081C::getCycleTime()
{
    return adc_cycletime;
}

/**************************************************************************/
/*
        Sets the Alert Hold Status
*/
/**************************************************************************/
void ADC081C::setAlertHold(adcAlertHold_t alerthold)
{
    adc_alerthold = alerthold;
}

/**************************************************************************/
/*
        Gets the Alert Hold Status
*/
/**************************************************************************/
adcAlertHold_t ADC081C::getAlertHold()
{
    return adc_alerthold;
}

/**************************************************************************/
/*
        Sets the Alert Flag Enable Status
        This controls the alert status bit [D15] in the Conversion Result register
*/
/**************************************************************************/
void ADC081C::setAlertFlag(adcAlertFlag_t alertflag)
{
    adc_alertflag = alertflag;
}

/**************************************************************************/
/*
        Gets the Alert Flag Enable Status
*/
/**************************************************************************/
adcAlertFlag_t ADC081C::getAlertFlag()
{
    return adc_alertflag;
}

/**************************************************************************/
/*
        Sets the Alert Pin Enable Status
        This controls the ALERT output pin status.
*/
/**************************************************************************/
void ADC081C::setAlertPin(adcAlertPin_t alertpin)
{
    adc_alertpin = alertpin;
}

/**************************************************************************/
/*
        Gets the Alert Pin Enable Status
*/
/**************************************************************************/
adcAlertPin_t ADC081C::getAlertPin()
{
    return adc_alertpin;
}

/**************************************************************************/
/*
        Sets the Polarity
        This configures the active level polarity of the ALERT output pin.
*/
/**************************************************************************/
void ADC081C::setPolarity(adcPolarity_t polarity)
{
    adc_polarity = polarity;
}

/**************************************************************************/
/*
        Gets the Polarity
*/
/**************************************************************************/
adcPolarity_t ADC081C::getPolarity()
{
    return adc_polarity;
}

/**************************************************************************/
/*
        Sets the lower limit threshold used to determine the alert condition
        If the alerts are enabled and the measured conversion moves lower than this limit, 
        a VLOW alert is generated.
*/
/**************************************************************************/
uint16_t ADC081C::setAlertLowLimit(uint16_t limit)
{
    // mask off the invalid bits in case they were set
    limit &= 0x0FFF;
    return writeRegister(adc_i2cAddress, ADC081C_REG_POINTER_LOW_LIMIT, limit);
}

/**************************************************************************/
/*
        Sets the hysteresis value used to determine the alert condition
        If the alerts are enabled and the measured conversion moves higher than this limit,
        a VHIGH alert is generated.
*/
/**************************************************************************/
uint16_t ADC081C::setAlertHighLimit(uint16_t limit)
{
    // mask off the invalid bits in case they were set
    limit &= 0x0FFF;
    return writeRegister(adc_i2cAddress, ADC081C_REG_POINTER_HIGH_LIMIT, limit);
}

/**************************************************************************/
/*
        Sets the upper limit threshold used to determine the alert condition
        After a VHIGH or VLOW alert occurs, the conversion result must move within 
        the VHIGH or VLOW limit by more than this value to clear the alert condition
        If the Alert Hold bit is set in the configuration register, alert conditions
        will not self-clear.
*/
/**************************************************************************/
uint16_t ADC081C::setHysteresis(uint16_t hysteresis)
{
    // mask off the invalid bits in case they were set
    hysteresis &= 0x0FFF;
    return writeRegister(adc_i2cAddress, ADC081C_REG_POINTER_HYSTERESIS, hysteresis);
}

/**************************************************************************/
/*
        Gets the Lowest Conversion result recorded so far
        The value of this register will update automatically when the automatic 
        conversion mode is enabled, but is NOT updated in the normal mode.
*/
/**************************************************************************/
uint16_t ADC081C::getLowestConversion()
{
    return readRegister(adc_i2cAddress, ADC081C_REG_POINTER_LOWCONV);
}

/**************************************************************************/
/*
        Gets the Highest Conversion result recorded so far
        The value of this register will update automatically when the automatic 
        conversion mode is enabled, but is NOT updated in the normal mode.
*/
/**************************************************************************/
uint16_t ADC081C::getHighestConversion()
{
    return readRegister(adc_i2cAddress, ADC081C_REG_POINTER_HIGHCONV);
}

/**************************************************************************/
/*
        Clears the Lowest Conversion value recorded so far
*/
/**************************************************************************/
uint16_t ADC081C::clearLowestConversion()
{
    uint16_t lowClear = 0x0FFF;
    return writeRegister(adc_i2cAddress, ADC081C_REG_POINTER_LOWCONV, lowClear);
}

/**************************************************************************/
/*
        Clears the Highest Conversion value recorded so far
*/
/**************************************************************************/
uint16_t ADC081C::clearHighestConversion()
{
    uint16_t highClear = 0x0000;
    return writeRegister(adc_i2cAddress, ADC081C_REG_POINTER_HIGHCONV, highClear);
}

/**************************************************************************/
/*
        Reads the result of the most recent conversion. 
        In the normal mode, a new conversion is started whenever this register is read
*/
/**************************************************************************/
uint8_t ADC081C::Measure_Voltage()
{
    // Start with default values
    // Set the Cycle Time
    uint8_t config = adc_cycletime;
    
    // Set the Alert Hold Status
    config |= adc_alerthold;
    
    // Set the Alert Flag Enable Status
    config |= adc_alertflag;
    
    // Set the Alert Pin Enable Status
    config |= adc_alertpin;
    
    // Set the Polarity
    config |= adc_polarity;

    // Write config register to the ADC
    writeRegister(adc_i2cAddress, ADC081C_REG_POINTER_CONFIG, config);

    // Wait for the conversion to complete
    delay(adc_conversionDelay);

    // Read the conversion result
    // 8-bit unsigned result for the ADC081C
    uint16_t raw_voltage = readRegister(adc_i2cAddress, ADC081C_REG_POINTER_CONVERSION);
    raw_voltage &= 0x0FF0;
    raw_voltage >>= 4;
    
    return (uint8_t)raw_voltage;
}
