/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HIH7130
        This code is designed to work with the HIH7130_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/humidity-and-temperature-sensor-3-rh-1-0-c
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
    #define HIH7130_DEFAULT_ADDRESS          (0x27)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define HIH7130_CONVERSIONDELAY          (100)

class HIH7130
{
    protected:
        // Instance-specific properties
        uint8_t hih_conversionDelay;
        float humidity, temperature;
    
    public:
        uint8_t hih_i2cAddress;
        void getAddr_HIH7130(uint8_t i2cAddress);
        void readRegister(uint8_t i2cAddress);
        void begin(void);
        float Measure_Humidity(void);
        float Measure_Temperature(void);
  
    private:
};
