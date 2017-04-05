/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HIH9130
        This code is designed to work with the HIH9130_I2CS I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/humidity-and-temperature-sensor-1-7-rh-0-6-c
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
    #define HIH9130_DEFAULT_ADDRESS          (0x27)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define HIH9130_CONVERSIONDELAY          (100)

class HIH9130
{
    protected:
        // Instance-specific properties
        uint8_t hih_conversionDelay;
        float humidity, temperature;
    
    public:
        uint8_t hih_i2cAddress;
        void getAddr_HIH9130(uint8_t i2cAddress);
        void readRegister(uint8_t i2cAddress);
        void begin(void);
        float Measure_Humidity(void);
        float Measure_Temperature(void);
  
    private:
};
