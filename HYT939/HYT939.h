/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        HYT939
        This code is designed to work with the HYT939_I2CS I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Temperature?sku=HYT939_I2CS#tabs-0-product_tabset-2
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
    #define HYT939_DEFAULT_ADDRESS          (0x28)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define HYT939_CONVERSIONDELAY          (100)

class HYT939
{
    protected:
        // Instance-specific properties
        uint8_t hih_conversionDelay;
        float humidity, temperature;
    
    public:
        uint8_t hih_i2cAddress;
        void getAddr_HYT939(uint8_t i2cAddress);
        void readRegister(uint8_t i2cAddress);
        void begin(void);
        float Measure_Humidity(void);
        float Measure_Temperature(void);
  
    private:
};
