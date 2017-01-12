/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HIH6020
        This code is designed to work with the HIH6020_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=HIH6020_I2CS#tabs-0-product_tabset-2
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
    #define HIH6020_DEFAULT_ADDRESS          (0x27)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define HIH6020_CONVERSIONDELAY          (100)

class HIH6020
{
    protected:
        // Instance-specific properties
        uint8_t hih_conversionDelay;
        float humidity, temperature;
    
    public:
        uint8_t hih_i2cAddress;
        void getAddr_HIH6020(uint8_t i2cAddress);
        void readRegister(uint8_t i2cAddress);
        void begin(void);
        float Measure_Humidity(void);
        float Measure_Temperature(void);
  
    private:
};
