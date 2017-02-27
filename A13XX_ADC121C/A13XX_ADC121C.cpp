/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        A13XX_ADC121C
        This code is designed to work with the A13XX_ADC121C_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/hall-effect-sensor-0-5-mv-g-with-adc121c-12-bit-resolution?variant=25687579211#tabs-0-product_tabset-2
 */
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "A13XX_ADC121C.h"

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
        Instantiates a new A13XX_ADC121C class with appropriate properties
*/
/**************************************************************************/
void A13XX_ADC121C::getAddr_A13XX_ADC121C(uint8_t i2cAddress)
{
    adc_i2cAddress = i2cAddress;
    adc_conversionDelay = A13XX_ADC121C_CONVERSIONDELAY;
}

/**************************************************************************/
/*
        Sets up the Hardware
*/
/**************************************************************************/
void A13XX_ADC121C::begin()
{
    Wire.begin();
}

/**************************************************************************/
/*
        Reads 12-bits (2 bytes) from the device
        Total 2 bytes are received. The 2 bytes contains the results for
        Hall Effect Rotational Angle ranging from 0° to 360°
*/
/**************************************************************************/
void A13XX_ADC121C::readRegister(uint8_t i2cAddress)
{
    uint8_t Angle_Hi, Angle_Lo;
    uint16_t raw_magangle;
    
    Wire.beginTransmission(i2cAddress);
    delay(adc_i2cAddress);
    Wire.endTransmission();
    Wire.requestFrom(i2cAddress, (uint8_t)2);
    Angle_Hi = i2cread();
    Angle_Lo = i2cread();
    
    // Convert the data to 14-bits
    raw_magangle = ((Angle_Hi & 0x0F) << 8) | Angle_Lo;
    magAngle = (raw_magangle * 360.0) / 4096.0;
}

/**************************************************************************/
/*
 Gets the results for Hall Effect Sensor
 */
/**************************************************************************/
float A13XX_ADC121C::Measure_HallSensor()
{
    return magAngle;
}
