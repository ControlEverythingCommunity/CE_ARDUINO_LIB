/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ADC121C021_HIH-4030
        This code is designed to work with the ADC121C021_I2CS_HIH-4030 I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/products
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
/*                                                                                  ADC121C021021
                                                                                    (SOT)           (VSSOP)
                                                                                    ALERT       ADR1        ADR0
*/
    #define ADC121C021_HIH4030_DEFAULT_ADDRESS                  (0x50)      // 1010000      -----       Floating    Floating
    #define ADC121C021_HIH4030_ADDRESS_FLGND                    (0x51)      // 1010001      -----       Floating    GND
    #define ADC121C021_HIH4030_ADDRESS_FLVA                     (0x52)      // 1010010      -----       Floating    VA
    #define ADC121C021_HIH4030_ADDRESS_GNDFL                    (0x54)      // 1010100  Single Address  GND         Floating
    #define ADC121C021_HIH4030_ADDRESS_GNDGND                   (0x55)      // 1010101      -----       GND         GND
    #define ADC121C021_HIH4030_ADDRESS_GNDVA                    (0x56)      // 1010110      -----       GND         VA
    #define ADC121C021_HIH4030_ADDRESS_VAFL                     (0x58)      // 1011000      -----       VA          Floating
    #define ADC121C021_HIH4030_ADDRESS_VAGND                    (0x59)      // 1011001      -----       VA          GND
    #define ADC121C021_HIH4030_ADDRESS_VAVA                     (0x5A)      // 1011010      -----       VA          VA

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define ADC121C021_HIH4030_CONVERSIONDELAY                  (100)   // define the time interal(in ms) between each conversion

/**************************************************************************
    STATIC TEMPERATURE (in ºC)
**************************************************************************/
    #define ADC121C021_HIH4030_STATIC_TEMPERATURE               (25)

/**************************************************************************
    SUPPLY VOLTAGE (in V)
 **************************************************************************/
    #define ADC121C021_HIH4030_SUPPLY_VOLTAGE                   (5)

/**************************************************************************
    CONVERSION REGISTER
**************************************************************************/
    #define ADC121C021_HIH4030_REG_CONVERSION                   (0x00)


class ADC121C021_HIH4030
{
    protected:
        // Instance-specific properties
        uint8_t adc_conversionDelay;
        uint8_t adc_staticTemperature;
        uint8_t adc_supplyVoltage;
    
    public:
        uint8_t adc_i2cAddress;
        void getAddr_ADC121C021_HIH4030(uint8_t i2cAddress);
        void begin(void);
        float Measure_Voltage(void);
        float getSensorRH();
        float getTrueRH();


    private:
};
