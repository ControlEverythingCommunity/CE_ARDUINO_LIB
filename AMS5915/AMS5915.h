/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        AMS5915
        This code is designed to work with the AMS5915_I2CS I2C Mini Module available from ControlEverything.com.
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
    #define AMS5915_DEFAULT_ADDRESS          (0x28)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define AMS5915_CONVERSIONDELAY          (100)


class AMS5915
{
    protected:
        // Instance-specific properties
        uint8_t ams_conversionDelay;
        float pressure, temperature;
    
    public:
        uint8_t ams_i2cAddress;
        void getAddr_AMS5915(uint8_t i2cAddress);
        void begin(void);
        float getPressure(void);
        float getTemperature(void);
        void Measure_PressureAndTemperature(float pMin, float pMax);
  
    private:
};
