/****************************************************************************
 /*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        MCP3426
        This code is designed to work with the MCP3426_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Analog-Digital-Converters?sku=MCP3426_I2CADC#tabs-0-product_tabset-2
*/
/****************************************************************************/

#include <Wire.h>
#include <MCP3426.h>

/**************************************************************************/
/*
        Instantiates a new MCP3426 class with appropriate properties
*/
/***************************************************************************/
MCP3426::MCP3426(uint8_t devAddress)
{
    Wire.begin();
    devAddr = (uint8_t)(1101<<3);
    devAddr |= devAddress;
}

MCP3426::~MCP3426()
{
}

/***************************************************************************/
/*
        Verify the I2C connection and Sets up the Hardware
*/
/***************************************************************************/
bool MCP3426::testConnection()
{
    Wire.beginTransmission(devAddr);
    return (Wire.endTransmission() == 0);
}

/**************************************************************************/
/*
        Set the Configuration register with , Sample Rate, Conversion Mode and PGA Gain
        channel: This determines an ADC reading from the specified channel
        resolution: This determines the resolution (Sample Rate) for the device
        mode: This determines the current operational status of the device
        PGA: This configures the programmable gain amplifier
*/
/**************************************************************************/
void MCP3426::SetConfiguration(uint8_t channel, uint8_t resolution, bool mode, uint8_t PGA)
{
    GAIN = PGA;

    if(resolution!=12 && resolution!=14 && resolution!=16)
    {
        SPS = 12;
    }
    else
    {
        SPS = resolution;
    }

    MODE = mode;
    config = 0;
    config = config<<2;
    // Setting the Channel
    config |= (channel-1);
    config = config<<1;
    // Setting the Conversion Mode
    config |= mode;
    config = config<<2;
    // Setting the Resolution (Sample Rate)
    config |= int((SPS-12)/2);
    config = config<<2;
    // Setting the PGA Gain
    config |=int(log(PGA)/log(2));
    
    // Start a conversion using configuration settings
    Wire.beginTransmission(devAddr);
    // 128: This bit is the data ready flag
    // One-Shot Conversion mode
    // Initiate a new conversion
    Wire.write((config |= 128));
    Wire.endTransmission();
}

/**************************************************************************/
/*
        Check the adc conversion
*/
/**************************************************************************/
bool MCP3426::CheckConversion()
{
    uint8_t i = 0;
    no_of_bytes = 3;
    Wire.requestFrom(devAddr, no_of_bytes);

    while(Wire.available())
    {   data[i++] = Wire.read();

        testvar = data[no_of_bytes-1] >> 7;
    }
    return testvar;
}

/**************************************************************************/
/*
        Reads the conversion results, measuring the voltage
        for an ADC reading from the specified channel
        Generates a signed value since the difference can be either
        positive or negative
        The resolution makes the ouptut in 12/14/16-bit
        LSB = (2 * VREF) / 2^N = (2 * 2.048 V) / 2^N
        Where:
        N = Resolution, which is programmed in the Configuration Register: 12, 14, or 16
*/
/**************************************************************************/
long MCP3426::readADC()
{

    raw_adc = 0;

    while(CheckConversion() == 1);

    switch (SPS)
    {
  
        case 12:
                raw_adc = data[0];
                raw_adc &= 0b00001111;
                raw_adc = raw_adc << 8;
                raw_adc |= data[1];

                if(raw_adc > 2047)
                {
                    raw_adc = raw_adc - 4096;
                }
        
                // raw_adc = raw_adc * LSB(1 mV)/PGA for PGA = 1;
        
                break;
    
        case 14:
                raw_adc = data[0];
                raw_adc &= 0b00111111;
                raw_adc = raw_adc << 8;
                raw_adc |= data[1];

                if(raw_adc > 8191)
                {
                    raw_adc = raw_adc - 16384;
                }
        
                // raw_adc = raw_adc * LSB(250 µV)/PGA for PGA = 1;
       
                break;
    
        case 16:

                raw_adc = data[0];
                raw_adc = raw_adc << 8;
                raw_adc |= data[1];

                if(raw_adc > 32767)
                {
                    raw_adc = raw_adc - 65536;
                }
            
                // raw_adc = raw_adc * LSB(62.5 µV)/PGA for PGA = 1;
      
                break;
    }
    return raw_adc;
}
