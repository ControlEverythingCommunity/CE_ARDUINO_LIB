/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ADC121C_ACS714
        This code is designed to work with the ADC121C_I2CS_ACS714 I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/1-channel-dc-current-monitor-for-i2c
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "ADC121C_ACS714.h"

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
        Instantiates a new ADC121C_ACS714 class with appropriate properties
*/
/**************************************************************************/
void ADC121C_ACS714::getAddr_ADC121C_ACS714(uint8_t i2cAddress)
{
    ADC121C_ACS714_i2cAddress = i2cAddress;
    ADC121C_ACS714_conversionDelay = ADC121C_ACS714_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void ADC121C_ACS714::begin()
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
bool ADC121C_ACS714::getAlertStatus()
{
    // high order bit is the alert flag, mask off the rest
    bool alert = (readRegister(ADC121C_ACS714_i2cAddress, ADC121C_ACS714_REG_POINTER_CONVERSION) & 0x8000) ? true : false;
    
    if (alert)
    {
        // Read the alert low and high values
        // And set the appropriate member variables
        uint8_t status = readRegister8(ADC121C_ACS714_i2cAddress, ADC121C_ACS714_REG_POINTER_ALERT_STATUS);
        // Under Range Alert Flag
        if (status & 0x01)
            ADC121C_ACS714_alertLow = true;
        else
            ADC121C_ACS714_alertLow = false;
        
        // Over Range Alert Flag
        if (status & 0x02)
            ADC121C_ACS714_alertHigh = true;
        else
            ADC121C_ACS714_alertHigh = false;
    }
    
    return alert;
}

/**************************************************************************/
/*
        Get the current status of the Under Range Alert/adc_alertLow
        getAlertStatus() must be called to update this value
*/
/**************************************************************************/
bool ADC121C_ACS714::alertLowTriggered()
{
    return ADC121C_ACS714_alertLow;
}

/**************************************************************************/
/*
        Get the current status of the Over Range Alert/adc_alertHigh
        getAlertStatus() must be called to update this value
*/
/**************************************************************************/
bool ADC121C_ACS714::alertHighTriggered()
{
    return ADC121C_ACS714_alertHigh;
}

/**************************************************************************/
/*
        Clears the alert low and alert high flags. 
        This also clears the last stored alert values.
*/
/**************************************************************************/
void ADC121C_ACS714::clearAlertStatus()
{
    // Zero out both the low and high alert flags
    uint8_t status = 0x03;
    writeRegister8(ADC121C_ACS714_i2cAddress, ADC121C_ACS714_REG_POINTER_ALERT_STATUS, status);
    
    ADC121C_ACS714_alertHigh = false;
    ADC121C_ACS714_alertLow = false;
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
void ADC121C_ACS714::setCycleTime(ADC121C_ACS714CycleTime_t cycletime)
{
    ADC121C_ACS714_cycletime = cycletime;
}

/**************************************************************************/
/*
        Gets the Automatic Conversion Mode
*/
/**************************************************************************/
ADC121C_ACS714CycleTime_t ADC121C_ACS714::getCycleTime()
{
    return ADC121C_ACS714_cycletime;
}

/**************************************************************************/
/*
        Sets the Alert Hold Status
*/
/**************************************************************************/
void ADC121C_ACS714::setAlertHold(ADC121C_ACS714AlertHold_t alerthold)
{
    ADC121C_ACS714_alerthold = alerthold;
}

/**************************************************************************/
/*
        Gets the Alert Hold Status
*/
/**************************************************************************/
ADC121C_ACS714AlertHold_t ADC121C_ACS714::getAlertHold()
{
    return ADC121C_ACS714_alerthold;
}

/**************************************************************************/
/*
        Sets the Alert Flag Enable Status
        This controls the alert status bit [D15] in the Conversion Result register
*/
/**************************************************************************/
void ADC121C_ACS714::setAlertFlag(ADC121C_ACS714AlertFlag_t alertflag)
{
    ADC121C_ACS714_alertflag = alertflag;
}

/**************************************************************************/
/*
        Gets the Alert Flag Enable Status
*/
/**************************************************************************/
ADC121C_ACS714AlertFlag_t ADC121C_ACS714::getAlertFlag()
{
    return ADC121C_ACS714_alertflag;
}

/**************************************************************************/
/*
        Sets the Alert Pin Enable Status
        This controls the ALERT output pin status.
*/
/**************************************************************************/
void ADC121C_ACS714::setAlertPin(ADC121C_ACS714AlertPin_t alertpin)
{
    ADC121C_ACS714_alertpin = alertpin;
}

/**************************************************************************/
/*
        Gets the Alert Pin Enable Status
*/
/**************************************************************************/
ADC121C_ACS714AlertPin_t ADC121C_ACS714::getAlertPin()
{
    return ADC121C_ACS714_alertpin;
}

/**************************************************************************/
/*
        Sets the Polarity
        This configures the active level polarity of the ALERT output pin.
*/
/**************************************************************************/
void ADC121C_ACS714::setPolarity(ADC121C_ACS714Polarity_t polarity)
{
    ADC121C_ACS714_polarity = polarity;
}

/**************************************************************************/
/*
        Gets the Polarity
*/
/**************************************************************************/
ADC121C_ACS714Polarity_t ADC121C_ACS714::getPolarity()
{
    return ADC121C_ACS714_polarity;
}

/**************************************************************************/
/*
        Sets the lower limit threshold used to determine the alert condition
        If the alerts are enabled and the measured conversion moves lower than this limit, 
        a VLOW alert is generated.
*/
/**************************************************************************/
uint16_t ADC121C_ACS714::setAlertLowLimit(uint16_t limit)
{
    // mask off the invalid bits in case they were set
    limit &= 0x0FFF;
    return writeRegister(ADC121C_ACS714_i2cAddress, ADC121C_ACS714_REG_POINTER_LOW_LIMIT, limit);
}

/**************************************************************************/
/*
        Sets the hysteresis value used to determine the alert condition
        If the alerts are enabled and the measured conversion moves higher than this limit,
        a VHIGH alert is generated.
*/
/**************************************************************************/
uint16_t ADC121C_ACS714::setAlertHighLimit(uint16_t limit)
{
    // mask off the invalid bits in case they were set
    limit &= 0x0FFF;
    return writeRegister(ADC121C_ACS714_i2cAddress, ADC121C_ACS714_REG_POINTER_HIGH_LIMIT, limit);
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
uint16_t ADC121C_ACS714::setHysteresis(uint16_t hysteresis)
{
    // mask off the invalid bits in case they were set
    hysteresis &= 0x0FFF;
    return writeRegister(ADC121C_ACS714_i2cAddress, ADC121C_ACS714_REG_POINTER_HYSTERESIS, hysteresis);
}

/**************************************************************************/
/*
        Gets the Lowest Conversion result recorded so far
        The value of this register will update automatically when the automatic 
        conversion mode is enabled, but is NOT updated in the normal mode.
*/
/**************************************************************************/
uint16_t ADC121C_ACS714::getLowestConversion()
{
    return readRegister(ADC121C_ACS714_i2cAddress, ADC121C_ACS714_REG_POINTER_LOWCONV);
}

/**************************************************************************/
/*
        Gets the Highest Conversion result recorded so far
        The value of this register will update automatically when the automatic 
        conversion mode is enabled, but is NOT updated in the normal mode.
*/
/**************************************************************************/
uint16_t ADC121C_ACS714::getHighestConversion()
{
    return readRegister(ADC121C_ACS714_i2cAddress, ADC121C_ACS714_REG_POINTER_HIGHCONV);
}

/**************************************************************************/
/*
        Clears the Lowest Conversion value recorded so far
*/
/**************************************************************************/
uint16_t ADC121C_ACS714::clearLowestConversion()
{
    uint16_t lowClear = 0x0FFF;
    return writeRegister(ADC121C_ACS714_i2cAddress, ADC121C_ACS714_REG_POINTER_LOWCONV, lowClear);
}

/**************************************************************************/
/*
        Clears the Highest Conversion value recorded so far
*/
/**************************************************************************/
uint16_t ADC121C_ACS714::clearHighestConversion()
{
    uint16_t highClear = 0x0000;
    return writeRegister(ADC121C_ACS714_i2cAddress, ADC121C_ACS714_REG_POINTER_HIGHCONV, highClear);
}

/**************************************************************************/
/*
        Reads the result of the most recent conversion. 
        In the normal mode, a new conversion is started whenever this register is read
*/
/**************************************************************************/
uint16_t ADC121C_ACS714::Measure_Voltage()
{
    // Start with default values
    // Set the Cycle Time
    uint8_t config = ADC121C_ACS714_cycletime;
    
    // Set the Alert Hold Status
    config |= ADC121C_ACS714_alerthold;
    
    // Set the Alert Flag Enable Status
    config |= ADC121C_ACS714_alertflag;
    
    // Set the Alert Pin Enable Status
    config |= ADC121C_ACS714_alertpin;
    
    // Set the Polarity
    config |= ADC121C_ACS714_polarity;

    // Write config register to the ADC
    writeRegister8(ADC121C_ACS714_i2cAddress, ADC121C_ACS714_REG_POINTER_CONFIG, config);

    // Wait for the conversion to complete
    delay(ADC121C_ACS714_conversionDelay);

    // Read the conversion result
    // 12-bit unsigned result for the ADC121C_ACS714
    return readRegister(ADC121C_ACS714_i2cAddress, ADC121C_ACS714_REG_POINTER_CONVERSION);
}

/**************************************************************************/
/*
        Gets the Current Value
 */
/**************************************************************************/
float ADC121C_ACS714::Measure_Current()
{
    /*
     ACS714-5.0 A Current Sensor
     Sensitivity = 0.185 V/A
     For VCC = 5.0 V, and ADC VREF = VCC, the relationship between Input Voltage and ADC Counts is,
     
     Count = Resolution/VCC * VIN
     But, VIN = VCC / 2 + Sensitivity * Current
     So, Count = Resolution / VCC * (VCC / 2 + Sensitivity * Current)
     Solving for Curent, we get
     Current = 0.0066 * Count - 13.51
    */
    
    float current = 0.0;
    for(int i = 0; i < ADC121C_ACS714_SAMPLE_TIMES; i++)
    {
        current += (VCC / (SENSITIVITY * ADC_RESOLUTION)) * (Measure_Voltage() - (ADC_RESOLUTION / 2));
        delay(ADC121C_ACS714_SAMPLE_INTERVAL);
            current = current / 100.0;
    }
    return current;
}


