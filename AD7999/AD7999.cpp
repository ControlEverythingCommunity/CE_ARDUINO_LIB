/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        AD7999
        This code is designed to work with the AD7999_I2CADC I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/ad7999-4-channel-8-10-12-bit-analog-to-digital-converter
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "AD7999.h"

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
static uint8_t writeRegister8(uint8_t i2cAddress, uint8_t value)
{
    Wire.beginTransmission(i2cAddress);
    i2cwrite((uint8_t)value);
    Wire.endTransmission();
}

/**************************************************************************/
/*
        Reads 16-bits from the destination register
*/
/**************************************************************************/
static uint16_t readRegister(uint8_t i2cAddress)
{
    Wire.beginTransmission(i2cAddress);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)2);
    return (int16_t)((i2cread() << 8) | i2cread());
}

/**************************************************************************/
/*
        Instantiates a new AD7999 class with appropriate properties
*/
/**************************************************************************/
void AD7999::getAddr_AD7999(uint8_t i2cAddress)
{
    ad_i2cAddress = i2cAddress;
    ad_conversionDelay = AD7999_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
bool AD7999::begin()
{
    Wire.begin();
    
    return true;
}

/**************************************************************************/
/*
        Sets the Analog Input Channel(s) to be Converted
*/
/**************************************************************************/
void AD7999::setChannel(adChannel_t channel)
{
    ad_channel = channel;
}

/**************************************************************************/
/*
        Gets the Analog Input Channel(s) to be Converted
*/
/**************************************************************************/
adChannel_t AD7999::getChannel()
{
    return ad_channel;
}

/**************************************************************************/
/*
        Sets the Supply Voltage as the Reference or Choose to use an External Reference
*/
/**************************************************************************/
void AD7999::setSupply(adSupply_t supply)
{
    ad_supply = supply;
}

/**************************************************************************/
/*
        Gets the Supply Voltage as the Reference or Choose to use an External Reference
*/
/**************************************************************************/
adSupply_t AD7999::getSupply()
{
    return ad_supply;
}

/**************************************************************************/
/*
        Sets the Filtering on SDA and SCL
*/
/**************************************************************************/
void AD7999::setFiltering(adFiltering_t filtering)
{
    ad_filtering = filtering;
}

/**************************************************************************/
/*
        Gets the Filtering on SDA and SCL
*/
/**************************************************************************/
adFiltering_t AD7999::getFiltering()
{
    return ad_filtering;
}

/**************************************************************************/
/*
        Sets the Bit Trial Delay
*/
/**************************************************************************/
void AD7999::setBitTrialDelay(adBitTrialDelay_t bittrialdelay)
{
    ad_bittrialdelay = bittrialdelay;
}

/**************************************************************************/
/*
        Gets the Bit Trial Delay
*/
/**************************************************************************/
adBitTrialDelay_t AD7999::getBitTrialDelay()
{
    return ad_bittrialdelay;
}

/**************************************************************************/
/*
        Sets the Sample Delay
*/
/**************************************************************************/
void AD7999::setSampleDelay(adSampleDelay_t sampledelay)
{
    ad_sampledelay = sampledelay;
}

/**************************************************************************/
/*
        Gets the Sample Delay
*/
/**************************************************************************/
adSampleDelay_t AD7999::getSampleDelay()
{
    return ad_sampledelay;
}

/**************************************************************************/
/*
        Reads the result of the most recent conversion. 
        In the normal mode, a new conversion is started whenever this register is read
*/
/**************************************************************************/
uint8_t AD7999::Measure_Voltage()
{
    // Set Up the Configuration Register

    uint8_t config =    AD7999_REG_CONFIG_REF_SEL_SUPPLY    |   // The supply is Used as the Reference, and the Device acts as a 4-Channel Input Part
                        AD7999_REG_CONFIG_FLTR_ENABLED      |   // Filtering on SDA and SCL is Enabled
                        AD7999_REG_CONFIG_BTD_ON            |   // Bit Trial Delay Mechanism is Implemented
                        AD7999_REG_CONFIG_SD_ON;                // Sample Delay Mechanism is Implemented
    
    // Set the Analog Input Channel(s) to be Converted
    
    config |= ad_channel;
    
    // Write Configuration register to the ADC
    writeRegister8(ad_i2cAddress, config);

    // Wait for the conversion to complete
    delay(ad_conversionDelay);

    // Read the conversion result
    // 8-bit unsigned result for the AD7999
    uint16_t raw_voltage = readRegister(ad_i2cAddress);
    raw_voltage &= 0x0FF0;
    raw_voltage >>= 4;
    
    return (uint8_t)raw_voltage;
}
