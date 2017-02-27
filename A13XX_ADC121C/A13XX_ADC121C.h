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

/**************************************************************************
    I2C ADDRESS/BITS
**************************************************************************/
/*                                                                                  A13XX_ADC121
                                                                                    (SOT)           (VSSOP)
                                                                                    ALERT       ADR1        ADR0
*/
    #define A13XX_ADC121C_DEFAULT_ADDRESS                  (0x50)      // 1010000      -----       Floating    Floating
    #define A13XX_ADC121C_ADDRESS_FLGND                    (0x51)      // 1010001      -----       Floating    GND
    #define A13XX_ADC121C_ADDRESS_FLVA                     (0x52)      // 1010010      -----       Floating    VA
    #define A13XX_ADC121C_ADDRESS_GNDFL                    (0x54)      // 1010100  Single Address  GND         Floating
    #define A13XX_ADC121C_ADDRESS_GNDGND                   (0x55)      // 1010101      -----       GND         GND
    #define A13XX_ADC121C_ADDRESS_GNDVA                    (0x56)      // 1010110      -----       GND         VA
    #define A13XX_ADC121C_ADDRESS_VAFL                     (0x58)      // 1011000      -----       VA          Floating
    #define A13XX_ADC121C_ADDRESS_VAGND                    (0x59)      // 1011001      -----       VA          GND
    #define A13XX_ADC121C_ADDRESS_VAVA                     (0x5A)      // 1011010      -----       VA          VA

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define A13XX_ADC121C_CONVERSIONDELAY                  (100)   // define the time interal(in ms) between each conversion


class A13XX_ADC121C
{
    protected:
        // Instance-specific properties
        uint8_t adc_conversionDelay;
        float magAngle;
    
    public:
        uint8_t adc_i2cAddress;
        void getAddr_A13XX_ADC121C(uint8_t i2cAddress);
        void begin(void);
        void readRegister(uint8_t i2cAddress);
        float Measure_HallSensor(void);
    
    private:
};
